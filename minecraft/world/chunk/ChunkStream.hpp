//
// Created by robcholz on 5/4/23.
//
#pragma once

#ifndef MINECRAFT_CHUNKSTREAM_HPP
#define MINECRAFT_CHUNKSTREAM_HPP

#include <atomic>
#include <deque>
#include <list>
#include <memory>
#include <mutex>
#include <semaphore>
#include <numeric>
#include <set>
#include <thread>
#include <utility>
#include <vector>
#include "Chunk.hpp"
#include "ChunkStreamAccess.hpp"
#include "util/Utils.hpp"
#include "world/WorldAccess.hpp"
#include "world/storage/SaveHelper.hpp"

namespace chunk {
/// @brief a concurrent chunk loader
class ChunkStream : public ChunkStreamAccess {
 private:
  using String = std::string;
  using ChunkPosT = coordinate::ChunkPositionT;

 public:
  using DistanceT = unsigned short;

  ChunkStream() = delete;

  explicit ChunkStream(WorldAccess* worldAccess,
                       DistanceT simulationDistance,
                       DistanceT renderDistance){
    PLOG_INFO << "class instance ChunkStream initialized!";
    this->worldAccess = worldAccess;
    this->saveHelper = std::make_unique<SaveHelper>("TestChunkSave");
    this->regionFileHelper =
        std::make_unique<FileHelper>(saveHelper->getDirectory() + "/region");
    setSimulationDistance(simulationDistance);
    setRenderDistance(renderDistance);
    this->chunkIOThread = std::thread{&ChunkStream::chunkIOThreadMain, this};
    chunkIOThread.detach();
  }

  ~ChunkStream() override {
    for (const auto& v : chunkSimulationMap) {
      removeFromSimulationChunksInFuture(v.first);
    }
    removeRedundantChunks();
    threadTryStopFlag = true;
    threadIsStoppedFlag.wait(false);  // wait until the thread is actually
                                      // stopped (threadIsStoppedFlag==true)
    PLOG_INFO << "class instance ChunkStream terminated!";
  }

  virtual void onUpdate() {}

  void onChunkRender(std::function<void(chunk::Chunk&)> func) {
    onChunkRendered = std::move(func);
  }

  void setRenderDistance(DistanceT distance) {
    if (distance >= simulationDistance)
      renderDistance = simulationDistance;
    else
      renderDistance = distance;
  }

  void setSimulationDistance(DistanceT distance) {
    simulationDistance = distance;
  }

  void setChunkGenerator(
      std::function<std::unique_ptr<Chunk>(ChunkPosT)> function) {
    chunkGenerator = std::move(function);
  }

  std::optional<std::reference_wrapper<Chunk>> getChunk(ChunkPosT chunkPos) {
    if (chunkSimulationMap.contains(chunkPos)) {
      return std::make_optional(std::ref(*chunkSimulationMap[chunkPos]));
    }
    PLOG_VERBOSE << "Cannot find the chunk with chunkPosition: " << chunkPos;
    return std::nullopt;
  }

  int getRenderedChunks() override { return (int)chunkRenderingMap.size(); }

  uint16_t getUpdatedChunks() const override {
    return (uint16_t)chunkSimulationMap.size();
  }

  int getDeletedChunks() override { return (int)chunkDeletingList.size(); }

  int getCachedChunks() override {
    std::scoped_lock<std::mutex> cached_chunk_lock(cachedChunksMutex);
    return (int)cachedChunks.size();
  }

  [[nodiscard]] Intervali getRenderInterval() const override {
    return renderInterval;
  }

  [[nodiscard]] Intervali getSimulationInterval() const override {
    return simulationInterval;
  }

  [[nodiscard]] bool isChunksLoaded() const override {
    return !chunkSimulationMap.empty();
  }

  void update() {
    updateStream();
    onUpdate();
    updateChunks();
  }

 private:
  WorldAccess* worldAccess = nullptr;
  std::unordered_map<ChunkPosT, Chunk*>
      chunkRenderingMap;  // chunks that will be rendered
  std::unordered_map<ChunkPosT, std::unique_ptr<Chunk>>
      chunkSimulationMap;                  // chunks that will be updated
  std::list<ChunkPosT> chunkDeletingList;  // chunks that will be deleted

