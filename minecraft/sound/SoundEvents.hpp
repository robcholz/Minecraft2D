//
// Created by robcholz on 6/26/23.
//

#ifndef MINECRAFT_SOUNDEVENTS_HPP
#define MINECRAFT_SOUNDEVENTS_HPP

#include "SoundEvent.hpp"
#include "util/Identifier.hpp"
#include "MusicSoundEvent.hpp"


class SoundEvents{
private:
	using SoundEventPtr = SoundEvent::SoundEventPtr;
	using MusicSoundEventPtr = MusicSoundEvent::MusicSoundEventPtr;
	using String = std::string;
public:
	MusicSoundEventPtr MUSIC_MENU;
	MusicSoundEventPtr MUSIC_HAL;
	MusicSoundEventPtr MUSIC_NUANCE;
	SoundEventPtr STEP_SOUND_GRASS;
	SoundEventPtr STEP_SOUND_STONE;
	SoundEventPtr STEP_SOUND_BEDROCK;
	SoundEventPtr CLICK_SOUND_GUI;

	SoundEvents(const SoundEvents&) = delete;

	void operator=(const SoundEvents&) = delete;

	/**
	 * @brief singleton here. this is thread-safe since c++11.
	 * @return unique instance
	 */
	static SoundEvents& getInstance() {
		static SoundEvents sound_events;
		return sound_events;
	}

private:
	// TODO need to implement an essential sound pool here to save mm
	std::list<SoundEventPtr> soundEventsRegistryList;
	std::list<MusicSoundEventPtr> musicSoundEventsRegistryList;

	SoundEvents() {
		STEP_SOUND_GRASS = registerSoundEvent("step.grass");
		STEP_SOUND_STONE = registerSoundEvent("step.stone");
		STEP_SOUND_BEDROCK = registerSoundEvent("step.grass");
		CLICK_SOUND_GUI = registerSoundEvent("effect.gui_button_click");

		MUSIC_MENU = registerMusicSoundEvent("menu.menu");
		MUSIC_HAL = registerMusicSoundEvent("game.hal");
		MUSIC_NUANCE = registerMusicSoundEvent("game.nuance");
	}

	~SoundEvents() {
		for (auto v: soundEventsRegistryList)
			delete v;
		for (auto v: musicSoundEventsRegistryList)
			delete v;
	}

	SoundEventPtr registerSoundEvent(const String& id) {
		auto sound_event = new SoundEvent(Identifier(id, Identifier::Category::SOUND));
		soundEventsRegistryList.push_back(sound_event);
		return sound_event;
	}

	MusicSoundEventPtr registerMusicSoundEvent(const String& id) {
		auto music_sound_event = new MusicSoundEvent(SoundEvent(Identifier("music." + id, Identifier::Category::SOUND)), 1, 1.0f);
		musicSoundEventsRegistryList.push_back(music_sound_event);
		return music_sound_event;
	}
};

#endif //MINECRAFT_SOUNDEVENTS_HPP
