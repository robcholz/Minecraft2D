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
	using SoundEventPtr = std::shared_ptr<SoundEvent>;
	using MusicSoundEventPtr = std::shared_ptr<MusicSoundEvent>;
public:
	MusicSoundEventPtr BACKGROUND_MUSIC_1, BACKGROUND_MUSIC_2, BACKGROUND_MUSIC_3, BACKGROUND_MUSIC_4, BACKGROUND_MUSIC_5, BACKGROUND_MUSIC_6;
	SoundEventPtr STEP_SOUND_GRASS_1, STEP_SOUND_GRASS_2, STEP_SOUND_GRASS_3, STEP_SOUND_GRASS_4, STEP_SOUND_GRASS_5, STEP_SOUND_GRASS_6;
	SoundEventPtr STEP_SOUND_STONE_1, STEP_SOUND_STONE_2, STEP_SOUND_STONE_3, STEP_SOUND_STONE_4, STEP_SOUND_STONE_5, STEP_SOUND_STONE_6;
	SoundEventPtr GUI_CLICK_SOUND_1;

	SoundEvents(const SoundEvents&) = delete;
	void operator=(const SoundEvents&) = delete;

	static SoundEvents& getInstance(){
		static SoundEvents sound_events;
		return sound_events;
	}

private:
	SoundEvents() {
		STEP_SOUND_GRASS_1 = registerSoundEvent("step.grass1");
		STEP_SOUND_GRASS_2 = registerSoundEvent("step.grass2");
		STEP_SOUND_GRASS_3 = registerSoundEvent("step.grass3");
		STEP_SOUND_GRASS_4 = registerSoundEvent("step.grass4");
		STEP_SOUND_GRASS_5 = registerSoundEvent("step.grass5");
		STEP_SOUND_GRASS_6 = registerSoundEvent("step.grass6");

		STEP_SOUND_STONE_1 = registerSoundEvent("step.stone1");
		STEP_SOUND_STONE_2 = registerSoundEvent("step.stone2");
		STEP_SOUND_STONE_3 = registerSoundEvent("step.stone3");
		STEP_SOUND_STONE_4 = registerSoundEvent("step.stone4");
		STEP_SOUND_STONE_5 = registerSoundEvent("step.stone5");
		STEP_SOUND_STONE_6 = registerSoundEvent("step.stone6");

		GUI_CLICK_SOUND_1= registerSoundEvent("effect.gui_button_click");

		BACKGROUND_MUSIC_1 = registerMusicSoundEvent("beginning");
		BACKGROUND_MUSIC_2 = registerMusicSoundEvent("beginning_2");
		BACKGROUND_MUSIC_3 = registerMusicSoundEvent("floating_trees");
		BACKGROUND_MUSIC_4 = registerMusicSoundEvent("moog_city_2");
		BACKGROUND_MUSIC_5 = registerMusicSoundEvent("mutation");
		BACKGROUND_MUSIC_6 = registerMusicSoundEvent("oxygene");
	}

	static SoundEventPtr registerSoundEvent(const std::string& id) {
		return std::make_shared<SoundEvent>(Identifier(id, Identifier::Category::SOUND));
	}

	static MusicSoundEventPtr registerMusicSoundEvent(const std::string& id) {
		return std::make_shared<MusicSoundEvent>(SoundEvent(Identifier(id, Identifier::Category::SOUND)), 0.5f, 1.0f);
	}
};

#endif //RUNCRAFT_SOUNDEVENTS_HPP
