//
// Created by robcholz on 6/26/23.
//

#ifndef RUNCRAFT_MUSICSOUNDEVENT_HPP
#define RUNCRAFT_MUSICSOUNDEVENT_HPP

#include "SoundEvent.hpp"

class MusicSoundEvent {
private:
protected:
public:
	MusicSoundEvent(const SoundEvent& soundEvent, int minDelay, int maxDelay, bool replaceCurrentMusic= false) {
		this->soundEvent = soundEvent;
		this->minDelay = minDelay;
		this->maxDelay = maxDelay;
		this->replaceCurrentMusic = replaceCurrentMusic;
	}

	SoundEvent& getSoundEvent() { return soundEvent; }

	[[nodiscard]]
	int getMinDelay() const { return minDelay; }

	[[nodiscard]]
	int getMaxDelay() const { return maxDelay; }

	[[nodiscard]]
	bool getReplaceCurrentMusic() const { return replaceCurrentMusic; }

private:
	SoundEvent soundEvent;
	int minDelay = 0;
	int maxDelay = 0;
	bool replaceCurrentMusic = false;
};

#endif //RUNCRAFT_MUSICSOUNDEVENT_HPP
