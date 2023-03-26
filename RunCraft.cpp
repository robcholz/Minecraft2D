#include <SFML/Graphics.hpp>
#include "game/Game.hpp"
#include "game/Events/SystemEvents.hpp"

int main() {
    Game game("RunCrafts");
	SystemEvents game_main_event(&game);
    GameInfo.setRenderer(game.getRenderer());

    game.getRenderer()->getWindow().setVerticalSyncEnabled(true);

    game.initResource();


    while (game.getRenderer()->getWindow().isOpen()) {
		game_main_event.listen();
        //game.getRenderer()->getWindow().clear();
        game.renderMainMenu();

        game.getRenderer()->getWindow().display();
		/*
		std::cout << sf::Mouse::getPosition(game.getRenderer()->getWindow()).x << " "
                  << sf::Mouse::getPosition(game.getRenderer()->getWindow()).y << std::endl;
		*/
    }
}