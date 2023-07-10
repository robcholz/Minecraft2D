//
// Created by robcholz on 6/26/23.
//

#ifndef RUNCRAFT_SOUNDMANAGER_HPP
#define RUNCRAFT_SOUNDMANAGER_HPP

#include <queue>
#include <SFML/Audio/Sound.hpp>
#include <utility>
#include <memory>
#include "MusicSoundEvent.hpp"
#include "SoundEvent.hpp"

class SoundManager {
protected:
public:
	SoundManager()=default;
	~SoundManager() {
		musicSoundEventPlayer.stop();
		soundEventPlayer.stop();
	}

	SoundManager& addSound(std::shared_ptr<MusicSoundEvent> musicSoundEvent) {
		musicSoundEventQueue.push(std::shared_ptr<MusicSoundEvent>(std::move(musicSoundEvent)));
		return *this;
	}

	SoundManager& addSound(std::shared_ptr<SoundEvent> soundEvent) {
		if (soundEvent.get() != soundEventCurrent)
			soundEventQueue.push(std::shared_ptr<SoundEvent>(std::move(soundEvent)));
		return *this;
	}

	void stopCurrentPlaying() {
		musicSoundEventPlayer.stop();
		soundEventPlayer.stop();
	}

	void update() {
		updateMusicSoundEvent();
		updateSoundEvent();
	}

private:
	std::priority_queue<std::shared_ptr<MusicSoundEvent>> musicSoundEventQueue;
	std::priority_queue<std::shared_ptr<SoundEvent>> soundEventQueue;
	MusicSoundEvent* musicSoundEventCurrent = nullptr;
	SoundEvent* soundEventCurrent = nullptr;
	sf::Sound musicSoundEventPlayer;
	sf::Sound soundEventPlayer;

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
			musicSoundEventCurrent = musicSoundEventQueue.top().get();
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
			soundEventCurrent = soundEventQueue.top().get();
			soundEventPlayer.setBuffer(soundEventCurrent->getSound());
			soundEventPlayer.play();
		}
	}
};

#endif //RUNCRAFT_SOUNDMANAGER_HPP
