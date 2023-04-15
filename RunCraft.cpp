#include <SFML/Graphics.hpp>
#include "runcraft/events/SystemEvents.hpp"
#include "client/Menu.hpp"
#include "block/Blocks.hpp"
#include "util/GameLogger.hpp"

int main() {
	GameLogger game_logger;
	Render render("RunCrafts");
	GameInfo.setRenderer(&render);
	Menu game_menu;
	SystemEvents game_main_event(&render);

	//sf::Sprite sprite;
	//BlockTextureLoader blockTexture("grass_block");
	//sprite.setTexture(*blockTexture.getBlockTextureTile(BlockDirectionType::UP));

	render.getWindow().setVerticalSyncEnabled(true);

	while (GameInfo.getRender()->getWindow().isOpen()) {
		game_main_event.listen();
		GameInfo.listen(sf::Mouse::getPosition(GameInfo.getRender()->getWindow()),
		                sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));

		game_menu.renderMainMenu();
		//GameInfo.getRender()->getWindow().draw(sprite);

		GameInfo.getRender()->getWindow().display();
	}
}