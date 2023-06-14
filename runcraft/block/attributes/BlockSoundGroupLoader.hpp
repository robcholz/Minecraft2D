//
// Created by robcholz on 6/9/23.
//

#ifndef RUNCRAFT_BLOCKSOUNDGROUPLOADER_HPP
#define RUNCRAFT_BLOCKSOUNDGROUPLOADER_HPP

#include "sound/SoundEvent.hpp"

class BlockSoundGroupLoader {
protected:
public:
	explicit BlockSoundGroupLoader(float volume, float pitch, SoundEvent& breakSound, SoundEvent& stepSound, SoundEvent& placeSound, SoundEvent& hitSound,
	                               SoundEvent& fallSound) {
		this->volume = volume;
		this->pitch = pitch;
		this->breakSound = breakSound;
		this->stepSound = stepSound;
		this->placeSound = placeSound;
		this->hitSound = hitSound;
		this->fallSound = fallSound;
	}

	[[nodiscard]] float getVolume() const { return volume; }

	[[nodiscard]] float getPitch() const { return pitch; }

	SoundEvent& getBreakSound() { return breakSound; }

	SoundEvent& getStepSound() { return stepSound; }

	SoundEvent& getPlaceSound() { return placeSound; }

	SoundEvent& getHitSound() { return hitSound; }

	SoundEvent& getFallSound() { return fallSound; }

private:
	float volume;
	float pitch;
	SoundEvent breakSound;
	SoundEvent stepSound;
	SoundEvent placeSound;
	SoundEvent hitSound;
	SoundEvent fallSound;
};

#endif //RUNCRAFT_BLOCKSOUNDGROUPLOADER_HPP
