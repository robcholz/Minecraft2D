//
// Created by robcholz on 3/17/23.
//

#ifndef RUNCRAFT_BACKGROUND_HPP
#define RUNCRAFT_BACKGROUND_HPP

#include "../../GameInfo.hpp"
#include "../../Render/SingleComponent/Texture.hpp"

class Background : public Texture {
public:
    explicit Background(const std::string &filename) : Texture(filename) {}

    Background &fitToScreen() {
        sprite.scale((float) GameInfo.getScreenWidth() / static_cast<float>(texture.getSize().x),
                     (float) GameInfo.getScreenHeight() / static_cast<float>(texture.getSize().y));
        return *this;
    }

    sf::Drawable *getRenderAble() override {
        return &sprite;
    }

    void render() override {
        GameInfo.getRender()->render(sprite);
    }
};

#endif //RUNCRAFT_BACKGROUND_HPP
