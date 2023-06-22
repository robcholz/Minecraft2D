//
// Created by robcholz on 6/16/23.
//

#ifndef RUNCRAFT_EXPERIENCEBARHUD_HPP
#define RUNCRAFT_EXPERIENCEBARHUD_HPP

#include <vector>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "entity/EntityAccess.hpp"
#include "world/WorldAccess.hpp"
#include "Log.h"
#include "client/gui/GUI.hpp"

namespace hud {

	class ExperienceBarHud : public GUI {
	private:
		using String = std::string;
		using PixelPosT=coordinate::PixelPositonT;
	protected:
		String iconAssetPath = guiFilePath + "/icons.png";
	public:
		explicit ExperienceBarHud(WorldAccess* worldAccess) {
			this->worldAccess = worldAccess;
			// empty experience bar (0,64) (182,69)
			loadSprite(&emptyExperienceBarTexture, &emptyExperienceBarSprite, sf::IntRect{0, 64, 182, 5});
			// full experience bar (0,69) (182,74)
			loadSprite(&fullExperienceBarTexture, &fullExperienceBarSprite, sf::IntRect{0, 69, 182, 5});
			// empty experience slot (1,70) (9,72)
			//loadSprite(&leftExperienceSlotTexture, &leftExperienceSlotSprite, sf::IntRect{0, 0, 0, 0});
		}

		void update() {
			updateHudPosition();
		}

		void render() override {
			GameInfo.getRender()->render(emptyExperienceBarSprite);
		}

	private:
		WorldAccess* worldAccess;
		std::vector<std::unique_ptr<sf::Sprite>> experienceBarContainer;
		sf::Texture emptyExperienceBarTexture;
		sf::Texture fullExperienceBarTexture;
		sf::Texture emptyExperienceSlotTexture;

		sf::Sprite emptyExperienceBarSprite;
		sf::Sprite fullExperienceBarSprite;
		sf::Sprite emptyExperienceSlotSprite;

		PixelPosT x_pos = 0;
		PixelPosT z_pos = 0;

		void setPosition(PixelPosT x,PixelPosT z){
			x_pos=x;
			z_pos=z;
			emptyExperienceBarSprite.setPosition((float)x_pos,(float)z_pos);
		}

		void loadSprite(sf::Texture* texture, sf::Sprite* sprite, sf::IntRect intRect) {
			static auto pixelMapVal = GameInfo.getConstExternalData()->windowState.actualPixelToOnePixel;
			texture->loadFromFile(iconAssetPath, intRect);
			sprite->setTexture(*texture);
			sprite->setScale(pixelMapVal, pixelMapVal);
		}

		void updateHudPosition(){
			auto pixelMapVal = GameInfo.getConstExternalData()->windowState.actualPixelToOnePixel;
			auto screenHeight = (float) GameInfo.getConstExternalData()->windowState.getScreenHeight();
			auto x = this->worldAccess->getView().getCenter().x - 182 * pixelMapVal / 2;
			auto z = this->worldAccess->getView().getCenter().y + screenHeight / 2 - 21 * pixelMapVal - 8 * pixelMapVal;
			setPosition(floor(x),floor(z));
		}
	};
}

#endif //RUNCRAFT_EXPERIENCEBARHUD_HPP
