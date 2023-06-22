#include <SFML/Graphics.hpp>
#include "runcraft/events/SystemEvents.hpp"
#include "client/Menu.hpp"
#include "util/GameLogger.hpp"
#include "world/World.hpp"
#include "client/scene/SceneManager.hpp"

using namespace block;

int main() {
	GameLogger game_logger;
	Render render("RunCrafts");
	GameInfo.setRenderer(&render);
	auto keyboard = input::PeripheralsFactory::getKeyboard();
	auto mouse = input::PeripheralsFactory::getMouse();
	auto game_main_event = SystemEvents::getInstance();


	SceneManager scene_manager;
	scene_manager.addScene("menu", []() { return new Menu; })
	             .addScene("world", []() { return new World; })
				 .setPair("menu","world")
	             .setEntry("menu");

	sf::Color color(120,167,255);

	GameInfo.getConstExternalData()->windowState.getRender()->getWindowConfig().window->setVerticalSyncEnabled(true);

	while (GameInfo.getRender()->getWindowConfig().window->isOpen()) {
		game_main_event->update();
		keyboard->update();
		mouse->update();
		GameInfo.getRender()->getWindowConfig().window->clear(color);

		scene_manager.update();

		GameInfo.getRender()->getWindowConfig().window->display();
	}
}