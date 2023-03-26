//
// Created by robcholz on 3/25/23.
//

#ifndef RUNCRAFT_SCREEN_HPP
#define RUNCRAFT_SCREEN_HPP

#pragma once

#include "../widget/WidgetManager.hpp"

class Screen : public GUI {
private:
	Background *background;
	WidgetManager *widgetManager;
public:
	explicit Screen(std::string &backgroundTexturePath,WidgetManager *widgetManager) {
		background = new Background(guiFilePath + backgroundTexturePath);
		this->widgetManager=widgetManager;
		background->composeToScreen();
	}

	~Screen(){delete background;}

	void render() override {
		background->render();
		//TODO:
		//widgetManager->listen();
	}
};

#endif //RUNCRAFT_SCREEN_HPP
