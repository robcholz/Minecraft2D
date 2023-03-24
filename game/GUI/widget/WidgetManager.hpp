//
// Created by robcholz on 3/17/23.
//

#ifndef RUNCRAFT_WIDGETMANAGER_HPP
#define RUNCRAFT_WIDGETMANAGER_HPP

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../GUI.hpp"
#include "Widget.hpp"
#include "ButtonManager.hpp"

class WidgetManager {
private:
	ButtonManager *buttonManager;
public:
	WidgetManager() = default;

	WidgetManager &addManager(ButtonManager *manager) {
		buttonManager = manager;
		return *this;
	}

	~WidgetManager() = default;

	void listen(sf::Vector2i mousePosition, bool isPressed) {
		buttonManager->listen(mousePosition, isPressed);
	}
};

#endif //RUNCRAFT_WIDGETMANAGER_HPP
