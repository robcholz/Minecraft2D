//
// Created by robcholz on 3/31/23.
//

#ifndef RUNCRAFT_MENU_HPP
#define RUNCRAFT_MENU_HPP

#pragma once

#include "util/Utils.hpp"
#include "client/GameInfo.hpp"
#include "Initializers/ConsoleInitializer.h"
#include "client/gui/screen/Background.hpp"
#include "client/gui/widget/ButtonWidget.hpp"
#include "client/gui/widget/TexturedButtonWidget.hpp"
#include "client/gui/widget/SoundSliderWidget.hpp"
#include "client/gui/widget/TextFieldWidget.hpp"
#include "client/gui/screen/ScreenManager.hpp"
#include "sound/Audio.hpp"
#include "client/scene/SceneAccess.hpp"

class Menu : public SceneAccess {
protected:
	void onInitialize() {
		backgroundTexture.fitToScreen();
		settingBackground.composeToScreen();
		initAudio();
		initWidget();
		backgroundMusic.playRandomly();

		PLOG_DEBUG << "Initialize assets.";
	}

	void initAudio() {
		backgroundMusic.addAudio("oxygène")
		               .addAudio("beginning")
		               .addAudio("beginning_2")
		               .addAudio("moog_city_2")
		               .addAudio("floating_trees")
		               .addAudio("mutation");

		PLOG_DEBUG << "Initialize audio assets";
	}

