//
// Created by robcholz on 6/26/23.
//

#ifndef MINECRAFT_SOUNDMANAGER_HPP
#define MINECRAFT_SOUNDMANAGER_HPP

#include <queue>
#include <SFML/Audio/Sound.hpp>
#include <utility>
#include <memory>
#include "MusicSoundEvent.hpp"
#include "SoundEvents.hpp"

class SoundManager {
public:
	using SoundVolume = unsigned char;
	enum Sound {
		MASTER,
		MUSIC,
		JUKEBOX,
		WEATHER,
		BLOCKS,
		HOSTILE_CREATURES,
		FRIENDLY_CREATURES,
		PLAYERS,
		ENVIRONMENT
	};

	SoundManager() = default;

	~SoundManager() {
		musicSoundEventPlayer.stop();
		soundEventPlayer.stop();
	}

	SoundManager& addSound(MusicSoundEvent::MusicSoundEventPtr musicSoundEvent) {
		musicSoundEventQueue.push(musicSoundEvent);
		return *this;
	}

	SoundManager& addSound(SoundEvent::SoundEventPtr soundEvent) {
		if (soundEvent != soundEventCurrent)
			soundEventQueue.push(soundEvent);
		return *this;
	}

	// using std::optional<> here since some of the blocks might not have yet bound to a sound object
	SoundManager& addSound(std::optional<SoundEvent::SoundEventPtr> soundEvent) {
		if (soundEvent == std::nullopt)
			return *this;
		if (soundEvent != soundEventCurrent)
			soundEventQueue.push(soundEvent.value());
		return *this;
	}

	void clearMusicSoundEventQueue() {
		musicSoundEventPlayer.stop();
		while (!musicSoundEventQueue.empty())
			musicSoundEventQueue.pop();
		musicSoundEventPlayer.resetBuffer();
		musicSoundEventCurrent = nullptr;
	}

	void clearSoundEventQueue() {
		soundEventPlayer.stop();
		while (!soundEventQueue.empty())
			soundEventQueue.pop();
		soundEventPlayer.resetBuffer();
		soundEventCurrent = nullptr;
	}

	void stopCurrentPlaying() {
		musicSoundEventPlayer.stop();
		soundEventPlayer.stop();
	}

	void setVolume(Sound sound, SoundVolume volume) {
		updateSignalQueue.push(true);
		switch (sound) {
			case MASTER:
				soundLevel.masterVolume = volume;
				break;
			case MUSIC:
				soundLevel.music = volume;
				break;
			case JUKEBOX:
				soundLevel.jukeBoxOrNoteblocks = volume;
				break;
			case WEATHER:
				soundLevel.weather = volume;
				break;
			case BLOCKS:
				soundLevel.blocks = volume;
				break;
			case HOSTILE_CREATURES:
				soundLevel.hostileCreatures = volume;
				break;
			case FRIENDLY_CREATURES:
				soundLevel.friendlyCreatures = volume;
				break;
			case PLAYERS:
				soundLevel.players = volume;
				break;
			case ENVIRONMENT:
				soundLevel.ambientAndEnvironment = volume;
				break;
		}
	}

	void update() {
		updateSoundSettings();
		updateMusicSoundEvent();
		updateSoundEvent();
	}

private:
	std::priority_queue<MusicSoundEvent*> musicSoundEventQueue;
	std::priority_queue<SoundEvent*> soundEventQueue;
	std::queue<bool> updateSignalQueue;
	MusicSoundEvent* musicSoundEventCurrent = nullptr;
	SoundEvent* soundEventCurrent = nullptr;
	sf::Sound musicSoundEventPlayer;
	sf::Sound soundEventPlayer;
	struct SoundLevel {
		SoundVolume masterVolume = 100;
		SoundVolume music = 100, jukeBoxOrNoteblocks = 100;
		SoundVolume weather = 100, blocks = 36;
		SoundVolume hostileCreatures = 100, friendlyCreatures = 100;
		SoundVolume players = 100, ambientAndEnvironment = 100;
	} soundLevel{};

	void updateSoundSettings() {
		if (updateSignalQueue.empty())
			return;
		musicSoundEventPlayer.setVolume((float) soundLevel.masterVolume * (float) soundLevel.music / 100);
		soundEventPlayer.setVolume(soundLevel.masterVolume);
		updateSignalQueue.pop();
	}

	void updateMusicSoundEvent() {
		if (musicSoundEventCurrent != nullptr && musicSoundEventPlayer.getStatus() == sf::Sound::Status::Stopped) {
			musicSoundEventQueue.pop();
			if (musicSoundEventQueue.empty()) {
				musicSoundEventCurrent = nullptr;
				return;
			}
		}
		if (musicSoundEventPlayer.getStatus() == sf::Sound::Status::Stopped) {
			if (musicSoundEventQueue.empty()) return;
			musicSoundEventQueue.top()->getSoundEvent().loadSound();
			musicSoundEventCurrent = musicSoundEventQueue.top();
			musicSoundEventPlayer.setBuffer(musicSoundEventCurrent->getSoundEvent().getSound());
			musicSoundEventPlayer.play();
			PLOG_DEBUG << "Current playing: " << musicSoundEventCurrent->getSoundEvent().getID().toString();
		}
	}

	void updateSoundEvent() {
		if (soundEventCurrent != nullptr && soundEventPlayer.getStatus() == sf::Sound::Status::Stopped) {
			soundEventQueue.pop();
			if (soundEventQueue.empty()) {
				soundEventCurrent = nullptr;
				return;
			}
		}
		if (soundEventPlayer.getStatus() == sf::Sound::Status::Stopped) {
			if (soundEventQueue.empty()) return;
			soundEventQueue.top()->loadSound();
			soundEventCurrent = soundEventQueue.top();
			soundEventPlayer.setBuffer(soundEventCurrent->getSound());
			soundEventPlayer.play();
		}
	}
};

#endif //MINECRAFT_SOUNDMANAGER_HPP
