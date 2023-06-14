//
// Created by robcholz on 5/4/23.
//

#ifndef RUNCRAFT_WORLD_HPP
#define RUNCRAFT_WORLD_HPP

#include <memory>
#include "client/player/Player.hpp"
#include "world/chunk/ChunkStream.hpp"

class World : public WorldAccess {
private:
	float screenWidth = (float) GameInfo.getConstExternalData()->windowState.getScreenWidth();
	float screenHeight = (float) GameInfo.getConstExternalData()->windowState.getScreenHeight();
	sf::View view{sf::FloatRect{0.f, 0.f, screenWidth, screenHeight}};
	std::unique_ptr<Player> player;
	std::unique_ptr<chunk::ChunkStream> chunkStream;

	void updateCamera() {
		view.move((float) player->getEntityPosition().getPixelPosition().getOffset().x, (float) player->getEntityPosition().getPixelPosition().getOffset().z);
		GameInfo.getConstExternalData()->windowState.getRender()->getWindowConfig().window->setView(view);
		view.setCenter(player->getEntityPosition().getPixelPosition().getFloatX(), player->getEntityPosition().getPixelPosition().getFloatZ());
	}

public:
	explicit World() {
		player = std::make_unique<Player>(this);
		player->getEntityPosition().setPosition(0, 8);
		chunkStream = std::make_unique<chunk::ChunkStream>(this, 4, 2);
		chunkStream->setChunkGenerator([](int chunkPos) { return new chunk::Chunk(chunkPos); });
	}

	Player* getPlayer() override {
		return player.get();
	}

	chunk::ChunkStream* getChunkStream() override {
		return chunkStream.get();
	}

	void update() {
		chunkStream->update();
		player->update();
		updateCamera();
		auto test = chunk::Chunk::toChunkSettings(player->getEntityPosition().getBlockPosition());
		PLOG_DEBUG << "BlockPos X: " << player->getEntityPosition().getBlockPosition().getX()
		           << " Y: " << player->getEntityPosition().getBlockPosition().getZ();
		PLOG_DEBUG<<"ChunkPos: "<<test.chunkPos<<" X: "<<test.chunkBlockPos.getX()<<" Y: "<<test.chunkBlockPos.getZ();
	}

	void render() {
		chunkStream->render();
		player->render();
	}


};

#endif //RUNCRAFT_WORLD_HPP
