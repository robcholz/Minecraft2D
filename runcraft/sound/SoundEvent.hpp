
//
// Created by robcholz on 6/9/23.
//

#ifndef RUNCRAFT_SOUNDEVENT_HPP
#define RUNCRAFT_SOUNDEVENT_HPP

#include <SFML/Audio/SoundBuffer.hpp>
#include "util/Identifier.hpp"

class SoundEvent {
public:
	SoundEvent() = default;

	explicit SoundEvent(const Identifier& id) {
		this->id = id;
		//loadSound();
	}

	SoundEvent(const SoundEvent& soundEvent) {
		this->id = soundEvent.id;
		this->soundBuffer = soundEvent.soundBuffer;
	}

	void loadSound() {
		soundBuffer.loadFromFile(id.getAbsolutePath());
	}

	sf::SoundBuffer& getSound() { return soundBuffer; }

	Identifier& getID() { return id; }

private:
	Identifier id;
	sf::SoundBuffer soundBuffer;
};

#endif //RUNCRAFT_SOUNDEVENT_HPP
