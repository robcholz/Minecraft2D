//
// Created by robcholz on 3/8/23.
//

#ifndef RUNCRAFT_BUTTON_HPP
#define RUNCRAFT_BUTTON_HPP

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include "../Game.hpp"
#include "../GameInfo.hpp"
#include "Widget.hpp"

class Button : public Widget {
private:
    sf::Texture buttonNormal;
    sf::Texture buttonClicked;
    sf::RectangleShape *buttonCurrentPtr;
    sf::Text message;

    std::shared_ptr<sf::IntRect> *intRectNormal;
    std::shared_ptr<sf::IntRect> *intRectClicked;
    std::shared_ptr<sf::Vector2f> *buttonSize;

    struct size {
        int width;
        int height;
    } size{};
    bool state;

    AudioPlayer *audio_player = new AudioPlayer("effect/gui_button_click");
public:
    explicit Button(const std::string &words, int width = 200, int height = 20,
                    sf::Vector2f *position = new sf::Vector2f(0, 0)) {
        size.width = width;
        size.height = height;

        buttonSize = new std::shared_ptr<sf::Vector2f>(new sf::Vector2f((float) width, (float) height));

        buttonCurrentPtr = new sf::RectangleShape(**buttonSize);
        intRectNormal = new std::shared_ptr<sf::IntRect>(new sf::IntRect(0, 66, 200, 20));
        intRectClicked = new std::shared_ptr<sf::IntRect>(new sf::IntRect(0, 86, 200, 20));

        buttonNormal.loadFromFile(widgetAssetPath, **intRectNormal);
        buttonClicked.loadFromFile(widgetAssetPath, **intRectClicked);

        buttonCurrentPtr->setTexture(&buttonNormal);
        buttonCurrentPtr->setScale((float) width / 200, (float) height / 20);
        buttonCurrentPtr->setPosition(*position);

        state = false;

        message.setString(words);
        message.setPosition(position->x + 3, position->y + 3);
        message.setScale(1, 1);
    }

    ~Button() {
        delete audio_player;
    }

    void checkClick(sf::Vector2i mousePos, bool pressed_) {
        if (mousePos.x > buttonCurrentPtr->getPosition().x
            && mousePos.x < (buttonCurrentPtr->getPosition().x + size.width)) {
            if (mousePos.y > buttonCurrentPtr->getPosition().y
                && mousePos.y < (buttonCurrentPtr->getPosition().y + size.height)) {
                if (pressed_) {
                    audio_player->play();
                    std::cout << "Yes" << std::endl;
                }
                if (!pressed()) {
                    setState(!state);
                }
            }
        } else {
            if (pressed()) {
                setState(false);
            }
        }
    }

    void setState(bool which) {
        state = which;
        if (state) {
            buttonCurrentPtr->setTexture(&buttonClicked);
            return;
        }
        buttonCurrentPtr->setTexture(&buttonNormal);
    }

    Button &setScale(float factorX, float factorY) {
        message.setScale(factorX, factorY);
        return *this;
    }

    Button &setScale(sf::Vector2f &factors) {
        message.setScale(factors);
        return *this;
    }

    Button &setText(std::string words) {
        message.setString(words);
        return *this;
    }

    bool pressed() const { return state; }

    [[maybe_unused]] struct size getSize() const { return size; }

    Button *getRenderable(){
        return this;
    }

    sf::Drawable *getRenderAble() override { return nullptr; }

    sf::String getText() { return message.getString(); }

    void render() override {
        GameInfo.getRender()->render(this);
        GameInfo.getRender()->render(message);
    }
};

#endif //RUNCRAFT_BUTTON_HPP
