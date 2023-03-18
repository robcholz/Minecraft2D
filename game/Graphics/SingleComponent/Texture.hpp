//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_TEXTURE_HPP
#define RUNCRAFT_TEXTURE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../../GUI/GUI.hpp"
#include "SingleTextureComponent.hpp"

class Texture : public sf::Sprite, public SingleTextureComponent{
protected:
    sf::Sprite sprite;
    sf::Texture texture;

public:
    Texture()= default;
    explicit Texture(const std::string &filename) : Sprite(texture) {
        if (!texture.loadFromFile(filePath + filename)){
            PLOG_DEBUG << ("CANNOT load texture from " + (filePath + filename));
        }
        sprite.setTexture(texture);
    }

    ~Texture() override =default;

    Texture &load(const std::string &filename){
        if (!texture.loadFromFile(filePath + filename)){
            PLOG_DEBUG << "CANNOT load texture from " + filePath + filename;
        }
        sprite.setTexture(texture);

        return *this;
    }
};

#endif //RUNCRAFT_TEXTURE_HPP
