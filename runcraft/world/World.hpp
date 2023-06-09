//
// Created by robcholz on 5/4/23.
//

#ifndef RUNCRAFT_WORLD_HPP
#define RUNCRAFT_WORLD_HPP

#include "world/chunk/ChunkStream.hpp"

namespace world {

	class World {
	protected:
	private:
		float screenWidth = (float) GameInfo.getConstExternalData()->windowState.getScreenWidth();
		float screenHeight = (float) GameInfo.getConstExternalData()->windowState.getScreenHeight();
		sf::View view{sf::FloatRect{0.f, 0.f, screenWidth, screenHeight}};
		Player player;
		std::unique_ptr<chunk::ChunkStream> chunkStream;
	public:
		explicit World() {
			chunkStream = std::make_unique<chunk::ChunkStream>(&player, 4, 2);
			chunkStream->setChunkGenerator([](int chunkPos){return new chunk::Chunk(chunkPos);});
		}

		void update() {
			player.update();
			chunkStream->update();
			view.move(player.getOffset().x, player.getOffset().z);
			GameInfo.getConstExternalData()->windowState.getRender()->getWindowConfig().window->setView(view);
			view.setCenter(player.getPosition()->getX(),player.getPosition()->getZ());
		}

		void render() {
			chunkStream->render();
			player.render();
		}
	};
}

#endif //RUNCRAFT_WORLD_HPP
