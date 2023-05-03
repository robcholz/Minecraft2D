#include <SFML/Graphics.hpp>
#include "runcraft/events/SystemEvents.hpp"
#include "client/Menu.hpp"
#include "util/GameLogger.hpp"
#include "client/player/Player.hpp"

int main() {
	GameLogger game_logger;
	Render render("RunCrafts");
	GameInfo.setRenderer(&render);
	SystemEvents game_main_event;
	//Menu menu;
	chunk::Chunk chunk(0);
	chunk::Chunk chunk_1(1);
	chunk::Chunk chunk_2(2);
	chunk::Chunk chunk_3(3);
	chunk::Chunk chunk_4(4);
	chunk::Chunk chunk_5(5);
	chunk::Chunk chunk_6(6);
	//SceneManager scene_manager;

	//Player player;

	GameInfo.getConstExternalData()->windowState.getRender()->getWindowConfig().window->setVerticalSyncEnabled(true);

	while (GameInfo.getRender()->getWindowConfig().window->isOpen()) {
		game_main_event.listen();
		GameInfo.getExternalData()->peripheralState.listen(
				sf::Mouse::getPosition(*GameInfo.getRender()->getWindowConfig().window),
				sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
		GameInfo.getRender()->getWindowConfig().window->clear();


		//menu.render();
		chunk.render(); // for-DEBUG only
		chunk_1.render();
		chunk_2.render();
		chunk_3.render();
		chunk_4.render();
		chunk_5.render();
		chunk_6.render();

		GameInfo.getRender()->getWindowConfig().window->display();
	}
}