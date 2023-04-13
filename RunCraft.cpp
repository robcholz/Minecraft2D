#include <SFML/Graphics.hpp>
#include "runcraft/events/SystemEvents.hpp"
#include "client/Menu.hpp"
#include "resource/TranslatableText.hpp"

int main() {
	Render render("RunCrafts");
	GameInfo.setRenderer(&render);
	Menu game_menu;
	SystemEvents game_main_event(&render);

	render.getWindow().setVerticalSyncEnabled(true);

	std::cout<<TranslatableText::getTranslatable("singleplayer",translatable::Category::GUI_BUTTON);

	while (GameInfo.getRender()->getWindow().isOpen()) {
		game_main_event.listen();
		GameInfo.listen(sf::Mouse::getPosition(GameInfo.getRender()->getWindow()),
		                sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));

		game_menu.renderMainMenu();

		GameInfo.getRender()->getWindow().display();
	}
}