  // these members are only used in chunkIOThread
  std::thread chunkIOThread;  // dedicated thread for chunk loading & saving
  std::set<ChunkPosT> cachedChunks;  // chunks that have been cached
  std::deque<std::unique_ptr<Chunk>> chunkSaveRequestQueue;
  std::deque<ChunkPosT> chunkLoadRequestQueue;
  std::deque<std::unique_ptr<Chunk>> chunkNewlyLoadedQueue;
  std::mutex cachedChunksMutex;
  std::mutex chunkSaveRequestQueueMutex;
  std::mutex chunkLoadRequestQueueMutex;
  std::mutex chunkNewlyLoadedQueueMutex;
  std::counting_semaphore<std::numeric_limits<std::ptrdiff_t>::max()> threadHasTask{0};
  std::atomic_bool threadTryStopFlag{false};
  std::atomic_bool threadIsStoppedFlag{true};
  std::unique_ptr<SaveHelper> saveHelper;
  std::unique_ptr<FileHelper> regionFileHelper;

  std::function<std::unique_ptr<chunk::Chunk>(ChunkPosT)> chunkGenerator =
      nullptr;
  std::function<void(chunk::Chunk&)> onChunkRendered = nullptr;
  DistanceT renderDistance = 0, simulationDistance = 0;
  Intervali renderInterval{}, simulationInterval{};
  ChunkPosT playerChunkPos = 0;

  void updateInterval() {
    auto x_player = worldAccess->getPlayer()
                        ->getEntityPosition()
                        .get<coordinate::PixelPos>()
                        .x;
    playerChunkPos = Chunk::convertToChunkPos(x_player);
    renderInterval.lower = playerChunkPos - renderDistance;
    renderInterval.upper = playerChunkPos + renderDistance;
    simulationInterval.lower = playerChunkPos - simulationDistance;
    simulationInterval.upper = playerChunkPos + simulationDistance;
  }

  void addToSimulationChunksInFuture(ChunkPosT chunkPos) {
    sendLoadChunkRequest(chunkPos);
    auto chunk = chunk::Chunk::emptyChunk(chunkPos);
    addToSimulationChunks(std::move(chunk));
  }

  void addToSimulationChunks(std::unique_ptr<Chunk> chunk) {
    auto chunk_pos = chunk->getChunkPosition();
    if (chunkSimulationMap.contains(chunk_pos)) {
      chunkSimulationMap.erase(chunk_pos);
      chunkRenderingMap.erase(chunk_pos);
    }
    chunkSimulationMap.insert({chunk_pos, std::move(chunk)});
  }

  void removeFromSimulationChunksInFuture(ChunkPosT chunkPos) {
    chunkDeletingList.push_front(chunkPos);
  }

  void removeRedundantChunks() {
    for (auto chunkPos : chunkDeletingList) {
      auto chunk = std::move(chunkSimulationMap[chunkPos]);
      sendUnloadChunkRequest(std::move(chunk));
      chunkSimulationMap.erase(chunkPos);
    }
    chunkDeletingList.clear();
  }

  void updateSimulationChunks() {
    auto loaded_chunk = tryGetLoadedChunk();
    if (loaded_chunk.has_value()) {
      PLOG_DEBUG << "received and added to simulation chunk list";
      addToSimulationChunks(std::move(loaded_chunk.value()));
    }
    for (const auto& v : chunkSimulationMap) {
      auto chunkPos = v.first;
      if (!(simulationInterval.lower <= chunkPos &&
            chunkPos <= simulationInterval.upper)) {
        removeFromSimulationChunksInFuture(
            chunkPos);  // chunk will be removed in the future, since we cannot
                        // edit container during iteration
      }
    }
    for (auto chunkPos = simulationInterval.lower;
         chunkPos <= simulationInterval.upper; chunkPos++) {
      if (!chunkSimulationMap.contains(chunkPos))
        addToSimulationChunksInFuture(chunkPos);  // load chunks
    }
  }

  void updateRenderingChunks() {
    for (auto v : chunkDeletingList) {
      chunkRenderingMap.erase(v);
    }
    for (const auto& v : chunkSimulationMap) {
      auto chunkPos = v.first;
      auto chunk = v.second.get();
      [[unlikely]] if (!(renderInterval.lower <= chunkPos &&
                         chunkPos <= renderInterval.upper)) {
        chunkRenderingMap.erase(chunkPos);
      }  // chunk has been de-rendered
      else {
        if (!chunkRenderingMap.contains(chunkPos)) {
          chunkRenderingMap.insert({chunkPos, chunk});
          onChunkRendered(*chunkRenderingMap[chunkPos]);
        }  // not found
      }    // chunk should be rendered
    }
  }

  void updateStream() {
    // orders cannot be reversed
    updateInterval();
    updateSimulationChunks();
    updateRenderingChunks();
    removeRedundantChunks();
  }

