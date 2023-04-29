#include <SFML/Graphics.hpp>
#include "runcraft/events/SystemEvents.hpp"
#include "client/Menu.hpp"
#include "util/GameLogger.hpp"
#include "client/player/Player.hpp"
#include "client/scene/SceneManager.hpp"

int main() {
	GameLogger game_logger;
	Render render("RunCrafts");
	GameInfo.setRenderer(&render);
	SystemEvents game_main_event;
	Menu *menu_ptr;
	chunk::Chunk *chunk;
	SceneManager scene_manager;
	scene_manager.newScene("game_menu", nullptr, [&menu_ptr] { menu_ptr = new Menu; }, [&menu_ptr] { menu_ptr->render(); }, [&menu_ptr] { delete menu_ptr; })
	             .newScene("chunk", nullptr, [&chunk] { chunk = new chunk::Chunk; }, [&chunk] { chunk->render(); }, [&chunk] { delete chunk; })
	             .setEntry("game_menu");

	//Player player;

	GameInfo.getConstExternalData()->windowState.getRender()->getWindowConfig().window->setVerticalSyncEnabled(true);

	while (GameInfo.getRender()->getWindowConfig().window->isOpen()) {
		game_main_event.listen();
		GameInfo.getExternalData()->peripheralState.listen(
				sf::Mouse::getPosition(*GameInfo.getRender()->getWindowConfig().window),
				sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
		GameInfo.getRender()->getWindowConfig().window->clear();


		scene_manager.render();
		//chunk.render(); // for-DEBUG only

		GameInfo.getRender()->getWindowConfig().window->display();
	}
}