	void initWidget() {
		screenManager.addScreen(&backgroundMenuScreen)
		             .addScreen(&backgroundBiomeSettingScreen)
		             .addScreen(&singleplayerSettingScreen)
		             .addScreen(&settingVolumeScreen)
		             .setEntry(&backgroundMenuScreen);

		backgroundMenuScreen.addCallbackScreen(&backgroundBiomeSettingScreen, &backgroundMenuOptions)
		                    .addCallbackScreen(&singleplayerSettingScreen, &backgroundMenuSinglePlayer)
		                    .addWidget(&backgroundMenuOptions)
		                    .addWidget(&backgroundMenuSinglePlayer)
		                    .addWidget(&backgroundMenuLanguage)
		                    .addWidget(&backgroundMenuWhat)
		                    .addWidget(&backgroundMenuLanguageTexturedButton)
		                    .addWidget(&backgroundMenuQuitGame);
		backgroundMenuQuitGame.executeFuncWhenActivated([] {
			PLOG_DEBUG << "Cancel runcraft!";
			GameInfo.getRender()->getWindowConfig().window->close();
		});

		settingVolumeScreen.addCallbackScreen(&backgroundBiomeSettingScreen, &settingVolumeBack)
		                   .addWidget(&settingVolumeSliderTitle)
		                   .addWidget(&settingVolumeSliderAmbientEnvironment)
		                   .addWidget(&settingVolumeSliderBlocks)
		                   .addWidget(&settingVolumeSliderFriendlyCreatures)
		                   .addWidget(&settingVolumeSliderHostileCreatures)
		                   .addWidget(&settingVolumeSliderJukeboxNoteblocks)
		                   .addWidget(&settingVolumeSliderMasterVolume)
		                   .addWidget(&settingVolumeSliderMusic)
		                   .addWidget(&settingVolumeSliderPlayers)
		                   .addWidget(&settingVolumeSliderWeather)
		                   .addWidget(&settingVolumeBack);
		settingVolumeSliderAmbientEnvironment.varToChangeWhenMoved(&GameInfo.getInternalData()->soundLevel.ambientAndEnvironment);
		settingVolumeSliderBlocks.varToChangeWhenMoved(&GameInfo.getInternalData()->soundLevel.blocks);
		settingVolumeSliderFriendlyCreatures.varToChangeWhenMoved(&GameInfo.getInternalData()->soundLevel.friendlyCreatures);
		settingVolumeSliderHostileCreatures.varToChangeWhenMoved(&GameInfo.getInternalData()->soundLevel.hostileCreatures);
		settingVolumeSliderJukeboxNoteblocks.varToChangeWhenMoved(&GameInfo.getInternalData()->soundLevel.jukeBoxOrNoteblocks);
		settingVolumeSliderMasterVolume.varToChangeWhenMoved(&GameInfo.getInternalData()->soundLevel.masterVolume);
		settingVolumeSliderMusic.varToChangeWhenMoved(&GameInfo.getInternalData()->soundLevel.music)
		                        .executeFuncWhenActivated([this]() { backgroundMusic.changeVolume((GameInfo.getConstInternalData()->soundLevel.music)); });
		settingVolumeSliderPlayers.varToChangeWhenMoved(&GameInfo.getInternalData()->soundLevel.players);
		settingVolumeSliderWeather.varToChangeWhenMoved(&GameInfo.getInternalData()->soundLevel.weather);

		singleplayerSettingScreen.addCallbackScreen(&backgroundMenuScreen, &singleplayerSettingBack)
		                         .addWidget(&singleplayerGenerateWorld)
		                         .addWidget(&singleplayerSettingBack);
		singleplayerGenerateWorld.executeFuncWhenActivated([this] { terminateScene(); });

		backgroundBiomeSettingScreen.addCallbackScreen(&backgroundMenuScreen, &backgroundSettingBack)
		                            .addCallbackScreen(&settingVolumeScreen, &backgroundSettingVolume)
		                            .addWidget(&backgroundSettingBack)
		                            .addWidget(&backgroundSettingBiomeSnowyPlains)
		                            .addWidget(&backgroundSettingBiomePlains)
		                            .addWidget(&backgroundSettingBiomeForest)
		                            .addWidget(&backgroundSettingBiomeBirchForest)
		                            .addWidget(&backgroundSettingBiomeDesert)
		                            .addWidget(&backgroundSettingBiomeJungle)
		                            .addWidget(&backgroundSettingVolume);

		backgroundSettingBiomeSnowyPlains.executeFuncWhenActivated([] { GameInfo.getInternalData()->biome = internal_data::Biome::SNOWY_PLAINS; });
		backgroundSettingBiomePlains.executeFuncWhenActivated([] { GameInfo.getInternalData()->biome = internal_data::Biome::PLAINS; });
		backgroundSettingBiomeForest.executeFuncWhenActivated([] { GameInfo.getInternalData()->biome = internal_data::Biome::FOREST; });
		backgroundSettingBiomeBirchForest.executeFuncWhenActivated([] { GameInfo.getInternalData()->biome = internal_data::Biome::BIRCH_FOREST; });
		backgroundSettingBiomeDesert.executeFuncWhenActivated([] { GameInfo.getInternalData()->biome = internal_data::Biome::DESERT; });
		backgroundSettingBiomeJungle.executeFuncWhenActivated([] { GameInfo.getInternalData()->biome = internal_data::JUNGLE; });

		PLOG_DEBUG << "Initialize widget components";
	}

private:
	int screenWidth = (int) GameInfo.getConstExternalData()->windowState.getScreenWidth();
	int screenHeight = (int) GameInfo.getConstExternalData()->windowState.getScreenHeight();

	AudioList backgroundMusic;

	Background backgroundTexture{"background.png"};
	Background settingBackground{"options_background.png"};

	/*main menu*/
	ButtonWidget backgroundMenuSinglePlayer{"singleplayer", 800, 80, true, screenWidth / 2 - 800 / 2, 432};
	ButtonWidget backgroundMenuOptions{"options", 800, 80, true, screenWidth / 2 - 800 / 2, 528};
	ButtonWidget backgroundMenuWhat{"something_uncertain", 800, 80, true, screenWidth / 2 - 800 / 2, 624};
	ButtonWidget backgroundMenuLanguage{"language", 380, 80, true, screenWidth / 2 - 800 / 2, 768};
	ButtonWidget backgroundMenuQuitGame{"quit_game", 380, 80, true, screenWidth / 2 + 800 / 2 - 380, 768};
	TexturedButtonWidget backgroundMenuLanguageTexturedButton{32, 32, true, screenWidth / 2 - 500, 768};
	Screen backgroundMenuScreen{&backgroundTexture};

