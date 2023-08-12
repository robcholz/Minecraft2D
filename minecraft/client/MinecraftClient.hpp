//
// Created by robcholz on 6/15/23.
//

#ifndef MINECRAFT_MINECRAFTCLIENT_HPP
#define MINECRAFT_MINECRAFTCLIENT_HPP


#include "sound/SoundManager.hpp"
#include "client/scene/SceneManager.hpp"
#include "MinecraftClientAccess.hpp"
#include "Menu.hpp"
#include "world/World.hpp"
#include "util/GameLogger.hpp"
#include "MinecraftVersion.hpp"


class MinecraftClient : public MinecraftClientAccess {
protected:
	void onConfig() {
		MinecraftVersion::init();
		RenderSystem::init(MinecraftVersion::getProductionName(), "grass_block_side");
		mouse = input::PeripheralsFactory::getMouse();
		keyboard = input::PeripheralsFactory::getKeyboard();
		gameEvents = std::make_unique<SystemEvents>(this);
		sceneManager = std::make_unique<SceneManager>(this);

		gameEvents->onGamePause([this]() {
			this->sceneManager->pause();
		});
		gameEvents->onGameResume([this]() {
			this->sceneManager->resume();
		});
		gameEvents->onWindowResize([]() {
			PLOG_DEBUG << "Yes";
			//sf::FloatRect visibleArea(0, 0, , RenderSystem::getScreenHeight());
			//RenderSystem::getWindow()->setView(sf::View(visibleArea));
		});
	}

	void onUpdate() {
		gameEvents->update();
		keyboard->update();
		mouse->update();
		sceneManager->update();
		soundManager.update();
	}

	void onRender() {
		sceneManager->render();
	}

public:
	MinecraftClient(){
		onConfig();
		sceneManager->addScene("menu", [this]() { return new Menu(this); })
		            .addScene("world", [this]() { return new World(this); })
		            .setPair("menu", "world")
		            .setEntry("menu");
	}

	~MinecraftClient() {
		RenderSystem::exit();
	};

	void run() {
		while (RenderSystem::getWindow()->isOpen()) {
			RenderSystem::clearWindow(sf::Color(120, 167, 255));
			RenderSystem::updateScreenData();
			onUpdate();
			onRender();
			RenderSystem::display();
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
	SoundManager soundManager;
	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<SystemEvents> gameEvents;
	input::Mouse mouse;
	input::Keyboard keyboard;
};

#endif //MINECRAFT_MINECRAFTCLIENT_HPP
