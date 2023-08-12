
//
// Created by robcholz on 6/9/23.
//

#ifndef MINECRAFT_SOUNDEVENT_HPP
#define MINECRAFT_SOUNDEVENT_HPP

#include <SFML/Audio/SoundBuffer.hpp>
#include "util/Identifier.hpp"
#include "util/math/Random.hpp"
#include "resource/FileHelper.hpp"

class SoundEvent {
private:
	using String = std::string;
public:
	using SoundEventPtr = SoundEvent*;

	explicit SoundEvent(const Identifier& id) {
		this->identifier = std::make_unique<Identifier>(id);
		search(soundsList);
	}

	SoundEvent(const SoundEvent& soundEvent) {
		this->identifier = std::make_unique<Identifier>(*soundEvent.identifier);
		this->currentIdentifier = soundEvent.currentIdentifier;
		this->soundBuffer = soundEvent.soundBuffer;
		this->soundsList = soundEvent.soundsList;
	}

	~SoundEvent() = default;

	void loadSound() {
		if (!currentIdentifier)
			delete currentIdentifier;
		currentIdentifier = new Identifier(Identifier(Random::randomElement(soundsList), Identifier::Category::SOUND));
		soundBuffer.loadFromFile(currentIdentifier->getAbsolutePath());
	}

	sf::SoundBuffer& getSound() { return soundBuffer; }

	Identifier& getID() {
		if (!currentIdentifier)
			return *identifier;
		return *currentIdentifier;
	}

private:
	std::unique_ptr<Identifier> identifier;
	Identifier* currentIdentifier = nullptr;
	sf::SoundBuffer soundBuffer;
	std::vector<String> soundsList;

	static String getRelativePath(const String& path) {
		auto index = utils::nthOccurrence(path, "/sounds/", 1) + 8;
		auto str = path.substr(index, path.length() - index);
		str = str.substr(0, str.find_last_of('.'));
		return str;
	}

	void search(std::vector<String>& list) {
		String path = this->identifier->getAbsolutePath();
		if (FileHelper::isExisted(path)) {
			list.push_back(getRelativePath(path));
		}
		if (FileHelper::isExisted(FileHelper::appendFilename(path, "1"))) {
			int i = 1;
			while (FileHelper::isExisted(FileHelper::appendFilename(path, std::to_string(i)))) {
				list.push_back(getRelativePath(path) + std::to_string(i));
				++i;
			}
		}
	}
};

#endif //MINECRAFT_SOUNDEVENT_HPP
