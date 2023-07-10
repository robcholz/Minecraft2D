//
// Created by robcholz on 3/17/23.
//

#ifndef RUNCRAFT_BACKGROUND_HPP
#define RUNCRAFT_BACKGROUND_HPP

#pragma once

#include "client/GameInfo.hpp"
#include "client/gui/widget/texture/Texture.hpp"

class Background : public Texture {
private:
	using String=std::string;
public:
	explicit Background(const String& id) : Texture(id) {}

	~Background() override {
		textureAtlas.clear();
	}

	Background& fitToScreen() {
		textureAtlas.front()->scale((float) GameInfo.getConstExternalData()->windowState.getScreenWidth() / static_cast<float>(sourceTexture.getSize().x),
		                            (float) GameInfo.getConstExternalData()->windowState.getScreenHeight() / static_cast<float>(sourceTexture.getSize().y));
		return *this;
	}

	Background& composeToScreen() {
		textureAtlas.clear();
		float scale = roundf((float) GameInfo.getConstExternalData()->windowState.getScreenWidth() / 200);
		int texture_width = (int) ((float) sourceTexture.getSize().x * scale);
		for (int image_x = 0; image_x < GameInfo.getConstExternalData()->windowState.getScreenWidth(); image_x += texture_width) {
			for (int image_y = 0; image_y < GameInfo.getConstExternalData()->windowState.getScreenHeight(); image_y += texture_width) {
				auto texture_atlas = std::make_unique<sf::Sprite>(sourceTexture);
				texture_atlas->setScale(scale, scale);
				texture_atlas->setPosition((float) image_x, (float) image_y);
				textureAtlas.push_front(std::move(texture_atlas));
			}
		}
		return *this;
	}

	void update() override{}

	void render() override {
		for (auto const& texture_atlas: textureAtlas) {
			GameInfo.getRender()->render(*texture_atlas);
		}
	}
};

#endif //RUNCRAFT_BACKGROUND_HPP
