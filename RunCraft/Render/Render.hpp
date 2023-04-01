//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_RENDER_HPP
#define RUNCRAFT_RENDER_HPP

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include "Log.h"
#include "GameInfo.hpp"
#include "Init.h"

class Render {
private:
	sf::RenderWindow *window;
	unsigned int screenWidth;
	unsigned int screenHeight;

public:
	explicit Render(const std::string &windowName) {
		screenWidth = (int) ((float) sf::VideoMode::getDesktopMode().width * 19 / 32);
		screenHeight = (int) ((float) sf::VideoMode::getDesktopMode().height * 8 / 15);
		window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight),
		                              windowName);

		PLOG_DEBUG << "Initialize the RunCraft main renderer. Parameters: Name: " + windowName + " Size: " +
		              std::to_string(screenWidth) + "*" + std::to_string(screenHeight);
	}

	[[maybe_unused]] Render(int screenWidth, int screenHeight, const std::string &windowName) {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), windowName);
	}

	sf::RenderWindow &getWindow() { return *window; }

	[[nodiscard]] unsigned int getScreenWidth() const { return window->getSize().x; }

	[[nodiscard]] unsigned int getScreenHeight() const { return window->getSize().y; }

	void render(const sf::Drawable &drawable, const sf::RenderStates &states = sf::RenderStates::Default) {
		window->draw(drawable, states);
	}
};

#endif //RUNCRAFT_RENDER_HPP
