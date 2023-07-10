//
// Created by robcholz on 6/15/23.
//

#ifndef RUNCRAFT_INGAMEBARHUD_HPP
#define RUNCRAFT_INGAMEBARHUD_HPP

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


namespace hud {
	class InGameBarHud : public GUI {
	protected:
	public:
		explicit InGameBarHud(WorldAccess* worldAccess) {
			healthBar = std::make_unique<HealthBarHud>(worldAccess);
			itemBar = std::make_unique<ItemBarHud>(worldAccess);
			experienceBar = std::make_unique<ExperienceBarHud>(worldAccess);
			hungerBarHud = std::make_unique<HungerBarHud>(worldAccess);
		}

		void update() override {
			healthBar->update();
			itemBar->update();
			experienceBar->update();
			hungerBarHud->update();
		}

		void render() override {
			healthBar->render();
			itemBar->render();
			experienceBar->render();
			hungerBarHud->render();
		}

	private:
		std::unique_ptr<HealthBarHud> healthBar;
		std::unique_ptr<ItemBarHud> itemBar;
		std::unique_ptr<ExperienceBarHud> experienceBar;
		std::unique_ptr<HungerBarHud> hungerBarHud;
	};
}

#endif //RUNCRAFT_INGAMEBARHUD_HPP
