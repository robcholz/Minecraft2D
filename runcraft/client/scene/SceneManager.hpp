//
// Created by robcholz on 4/29/23.
//

#ifndef RUNCRAFT_SCENEMANAGER_HPP
#define RUNCRAFT_SCENEMANAGER_HPP

#include <map>
#include <string>
#include <functional>
#include "SceneAccess.hpp"


class SceneManager {
private:
	using String = std::string;
	std::map<String, std::function<SceneAccess*()>> sceneCallableMap;
	std::map<String, String> pairMap;

	SceneAccess* scene = nullptr;
	String sceneName;

public:
	explicit SceneManager() = default;

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

	void update() {
		if (scene->isRunning()) {
			scene->onUpdate();
			scene->onRender();
		} else {
			if (scene->isPaused()) {
				scene->onRender();
			}
			if (scene->isTerminated()) {
				sceneName = pairMap[sceneName];
				delete scene;
				scene = sceneCallableMap[sceneName]();
			}
		}
	}
};

#endif //RUNCRAFT_SCENEMANAGER_HPP
