//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_RENDER_HPP
#define RUNCRAFT_RENDER_HPP

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include "Log.h"
#include "client/GameInfo.hpp"
#include "Init.h"

typedef sf::View Camera;

class Render {
private:
	struct ScreenConfig {
		sf::RenderWindow *window;
		Camera camera;
		unsigned int screenWidth;
		unsigned int screenHeight;
	} screenConfig{};

public:
	explicit Render(const std::string &windowName) {
		screenConfig.screenWidth = (int) ((float) sf::VideoMode::getDesktopMode().width * 19 / 32);
		screenConfig.screenHeight = (int) ((float) sf::VideoMode::getDesktopMode().height * 8 / 15);
		screenConfig.window = new sf::RenderWindow(sf::VideoMode(screenConfig.screenWidth, screenConfig.screenHeight),
		                                           windowName);
		screenConfig.camera = screenConfig.window->getDefaultView();

		PLOG_DEBUG << "Initialize the runcraft main renderer. Parameters: Name: " + windowName + " Size: " +
		              std::to_string(screenConfig.screenWidth) + "*" + std::to_string(screenConfig.screenHeight);
	}

	~Render() {
		delete screenConfig.window;
	}

	[[nodiscard]] ScreenConfig getWindowConfig() const { return screenConfig; }
	Camera *getCamera(){return &screenConfig.camera;}

	void updateScreenConfig() {
		screenConfig.screenWidth = sf::VideoMode::getDesktopMode().width;
		screenConfig.screenHeight = sf::VideoMode::getDesktopMode().height;
	}

	void render(const sf::Drawable &drawable, const sf::RenderStates &states = sf::RenderStates::Default) const {
		screenConfig.window->draw(drawable, states);
	}

};

#endif //RUNCRAFT_RENDER_HPP
