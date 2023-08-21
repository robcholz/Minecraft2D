//
// Created by robcholz on 8/21/23.
//

#ifndef MINECRAFT_2D_WORLDRENDERER_HPP
#define MINECRAFT_2D_WORLDRENDERER_HPP


#include <SFML/Graphics/View.hpp>
#include "world/WorldAccess.hpp"
#include "RenderSystem.hpp"


class WorldRenderer {
private:
	using ChunkPosT = coordinate::ChunkPositionT;
protected:
public:
	WorldRenderer() = delete;

	explicit WorldRenderer(WorldAccess* worldAccess) {
		this->worldAccess = worldAccess;
	}

	WorldRenderer(const WorldRenderer&) = delete;

	WorldRenderer& operator=(const WorldRenderer&) = delete;

	~WorldRenderer() = default;

	void render() {
		renderChunks();
	}

private:
	WorldAccess* worldAccess = nullptr;

	void renderChunks() {
		// we need to transform this rect to block position
		auto center = worldAccess->getView().getCenter();
		auto size = worldAccess->getView().getSize();
		auto viewArea = sf::FloatRect(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);
		auto leftBlockCorner = coordinate::Position<coordinate::PixelPos>(math::ceil(viewArea.left),
		                                                                  math::ceil(viewArea.top + viewArea.height)).get<coordinate::BlockPos>();
		auto rightBlockCorner = coordinate::Position<coordinate::PixelPos>(math::ceil(viewArea.left + viewArea.width),
		                                                                   math::ceil(viewArea.top - viewArea.height)).get<coordinate::BlockPos>();
		for (auto x_world = leftBlockCorner.x; x_world <= rightBlockCorner.x; x_world++) {
			for (auto z_world = leftBlockCorner.z; z_world <= rightBlockCorner.z; z_world++) {
				auto block = worldAccess->getChunkManager()->getBlock(coordinate::BlockPos(x_world, z_world));
				RenderSystem::render(*block);
			}
		}

	}
};

#endif //MINECRAFT_2D_WORLDRENDERER_HPP
