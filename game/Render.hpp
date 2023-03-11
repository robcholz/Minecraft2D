//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_RENDER_HPP
#define RUNCRAFT_RENDER_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include "widget/Widgets.hpp"

class Render {
private:
    sf::RenderWindow *window;
    unsigned int screenWidth;
    unsigned int screenHeight;

public:
    explicit Render(const std::string &windowName) {
        screenWidth = sf::VideoMode::getDesktopMode().height * 8 / 9;
        screenHeight = sf::VideoMode::getDesktopMode().height / 2;
        window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), windowName);

        //TODO:VSYNC is on here
        window->setVerticalSyncEnabled(true);
    }

    Render(int screenWidth, int screenHeight, const std::string &windowName) {
        screenWidth = screenWidth;
        screenHeight = screenHeight;
        window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), windowName);
    }

    sf::RenderWindow &getWindow() { return *window; }

    [[nodiscard]] unsigned int getScreenWidth() const { return window->getSize().x; }

    [[nodiscard]] unsigned int getScreenHeight() const { return window->getSize().y; }

    void render(const sf::Drawable &drawable, const sf::RenderStates &states = sf::RenderStates::Default) {
        window->draw(drawable, states);
    }

    void render(Widgets *widget) {
        window->draw(*widget->getRenderAble());
    }


};

#endif //RUNCRAFT_RENDER_HPP
