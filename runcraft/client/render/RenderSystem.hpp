//
// Created by robcholz on 3/11/23.
//
#pragma once

#ifndef RUNCRAFT_RENDERSYSTEM_HPP
#define RUNCRAFT_RENDERSYSTEM_HPP


#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include "Log.h"
#include "client/GameInfo.hpp"


class RenderSystem {
private:
	using Camera = sf::View;
	using String = std::string;

	struct ScreenConfig;
public:
	explicit RenderSystem(const String& windowName) {
		screenConfig.screenWidth = (int) ((float) sf::VideoMode::getDesktopMode().width * 19 / 32);
		screenConfig.screenHeight = (int) ((float) sf::VideoMode::getDesktopMode().height * 8 / 15);
		screenConfig.window = new sf::RenderWindow(sf::VideoMode(screenConfig.screenWidth, screenConfig.screenHeight),
		                                           windowName);
		screenConfig.camera = screenConfig.window->getDefaultView();

		PLOG_DEBUG << "Initialize the runcraft main renderer. Parameters: Name: " + windowName + " Size: " +
		              std::to_string(screenConfig.screenWidth) + "*" + std::to_string(screenConfig.screenHeight);
	}

	~RenderSystem() {
		delete screenConfig.window;
	}

	struct ScreenConfig getWindowConfig() { return screenConfig; }

	Camera* getCamera() { return &screenConfig.camera; }

	void updateScreenConfig() {
		screenConfig.screenWidth = sf::VideoMode::getDesktopMode().width;
		screenConfig.screenHeight = sf::VideoMode::getDesktopMode().height;
	}

	void render(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default) const {
		screenConfig.window->draw(drawable, states);
	}

private:
	struct ScreenConfig {
		sf::RenderWindow* window{};
		Camera camera;
		unsigned int screenWidth{};
		unsigned int screenHeight{};
	};
	ScreenConfig screenConfig;
};

#endif //RUNCRAFT_RENDERSYSTEM_HPP
