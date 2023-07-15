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
		gameEvents = std::make_unique<SystemEvents>(this);
		sceneManager = std::make_unique<SceneManager>(this);

		gameEvents->onGamePause([this]() {
			this->sceneManager->pause();
		});
		gameEvents->onGameResume([this](){
			this->sceneManager->resume();
		});
	}

	void onUpdate() {
		gameEvents->update();
		keyboard->update();
		mouse->update();
		sceneManager->update();
		soundManager.update();
	}

	void onRender(){
		sceneManager->render();
	}

public:
	RuncraftClient() {
		onConfig();

		sceneManager->addScene("menu", [this]() { return new Menu(this); })
		            .addScene("world", [this]() { return new World(this); })
		            .setPair("menu", "world")
		            .setEntry("menu");
	}

	~RuncraftClient() = default;

	void run() {
		while (GameInfo.getRender()->getWindowConfig().window->isOpen()) {
			GameInfo.getRender()->getWindowConfig().window->clear();
			onUpdate();
			onRender();
			GameInfo.getRender()->getWindowConfig().window->display();
		}
	}

	SoundManager* getSoundManager() override {
		return &soundManager;
	}

	SceneManager* getSceneManager() override {
		return sceneManager.get();
	}

	input::mouse::Mouse* getMouse() override {
		return mouse.get();
	}

	input::keyboard::Keyboard* getKeyboard() override {
		return keyboard.get();
	}

private:
	GameLogger gameLogger;
	SoundManager soundManager;
	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<SystemEvents> gameEvents;
	input::Mouse mouse;
	input::Keyboard keyboard;
	RenderSystem render{"RunCrafts"};
};

#endif //RUNCRAFT_RUNCRAFTCLIENT_HPP
