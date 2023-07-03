//
// Created by robcholz on 6/15/23.
//

#ifndef RUNCRAFT_RUNCRAFTCLIENT_HPP
#define RUNCRAFT_RUNCRAFTCLIENT_HPP

#include "sound/SoundManager.hpp"
#include "RuncraftClientAccess.hpp"
#include "client/scene/SceneManager.hpp"
#include "Menu.hpp"
#include "world/World.hpp"
#include "util/GameLogger.hpp"
#include "GameInfo.hpp"

class RuncraftClient : public RuncraftClientAccess {
protected:
	void onConfig() {
		GameInfo.setRenderer(&render);
		GameInfo.getConstExternalData()->windowState.getRender()->getWindowConfig().window->setVerticalSyncEnabled(true);
		mouse = input::PeripheralsFactory::getMouse();
		keyboard = input::PeripheralsFactory::getKeyboard();
		gameEvents = SystemEvents::getInstance();
	}

	void onUpdate() {
		gameEvents->update();
		keyboard->update();
		mouse->update();
		sceneManager.update();
		soundManager.update();
	}

public:
	RuncraftClient() {
		onConfig();

		sceneManager.addScene("menu", [this]() { return new Menu(this); })
		            .addScene("world", [this]() { return new World(this); })
					.setPair("menu", "world")
		            .setEntry("menu");
	}

	void run() {
		while (GameInfo.getRender()->getWindowConfig().window->isOpen()) {
			GameInfo.getRender()->getWindowConfig().window->clear();
			onUpdate();
			GameInfo.getRender()->getWindowConfig().window->display();
		}
	}

	~RuncraftClient() = default;

private:
	GameLogger gameLogger;
	SoundManager soundManager;
	SceneManager sceneManager;
	input::Mouse mouse;
	input::Keyboard keyboard;
	std::shared_ptr<SystemEvents> gameEvents;
	RenderSystem render{"RunCrafts"};

	SoundManager* getSoundManager() override {
		return &soundManager;
	}
};

#endif //RUNCRAFT_RUNCRAFTCLIENT_HPP
