//
// Created by robcholz on 6/11/23.
//

#ifndef MINECRAFT_CHUNKSTREAMACCESS_HPP
#define MINECRAFT_CHUNKSTREAMACCESS_HPP

namespace chunk {
class Chunk;

class ChunkStreamAccess {
 public:
  virtual ~ChunkStreamAccess() = default;

  virtual int getRenderedChunks() = 0;

  virtual uint16_t getUpdatedChunks() const = 0;

  virtual int getDeletedChunks() = 0;

  virtual int getCachedChunks() = 0;

  [[nodiscard]] virtual Intervali getRenderInterval() const = 0;

  [[nodiscard]] virtual Intervali getSimulationInterval() const = 0;

  [[nodiscard]] virtual bool isChunksLoaded() const = 0;
};
}  // namespace chunk

#endif  // MINECRAFT_CHUNKSTREAMACCESS_HPP
