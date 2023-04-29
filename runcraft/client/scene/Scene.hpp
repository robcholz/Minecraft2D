//
// Created by robcholz on 4/29/23.
//

#ifndef RUNCRAFT_SCENE_HPP
#define RUNCRAFT_SCENE_HPP

class Scene {
private:

public:
	virtual void onInitialize() = 0;

	virtual void onRender() = 0;

	virtual void onDestroy() = 0;
};

#endif //RUNCRAFT_SCENE_HPP
