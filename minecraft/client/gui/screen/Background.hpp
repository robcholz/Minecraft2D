//
// Created by robcholz on 3/17/23.
//

#ifndef MINECRAFT_BACKGROUND_HPP
#define MINECRAFT_BACKGROUND_HPP

#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "client/render/RenderSystem.hpp"


class Background {
private:
	using String=std::string;
public:
	explicit Background(const String& id) {
		identifier = std::make_unique<Identifier>(id, Identifier::Category::GUI);
		if (!sourceTexture.loadFromFile(identifier->getAbsolutePath())) {
			PLOG_ERROR << ("CANNOT update sourceTexture from " + identifier->getAbsolutePath());
		}
		textureAtlas.push_front(std::make_unique<sf::Sprite>(sourceTexture));
	}

	~Background() {
		textureAtlas.clear();
	}

	Background& fitToScreen() {
		textureAtlas.front()->scale((float) RenderSystem::getScreenWidth() / static_cast<float>(sourceTexture.getSize().x),
		                            (float)RenderSystem::getScreenHeight()  / static_cast<float>(sourceTexture.getSize().y));
		return *this;
	}

	Background& composeToScreen() {
		textureAtlas.clear();
		float scale = roundf((float) RenderSystem::getScreenWidth()  / 200);
		int texture_width = (int) ((float) sourceTexture.getSize().x * scale);
		for (int image_x = 0; image_x < RenderSystem::getScreenWidth(); image_x += texture_width) {
			for (int image_y = 0; image_y < RenderSystem::getScreenHeight() ; image_y += texture_width) {
				auto texture_atlas = std::make_unique<sf::Sprite>(sourceTexture);
				texture_atlas->setScale(scale, scale);
				texture_atlas->setPosition((float) image_x, (float) image_y);
				textureAtlas.push_front(std::move(texture_atlas));
			}
		}
		return *this;
	}

	void update() {}

	void render() {
		for (auto const& texture_atlas: textureAtlas) {
			RenderSystem::render(*texture_atlas);
		}
	}
private:
	std::unique_ptr<Identifier> identifier;
	sf::Texture sourceTexture;
	std::list<std::unique_ptr<sf::Sprite>> textureAtlas;
};

#endif //MINECRAFT_BACKGROUND_HPP
