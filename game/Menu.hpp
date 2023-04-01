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

class Menu {
private:
	struct menu {
		/*Main Renderer*/
		Render *renderer;
	} menu{};

	AudioList backgroundMusic;
	Once backgroundOnce;

	/*main menu*/
	Button *backgroundMenuSinglePlayer = new Button("Singleplayer", 800, 80, true, new sf::Vector2i(800 - 800 / 2, 400));
	Button *backgroundMenuOptions = new Button("Options", 800, 80, true, new sf::Vector2i(800 - 800 / 2, 500));
	Button *backgroundMenuLanguage = new Button("Language", 380, 80, true, new sf::Vector2i(800 - 800 / 2, 650));
	Button *backgroundMenuQuitGame = new Button("Quit Game", 380, 80, true, new sf::Vector2i(800 + 800 / 2 - 380, 650));
	Background *backgroundTexture = new Background("background.png");
	Screen *backgroundMenuScreen = new Screen(backgroundTexture);

	/*settings*/
	Button *backgroundSettingBiomeSnowyPlains = new Button("Snowy Plains", 590, 80, true, new sf::Vector2i(800 - 1200 / 2, 400));
	Button *backgroundSettingBiomePlains = new Button("Plains", 590, 80, true, new sf::Vector2i(800 + 1200 / 2 - 590, 400));
	Button *backgroundSettingBiomeForest = new Button("Forest", 590, 80, true, new sf::Vector2i(800 - 1200 / 2, 500));
	Button *backgroundSettingBiomeDesert = new Button("Desert", 590, 80, true, new sf::Vector2i(800 + 1200 / 2 - 590, 500));
	Button *backgroundSettingBiomeBirchForest = new Button("Birch Forest", 590, 80, true, new sf::Vector2i(800 - 1200 / 2, 600));
	Button *backgroundSettingBiomeJungle = new Button("Jungle", 590, 80, true, new sf::Vector2i(800 + 1200 / 2 - 590, 600));
	Button *backgroundSettingBack = new Button("Done", 780, 80, true, new sf::Vector2i(800 - 780 / 2, 750));
	Slider *backgroundSettingVolumeSlider = new Slider("Sound Volume", 1200, 80, true, new sf::Vector2i(800 - 1200 / 2, 200));
	Screen *backgroundBiomeSettingScreen = new Screen(backgroundTexture);

	/*singleplayer setting page*/
	Button *singleplayerSettingBack = new Button("Done", 300, 80, true, new sf::Vector2i(800 - 300 / 2, 800));
	Background *singleplayerSettingBackground = new Background("options_background.png");
	Screen *singleplayerSettingScreen = new Screen(singleplayerSettingBackground);

	ScreenManager screenManager;
public:
	explicit Menu(const std::string &windowName) {
		menu.renderer = new Render(windowName);
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

	Render *getRenderer() const { return menu.renderer; }

	void initResource() {
		initAudio();
		initWidget();

		PLOG_DEBUG << "Initialize resources.";
	}

	void renderMainMenu() {
		if (backgroundOnce.runOnce()) {
			backgroundTexture->fitToScreen();
			singleplayerSettingBackground->composeToScreen();
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
				.setEntry(backgroundMenuScreen);
		backgroundMenuScreen->addCallbackScreen(backgroundBiomeSettingScreen, backgroundMenuOptions)
				.addCallbackScreen(singleplayerSettingScreen, backgroundMenuSinglePlayer)
				.addWidget(backgroundMenuOptions)
				.addWidget(backgroundMenuSinglePlayer)
				.addWidget(backgroundMenuLanguage)
				.addWidget(backgroundMenuQuitGame);
		backgroundMenuQuitGame->actionsToExecWhenClicked([] {
			PLOG_DEBUG << "Cancel RunCraft!";
			GameInfo.getRender()->getWindow().close();
		});

		singleplayerSettingScreen->addCallbackScreen(backgroundMenuScreen, singleplayerSettingBack)
				.addWidget(singleplayerSettingBack);

		backgroundBiomeSettingScreen->addCallbackScreen(backgroundMenuScreen, backgroundSettingBack)
				.addWidget(backgroundSettingBack)
				.addWidget(backgroundSettingBiomeSnowyPlains)
				.addWidget(backgroundSettingBiomePlains)
				.addWidget(backgroundSettingBiomeForest)
				.addWidget(backgroundSettingBiomeBirchForest)
				.addWidget(backgroundSettingBiomeDesert)
				.addWidget(backgroundSettingBiomeJungle)
				.addWidget(backgroundSettingVolumeSlider);

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
