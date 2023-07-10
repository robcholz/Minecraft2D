//
// Created by robcholz on 6/16/23.
//

#ifndef RUNCRAFT_ITEMBARHUD_HPP
#define RUNCRAFT_ITEMBARHUD_HPP

#include <vector>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "entity/EntityAccess.hpp"
#include "world/WorldAccess.hpp"
#include "Log.h"
#include "client/gui/GUI.hpp"

namespace hud {
	class ItemBarHud : public GUI {
	private:
		using String = std::string;
		using PixelPosT = coordinate::PixelPositonT;
	public:
		explicit ItemBarHud(WorldAccess* worldAccess) {
			this->worldAccess = worldAccess;
			identifier = std::make_unique<Identifier>("widgets", Identifier::Category::GUI);
			iconAssetPath = identifier->getAbsolutePath();
			itemBarTexture.loadFromFile(iconAssetPath, sf::IntRect{0, 0, 182, 21}); // (0,0) (181,21)
			onFocusedItemTexture.loadFromFile(iconAssetPath, sf::IntRect{0, 22, 24, 24}); // (0,22) (24,46)
			scale();
		}

		void update() override {
			updatePosition();
			focus(8);
		}

		void render() override {
			GameInfo.getRender()->render(itemBarSprite);
			GameInfo.getRender()->render(onFocusedItemSprite);
		}

	private:
		std::unique_ptr<Identifier> identifier;
		sf::Texture itemBarTexture;
		sf::Texture onFocusedItemTexture;
		sf::Sprite itemBarSprite;
		sf::Sprite onFocusedItemSprite;
		WorldAccess* worldAccess = nullptr;
		String iconAssetPath;
		PixelPosT x_pos = 0;
		PixelPosT z_pos = 0;

		void scale() {
			auto pixelMapVal = GameInfo.getConstExternalData()->windowState.actualPixelToOnePixel;
			itemBarSprite.setTexture(itemBarTexture);
			onFocusedItemSprite.setTexture(onFocusedItemTexture);
			itemBarSprite.setScale(pixelMapVal, pixelMapVal);
			onFocusedItemSprite.setScale(pixelMapVal, pixelMapVal);
		}

		void updatePosition() {
			auto screenHeight = (float) GameInfo.getConstExternalData()->windowState.getScreenHeight();
			auto x = this->worldAccess->getView().getCenter().x - itemBarSprite.getGlobalBounds().width / 2;
			auto z = this->worldAccess->getView().getCenter().y + screenHeight / 2 - itemBarSprite.getGlobalBounds().height;
			setPosition(floor(x), floor(z));
		}

		void setPosition(PixelPosT x, PixelPosT z) {
			this->x_pos = x;
			this->z_pos = z;
			itemBarSprite.setPosition((float) x_pos, (float) z_pos);
		}

		/**
		 * @param number
		 * @Range 0-8
		 */
		void focus(int number) {
			auto pixelMapVal = GameInfo.getConstExternalData()->windowState.actualPixelToOnePixel;
			onFocusedItemSprite.setPosition(floor((float) number * 20 * pixelMapVal + (float) x_pos - pixelMapVal),
			                                floor((float) z_pos - pixelMapVal));
		}
	};
}

#endif //RUNCRAFT_ITEMBARHUD_HPP
