//
// Created by robcholz on 5/4/23.
//

#ifndef MINECRAFT_CHUNKSTREAM_HPP
#define MINECRAFT_CHUNKSTREAM_HPP

#include <list>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include "Chunk.hpp"
#include "ChunkStreamAccess.hpp"
#include "util/Utils.hpp"
#include "world/WorldAccess.hpp"
#include "world/storage/SaveHelper.hpp"

namespace chunk {
class ChunkStream : public ChunkStreamAccess {
 private:
  using String = std::string;
  using ChunkPosT = coordinate::ChunkPositionT;

 public:
  using DistanceT = unsigned short;

  ChunkStream() = delete;

  explicit ChunkStream(WorldAccess* worldAccess,
                       DistanceT simulationDistance,
                       DistanceT renderDistance) {
    this->worldAccess = worldAccess;
    saveHelper = std::make_unique<SaveHelper>("TestChunkSave");
    regionFileHelper =
        std::make_unique<FileHelper>(saveHelper->getDirectory() + "/region");
    setSimulationDistance(simulationDistance);
    setRenderDistance(renderDistance);
    indexRegions();
  }

  virtual ~ChunkStream() {
    for (const auto& v : chunkSimulationMap)
      saveHelper->saveChunk(*v.second);
  }

  virtual void onUpdate() {}

  void onChunkRender(std::function<void(ChunkPosT)> func) {
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

  int getCachedChunks() override { return (int)cachedChunks.size(); }

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
  std::set<ChunkPosT> cachedChunks;        // chunks that have been cached
  std::list<ChunkPosT> chunkDeletingList;  // chunks that will be deleted
  std::unique_ptr<SaveHelper> saveHelper;
  std::unique_ptr<FileHelper> regionFileHelper;
  std::function<std::unique_ptr<chunk::Chunk>(ChunkPosT)> chunkGenerator =
      nullptr;
  std::function<void(ChunkPosT)> onChunkRendered = nullptr;
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

  void indexRegions() {
    for (const auto& filepath : *regionFileHelper->getFilesInDirectory()) {
      auto filename = filepath.substr(filepath.find_last_of("/\\") + 1);
      auto second_dot_occur = utils::nthOccurrence(filename, ".", 2);
      auto third_dot_occur = utils::nthOccurrence(filename, ".", 3);
      coordinate::ChunkPositionT chunkPos = std::stoi(filename.substr(
          second_dot_occur + 1, third_dot_occur - second_dot_occur));
      cachedChunks.insert(chunkPos);
    }
  }

  void addToIndexedRegions(ChunkPosT chunkPos) {
    cachedChunks.insert(chunkPos);
  }

  std::unique_ptr<chunk::Chunk> loadChunk(ChunkPosT chunkPos) {
    std::unique_ptr<chunk::Chunk> chunk;
    if (cachedChunks.count(chunkPos))
      chunk = saveHelper->loadChunk(chunkPos);
    else {
      chunk = chunkGenerator(chunkPos);
      addToIndexedRegions(chunkPos);
    }
    return chunk;
  }

  void addToSimulationChunks(ChunkPosT chunkPos) {
    // chunk is loaded here
    auto chunk = loadChunk(chunkPos);
    saveHelper->saveChunk(*chunk);
    chunkSimulationMap.insert({chunkPos, std::move(chunk)});
  }

  void removeFromSimulationChunksInFuture(ChunkPosT chunkPos) {
    // chunk will be freed after this scope
    const auto& chunk = chunkSimulationMap[chunkPos];
    saveHelper->saveChunk(*chunk);
    chunkDeletingList.push_front(chunkPos);
  }

  void removeRedundantChunks() {
    for (auto chunkPos : chunkDeletingList) {
      auto chunk = std::move(chunkSimulationMap[chunkPos]);
      chunkSimulationMap.erase(chunkPos);
    }
    chunkDeletingList.clear();
  }

  void updateSimulationChunks() {
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
        addToSimulationChunks(chunkPos);  // load chunks
    }
  }

  void updateRenderingChunks() {
    for (auto v : chunkDeletingList) {
      chunkRenderingMap.erase(v);
    }
    chunkDeletingList.clear();
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
          onChunkRendered(chunkPos);
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
};
}  // namespace chunk

#endif  // MINECRAFT_CHUNKSTREAM_HPP
