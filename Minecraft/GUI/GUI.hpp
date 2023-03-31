//
// Created by robcholz on 3/17/23.
//

#ifndef RUNCRAFT_GUI_HPP
#define RUNCRAFT_GUI_HPP

#pragma once

#include <string>
#include <SFML/Graphics/Drawable.hpp>

class GUI {
protected:
    std::string guiFilePath = "../resources/gui/";
	std::string fontFilePath="../resources/font/";
public:
    virtual void render()=0;
};

#endif //RUNCRAFT_GUI_HPP
