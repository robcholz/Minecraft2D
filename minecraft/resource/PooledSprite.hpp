//
// Created by robcholz on 8/21/23.
//

#ifndef MINECRAFT_2D_POOLEDSPRITE_HPP
#define MINECRAFT_2D_POOLEDSPRITE_HPP


#include <SFML/Graphics/Sprite.hpp>
#include <mempool/MemoryPool.h>


namespace resource {
	class SpritePool : public AppShift::Memory::MemoryPool {
	public:
		SpritePool(const SpritePool&) = delete;

		SpritePool& operator=(const SpritePool&) = delete;

		static SpritePool& getInstance() {
			static SpritePool instance;
			return instance;
		}

	private:
		SpritePool() : AppShift::Memory::MemoryPool() {}

		~SpritePool() = default;
	};

	class Sprite : public sf::Sprite {
	public:
		void* operator new(size_t size) {
			void* memory = SpritePool::getInstance().allocate(size);
			return memory;
		}

		void operator delete(void* memory) {
			SpritePool::getInstance().free(memory);
		}
	};
}

#endif //MINECRAFT_2D_POOLEDSPRITE_HPP
