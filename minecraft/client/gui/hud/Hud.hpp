//
// Created by robcholz on 7/20/23.
//

#ifndef MINECRAFT_HUD_HPP
#define MINECRAFT_HUD_HPP

#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include "world/poi/Coordinate.hpp"

namespace hud {

	class Hud {
	protected:
		using String = std::string;
		using HudPixelPosT = coordinate::PixelPositonT;
		WorldAccess* worldAccess = nullptr;
		float screenWidth = -1, screenHeight = -1;

		Hud() {
			if (scheduledUpdaterTicks != -1)
				currentTicksCounter = scheduledUpdaterTicks;
		}

		/**
		 * @brief Update the scale of sprites to match the pixel map.
		 * @param sprite
		 */
		static void setSpriteScale(sf::Sprite* sprite) {
			auto pixelMapVal = RenderSystem::Settings::actualPixelToOnePixel;
			sprite->setScale(pixelMapVal, pixelMapVal);
		}

		/**
		 * @brief Map a value to the pixel map.
		 * @param v
		 * @return
		 */
		static float mapTo(HudPixelPosT v) {
			auto pixelMapVal = RenderSystem::Settings::actualPixelToOnePixel;
			return (float) v * pixelMapVal;
		}

		/**
		 * @brief Map a value to the pixel map.
		 * @param x
		 * @param z
		 * @return
		 */
		sf::Vector2f convertToPixPos(float x, float z) {
			auto width = (float) RenderSystem::getScreenWidth();
			auto height = (float) RenderSystem::getScreenHeight();
			auto left = worldAccess->getView().getCenter().x - width / 2.0f;
			auto top = worldAccess->getView().getCenter().y - height / 2.0f;
			sf::Vector2f vec;
			vec.x = (float) x + left;
			vec.y = (float) z + top;
			return vec;
		}

		/**
		 * @brief get the center of the screen.
		 * @return
		 */
		sf::Vector2f getCenter() {
			return worldAccess->getView().getCenter();
		}

		/**
		 * @brief set the updating ticks
		 * @param ticks
		 */
		void setScheduledUpdaterTicks(int ticks) {
			this->scheduledUpdaterTicks = ticks;
		}

		virtual void setPosition(HudPixelPosT x, HudPixelPosT z) {
			this->x_pos = x;
			this->z_pos = z;
		}

		[[nodiscard]]
		bool isOnScheduledUpdate() const {
			return (currentTicksCounter == 0);
		}

		virtual void onScheduledUpdate() {
		}

		virtual void updatePosition() {
		}

		void updateData() {
			screenHeight = (float) RenderSystem::getScreenHeight();
			screenWidth = (float) RenderSystem::getScreenWidth();
			x_pos = (HudPixelPosT) (getCenter().x - screenWidth / 2);
			z_pos = (HudPixelPosT) (getCenter().y - screenHeight / 2);
			if (currentTicksCounter <= 0)
				currentTicksCounter = scheduledUpdaterTicks;
			if (scheduledUpdaterTicks != -1)
				--currentTicksCounter;
			if (isOnScheduledUpdate())
				onScheduledUpdate();
		}

		[[nodiscard]]
		HudPixelPosT getScreenCornerX() const {
			return x_pos;
		}

		[[nodiscard]]
		HudPixelPosT getScreenCornerZ() const {
			return z_pos;
		}

	private:
		HudPixelPosT x_pos = 0;
		HudPixelPosT z_pos = 0;
		int scheduledUpdaterTicks = -1;
		int currentTicksCounter = -1;
	};
}
#endif //MINECRAFT_HUD_HPP
