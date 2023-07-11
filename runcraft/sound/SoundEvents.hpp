//
// Created by robcholz on 6/26/23.
//

#ifndef RUNCRAFT_SOUNDEVENTS_HPP
#define RUNCRAFT_SOUNDEVENTS_HPP

#include "SoundEvent.hpp"
#include "util/Identifier.hpp"
#include "MusicSoundEvent.hpp"


class SoundEvents {
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
	SoundEventPtr CLICK_SOUND_GUI;

	SoundEvents(const SoundEvents&) = delete;

	void operator=(const SoundEvents&) = delete;

	static SoundEvents& getInstance() {
		static SoundEvents sound_events;
		return sound_events;
	}

private:
	SoundEvents() {
		STEP_SOUND_GRASS = registerSoundEvent("step.grass");
		STEP_SOUND_STONE = registerSoundEvent("step.stone");
		CLICK_SOUND_GUI = registerSoundEvent("effect.gui_button_click");

		MUSIC_MENU = registerMusicSoundEvent("menu.menu");
		MUSIC_HAL = registerMusicSoundEvent("game.hal");
		MUSIC_NUANCE = registerMusicSoundEvent("game.nuance");
	}

	static SoundEventPtr registerSoundEvent(const String& id) {
		return std::make_shared<SoundEvent>(Identifier(id, Identifier::Category::SOUND));
	}

	static MusicSoundEventPtr registerMusicSoundEvent(const String& id) {
		return std::make_shared<MusicSoundEvent>(SoundEvent(Identifier("music." + id, Identifier::Category::SOUND)), 0.5f, 1.0f);
	}
};

#endif //RUNCRAFT_SOUNDEVENTS_HPP
