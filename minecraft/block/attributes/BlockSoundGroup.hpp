//
// Created by robcholz on 8/8/23.
//

#ifndef MINECRAFT_2D_BLOCKSOUNDGROUP_HPP
#define MINECRAFT_2D_BLOCKSOUNDGROUP_HPP

#include <map>
#include <optional>
#include <utility>
#include "Block.hpp"
#include "sound/SoundEvent.hpp"

class BlockSoundGroup {
 private:
  using String = std::string;

 public:
  static void bind(const block::Block::BlockPtr& block,
                   SoundEvent::SoundEventPtr soundEvent) {
    soundGroups[block->getSerialID()] = soundEvent;
  }

  static std::optional<SoundEvent::SoundEventPtr> get(
      const block::Block* block) {
    if (soundGroups.contains(block->getSerialID())) {
      return std::make_optional(soundGroups[block->getSerialID()]);
    }
    return std::nullopt;
  }

 private:
  static inline std::map<block::ID::SerialIDT, SoundEvent::SoundEventPtr>
      soundGroups;
};

#endif  // MINECRAFT_2D_BLOCKSOUNDGROUP_HPP
