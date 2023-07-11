
//
// Created by robcholz on 6/9/23.
//

#ifndef RUNCRAFT_SOUNDEVENT_HPP
#define RUNCRAFT_SOUNDEVENT_HPP

#include <SFML/Audio/SoundBuffer.hpp>
#include "util/Identifier.hpp"
#include "util/math/Random.hpp"
#include "resource/FileHelper.hpp"

class SoundEvent {
private:
	using String = std::string;
public:
	using SoundEventPtr = std::shared_ptr<SoundEvent>;

	explicit SoundEvent(const Identifier& id) {
		this->identifier = std::make_unique<Identifier>(id);
		search(soundsList);
	}

	SoundEvent(const SoundEvent& soundEvent) {
		this->identifier = std::make_unique<Identifier>(*soundEvent.identifier);
		this->soundBuffer = soundEvent.soundBuffer;
		this->soundsList = soundEvent.soundsList;
	}

	~SoundEvent() = default;

	void loadSound() {
		soundBuffer.loadFromFile(Identifier(Random::randomElement(soundsList), Identifier::Category::SOUND).getAbsolutePath());
	}

	sf::SoundBuffer& getSound() { return soundBuffer; }

	Identifier& getID() { return *identifier; }

private:
	std::unique_ptr<Identifier> identifier;
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
		if (FileHelper::isExisted(FileHelper::appendFilename(this->identifier->getAbsolutePath(), "1"))) {
			int i = 1;
			while (FileHelper::isExisted(FileHelper::appendFilename(this->identifier->getAbsolutePath(), std::to_string(i)))) {
				list.push_back(getRelativePath(path) + std::to_string(i));
				++i;
			}
		}
	}
};

#endif //RUNCRAFT_SOUNDEVENT_HPP
