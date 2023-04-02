//
// Created by robcholz on 3/31/23.
//

#ifndef RUNCRAFT_MENU_HPP
#define RUNCRAFT_MENU_HPP

#pragma once


#include "util/utils.hpp"
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
	ButtonWidget *backgroundMenuSinglePlayer = new ButtonWidget("Singleplayer", 800, 80, true, screenWidth / 2 - 800 / 2, 432);
	ButtonWidget *backgroundMenuOptions = new ButtonWidget("Options...", 800, 80, true, screenWidth / 2 - 800 / 2, 528);
	ButtonWidget *backgroundMenuWhat = new ButtonWidget("Something uncertain", 800, 80, true, screenWidth / 2 - 800 / 2, 624);
	ButtonWidget *backgroundMenuLanguage = new ButtonWidget("Language", 380, 80, true, screenWidth / 2 - 800 / 2, 768);
	ButtonWidget *backgroundMenuQuitGame = new ButtonWidget("Quit Game", 380, 80, true, screenWidth / 2 + 800 / 2 - 380, 768);
	TexturedButtonWidget *backgroundMenuLanguageTexturedButton = new TexturedButtonWidget(32, 32, true, screenWidth / 2 - 500, 768);
	Screen *backgroundMenuScreen = new Screen(backgroundTexture);

	/*settings*/
	ButtonWidget *backgroundSettingBiomeSnowyPlains = new ButtonWidget("Snowy Plains", 590, 80, true, screenWidth / 2 - 1200 / 2, 400);
	ButtonWidget *backgroundSettingBiomePlains = new ButtonWidget("Plains", 590, 80, true, screenWidth / 2 + 1200 / 2 - 590, 400);
	ButtonWidget *backgroundSettingBiomeForest = new ButtonWidget("Forest", 590, 80, true, screenWidth / 2 - 1200 / 2, 500);
	ButtonWidget *backgroundSettingBiomeDesert = new ButtonWidget("Desert", 590, 80, true, screenWidth / 2 + 1200 / 2 - 590, 500);
	ButtonWidget *backgroundSettingBiomeBirchForest = new ButtonWidget("Birch Forest", 590, 80, true, screenWidth / 2 - 1200 / 2, 600);
	ButtonWidget *backgroundSettingBiomeJungle = new ButtonWidget("Jungle", 590, 80, true, screenWidth / 2 + 1200 / 2 - 590, 600);
	ButtonWidget *backgroundSettingVolume = new ButtonWidget("Music", 1200, 80, true, screenWidth / 2 - 1200 / 2, 200);
	ButtonWidget *backgroundSettingBack = new ButtonWidget("Done", 790, 80, true, screenWidth / 2 - 780 / 2, screenHeight - 80 - 50);
	Screen *backgroundBiomeSettingScreen = new Screen(backgroundTexture);

	/*music settings*/
	TextFieldWidget *settingVolumeSliderTitle = new TextFieldWidget("Music & sound Options", 70, true, screenWidth / 2 - 440 / 2, 20);
	SoundSliderWidget *settingVolumeSliderMasterVolume = new SoundSliderWidget("Master Volume", 1240, 80, true, screenWidth / 2 - 620, 115);
	SoundSliderWidget *settingVolumeSliderMusic = new SoundSliderWidget("Music", 590, 80, true, screenWidth / 2 - 620, 225);
	SoundSliderWidget *settingVolumeSliderJukeboxNoteblocks = new SoundSliderWidget("Jukebox/Noteblocks", 590, 80, true,
	                                                                                screenWidth / 2 + 30, 225);
	SoundSliderWidget *settingVolumeSliderWeather = new SoundSliderWidget("Weather", 590, 80, true, screenWidth / 2 - 620, 335);
	SoundSliderWidget *settingVolumeSliderBlocks = new SoundSliderWidget("Blocks", 36, 590, 80, true, screenWidth / 2 + 30, 335);
	SoundSliderWidget *settingVolumeSliderHostileCreatures = new SoundSliderWidget("Hostile Creatures", 590, 80, true,
	                                                                               screenWidth / 2 - 620, 445);
	SoundSliderWidget *settingVolumeSliderFriendlyCreatures = new SoundSliderWidget("Friendly Creatures", 590, 80, true,
	                                                                                screenWidth / 2 + 30, 445);
	SoundSliderWidget *settingVolumeSliderPlayers = new SoundSliderWidget("Players", 590, 80, true, screenWidth / 2 - 620, 555);
	SoundSliderWidget *settingVolumeSliderAmbientEnvironment = new SoundSliderWidget("Ambient/Environment", 590, 80, true,
	                                                                                 screenWidth / 2 + 30, 555);
	ButtonWidget *settingVolumeBack = new ButtonWidget("Done", 790, 80, true, screenWidth / 2 - 390, 555 + 260);
	Screen *settingVolumeScreen = new Screen(settingBackground);

	/*singleplayer page*/
	ButtonWidget *singleplayerSettingBack = new ButtonWidget("Done", 790, 80, true, screenWidth / 2 - 790 / 2, screenHeight - 80 - 50);
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

		PLOG_DEBUG << "Initialize resources.";
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
				.addAudio("moog_city_2");

		PLOG_DEBUG << "Initialize audio resources";
	}

	void initWidget() {
		screenManager
				.addScreen(backgroundMenuScreen)
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
			PLOG_DEBUG << "Cancel RunCraft!";
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
		settingVolumeSliderAmbientEnvironment->varToChangeWhenMoved(&GameInfo.getGameGlobalData()->soundLevel.ambientAndEnvironment);
		settingVolumeSliderBlocks->varToChangeWhenMoved(&GameInfo.getGameGlobalData()->soundLevel.blocks);
		settingVolumeSliderFriendlyCreatures->varToChangeWhenMoved(&GameInfo.getGameGlobalData()->soundLevel.friendlyCreatures);
		settingVolumeSliderHostileCreatures->varToChangeWhenMoved(&GameInfo.getGameGlobalData()->soundLevel.hostileCreatures);
		settingVolumeSliderJukeboxNoteblocks->varToChangeWhenMoved(&GameInfo.getGameGlobalData()->soundLevel.jukeBoxOrNoteblocks);
		settingVolumeSliderMasterVolume->varToChangeWhenMoved(&GameInfo.getGameGlobalData()->soundLevel.masterVolume);
		settingVolumeSliderMusic->varToChangeWhenMoved(&GameInfo.getGameGlobalData()->soundLevel.music)
				.actionsToExecWhenMoved([this]() { backgroundMusic.changeVolume((GameInfo.getGameGlobalData()->soundLevel.music)); });
		settingVolumeSliderPlayers->varToChangeWhenMoved(&GameInfo.getGameGlobalData()->soundLevel.players);
		settingVolumeSliderWeather->varToChangeWhenMoved(&GameInfo.getGameGlobalData()->soundLevel.weather);

		singleplayerSettingScreen->addCallbackScreen(backgroundMenuScreen, singleplayerSettingBack)
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

		backgroundSettingBiomeSnowyPlains->actionsToExecWhenClicked(
				[] { GameInfo.getGameGlobalData()->biome = game_data::Biome::SNOWY_PLAINS; });
		backgroundSettingBiomePlains->actionsToExecWhenClicked(
				[] { GameInfo.getGameGlobalData()->biome = game_data::Biome::PLAINS; });
		backgroundSettingBiomeForest->actionsToExecWhenClicked(
				[] { GameInfo.getGameGlobalData()->biome = game_data::Biome::FOREST; });
		backgroundSettingBiomeBirchForest->actionsToExecWhenClicked(
				[] { GameInfo.getGameGlobalData()->biome = game_data::Biome::BIRCH_FOREST; });
		backgroundSettingBiomeDesert->actionsToExecWhenClicked(
				[] { GameInfo.getGameGlobalData()->biome = game_data::Biome::DESERT; });
		backgroundSettingBiomeJungle->actionsToExecWhenClicked(
				[] { GameInfo.getGameGlobalData()->biome = game_data::JUNGLE; });

		PLOG_DEBUG << "Initialize widget components";
	}
};

#endif //RUNCRAFT_MENU_HPP
