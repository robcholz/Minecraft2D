//
// Created by robcholz on 3/4/23.
//

#ifndef RUNCRAFT_GAME_HPP
#define RUNCRAFT_GAME_HPP

#include "Audio.hpp"
#include "widget/Texture.hpp"
#include "util/utils.hpp"
#include "widget/Button.hpp"

class Game {
private:
    /*Main Renderer*/
    Render *renderer;

    /*AudioList*/
    AudioList backgroundMusic;
    Once backgroundOnce;
    Texture *backgroundTexture = new Texture("background.png");
    Button *backgroundSinglePlayer{};
    Button *backgroundOptions{};
public:
    explicit Game(const std::string &windowName) {
        renderer = new Render(windowName);
        //backgroundSinglePlayer = new Button("Singleplayer", new sf::Vector2f(100, 100));
        //backgroundOptions = new Button("Options", new sf::Vector2f(200, 200));
    }

    Render *getRenderer() { return renderer; }

    void initResource() {
        initAudio();
    }

    void renderMainMenu() {
        if (backgroundOnce.once()) {
            backgroundTexture->fitToScreen();
            backgroundMusic.playRandomly();
        }
        backgroundTexture->render();
        //backgroundSinglePlayer->render();
        //backgroundOptions->render();
    }

    void initAudio() {
        /*audio resources -- background*/
        backgroundMusic.addAudio("oxygène")
                .addAudio("beginning")
                .addAudio("beginning_2")
                .addAudio("moog_city_2");
    }

    [[nodiscard]] unsigned int getScreenWidth() const { return renderer->getWindow().getSize().x; }

    [[nodiscard]] unsigned int getScreenHeight() const { return renderer->getWindow().getSize().y; }

};


#endif //RUNCRAFT_GAME_HPP
