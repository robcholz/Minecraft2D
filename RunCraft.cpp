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
	//Menu menu;

 	world::World world;

	GameInfo.getConstExternalData()->windowState.getRender()->getWindowConfig().window->setVerticalSyncEnabled(true);

	while (GameInfo.getRender()->getWindowConfig().window->isOpen()) {
		game_main_event.listen();
		GameInfo.getExternalData()->peripheralState.listen(
				sf::Mouse::getPosition(*GameInfo.getRender()->getWindowConfig().window),
				sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));

		GameInfo.getRender()->getWindowConfig().window->clear();

		keyboard->update();

		//menu.render();
		world.update();// for-DEBUG only
		world.render();

		GameInfo.getRender()->getWindowConfig().window->display();
	}
}