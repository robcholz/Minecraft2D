//
// Created by robcholz on 6/16/23.
//

#ifndef RUNCRAFT_HEALTHBARHUD_HPP
#define RUNCRAFT_HEALTHBARHUD_HPP

#include <vector>
#include <memory>
#include <optional>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "entity/EntityAccess.hpp"
#include "world/WorldAccess.hpp"
#include "client/gui/GUI.hpp"
#include "Log.h"
#include "util/math/Random.hpp"

namespace hud {
	class HealthBarHud : public GUI {
	private:
		using String = std::string;
		using PixelPosT = coordinate::PixelPositonT;
		using SpriteSmartPtr = std::unique_ptr<sf::Sprite>;
	protected:
		String iconAssetPath = guiFilePath + "/icons.png";
	public:
		explicit HealthBarHud(WorldAccess* worldAccess) {
			this->worldAccess = worldAccess;
			// (16,0) (24,8)
			darkOutlineHeartTexture.loadFromFile(iconAssetPath, sf::IntRect{16, 0, 9, 9});
			// (25,0) (33,8)
			whiteOutlineHeartTexture.loadFromFile(iconAssetPath, sf::IntRect{25, 0, 9, 9});
			// (53,1) (59,7)
			redHeartTexture.loadFromFile(iconAssetPath, sf::IntRect{53, 1, 7, 7});
			// (62,1) (68,7)
			halfRedHeartTexture.loadFromFile(iconAssetPath, sf::IntRect{62, 1, 7, 7});
			// (71,1) (77,7)
			pinkHeartTexture.loadFromFile(iconAssetPath, sf::IntRect{71, 1, 7, 7});
			// (80,1) (86,7)
			halfPinkHeartTexture.loadFromFile(iconAssetPath, sf::IntRect{80, 1, 7, 7});
			// (180,1) (186,7)
			emptyHeartTexture.loadFromFile(iconAssetPath, sf::IntRect{180, 1, 7, 7});

			createCombinedHeart();
			setHeartCapacity(MAX_HEARTS);
			for (auto i = 0; i < MAX_HEARTS; ++i) loadHeartOutline(i, &darkOutlineHeartTexture);
			for (auto j = 0; j < MAX_HEARTS; ++j) loadHeart(j, &redHeartTexture);
			PLOG_DEBUG << "Loaded HealthBar HUD!";
		}

		void setPosition(PixelPosT x, PixelPosT z) {
			x_pos = x;
			z_pos = z;
		}

		void update() {
			updateHudPosition();
			updateHealth();
		}

		void render() override {
			for (auto& v: healthBarSpritesContainer) {
				GameInfo.getRender()->render(*v.first->get());
				GameInfo.getRender()->render(*v.second->get());
			}
		}

	private:
		// pair.first is the outline sprite; pair.second is the heart sprite
		std::vector<std::pair<std::optional<SpriteSmartPtr>, std::optional<SpriteSmartPtr>>> healthBarSpritesContainer; // 10 by default is 20 health value
		sf::Texture darkOutlineHeartTexture;
		sf::Texture whiteOutlineHeartTexture;
		sf::Texture redHeartTexture;
		sf::Texture pinkHeartTexture;
		sf::Texture halfRedHeartTexture;
		sf::Texture halfPinkHeartTexture;
		sf::Texture redPinkHeartTexture;
		sf::Texture emptyHeartTexture;
		WorldAccess* worldAccess = nullptr;
		static constexpr int MAX_HEARTS = 10;
		static constexpr int DAMAGE_EFFECT_DURATION = 60 * 3; // seconds
		short last_health = 0;
		PixelPosT x_pos = 0;
		PixelPosT z_pos = 0;

		static inline int delay_timer = -1;

		void createCombinedHeart() {
			auto combined_image = pinkHeartTexture.copyToImage();
			for (auto x = 0; x < 4; ++x) {
				for (int y = 0; y < 7; ++y) {
					combined_image.setPixel(x, y, halfRedHeartTexture.copyToImage().getPixel(x, y));
				}
			}
			redPinkHeartTexture.loadFromImage(combined_image);
		}

		void setHeartCapacity(int number) {
			for (auto i = 0; i < number; ++i) {
				auto sprite_heart = std::make_unique<sf::Sprite>();
				auto sprite_outline = std::make_unique<sf::Sprite>();
				healthBarSpritesContainer.emplace_back(std::move(sprite_outline), std::move(sprite_heart));
			}
		}

		void loadHeartOutline(int number, sf::Texture* outlineTexture) {
			auto pixelMapVal = GameInfo.getConstExternalData()->windowState.actualPixelToOnePixel;
			auto sprite_outline = std::make_unique<sf::Sprite>(*outlineTexture);
			sprite_outline->setScale(pixelMapVal, pixelMapVal);
			sprite_outline->setPosition((float) x_pos + (float) number * (sprite_outline->getGlobalBounds().width - pixelMapVal), (float) z_pos);
			healthBarSpritesContainer[number].first = std::move(sprite_outline);
		}

