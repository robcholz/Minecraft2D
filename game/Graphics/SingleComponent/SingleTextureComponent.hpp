//
// Created by robcholz on 3/18/23.
//

#ifndef RUNCRAFT_SINGLETEXTURECOMPONENT_HPP
#define RUNCRAFT_SINGLETEXTURECOMPONENT_HPP

#include <SFML/Graphics/Drawable.hpp>
#include "../../GUI/GUI.hpp"

class SingleTextureComponent: public GUI{
public:
    virtual sf::Drawable *getRenderAble()=0;
};

#endif //RUNCRAFT_SINGLETEXTURECOMPONENT_HPP
