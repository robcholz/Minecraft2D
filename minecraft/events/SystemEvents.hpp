//
// Created by robcholz on 3/25/23.
//
#pragma once

#ifndef MINECRAFT_SYSTEMEVENTS_HPP
#define MINECRAFT_SYSTEMEVENTS_HPP


#include <SFML/Window/Event.hpp>
#include <memory>
#include <functional>
#include "client/MinecraftClientAccess.hpp"
#include "client/input/Input.hpp"

class SystemEvents {
public:
	explicit SystemEvents(MinecraftClientAccess* minecraftClientAccess) {
		this->minecraftClientAccess = minecraftClientAccess;
		SystemEvents::instance = this;
	}

	sf::Event* getEvent() { return &event; }

	void update() {
		while (RenderSystem::getWindow()->pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					RenderSystem::getWindow()->close();
					PLOG_DEBUG << "Cancel the minecraft!";
					break;
				case sf::Event::Resized:
					resizeWindow();
					{
						PLOG_DEBUG<<event.size.width<<" "<<event.size.height;
						sf::FloatRect visibleArea(0, 0,event.size.width,event.size.height );
						RenderSystem::getWindow()->setView(sf::View(visibleArea));
					}
					break;
				case sf::Event::LostFocus:
					pauseGame();
					break;
				case sf::Event::GainedFocus:
					resumeGame();
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
					//GameInfo.getExternalData()->peripheralState.mouseWheelScroll
					break;
				case sf::Event::TouchEnded:
					break;
				case sf::Event::SensorChanged:
					break;
				case sf::Event::Count:// Dont use this
					break;
			}
		}
	}

	void onGamePause(std::function<void()> ppauseGame) {
		this->pauseGame = std::move(ppauseGame);
	}

	void onGameResume(std::function<void()> presumeGame) {
		this->resumeGame = std::move(presumeGame);
	}

	void onWindowResize(std::function<void()> presizeWindow) {
		this->resizeWindow = std::move(presizeWindow);
	}

	static SystemEvents* getInstance() {
		return SystemEvents::instance;
	}

private:
	MinecraftClientAccess* minecraftClientAccess = nullptr;
	static inline SystemEvents* instance = nullptr;
	sf::Event event{};
	std::function<void()> resumeGame;
	std::function<void()> pauseGame;
	std::function<void()> resizeWindow;
};

#endif //MINECRAFT_SYSTEMEVENTS_HPP
