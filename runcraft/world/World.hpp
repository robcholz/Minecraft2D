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
		chunk::ChunkStream chunkStream;
	public:
		void load() {

		}

		void render() {
			chunkStream.render();
		}
	};
}

#endif //RUNCRAFT_WORLD_HPP
