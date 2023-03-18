#include <SFML/Graphics.hpp>
#include "game/Game.hpp"

int main() {
    Game game("RunCrafts");
    GameInfo.setRenderer(game.getRenderer());
    game.getRenderer()->getWindow().setVerticalSyncEnabled(true);

    game.initResource();

    std::cout << GameInfo.getScreenWidth() << " " << GameInfo.getScreenHeight() << std::endl;


    while (game.getRenderer()->getWindow().isOpen()) {
        sf::Event event{};

        while (game.getRenderer()->getWindow().pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                game.getRenderer()->getWindow().close();
                PLOG_DEBUG << "Cancel the game!";
            }
        }
        game.getRenderer()->getWindow().clear();
        game.renderMainMenu();
        game.getRenderer()->getWindow().display();
        std::cout << sf::Mouse::getPosition(game.getRenderer()->getWindow()).x << " "
                  << sf::Mouse::getPosition(game.getRenderer()->getWindow()).y << std::endl;
    }
}