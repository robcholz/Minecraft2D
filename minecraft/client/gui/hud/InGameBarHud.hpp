//
// Created by robcholz on 6/15/23.
//

#ifndef MINECRAFT_INGAMEBARHUD_HPP
#define MINECRAFT_INGAMEBARHUD_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <vector>
#include "client/gui/GUI.hpp"
#include "Log.h"
#include "world/WorldAccess.hpp"
#include "entity/EntityAccess.hpp"
#include "HealthBarHud.hpp"
#include "ItemBarHud.hpp"
#include "ExperienceBarHud.hpp"
#include "HungerBarHud.hpp"
#include "DebugHud.hpp"


namespace hud {
	class InGameBarHud : public GUI {
	protected:
	public:
		explicit InGameBarHud(MinecraftClientAccess* minecraftClientAccess, WorldAccess* worldAccess) {
			healthBar = std::make_unique<HealthBarHud>(worldAccess);
			itemBar = std::make_unique<ItemBarHud>(worldAccess);
			experienceBar = std::make_unique<ExperienceBarHud>(worldAccess);
			hungerBarHud = std::make_unique<HungerBarHud>(worldAccess);
			debugHud = std::make_unique<DebugHud>(minecraftClientAccess, worldAccess);
		}

		void update() override {
			healthBar->update();
			itemBar->update();
			experienceBar->update();
			hungerBarHud->update();
			debugHud->update();
		}

		void render() override {
			healthBar->render();
			itemBar->render();
			experienceBar->render();
			hungerBarHud->render();
			debugHud->render();
		}

	private:
		std::unique_ptr<HealthBarHud> healthBar;
		std::unique_ptr<ItemBarHud> itemBar;
		std::unique_ptr<ExperienceBarHud> experienceBar;
		std::unique_ptr<HungerBarHud> hungerBarHud;
		std::unique_ptr<DebugHud> debugHud;
	};
}

#endif //MINECRAFT_INGAMEBARHUD_HPP
