//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_WIDGETS_HPP
#define RUNCRAFT_WIDGETS_HPP

#include <SFML/Graphics/Drawable.hpp>

class Widgets {
public:
    virtual sf::Drawable *getRenderAble() = 0;
    virtual void render()=0;
};

#endif //RUNCRAFT_WIDGETS_HPP
