//
// Created by robcholz on 3/25/23.
//

#ifndef RUNCRAFT_SYSTEMEVENTS_HPP
#define RUNCRAFT_SYSTEMEVENTS_HPP

#pragma once

#include <SFML/Window/Event.hpp>
#include "Menu.hpp"

class SystemEvents {
private:
	Render *render = nullptr;
	sf::Event event{};
public:
	explicit SystemEvents(Render *render) { this->render = render; }

	void listen() {
		while (render->getWindow().pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					render->getWindow().close();
					PLOG_DEBUG << "Cancel the RunCraft!";
					break;
				case sf::Event::Resized:
					break;
				case sf::Event::LostFocus:
					break;
				case sf::Event::GainedFocus:
					break;
				case sf::Event::TextEntered:
					break;
				case sf::Event::KeyPressed:
					break;
				case sf::Event::KeyReleased:
					break;
				case sf::Event::MouseWheelMoved:
					break;
				case sf::Event::MouseWheelScrolled:
					break;
				case sf::Event::MouseButtonPressed:
					break;
				case sf::Event::MouseButtonReleased:
					break;
				case sf::Event::MouseMoved:
					break;
				case sf::Event::MouseEntered:
					break;
				case sf::Event::MouseLeft:
					break;
				case sf::Event::JoystickButtonPressed:
					break;
				case sf::Event::JoystickButtonReleased:
					break;
				case sf::Event::JoystickMoved:
					break;
				case sf::Event::JoystickConnected:
					break;
				case sf::Event::JoystickDisconnected:
					break;
				case sf::Event::TouchBegan:
					break;
				case sf::Event::TouchMoved:
					break;
				case sf::Event::TouchEnded:
					break;
				case sf::Event::SensorChanged:
					break;
				case sf::Event::Count:
					break;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
				render->getWindow().close();
				PLOG_DEBUG << "Cancel the RunCraft!";
			}
		}
	}
};

#endif //RUNCRAFT_SYSTEMEVENTS_HPP
