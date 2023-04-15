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

class Menu {
private:
	int screenWidth = (int) GameInfo.getScreenWidth();
	int screenHeight = (int) GameInfo.getScreenHeight();
	AudioList backgroundMusic;
	Once backgroundOnce;

	Background *backgroundTexture = new Background("background.png");
	Background *settingBackground = new Background("options_background.png");

	/*main menu*/
	ButtonWidget *backgroundMenuSinglePlayer = new ButtonWidget("singleplayer", 800, 80, true, screenWidth / 2 - 800 / 2, 432);
	ButtonWidget *backgroundMenuOptions = new ButtonWidget("options", 800, 80, true, screenWidth / 2 - 800 / 2, 528);
	ButtonWidget *backgroundMenuWhat = new ButtonWidget("something_uncertain", 800, 80, true, screenWidth / 2 - 800 / 2, 624);
	ButtonWidget *backgroundMenuLanguage = new ButtonWidget("language", 380, 80, true, screenWidth / 2 - 800 / 2, 768);
	ButtonWidget *backgroundMenuQuitGame = new ButtonWidget("quit_game", 380, 80, true, screenWidth / 2 + 800 / 2 - 380, 768);
	TexturedButtonWidget *backgroundMenuLanguageTexturedButton = new TexturedButtonWidget(32, 32, true, screenWidth / 2 - 500, 768);
	Screen *backgroundMenuScreen = new Screen(backgroundTexture);

	/*settings*/
	ButtonWidget *backgroundSettingBiomeSnowyPlains = new ButtonWidget("snowy_plains", 590, 80, true, screenWidth / 2 - 1200 / 2, 400);
	ButtonWidget *backgroundSettingBiomePlains = new ButtonWidget("plains", 590, 80, true, screenWidth / 2 + 1200 / 2 - 590, 400);
	ButtonWidget *backgroundSettingBiomeForest = new ButtonWidget("forest", 590, 80, true, screenWidth / 2 - 1200 / 2, 500);
	ButtonWidget *backgroundSettingBiomeDesert = new ButtonWidget("desert", 590, 80, true, screenWidth / 2 + 1200 / 2 - 590, 500);
	ButtonWidget *backgroundSettingBiomeBirchForest = new ButtonWidget("birch_forest", 590, 80, true, screenWidth / 2 - 1200 / 2, 600);
	ButtonWidget *backgroundSettingBiomeJungle = new ButtonWidget("jungle", 590, 80, true, screenWidth / 2 + 1200 / 2 - 590, 600);
	ButtonWidget *backgroundSettingVolume = new ButtonWidget("music", 1200, 80, true, screenWidth / 2 - 1200 / 2, 200);
	ButtonWidget *backgroundSettingBack = new ButtonWidget("done", 790, 80, true, screenWidth / 2 - 780 / 2, screenHeight - 80 - 50);
	Screen *backgroundBiomeSettingScreen = new Screen(backgroundTexture);

	/*music settings*/
	TextFieldWidget *settingVolumeSliderTitle = new TextFieldWidget("music_sound_options", 70, true, screenWidth / 2 - 440 / 2, 20);
	SoundSliderWidget *settingVolumeSliderMasterVolume = new SoundSliderWidget("master_volume", 1240, 80, true, screenWidth / 2 - 620, 115);
	SoundSliderWidget *settingVolumeSliderMusic = new SoundSliderWidget("music_volume", 590, 80, true, screenWidth / 2 - 620, 225);
	SoundSliderWidget *settingVolumeSliderJukeboxNoteblocks = new SoundSliderWidget("jukebox_noteblocks_volume", 590, 80, true, screenWidth / 2 + 30, 225);
	SoundSliderWidget *settingVolumeSliderWeather = new SoundSliderWidget("weather_volume", 590, 80, true, screenWidth / 2 - 620, 335);
	SoundSliderWidget *settingVolumeSliderBlocks = new SoundSliderWidget("blocks_volume", 36, 590, 80, true, screenWidth / 2 + 30, 335);
	SoundSliderWidget *settingVolumeSliderHostileCreatures = new SoundSliderWidget("hostile_creatures_volume", 590, 80, true, screenWidth / 2 - 620, 445);
	SoundSliderWidget *settingVolumeSliderFriendlyCreatures = new SoundSliderWidget("friendly_creatures_volume", 590, 80, true, screenWidth / 2 + 30, 445);
	SoundSliderWidget *settingVolumeSliderPlayers = new SoundSliderWidget("players_volume", 590, 80, true, screenWidth / 2 - 620, 555);
	SoundSliderWidget *settingVolumeSliderAmbientEnvironment = new SoundSliderWidget("ambient_environment_volume", 590, 80, true, screenWidth / 2 + 30, 555);
	ButtonWidget *settingVolumeBack = new ButtonWidget("done", 790, 80, true, screenWidth / 2 - 390, 555 + 260);
	Screen *settingVolumeScreen = new Screen(settingBackground);

