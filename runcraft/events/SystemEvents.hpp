//
// Created by robcholz on 3/25/23.
//
#pragma once

#ifndef RUNCRAFT_SYSTEMEVENTS_HPP
#define RUNCRAFT_SYSTEMEVENTS_HPP


#include <SFML/Window/Event.hpp>
#include <memory>
#include <functional>
#include "client/GameInfo.hpp"
#include "client/RuncraftClientAccess.hpp"
#include "client/input/Input.hpp"

class SystemEvents {
public:
	explicit SystemEvents(RuncraftClientAccess* runcraftClientAccess) {
		this->runcraftClientAccess = runcraftClientAccess;
		SystemEvents::instance = this;
	}

	sf::Event* getEvent() { return &event; }

	void update() {
		while (render->getWindowConfig().window->pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					render->getWindowConfig().window->close();
					PLOG_DEBUG << "Cancel the runcraft!";
					break;
				case sf::Event::Resized:
					//GameInfo.getConstExternalData()->windowState.rendererPtr->updateScreenConfig();
					//GameInfo.getExternalData()->windowState.resized=|0x11;
					break;
				case sf::Event::LostFocus:
					onLostFocus();
					break;
				case sf::Event::GainedFocus:
					onGainedFocus();
					break;
				case sf::Event::TextEntered:
					break;
				case sf::Event::KeyPressed:
					break;
				case sf::Event::KeyReleased:
					break;
				case sf::Event::MouseWheelMoved:
					//[[deprecated]]
					break;
				case sf::Event::MouseWheelScrolled:
					GameInfo.getExternalData()->peripheralState.mouseWheelScrollDelta = event.mouseWheelScroll.delta;
					GameInfo.getExternalData()->windowState.zoomSize += (short) event.mouseWheelScroll.delta;
					if (GameInfo.getConstExternalData()->windowState.zoomSize > 100) {
						GameInfo.getExternalData()->windowState.zoomSize = 100;
					} else if (GameInfo.getConstExternalData()->windowState.zoomSize < 50)
						GameInfo.getExternalData()->windowState.zoomSize = 50;
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
					GameInfo.getExternalData()->peripheralState.touchBegan = true;
					GameInfo.getExternalData()->peripheralState.touchMoved = false;
					GameInfo.getExternalData()->peripheralState.touchEnded = false;
					break;
				case sf::Event::TouchMoved:
					GameInfo.getExternalData()->peripheralState.touchBegan = false;
					GameInfo.getExternalData()->peripheralState.touchMoved = true;
					GameInfo.getExternalData()->peripheralState.touchEnded = false;
					//GameInfo.getExternalData()->peripheralState.mouseWheelScroll
					break;
				case sf::Event::TouchEnded:
					GameInfo.getExternalData()->peripheralState.touchBegan = false;
					GameInfo.getExternalData()->peripheralState.touchMoved = false;
					GameInfo.getExternalData()->peripheralState.touchEnded = true;
					break;
				case sf::Event::SensorChanged:
					break;
				case sf::Event::Count:// Dont use this
					break;
			}
		}
	}

	void onGamePause(std::function<void()> ppauseGame) {
		this->pauseGame =  std::move(ppauseGame);
	}

	void onGameResume(std::function<void()> presumeGame) {
		this->resumeGame = std::move(presumeGame);
	}

	static SystemEvents* getInstance() {
		return SystemEvents::instance;
	}

private:
	RuncraftClientAccess* runcraftClientAccess = nullptr;
	static inline SystemEvents* instance = nullptr;
	RenderSystem* render = GameInfo.getRender();
	sf::Event event{};
	std::function<void()> resumeGame;
	std::function<void()> pauseGame;

	void onGainedFocus() {
		GameInfo.getExternalData()->windowState.lostFocus = false;
		GameInfo.getExternalData()->windowState.gainedFocus = true;
		resumeGame();
	}

	void onLostFocus() {
		GameInfo.getExternalData()->windowState.lostFocus = true;
		GameInfo.getExternalData()->windowState.gainedFocus = false;
		pauseGame();
	}
};

#endif //RUNCRAFT_SYSTEMEVENTS_HPP
