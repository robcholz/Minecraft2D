//
// Created by robcholz on 3/4/23.
//

#ifndef RUNCRAFT_GAME_HPP
#define RUNCRAFT_GAME_HPP

#pragma once

#include "Sound/Audio.hpp"
#include "util/utils.hpp"
#include "GameInfo.hpp"
#include "Initializers/ConsoleInitializer.h"
#include "GUI/screen/Background.hpp"
#include "GUI/widget/Button.hpp"
#include "GUI/widget/WidgetManager.hpp"
#include "GUI/widget/WidgetManager.hpp"

class Game {
private:
	struct game {
		/*Main Renderer*/
		Render *renderer;
	} game{};

	/*backGround!*/
	AudioList backgroundMusic;
	Once backgroundOnce;
	Background *backgroundTexture = new Background("background.png");
	Button *backgroundSinglePlayer = new Button("Singleplayer", 600, 80, true, new sf::Vector2i(800-600/2, 400));
	Button *backgroundOptions = new Button("Options", 600, 80, true, new sf::Vector2i(800 - 600 / 2, 600));
	Button *backgroundLanguage = new Button("Language", 600, 80, true, new sf::Vector2i(800 - 600 / 2, 500));

	WidgetManager *backgroundWidgetManager = new WidgetManager;

public:
	explicit Game(const std::string &windowName) {
		game.renderer = new Render(windowName);

		PLOG_DEBUG << "Game started!";
	}

	~Game() {
		delete game.renderer;
		delete backgroundTexture;
		delete backgroundSinglePlayer;
		delete backgroundOptions;
		delete backgroundLanguage;
		delete backgroundWidgetManager;
	}

	Render *getRenderer() const { return game.renderer; }

	void initResource() {
		initAudio();
		initWidget();

		PLOG_DEBUG << "Initialize resources.";
	}

	void renderMainMenu() {
		if (backgroundOnce.runOnce()) {
			backgroundTexture->fitToScreen();
			backgroundMusic.playRandomly();

			PLOG_DEBUG << "Rendered main menu!";
		}

		backgroundTexture->render();
		backgroundWidgetManager->listen(sf::Mouse::getPosition(this->getRenderer()->getWindow()),
		                                sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
	}

	void initAudio() {
		backgroundMusic.addAudio("oxygène")
				.addAudio("beginning")
				.addAudio("beginning_2")
				.addAudio("moog_city_2");

		PLOG_DEBUG << "Initialize audio resources";
	}

	void initWidget() {
		backgroundWidgetManager->addWidget(backgroundSinglePlayer)
				.addWidget(backgroundOptions)
				.addWidget(backgroundLanguage);

		PLOG_DEBUG << "Initialize widget components";
	}

	[[nodiscard]] unsigned int getScreenWidth() const { return game.renderer->getWindow().getSize().x; }

	[[nodiscard]] unsigned int getScreenHeight() const { return game.renderer->getWindow().getSize().y; }

};


#endif //RUNCRAFT_GAME_HPP
