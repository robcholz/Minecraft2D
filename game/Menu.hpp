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

	Background *backgroundTexture = new Background("background.png");
	/*main menu*/
	Button *backgroundMenuSinglePlayer = new Button("Singleplayer", 600, 80, true,
	                                                new sf::Vector2i(800 - 600 / 2, 400));
	Button *backgroundMenuOptions = new Button("Options", 600, 80, true, new sf::Vector2i(800 - 600 / 2, 500));
	Button *backgroundMenuLanguage = new Button("Language", 285, 80, true, new sf::Vector2i(800 - 600 / 2, 600));
	Button *backgroundMenuQuitGame = new Button("Quit Game", 285, 80, true,
	                                            new sf::Vector2i(800 - 600 / 2 + 600 - 285, 600));
	Screen *backgroundMenuScreen = new Screen(backgroundTexture);

	/*settings*/
	Button *backgroundBiomeSettingSnowyPlains = new Button("Snowy Plains", 285, 80, true,
	                                                       new sf::Vector2i(800 - 285 / 2, 400));
	Button *backgroundBiomeSettingPlains = new Button("Plains", 285, 80, true, new sf::Vector2i(800 - 285 / 2, 400));
	Button *backgroundBiomeSettingForest = new Button("Forest", 285, 80, true,
	                                                  new sf::Vector2i(800 - 285 / 2 + 285, 400));
	Button *backgroundBiomeSettingDesert = new Button("Desert", 285, 80, true, new sf::Vector2i(800 - 285 / 2, 500));
	Button *backgroundBiomeSettingBirchForest = new Button("Birch Forest", 285, 80, true,
	                                                       new sf::Vector2i(800 - 285 / 2 + 285, 500));
	Button *backgroundBiomeSettingBack = new Button("Back", 285, 80, true, new sf::Vector2i(800 - 285 / 2, 600));
	Screen *backgroundBiomeSettingScreen = new Screen(backgroundTexture);

	/*singleplayer setting page*/
	Button *singleplayerSettingBack=new Button("Back", 300, 80, true, new sf::Vector2i(800 - 300 / 2, 800));
	Background *singleplayerSettingBackground=new Background("options_background.png");
	Screen *singleplayerSettingScreen=new Screen(singleplayerSettingBackground);
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
				.addCallbackScreen(singleplayerSettingScreen,backgroundMenuSinglePlayer)
				.addWidget(backgroundMenuOptions)
				.addWidget(backgroundMenuSinglePlayer)
				.addWidget(backgroundMenuLanguage)
				.addWidget(backgroundMenuQuitGame);
		backgroundMenuQuitGame->actionToExecWhenClicked([] {
			PLOG_DEBUG << "Cancel RunCraft!";
			GameInfo.getRender()->getWindow().close();
		});

		singleplayerSettingScreen->addCallbackScreen(backgroundMenuScreen, singleplayerSettingBack)
			.addWidget(singleplayerSettingBack);

		backgroundBiomeSettingScreen->addCallbackScreen(backgroundMenuScreen, backgroundBiomeSettingBack)
				.addWidget(backgroundBiomeSettingBack)
				.addWidget(backgroundBiomeSettingSnowyPlains)
				.addWidget(backgroundBiomeSettingPlains)
				.addWidget(backgroundBiomeSettingForest)
				.addWidget(backgroundBiomeSettingBirchForest)
				.addWidget(backgroundBiomeSettingDesert);

		backgroundBiomeSettingSnowyPlains->actionToExecWhenClicked(
				[] { GameInfo.getGameGlobalData()->biome = game_data::Biome::SNOWY_PLAINS; });
		backgroundBiomeSettingPlains->actionToExecWhenClicked(
				[] { GameInfo.getGameGlobalData()->biome = game_data::Biome::PLAINS; });
		backgroundBiomeSettingForest->actionToExecWhenClicked(
				[] { GameInfo.getGameGlobalData()->biome = game_data::Biome::FOREST; });
		backgroundBiomeSettingBirchForest->actionToExecWhenClicked(
				[] { GameInfo.getGameGlobalData()->biome = game_data::Biome::BIRCH_FOREST; });
		backgroundBiomeSettingDesert->actionToExecWhenClicked(
				[] { GameInfo.getGameGlobalData()->biome = game_data::Biome::DESERT; });

		PLOG_DEBUG << "Initialize widget components";
	}
};

#endif //RUNCRAFT_MENU_HPP
