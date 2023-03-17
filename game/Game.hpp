//
// Created by robcholz on 3/4/23.
//

#ifndef RUNCRAFT_GAME_HPP
#define RUNCRAFT_GAME_HPP

#include "Audio.hpp"
#include "widget/Texture.hpp"
#include "util/utils.hpp"
#include "widget/Button.hpp"
#include "GameInfo.hpp"
#include "Initializers/ConsoleInitializer.h"
#include "widget/Background.hpp"

class Game {
private:
    /*Main Renderer*/
    Render *renderer;

    /*AudioList*/
    AudioList backgroundMusic;
    Once backgroundOnce;
    Background *backgroundTexture = new Background("background.png");
    Button *backgroundSinglePlayer = new Button("hello!", 300, 40, new sf::Vector2f(350, 200));
    Button *backgroundOptions;

public:
    explicit Game(const std::string &windowName) {
        renderer = new Render(windowName);
        //backgroundSinglePlayer = new Button("Singleplayer", new sf::Vector2f(100, 100));
        //backgroundOptions = new Button("Options", new sf::Vector2f(200, 200));

        PLOG_DEBUG << "Game started!";
    }

    Render *getRenderer() { return renderer; }

    void initResource() {
        initAudio();
        PLOG_DEBUG << "Initialize resources.";
    }

    void renderMainMenu() {
        if (backgroundOnce.once()) {
            backgroundTexture->fitToScreen();
            backgroundMusic.playRandomly();

            PLOG_DEBUG << "Rendered main menu!";
        }
        backgroundTexture->render();
        backgroundSinglePlayer->render();
        backgroundOptions->render();
    }

    void initAudio() {
        /*audio resources -- background*/
        backgroundMusic.addAudio("oxygène")
                .addAudio("beginning")
                .addAudio("beginning_2")
                .addAudio("moog_city_2");

        PLOG_DEBUG << "Initialize audio resources";
    }

    [[nodiscard]] unsigned int getScreenWidth() const { return renderer->getWindow().getSize().x; }

    [[nodiscard]] unsigned int getScreenHeight() const { return renderer->getWindow().getSize().y; }

};


#endif //RUNCRAFT_GAME_HPP
