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
#include "../../Game.hpp"
#include "../../GameInfo.hpp"
#include "WidgetManager.hpp"

class Button : public Widget {
private:
    sf::Texture buttonNormal;
    sf::Texture buttonClicked;
    sf::Sprite *buttonCurrentPtr;
    sf::Font font;
    sf::Text message;

    std::shared_ptr<sf::IntRect> *intRectNormal;
    std::shared_ptr<sf::IntRect> *intRectClicked;
    std::shared_ptr<sf::Vector2f> *buttonSize;

    int width;
    const int height=50;
    bool state = false;

    AudioPlayer *audio_player = new AudioPlayer("effect/gui_button_click");
public:
    explicit Button(const std::string &words, int width = 200, sf::Vector2f *position = new sf::Vector2f(0, 0)) {
        this->width = width;

        buttonSize = new std::shared_ptr<sf::Vector2f>(new sf::Vector2f((float) width, (float) height));

        buttonCurrentPtr = new sf::Sprite;
        intRectNormal = new std::shared_ptr<sf::IntRect>(new sf::IntRect(0, 66, 200, 20));
        intRectClicked = new std::shared_ptr<sf::IntRect>(new sf::IntRect(0, 86, 200, 20));

        buttonNormal.loadFromFile(widgetAssetPath, **intRectNormal);
        buttonClicked.loadFromFile(widgetAssetPath, **intRectClicked);

        buttonCurrentPtr->setTexture(buttonNormal, buttonSize);
        buttonCurrentPtr->setScale((float) width / 200, 1);
        buttonCurrentPtr->setPosition(*position);

        font.loadFromFile("../resources/font/runcraft.ttf");
        //message.setFont(font);
        message.setString(words);
        message.setPosition(position->x + (float)this->width / 2 - message.findCharacterPos(0).x, position->y);
        message.setCharacterSize(50);

        renderAble.drawable = buttonCurrentPtr;
        renderAble.text = &message;
    }

    ~Button() {
        delete audio_player;
    }

    void listen(sf::Vector2i mousePos, bool isPressed) override {
        if ((float) mousePos.x > buttonCurrentPtr->getPosition().x
            && (float) mousePos.x < (buttonCurrentPtr->getPosition().x + (float) width)) {
            if ((float) mousePos.y > buttonCurrentPtr->getPosition().y
                && (float) mousePos.y < (buttonCurrentPtr->getPosition().y + (float) height)) {
                if (isPressed) {
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
            buttonCurrentPtr->setTexture(buttonClicked);
            return;
        }
        buttonCurrentPtr->setTexture(buttonNormal);
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

    [[maybe_unused]] int getWidth() const { return width; }

    void render() override {
        GameInfo.getRender()->render(*renderAble.drawable);
        GameInfo.getRender()->render(*renderAble.text);
    }

    RenderAble *getWidgetRenderAble() override {
        return &renderAble;
    }
};

#endif //RUNCRAFT_BUTTON_HPP
