//
// Created by robcholz on 5/4/23.
//

#ifndef RUNCRAFT_WORLD_HPP
#define RUNCRAFT_WORLD_HPP

#include <memory>
#include "entity/player/PlayerEntity.hpp"
#include "world/chunk/ChunkStream.hpp"
#include "gen/WorldGeneration.hpp"
#include "client/gui/hud/InGameBarHud.hpp"

class World : public WorldAccess, public SceneAccess {
public:
	explicit World(RuncraftClientAccess* runcraftClientAccess) {
		this->runcraftClientAccess = runcraftClientAccess;
		runcraftClientAccess->getSoundManager()->stopCurrentPlaying();
		player = std::make_unique<PlayerEntity>(runcraftClientAccess,this);
		player->getEntityPosition().setPosition(0, 90);
		worldGeneration=std::make_unique<WorldGeneration>(1234567);
		chunkStream = std::make_unique<chunk::ChunkStream>(this, 4, 4);
		chunkStream->setChunkGenerator([this](int chunkPos) { return worldGeneration->getChunk(chunkPos); });
		hud = std::make_unique<hud::InGameBarHud>(this);
	}

	~World() override = default;

	PlayerEntity* getPlayer() override {
		return player.get();
	}

	chunk::ChunkStream* getChunkStream() override {
		return chunkStream.get();
	}

	sf::View& getView() override {
		return view;
	}

	void onUpdate() override {
		chunkStream->update();
		player->update();
		updateCamera();
		hud->update();
	}

	void onRender() override {
		chunkStream->render();
		player->render();
		hud->render();
	}

private:
	float screenWidth = (float) GameInfo.getConstExternalData()->windowState.getScreenWidth();
	float screenHeight = (float) GameInfo.getConstExternalData()->windowState.getScreenHeight();
	sf::View view{sf::FloatRect{0.f, 0.f, screenWidth, screenHeight}};
	std::unique_ptr<PlayerEntity> player;
	std::unique_ptr<WorldGeneration> worldGeneration;
	std::unique_ptr<chunk::ChunkStream> chunkStream;
	std::unique_ptr<hud::InGameBarHud> hud;
	RuncraftClientAccess* runcraftClientAccess = nullptr;

	void updateCamera() {
		view.move((float) player->getEntityPosition().getPixelPosition().getOffset().x, (float) player->getEntityPosition().getPixelPosition().getOffset().z);
		GameInfo.getConstExternalData()->windowState.getRender()->getWindowConfig().window->setView(view);
		view.setCenter(player->getEntityPosition().getPixelPosition().getFloatX(), player->getEntityPosition().getPixelPosition().getFloatZ());
	}
};

#endif //RUNCRAFT_WORLD_HPP
