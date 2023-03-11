#include <SFML/Graphics.hpp>
#include "game/Game.hpp"

int main() {
    Game game("RunCrafts");
    GameInfo.setRenderer(game.getRenderer());
    game.getRenderer()->getWindow().setVerticalSyncEnabled(true);

    game.initResource();

    while (game.getRenderer()->getWindow().isOpen()) {
        sf::Event event;

        while (game.getRenderer()->getWindow().pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                game.getRenderer()->getWindow().close();
        }
        game.getRenderer()->getWindow().clear();
        game.renderMainMenu();
        game.getRenderer()->getWindow().display();
    }
}