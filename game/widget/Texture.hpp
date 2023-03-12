//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_TEXTURE_HPP
#define RUNCRAFT_TEXTURE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../Game.hpp"
#include "Widgets.hpp"
#include "../GameInfo.hpp"

class Texture : public sf::Sprite, public Widgets {
private:
    sf::Sprite sprite;
    sf::Texture texture;

    std::string filepath = "../resources/gui/";
public:
    Texture()= default;
    explicit Texture(const std::string &filename) : Sprite(texture) {
        if (!texture.loadFromFile(filepath + filename))
            std::cout << "CANNOT load texture from " + filepath + filename << std::endl;
        sprite.setTexture(texture);
    }

    Texture &load(const std::string &filename){
        if (!texture.loadFromFile(filepath + filename))
            std::cout << "CANNOT load texture from " + filepath + filename << std::endl;
        sprite.setTexture(texture);

        return *this;
    }

    Texture &fitToScreen() {
        sprite.scale((float) GameInfo.getScreenWidth() / texture.getSize().x,
                     (float) GameInfo.getScreenHeight() / texture.getSize().y);
        return *this;
    }

    sf::Drawable *getRenderAble() override {
        return &sprite;
    }

    void render() override {
        GameInfo.getRender()->render(this);
    }
};

#endif //RUNCRAFT_TEXTURE_HPP
