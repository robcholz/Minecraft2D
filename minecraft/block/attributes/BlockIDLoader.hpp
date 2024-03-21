//
// Created by robcholz on 6/6/23.
//

#ifndef MINECRAFT_BLOCKIDLOADER_HPP
#define MINECRAFT_BLOCKIDLOADER_HPP

#include <map>

namespace block {
class BlockIDLoader {
 private:
  using String = std::string;

 public:
  BlockIDLoader() = delete;

  static int getBlockID(const String& id) { return loadID(id); }

  ~BlockIDLoader() = default;

 private:
  static inline std::map<String, int> blockIDs;
  static inline int serialID = 0;

  static int loadID(const String& id) {
    if (blockIDs[id] == 0)
      blockIDs[id] = ++serialID;
    return blockIDs[id];
  }
};
}  // namespace block

#endif  // MINECRAFT_BLOCKIDLOADER_HPP
