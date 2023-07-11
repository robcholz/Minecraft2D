//
// Created by robcholz on 6/26/23.
//

#ifndef RUNCRAFT_BLOCKSOUNDGROUP_HPP
#define RUNCRAFT_BLOCKSOUNDGROUP_HPP

#include <initializer_list>
#include "SoundEvent.hpp"
#include "block/attributes/Block.hpp"


class BlockSoundGroup {
private:
	using String = std::string;
protected:
public:
	explicit BlockSoundGroup(const block::Block::BlockPtr& block, std::initializer_list<SoundEvent::SoundEventPtr> soundEventsList) {
		for (const auto& soundEvent: soundEventsList) {
			soundEvent->loadSound();
		}
	}

	~BlockSoundGroup() = default;

private:
};

#endif //RUNCRAFT_BLOCKSOUNDGROUP_HPP
