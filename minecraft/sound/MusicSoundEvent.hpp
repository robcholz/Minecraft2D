//
// Created by robcholz on 6/26/23.
//

#ifndef MINECRAFT_MUSICSOUNDEVENT_HPP
#define MINECRAFT_MUSICSOUNDEVENT_HPP

#include "SoundEvent.hpp"

class MusicSoundEvent {
public:
	using MusicSoundEventPtr = MusicSoundEvent*;
	MusicSoundEvent(const SoundEvent& soundEvent, int minDelay, int maxDelay, bool replaceCurrentMusic= false) {
		this->soundEvent = std::make_unique<SoundEvent>(soundEvent);
		this->minDelay = minDelay;
		this->maxDelay = maxDelay;
		this->replaceCurrentMusic = replaceCurrentMusic;
	}

	~MusicSoundEvent()=default;

	SoundEvent& getSoundEvent() { return *soundEvent; }

	sf::SoundBuffer& getSound() {
		return soundEvent->getSound();
	}

	[[nodiscard]]
	int getMinDelay() const { return minDelay; }

	[[nodiscard]]
	int getMaxDelay() const { return maxDelay; }

	[[nodiscard]]
	bool getReplaceCurrentMusic() const { return replaceCurrentMusic; }

private:
	std::unique_ptr<SoundEvent> soundEvent;
	int minDelay = 0;
	int maxDelay = 0;
	bool replaceCurrentMusic = false;
};

#endif //MINECRAFT_MUSICSOUNDEVENT_HPP
