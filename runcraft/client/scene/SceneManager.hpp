//
// Created by robcholz on 4/29/23.
//

#ifndef RUNCRAFT_SCENEMANAGER_HPP
#define RUNCRAFT_SCENEMANAGER_HPP

#include <list>
#include <map>
#include <utility>


class SceneManager {
private:
	typedef std::function<void()> FuncPtr;
	FuncPtr currentScenePtr = nullptr;
	struct Scene { Scene *scene; FuncPtr onCreate = nullptr, onRender = nullptr, onDestroy = nullptr; };
	std::map<std::string, Scene> sceneMap;
public:
	explicit SceneManager() = default;

	SceneManager &newScene(const std::string &id, Scene *scene, FuncPtr funcOnCreate, FuncPtr funcOnRender, FuncPtr funcOnDestroy) {
		sceneMap.insert({id, Scene{scene, std::move(funcOnCreate), std::move(funcOnRender), std::move(funcOnDestroy)}});
		return *this;
	}

	void setEntry(const std::string &id) {
		sceneMap[id].onCreate();
		currentScenePtr = sceneMap[id].onRender;
	}

	void render() {
		currentScenePtr();
		/*
		if (!currentScenePtr->nextScene().empty()) {
			auto prevScenePtr = currentScenePtr;
			currentScenePtr = sceneMap[currentScenePtr->nextScene()];
			delete prevScenePtr;
		}
		 */
	}
};

#endif //RUNCRAFT_SCENEMANAGER_HPP
