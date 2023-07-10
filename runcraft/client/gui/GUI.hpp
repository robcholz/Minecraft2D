//
// Created by robcholz on 3/17/23.
//
#pragma once

#ifndef RUNCRAFT_GUI_HPP
#define RUNCRAFT_GUI_HPP


#include <string>
#include <SFML/Graphics/Drawable.hpp>

class GUI {
public:
	virtual void update() = 0;
	virtual void render() = 0;
};

#endif //RUNCRAFT_GUI_HPP
