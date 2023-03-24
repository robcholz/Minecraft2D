//
// Created by robcholz on 3/9/23.
//

#ifndef RUNCRAFT_AUDIO_HPP
#define RUNCRAFT_AUDIO_HPP

#include <string>
#include <iostream>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include "../util/Math_Helper.hpp"
#include "Log.h"

class Audio {
protected:
	// sound player
	sf::Sound sound;
	sf::SoundBuffer soundBuffer;
	// file path
	std::string audioPath;
	const std::string audioFileType = ".ogg";

public:
	bool isPlaying() {
		return sound.getStatus() == sf::SoundSource::Status::Playing;
	}

	void pause() { sound.pause(); }

	void stop() {
		if (isPlaying())
			sound.stop();
	}

	void play() {
		sound.play();
	}
};

class AudioList : public Audio {
private:
	sf::SoundBuffer soundBuffer;

	// where all the sounds stored here
	std::vector<std::string> playlist;

	void load(int n) {
		soundBuffer.loadFromFile(audioPath + playlist.at(n) + audioFileType);
	}

public:
	/**
	 * @param folderPath the path of the audio stored in the resources/sound/ directory.
	 * \n Default is ../resources/sound
	 */
	explicit AudioList(const std::string &folderPath = "../resources/sound/") {
		audioPath = folderPath;
	};

	~AudioList() {
		std::destroy(playlist.begin(), playlist.end());
	}

	/**
	 * @param filename the name of the .ogg file(doesn't contain file format)
	 * @return AudioList&
	 */
	AudioList &addAudio(const std::string &filename) {
		playlist.push_back(filename);
		sound.setBuffer(soundBuffer);
		if (playlist.size() == 1)
			load(0);
		return *this;
	}

	/**
	 * @brief randomly play a sound from the soundList
	 */
	void playRandomly() {
		stop();
		load(Random(0, (int) playlist.size()));
		Audio::play();
	}

	/**
	 * @brief play a sound by its name
	 * @param filename sound name
	 */
	void play(const std::string &filename) {
		for (auto &obj: playlist) {
			if (filename == obj) {
				stop();
				soundBuffer.loadFromFile(audioPath + filename + audioFileType);
				Audio::play();
			}
		}
	}
};

class AudioPlayer : public Audio {
private:
	std::string filename = "effect/gui_button_click";
public:
	AudioPlayer() {
		audioPath = "../resources/sound/" + filename + audioFileType;
		addAudio();
	}

	explicit AudioPlayer(const std::string &filename) {
		audioPath = "../resources/sound/" + filename + audioFileType;
		addAudio();
	};

	~AudioPlayer() = default;

	/**
	 * @brief add a single sound
	 * @return
	 */
	[[maybe_unused]] AudioPlayer &addAudio() {
		if (!soundBuffer.loadFromFile(audioPath)) {
			std::cout << "CANNOT load .ogg file from the directory " + audioPath
			          << " check the path again!"
			          << std::endl;
		}
		sound.setBuffer(soundBuffer);
		return *this;
	}
};

#endif //RUNCRAFT_AUDIO_HPP
