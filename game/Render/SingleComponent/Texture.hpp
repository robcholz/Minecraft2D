//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_TEXTURE_HPP
#define RUNCRAFT_TEXTURE_HPP

#include <list>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../../GUI/GUI.hpp"
#include "SingleTextureComponent.hpp"

#pragma once

class Texture : public sf::Sprite, public SingleTextureComponent{
protected:
    sf::Texture sourceTexture;
	std::list<sf::Sprite *> textureAtlas;

public:
    Texture()= default;
    explicit Texture(const std::string &filename) : Sprite(sourceTexture) {
        if (!sourceTexture.loadFromFile(filePath + filename)){
            PLOG_DEBUG << ("CANNOT load sourceTexture from " + (filePath + filename));
        }
		textureAtlas.push_front(new sf::Sprite(sourceTexture));
    }
};

#endif //RUNCRAFT_TEXTURE_HPP
