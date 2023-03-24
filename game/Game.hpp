//
// Created by robcholz on 3/4/23.
//

#ifndef RUNCRAFT_GAME_HPP
#define RUNCRAFT_GAME_HPP

#include "Sound/Audio.hpp"
#include "util/utils.hpp"
#include "GameInfo.hpp"
#include "Initializers/ConsoleInitializer.h"
#include "GUI/screen/Background.hpp"
#include "GUI/widget/Button.hpp"
#include "GUI/widget/WidgetManager.hpp"
#include "GUI/widget/ButtonManager.hpp"

class Game {
private:
	/*Main Renderer*/
	Render *renderer;

	/*backGround!*/
	AudioList backgroundMusic;
	Once backgroundOnce;
	Background *backgroundTexture = new Background("background.png");
	Button *backgroundSinglePlayer = new Button("Singleplayer", 200, new sf::Vector2f(600, 400));
	Button *backgroundOptions = new Button("Options", 200, new sf::Vector2f(800, 600));
	Button *backgroundLanguage = new Button("", 300, new sf::Vector2f(500, 500));

	WidgetManager *backgroundWidgetManager=new WidgetManager;
	ButtonManager *backgroundButtonManager=new ButtonManager;

public:
	explicit Game(const std::string &windowName) {
		renderer = new Render(windowName);

		PLOG_DEBUG << "Game started!";
	}

	~Game() {
		delete renderer;
		delete backgroundTexture;
		delete backgroundSinglePlayer;
		delete backgroundOptions;
		delete backgroundLanguage;
		delete backgroundWidgetManager;
		delete backgroundButtonManager;
	}

	Render *getRenderer() { return renderer; }

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
		backgroundWidgetManager->addManager(backgroundButtonManager);
		backgroundButtonManager->addButton(backgroundSinglePlayer)
				.addButton(backgroundOptions)
				.addButton(backgroundLanguage);

		PLOG_DEBUG << "Initialize widget components";
	}

	[[nodiscard]] unsigned int getScreenWidth() const { return renderer->getWindow().getSize().x; }

	[[nodiscard]] unsigned int getScreenHeight() const { return renderer->getWindow().getSize().y; }

};


#endif //RUNCRAFT_GAME_HPP
