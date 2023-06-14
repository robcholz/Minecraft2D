#include <SFML/Graphics.hpp>
#include "runcraft/events/SystemEvents.hpp"
#include "client/Menu.hpp"
#include "util/GameLogger.hpp"
#include "world/World.hpp"

using namespace block;

int main() {
	GameLogger game_logger;
	Render render("RunCrafts");
	GameInfo.setRenderer(&render);
	auto keyboard = input::PeripheralsFactory::getKeyboard();
	auto mouse=input::PeripheralsFactory::getMouse();
	auto game_main_event=SystemEvents::getInstance();
	//Menu menu;

 	World world;

	GameInfo.getConstExternalData()->windowState.getRender()->getWindowConfig().window->setVerticalSyncEnabled(true);

	while (GameInfo.getRender()->getWindowConfig().window->isOpen()) {
		game_main_event->update();

		GameInfo.getRender()->getWindowConfig().window->clear();

		keyboard->update();
		mouse->update();

		//menu.render();
		world.update();
		world.render();

		GameInfo.getRender()->getWindowConfig().window->display();
	}
}