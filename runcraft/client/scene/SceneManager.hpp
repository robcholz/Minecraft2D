//
// Created by robcholz on 4/29/23.
//

#ifndef RUNCRAFT_SCENEMANAGER_HPP
#define RUNCRAFT_SCENEMANAGER_HPP

#include <map>
#include <string>
#include <functional>
#include "client/RuncraftClientAccess.hpp"
#include "SceneAccess.hpp"


class SceneManager {
private:
	using String = std::string;
public:
	explicit SceneManager(RuncraftClientAccess* runcraftClientAccess) {
		this->runcraftClientAccess = runcraftClientAccess;
	}

	~SceneManager() = default;

	SceneManager& setEntry(const String& name) {
		scene = sceneCallableMap[name]();
		sceneName = name;
		return *this;
	}

	SceneManager& addScene(const String& name, std::function<SceneAccess*()> function) {
		sceneCallableMap.insert({name, std::move(function)});
		return *this;
	}

	SceneManager& setPair(const String& first, const String& second) {
		pairMap.insert({first, second});
		return *this;
	}

	void pause() {
		this->isPaused = true;
	}

	void resume() {
		this->isPaused = false;
	}

	void update() {
		if (!this->isPaused) {
			if (scene->isRunning()) {
				scene->onUpdate();
			} else {
				if (scene->isTerminated()) {
					this->runcraftClientAccess->getSoundManager()->clearQueue();
					sceneName = pairMap[sceneName];
					delete scene;
					scene = sceneCallableMap[sceneName]();
				}
			}
		}
	}

	void render() {
		if (scene->isRunning()) {
			scene->onRender();
		} else {
			if (scene->isPaused()) {
				scene->onRender();
			}
		}
	}

private:
	RuncraftClientAccess* runcraftClientAccess = nullptr;
	std::map<String, std::function<SceneAccess*()>> sceneCallableMap;
	std::map<String, String> pairMap;
	SceneAccess* scene = nullptr;
	bool isPaused = false;
	String sceneName;
};

#endif //RUNCRAFT_SCENEMANAGER_HPP
