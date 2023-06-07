#include <SFML/Graphics.hpp>
#include "runcraft/events/SystemEvents.hpp"
#include "client/Menu.hpp"
#include "util/GameLogger.hpp"
#include "client/player/Player.hpp"
#include "world/World.hpp"
#include "world/storage/SaveHelper.hpp"

using namespace block;

int main() {
	GameLogger game_logger;
	Render render("RunCrafts");
	GameInfo.setRenderer(&render);
	auto keyboard = input::PeripheralsFactory::getKeyboard();
	SystemEvents game_main_event;
	//block::blocks::Blocks Blocks;
	Menu menu;

	chunk::Chunk chunk_1(0);
	SaveHelper save_helper{"TestChunkSave",SaveHelper::ModeType::CREATE};
	save_helper.saveChunk(&chunk_1);
	SaveHelper save_helper_1{"TestChunkSave",SaveHelper::ModeType::READ};
	auto loadded=save_helper_1.loadChunk(0); // TODO: NOTE dataoverflowed while deserializing <- problem occurred here

	//Player player;

	GameInfo.getConstExternalData()->windowState.getRender()->getWindowConfig().window->setVerticalSyncEnabled(true);

	while (GameInfo.getRender()->getWindowConfig().window->isOpen()) {
		game_main_event.listen();
		GameInfo.getExternalData()->peripheralState.listen(
				sf::Mouse::getPosition(*GameInfo.getRender()->getWindowConfig().window),
				sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));

		GameInfo.getRender()->getWindowConfig().window->clear();

		keyboard->update();

		menu.render();
		//player.update();
		//chunk_1.render();
		//loadded->render();

		//loaded_chunk->render();
		//player.render();
		//world.load();// for-DEBUG only
		//world.render();

		GameInfo.getRender()->getWindowConfig().window->display();
	}
}