	/*singleplayer page*/
	ButtonWidget *singleplayerGenerateWorld = new ButtonWidget("generate_experimental_world", 800, 80, true, screenWidth / 2 - 800 / 2, screenHeight / 2 + 80);
	ButtonWidget *singleplayerSettingBack = new ButtonWidget("done", 790, 80, true, screenWidth / 2 - 790 / 2, screenHeight - 80 - 50);
	Screen *singleplayerSettingScreen = new Screen(settingBackground);

	ScreenManager screenManager;
public:
	explicit Menu() {
		PLOG_DEBUG << "Menu started!";
		initResource();
	}

	~Menu() {
		delete backgroundTexture;
		delete settingBackground;

		delete backgroundMenuSinglePlayer;
		delete backgroundMenuOptions;
		delete backgroundMenuWhat;
		delete backgroundMenuLanguage;
		delete backgroundMenuQuitGame;
		delete backgroundMenuLanguageTexturedButton;
		delete backgroundMenuScreen;

		delete backgroundSettingBiomeSnowyPlains;
		delete backgroundSettingBiomePlains;
		delete backgroundSettingBiomeForest;
		delete backgroundSettingBiomeDesert;
		delete backgroundSettingBiomeBirchForest;
		delete backgroundSettingBiomeJungle;
		delete backgroundSettingVolume;
		delete backgroundSettingBack;
		delete backgroundBiomeSettingScreen;

		/*music settings*/
		delete settingVolumeSliderTitle;
		delete settingVolumeSliderMasterVolume;
		delete settingVolumeSliderMusic;
		delete settingVolumeSliderJukeboxNoteblocks;
		delete settingVolumeSliderWeather;
		delete settingVolumeSliderBlocks;
		delete settingVolumeSliderHostileCreatures;
		delete settingVolumeSliderFriendlyCreatures;
		delete settingVolumeSliderPlayers;
		delete settingVolumeSliderAmbientEnvironment;
		delete settingVolumeBack;
		delete settingVolumeScreen;

		/*singleplayer page*/
		delete singleplayerSettingBack;
		delete singleplayerSettingScreen;
	}

	void initResource() {
		initAudio();
		initWidget();

		PLOG_DEBUG << "Initialize assets.";
	}

