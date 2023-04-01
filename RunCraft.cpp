#include <SFML/Graphics.hpp>
#include "RunCraft/Events/SystemEvents.hpp"
#include "game/Menu.hpp"

int main() {
	Menu game_menu("RunCrafts");
	SystemEvents game_main_event(&game_menu);
	GameInfo.setRenderer(game_menu.getRenderer());

	game_menu.getRenderer()->getWindow().setVerticalSyncEnabled(true);

	while (GameInfo.getRender()->getWindow().isOpen()) {
		game_main_event.listen();
		GameInfo.listen(sf::Mouse::getPosition(GameInfo.getRender()->getWindow()),
		                sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));

		game_menu.renderMainMenu();

		GameInfo.getRender()->getWindow().display();
	}
}