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
#include "../Game.hpp"
#include "Widgets.hpp"

class Button : public Widgets {
private:
    Texture button_normal;
    Texture button_clicked;
    Texture *button_current_ptr;
    sf::Text text;
    bool state;

    std::string button_path = "widgets.png";

    AudioPlayer *audio_player = new AudioPlayer("effect/gui_button_click");
public:
    explicit Button(const std::string &words, sf::Vector2f *location) {
        button_normal.load(button_path);
        button_normal.load(button_path);

        button_current_ptr = &button_normal;
        state = false;
        button_normal.setPosition(*location);
        button_clicked.setPosition(*location);
        text.setString(words);
        text.setPosition(location->x + 3, location->y + 3);
        text.setScale(1, 1);
    }

    ~Button() {
        delete audio_player;
        delete &text;
        delete &button_normal;
        delete &button_clicked;
    }

    void checkClick(sf::Vector2f mousePos) {
        if (mousePos.x > button_current_ptr->getPosition().x &&
            mousePos.x < (button_current_ptr->getPosition().x + button_current_ptr->getScale().x)) {
            if (mousePos.y > button_current_ptr->getPosition().y &&
                mousePos.y < (button_current_ptr->getPosition().y + button_current_ptr->getScale().y)) {
                setState(!state);
                audio_player->play();
            }
        }
    }

    void setState(bool which) {
        state = which;
        if (state) {
            button_current_ptr = &button_clicked;
            return;
        }
        button_current_ptr = &button_normal;
    }

    Button &setScale(float factorX, float factorY) {
        text.setScale(factorX, factorY);
        return *this;
    }

    Button &setScale(sf::Vector2f &factors) {
        text.setScale(factors);
        return *this;
    }

    Button &setText(std::string words) {
        text.setString(words);
        return *this;
    }

    bool getState() const { return state; }

    sf::Drawable *getRenderAble() override { return button_current_ptr->getRenderAble(); }

    sf::String getText() { return text.getString(); }

    void render() override {
        GameInfo.getRender()->render(this);
    }
};

#endif //RUNCRAFT_BUTTON_HPP
