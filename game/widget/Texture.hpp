//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_TEXTURE_HPP
#define RUNCRAFT_TEXTURE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "GUI.hpp"

class Texture : public sf::Sprite, public GUI{
protected:
    sf::Sprite sprite;
    sf::Texture texture;

public:
    Texture()= default;
    explicit Texture(const std::string &filename) : Sprite(texture) {
        if (!texture.loadFromFile(filePath + filename))
            std::cout << "CANNOT load texture from " + filePath + filename << std::endl;
        sprite.setTexture(texture);
    }

    ~Texture() override =default;

    Texture &load(const std::string &filename){
        if (!texture.loadFromFile(filePath + filename))
            std::cout << "CANNOT load texture from " + filePath + filename << std::endl;
        sprite.setTexture(texture);

        return *this;
    }
};

#endif //RUNCRAFT_TEXTURE_HPP
