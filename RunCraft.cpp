#include <SFML/Graphics.hpp>
#include "RunCraft/Events/SystemEvents.hpp"
#include "game/Menu.hpp"

int main() {
	Render render("RunCrafts");
	GameInfo.setRenderer(&render);
	Menu game_menu;
	SystemEvents game_main_event(&render);

	render.getWindow().setVerticalSyncEnabled(true);

	while (GameInfo.getRender()->getWindow().isOpen()) {
		game_main_event.listen();
		GameInfo.listen(sf::Mouse::getPosition(GameInfo.getRender()->getWindow()),
		                sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));

		game_menu.renderMainMenu();

		GameInfo.getRender()->getWindow().display();
	}
}