	/*settings*/
	ButtonWidget backgroundSettingBiomeSnowyPlains{"snowy_plains", 590, 80, true, screenWidth / 2 - 1200 / 2, 400};
	ButtonWidget backgroundSettingBiomePlains{"plains", 590, 80, true, screenWidth / 2 + 1200 / 2 - 590, 400};
	ButtonWidget backgroundSettingBiomeForest{"forest", 590, 80, true, screenWidth / 2 - 1200 / 2, 500};
	ButtonWidget backgroundSettingBiomeDesert{"desert", 590, 80, true, screenWidth / 2 + 1200 / 2 - 590, 500};
	ButtonWidget backgroundSettingBiomeBirchForest{"birch_forest", 590, 80, true, screenWidth / 2 - 1200 / 2, 600};
	ButtonWidget backgroundSettingBiomeJungle{"jungle", 590, 80, true, screenWidth / 2 + 1200 / 2 - 590, 600};
	ButtonWidget backgroundSettingVolume{"music", 1200, 80, true, screenWidth / 2 - 1200 / 2, 200};
	ButtonWidget backgroundSettingBack{"done", 790, 80, true, screenWidth / 2 - 780 / 2, screenHeight - 80 - 50};
	Screen backgroundBiomeSettingScreen{&backgroundTexture};

	/*music settings*/
	TextFieldWidget settingVolumeSliderTitle{"music_sound_options", 70, true, screenWidth / 2 - 440 / 2, 20};
	SoundSliderWidget settingVolumeSliderMasterVolume{"master_volume", 1240, 80, true, screenWidth / 2 - 620, 115};
	SoundSliderWidget settingVolumeSliderMusic{"music_volume", 590, 80, true, screenWidth / 2 - 620, 225};
	SoundSliderWidget settingVolumeSliderJukeboxNoteblocks{"jukebox_noteblocks_volume", 590, 80, true, screenWidth / 2 + 30, 225};
	SoundSliderWidget settingVolumeSliderWeather{"weather_volume", 590, 80, true, screenWidth / 2 - 620, 335};
	SoundSliderWidget settingVolumeSliderBlocks{"blocks_volume", 36, 590, 80, true, screenWidth / 2 + 30, 335};
	SoundSliderWidget settingVolumeSliderHostileCreatures{"hostile_creatures_volume", 590, 80, true, screenWidth / 2 - 620, 445};
	SoundSliderWidget settingVolumeSliderFriendlyCreatures{"friendly_creatures_volume", 590, 80, true, screenWidth / 2 + 30, 445};
	SoundSliderWidget settingVolumeSliderPlayers{"players_volume", 590, 80, true, screenWidth / 2 - 620, 555};
	SoundSliderWidget settingVolumeSliderAmbientEnvironment{"ambient_environment_volume", 590, 80, true, screenWidth / 2 + 30, 555};
	ButtonWidget settingVolumeBack{"done", 790, 80, true, screenWidth / 2 - 390, 555 + 260};
	Screen settingVolumeScreen{&settingBackground};

	/*singleplayer page*/
	ButtonWidget singleplayerGenerateWorld{"generate_experimental_world", 800, 80, true, screenWidth / 2 - 800 / 2, screenHeight / 2 + 80};
	ButtonWidget singleplayerSettingBack{"done", 790, 80, true, screenWidth / 2 - 790 / 2, screenHeight - 80 - 50};
	Screen singleplayerSettingScreen{&settingBackground};

	ScreenManager screenManager;
public:
	explicit Menu() {
		PLOG_DEBUG << "Menu started!";
		onInitialize();
	}

    ~Menu() override {
		backgroundMusic.stop();
	}

	void onUpdate() override {

	}

	void onRender() override {
		screenManager.render();
	}
};

#endif //RUNCRAFT_MENU_HPP
