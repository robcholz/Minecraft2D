//
// Created by robcholz on 3/31/23.
//

#ifndef RUNCRAFT_MENU_HPP
#define RUNCRAFT_MENU_HPP

#pragma once


#include "util/utils.hpp"
#include "GameInfo.hpp"
#include "Initializers/ConsoleInitializer.h"
#include "GUI/screen/Background.hpp"
#include "GUI/widget/Button.hpp"
#include "GUI/widget/Slider.hpp"
#include "Render/Render.hpp"
#include "Sound/Audio.hpp"
#include "GUI/screen/Screen.hpp"
#include "GUI/screen/ScreenManager.hpp"
#include "GUI/widget/ValueSlider.hpp"

class Menu {
private:
	int screenWidth = (int) GameInfo.getScreenWidth();
	int screenHeight = (int) GameInfo.getScreenHeight();
	AudioList backgroundMusic;
	Once backgroundOnce;

	Background *backgroundTexture = new Background("background.png");
	Background *settingBackground = new Background("options_background.png");

	/*main menu*/
	Button *backgroundMenuSinglePlayer = new Button("Singleplayer", 800, 80, true, screenWidth / 2 - 800 / 2, 432);
	Button *backgroundMenuOptions = new Button("Options", 800, 80, true, screenWidth / 2 - 800 / 2, 528);
	Button *backgroundMenuWhat = new Button("Something uncertain", 800, 80, true, screenWidth / 2 - 800 / 2, 624);
	Button *backgroundMenuLanguage = new Button("Language", 380, 80, true, screenWidth / 2 - 800 / 2, 768);
	Button *backgroundMenuQuitGame = new Button("Quit Game", 380, 80, true, screenWidth / 2 + 800 / 2 - 380, 768);
	Screen *backgroundMenuScreen = new Screen(backgroundTexture);

	/*settings*/
	Button *backgroundSettingBiomeSnowyPlains = new Button("Snowy Plains", 590, 80, true, screenWidth / 2 - 1200 / 2, 400);
	Button *backgroundSettingBiomePlains = new Button("Plains", 590, 80, true, screenWidth / 2 + 1200 / 2 - 590, 400);
	Button *backgroundSettingBiomeForest = new Button("Forest", 590, 80, true, screenWidth / 2 - 1200 / 2, 500);
	Button *backgroundSettingBiomeDesert = new Button("Desert", 590, 80, true, screenWidth / 2 + 1200 / 2 - 590, 500);
	Button *backgroundSettingBiomeBirchForest = new Button("Birch Forest", 590, 80, true, screenWidth / 2 - 1200 / 2, 600);
	Button *backgroundSettingBiomeJungle = new Button("Jungle", 590, 80, true, screenWidth / 2 + 1200 / 2 - 590, 600);
	Button *backgroundSettingVolume = new Button("Music", 1200, 80, true, screenWidth / 2 - 1200 / 2, 200);
	Button *backgroundSettingBack = new Button("Done", 790, 80, true, screenWidth / 2 - 780 / 2, screenHeight - 80 - 50);
	Screen *backgroundBiomeSettingScreen = new Screen(backgroundTexture);

	/*music settings*/
	ValueSlider *settingVolumeSliderMasterVolume = new ValueSlider("Master Volume", 1240, 80, true, screenWidth / 2 - 620, 115);
	ValueSlider *settingVolumeSliderMusic = new ValueSlider("Music", 590, 80, true, screenWidth / 2 - 620, 225);
	ValueSlider *settingVolumeSliderJukeboxNoteblocks = new ValueSlider("Jukebox/Noteblocks", 590, 80, true, screenWidth / 2 + 30, 225);
	ValueSlider *settingVolumeSliderWeather = new ValueSlider("Weather", 590, 80, true, screenWidth / 2 - 620, 335);
	ValueSlider *settingVolumeSliderBlocks = new ValueSlider("Blocks", 36, 590, 80, true, screenWidth / 2 + 30, 335);
	ValueSlider *settingVolumeSliderHostileCreatures = new ValueSlider("Hostile Creatures", 590, 80, true, screenWidth / 2 - 620, 445);
	ValueSlider *settingVolumeSliderFriendlyCreatures = new ValueSlider("Friendly Creatures", 590, 80, true, screenWidth / 2 + 30, 445);
	ValueSlider *settingVolumeSliderPlayers = new ValueSlider("Players", 590, 80, true, screenWidth / 2 - 620, 555);
	ValueSlider *settingVolumeSliderAmbientEnvironment = new ValueSlider("Ambient/Environment", 590, 80, true, screenWidth / 2 + 30, 555);
	Button *settingVolumeBack = new Button("Done", 790, 80, true, screenWidth / 2 - 390, 555 + 260);
	Screen *settingVolumeScreen = new Screen(settingBackground);

	/*singleplayer page*/
	Button *singleplayerSettingBack = new Button("Done", 790, 80, true, screenWidth / 2 - 790 / 2, screenHeight - 80 - 50);
	Screen *singleplayerSettingScreen = new Screen(settingBackground);

	ScreenManager screenManager;
public:
	explicit Menu() {
		PLOG_DEBUG << "Menu started!";
		initResource();
	}

	~Menu() {
		delete backgroundTexture;
		delete backgroundMenuSinglePlayer;
		delete backgroundMenuOptions;
		delete backgroundMenuLanguage;
		delete backgroundMenuScreen;
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
				.addWidget(backgroundMenuQuitGame);
		backgroundMenuQuitGame->actionsToExecWhenClicked([] {
			PLOG_DEBUG << "Cancel RunCraft!";
			GameInfo.getRender()->getWindow().close();
		});

		settingVolumeScreen->addCallbackScreen(backgroundBiomeSettingScreen, settingVolumeBack)
				.addWidget(settingVolumeSliderAmbientEnvironment)
				.addWidget(settingVolumeSliderBlocks)
				.addWidget(settingVolumeSliderFriendlyCreatures)
				.addWidget(settingVolumeSliderFriendlyCreatures)
				.addWidget(settingVolumeSliderHostileCreatures)
				.addWidget(settingVolumeSliderJukeboxNoteblocks)
				.addWidget(settingVolumeSliderMasterVolume)
				.addWidget(settingVolumeSliderMusic)
				.addWidget(settingVolumeSliderPlayers)
				.addWidget(settingVolumeSliderWeather)
				.addWidget(settingVolumeBack);

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