		void loadHeart(int number, sf::Texture* heartTexture) {
			auto pixelMapVal = GameInfo.getConstExternalData()->windowState.actualPixelToOnePixel;
			auto sprite_heart = std::make_unique<sf::Sprite>(*heartTexture);
			sprite_heart->setScale(pixelMapVal, pixelMapVal);
			auto sprite_outline = healthBarSpritesContainer[number].first->get();
			sprite_heart->setPosition((float) x_pos + (float) number * sprite_outline->getGlobalBounds().width + pixelMapVal * (1 - (float) number),
			                          (float) z_pos + pixelMapVal);
			healthBarSpritesContainer[number].second = std::move(sprite_heart);
		}

		void updateHeartOutlinePos(int number) {
			auto pixelMapVal = GameInfo.getConstExternalData()->windowState.actualPixelToOnePixel;
			auto sprite_outline = healthBarSpritesContainer[number].first->get();
			sprite_outline->setScale(pixelMapVal, pixelMapVal);
			sprite_outline->setPosition((float) x_pos + (float) number * (sprite_outline->getGlobalBounds().width - pixelMapVal), (float) z_pos);
		}

		void updateHeartPos(int number) {
			auto pixelMapVal = GameInfo.getConstExternalData()->windowState.actualPixelToOnePixel;
			auto sprite_outline = healthBarSpritesContainer[number].first->get();
			auto sprite_heart = healthBarSpritesContainer[number].second->get();
			sprite_heart->setPosition((float) x_pos + (float) number * sprite_outline->getGlobalBounds().width + pixelMapVal * (1 - (float) number),
			                          (float) sprite_outline->getGlobalBounds().top + pixelMapVal);
		}

		void updateHudPosition() {
			auto pixelMapVal = GameInfo.getConstExternalData()->windowState.actualPixelToOnePixel;
			auto screenHeight = (float) GameInfo.getConstExternalData()->windowState.getScreenHeight();
			auto x = this->worldAccess->getView().getCenter().x - 182 * pixelMapVal / 2;
			auto z = this->worldAccess->getView().getCenter().y + screenHeight / 2 - 21 * pixelMapVal - (9+10) * pixelMapVal;
			setPosition(floor(x), floor(z));
			for (auto i = 0; i < MAX_HEARTS; ++i) {
				updateHeartOutlinePos(i);
				updateHeartPos(i);
			}
		}

		void setHeartTexture(int number, sf::Texture* texture) {
			healthBarSpritesContainer[number].second->get()->setTexture(*texture);
		}

		void setHeartOutlineTexture(sf::Texture* texture) {
			for (int i = 0; i < MAX_HEARTS; ++i) {
				healthBarSpritesContainer[i].first->get()->setTexture(*texture);
			}
		}

		void shakeHearts() {
			auto move_up_down = [&](int number, bool move) {
				auto sprite_outline = healthBarSpritesContainer[number].first->get();
				if (move) {
					auto pixelMapVal = GameInfo.getConstExternalData()->windowState.actualPixelToOnePixel;
					sprite_outline->setPosition(sprite_outline->getPosition().x, sprite_outline->getPosition().y - pixelMapVal); // move up
				} else {
					sprite_outline->setPosition(sprite_outline->getPosition().x, (float) z_pos);
				}
				updateHeartPos(number);
			}; // tremble the given heart
			for (auto i = 0; i < MAX_HEARTS; ++i) move_up_down(i, math::Random::randomBool(1.f / 20));
		}

		void setHealthValue(short health) {
			auto hearts_num = health / 2;
			auto half_heart_num = health - hearts_num * 2;
			if (health != last_health || delay_timer == 0)
				for (auto i = 0; i < MAX_HEARTS; ++i)
					setHeartTexture(i, &emptyHeartTexture);
			if (health != last_health) {
				auto last_hearts_num = last_health / 2;
				auto last_half_heart_num = last_health - last_hearts_num * 2;
				for (auto j = 0; j < last_hearts_num; ++j)
					setHeartTexture(j, &pinkHeartTexture);
				if (last_half_heart_num != 0 && last_hearts_num < MAX_HEARTS)
					setHeartTexture(last_hearts_num, &halfPinkHeartTexture);
				for (auto k = 0; k < hearts_num; ++k)
					setHeartTexture(k, &redHeartTexture);
				if (half_heart_num != 0 && hearts_num < MAX_HEARTS)
					setHeartTexture(hearts_num, &redPinkHeartTexture);
				last_health = health;
			}

			if ((delay_timer / 10) % 2 == 0)
				setHeartOutlineTexture(&darkOutlineHeartTexture);
			else
				setHeartOutlineTexture(&whiteOutlineHeartTexture);

			if (delay_timer == 0) {
				for (auto k = 0; k < hearts_num; ++k)
					setHeartTexture(k, &redHeartTexture);
				if (half_heart_num != 0 && hearts_num < MAX_HEARTS)
					setHeartTexture(hearts_num, &halfRedHeartTexture);
				setHeartOutlineTexture(&darkOutlineHeartTexture);
			}
		}

		void updateHealth() {
			auto damaged = this->worldAccess->getPlayer()->isDamaged();
			auto health = this->worldAccess->getPlayer()->getHealth();
			if (damaged) delay_timer = DAMAGE_EFFECT_DURATION;
			if (health <= worldAccess->getPlayer()->getMaxHealth() / 3)
				shakeHearts();
			setHealthValue(floor(health));
			if (delay_timer >= 0) delay_timer--;
		}
	};
}

#endif //RUNCRAFT_HEALTHBARHUD_HPP