	void renderMainMenu() {
		if (backgroundOnce.runOnce()) {
			backgroundTexture->fitToScreen();
			settingBackground->composeToScreen();
			backgroundMusic.playRandomly();
			PLOG_DEBUG << "Rendered main menu!";
		}

		screenManager.setInputStatePtr(GameInfo.getInputState());
		screenManager.render();
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
		screenManager.addScreen(backgroundMenuScreen)
				.addScreen(backgroundBiomeSettingScreen)
				.addScreen(singleplayerSettingScreen)
				.addScreen(settingVolumeScreen)
				.setEntry(backgroundMenuScreen);

		backgroundMenuScreen->addCallbackScreen(backgroundBiomeSettingScreen, backgroundMenuOptions)
		                    .addCallbackScreen(singleplayerSettingScreen, backgroundMenuSinglePlayer)
		                    .addWidget(backgroundMenuOptions)
		                    .addWidget(backgroundMenuSinglePlayer)
		                    .addWidget(backgroundMenuLanguage)
		                    .addWidget(backgroundMenuWhat)
		                    .addWidget(backgroundMenuLanguageTexturedButton)
		                    .addWidget(backgroundMenuQuitGame);
		backgroundMenuQuitGame->actionsToExecWhenClicked([] {
			PLOG_DEBUG << "Cancel runcraft!";
			GameInfo.getRender()->getWindow().close();
		});

		settingVolumeScreen->addCallbackScreen(backgroundBiomeSettingScreen, settingVolumeBack)
		                   .addWidget(settingVolumeSliderTitle)
		                   .addWidget(settingVolumeSliderAmbientEnvironment)
		                   .addWidget(settingVolumeSliderBlocks)
		                   .addWidget(settingVolumeSliderFriendlyCreatures)
		                   .addWidget(settingVolumeSliderHostileCreatures)
		                   .addWidget(settingVolumeSliderJukeboxNoteblocks)
		                   .addWidget(settingVolumeSliderMasterVolume)
		                   .addWidget(settingVolumeSliderMusic)
		                   .addWidget(settingVolumeSliderPlayers)
		                   .addWidget(settingVolumeSliderWeather)
		                   .addWidget(settingVolumeBack);
		settingVolumeSliderAmbientEnvironment->varToChangeWhenMoved(&GameInfo.gameGlobalData.soundLevel.ambientAndEnvironment);
		settingVolumeSliderBlocks->varToChangeWhenMoved(&GameInfo.gameGlobalData.soundLevel.blocks);
		settingVolumeSliderFriendlyCreatures->varToChangeWhenMoved(&GameInfo.gameGlobalData.soundLevel.friendlyCreatures);
		settingVolumeSliderHostileCreatures->varToChangeWhenMoved(&GameInfo.gameGlobalData.soundLevel.hostileCreatures);
		settingVolumeSliderJukeboxNoteblocks->varToChangeWhenMoved(&GameInfo.gameGlobalData.soundLevel.jukeBoxOrNoteblocks);
		settingVolumeSliderMasterVolume->varToChangeWhenMoved(&GameInfo.gameGlobalData.soundLevel.masterVolume);
		settingVolumeSliderMusic->varToChangeWhenMoved(&GameInfo.gameGlobalData.soundLevel.music)
		                        .actionsToExecWhenMoved([this]() { backgroundMusic.changeVolume((GameInfo.gameGlobalData.soundLevel.music)); });
		settingVolumeSliderPlayers->varToChangeWhenMoved(&GameInfo.gameGlobalData.soundLevel.players);
		settingVolumeSliderWeather->varToChangeWhenMoved(&GameInfo.gameGlobalData.soundLevel.weather);

		singleplayerSettingScreen->addCallbackScreen(backgroundMenuScreen, singleplayerSettingBack)
		                         .addWidget(singleplayerGenerateWorld)
		                         .addWidget(singleplayerSettingBack);

		backgroundBiomeSettingScreen->addCallbackScreen(backgroundMenuScreen, backgroundSettingBack)
		                            .addCallbackScreen(settingVolumeScreen, backgroundSettingVolume)
		                            .addWidget(backgroundSettingBack)
		                            .addWidget(backgroundSettingBiomeSnowyPlains)
		                            .addWidget(backgroundSettingBiomePlains)
		                            .addWidget(backgroundSettingBiomeForest)
		                            .addWidget(backgroundSettingBiomeBirchForest)
		                            .addWidget(backgroundSettingBiomeDesert)
		                            .addWidget(backgroundSettingBiomeJungle)
		                            .addWidget(backgroundSettingVolume);

		backgroundSettingBiomeSnowyPlains->actionsToExecWhenClicked([] { GameInfo.gameGlobalData.biome = game_data::Biome::SNOWY_PLAINS; });
		backgroundSettingBiomePlains->actionsToExecWhenClicked([] { GameInfo.gameGlobalData.biome = game_data::Biome::PLAINS; });
		backgroundSettingBiomeForest->actionsToExecWhenClicked([] { GameInfo.gameGlobalData.biome = game_data::Biome::FOREST; });
		backgroundSettingBiomeBirchForest->actionsToExecWhenClicked([] { GameInfo.gameGlobalData.biome = game_data::Biome::BIRCH_FOREST; });
		backgroundSettingBiomeDesert->actionsToExecWhenClicked([] { GameInfo.gameGlobalData.biome = game_data::Biome::DESERT; });
		backgroundSettingBiomeJungle->actionsToExecWhenClicked([] { GameInfo.gameGlobalData.biome = game_data::JUNGLE; });

		PLOG_DEBUG << "Initialize widget components";
	}
};

#endif //RUNCRAFT_MENU_HPP