  void updateChunks() {
    for (const auto& v : chunkSimulationMap) {
      v.second->update();
    }
  }

  void sendLoadChunkRequest(ChunkPosT chunkPos) {
    std::scoped_lock<std::mutex> load_lock(chunkLoadRequestQueueMutex);
    chunkLoadRequestQueue.push_front(chunkPos);
    threadHasTask.release();
  }

  std::optional<std::unique_ptr<Chunk>> tryGetLoadedChunk() {
    std::scoped_lock<std::mutex> load_lock(chunkNewlyLoadedQueueMutex);
    if (!chunkNewlyLoadedQueue.empty()) {
      auto chunk = std::move(chunkNewlyLoadedQueue.back());
      chunkNewlyLoadedQueue.pop_back();
      return chunk;
    }
    return std::nullopt;
  }

  void sendUnloadChunkRequest(std::unique_ptr<Chunk> chunk) {
    std::scoped_lock<std::mutex> save_lock(chunkSaveRequestQueueMutex);
    chunkSaveRequestQueue.push_front(std::move(chunk));
    threadHasTask.release();
  }

  /// @warning only used in chunkIOThreadMain
  void indexRegions() {
    for (const auto& filepath : *regionFileHelper->getFilesInDirectory()) {
      auto filename = filepath.substr(filepath.find_last_of("/\\") + 1);
      auto second_dot_occur = utils::nthOccurrence(filename, ".", 2);
      auto third_dot_occur = utils::nthOccurrence(filename, ".", 3);
      coordinate::ChunkPositionT chunkPos = std::stoi(filename.substr(
          second_dot_occur + 1, third_dot_occur - second_dot_occur));
      std::scoped_lock<std::mutex> cached_chunk_lock(cachedChunksMutex);
      cachedChunks.insert(chunkPos);  // added to indexed chunks
    }
  }

  /// @warning only used in chunkIOThreadMain
  void processChunkLoadRequest() {
    PLOG_DEBUG << "process chunk load request";
    auto chunk_pos = chunkLoadRequestQueue.back();
    chunkLoadRequestQueue.pop_back();
    std::unique_ptr<chunk::Chunk> chunk;
    {
      std::scoped_lock<std::mutex> cached_chunk_lock(cachedChunksMutex);
      if (cachedChunks.count(chunk_pos)) {
        PLOG_DEBUG << "loaded chunk from cache files";
        chunk = saveHelper->loadChunk(chunk_pos);
      } else {
        PLOG_DEBUG << "generated chunk";
        chunk = chunkGenerator(chunk_pos);
        cachedChunks.insert(chunk_pos);  // added to indexed chunks
      }
    }
    // send processed chunk
    {
      PLOG_DEBUG << "sent processed chunk to queue";
      std::scoped_lock<std::mutex> newly_loaded_lock(chunkNewlyLoadedQueueMutex);
      chunkNewlyLoadedQueue.push_front(std::move(chunk));
    }
  }

  /// @warning only used in chunkIOThreadMain
  void processChunkSaveRequest() {
    PLOG_DEBUG << "processing chunk save request";
    auto chunk = std::move(chunkSaveRequestQueue.back());
    chunkSaveRequestQueue.pop_back();
    saveHelper->saveChunk(*chunk);
  }

  void chunkIOThreadMain() {
    threadIsStoppedFlag = false;
    indexRegions();
    PLOG_INFO << "chunk IO thread started!";
    while (!threadTryStopFlag) {
      threadHasTask.acquire(); // wait until there are tasks
      {
        std::scoped_lock<std::mutex> load_lock(chunkLoadRequestQueueMutex);
        if (!chunkLoadRequestQueue.empty()) {
          processChunkLoadRequest();
          threadHasTask.release();
        }
      }
      {
        std::scoped_lock<std::mutex> save_lock(chunkSaveRequestQueueMutex);
        if (!chunkSaveRequestQueue.empty()) {
          processChunkSaveRequest();
          threadHasTask.release();
        }
      }
    }
    // if thread is going to stop, check if there are chunks to save
    {
      std::scoped_lock<std::mutex> save_lock(chunkSaveRequestQueueMutex);
      while (!chunkSaveRequestQueue.empty())
        processChunkSaveRequest();
    }
    threadIsStoppedFlag = true;
    threadIsStoppedFlag.notify_one();
    PLOG_INFO << "chunk IO thread terminated!";
  }
};
}  // namespace chunk

#endif  // MINECRAFT_CHUNKSTREAM_HPP
