//
// Created by robcholz on 3/17/23.
//

#ifndef RUNCRAFT_GUI_HPP
#define RUNCRAFT_GUI_HPP

#include <string>
#include <SFML/Graphics/Drawable.hpp>

class GUI {
protected:
    std::string filePath = "../resources/gui/";
public:
    virtual sf::Drawable *getRenderAble() = 0;

    virtual void render() = 0;
};

#endif //RUNCRAFT_GUI_HPP
