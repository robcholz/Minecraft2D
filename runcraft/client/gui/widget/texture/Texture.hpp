//
// Created by robcholz on 3/11/23.
//
#pragma once

#ifndef RUNCRAFT_TEXTURE_HPP
#define RUNCRAFT_TEXTURE_HPP

#include <list>
#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "client/gui/GUI.hpp"


class Texture : public sf::Sprite, public GUI {
protected:
	using String = std::string;
public:
	Texture() = default;

	explicit Texture(const String& id) : Sprite(sourceTexture) {
		identifier = std::make_unique<Identifier>(id, Identifier::Category::GUI);
		if (!sourceTexture.loadFromFile(identifier->getAbsolutePath())) {
			PLOG_ERROR << ("CANNOT update sourceTexture from " + identifier->getAbsolutePath());
		}
		textureAtlas.push_front(std::make_unique<sf::Sprite>(sourceTexture));
	}

	~Texture() override= default;

protected:
	std::unique_ptr<Identifier> identifier;
	sf::Texture sourceTexture;
	std::list<std::unique_ptr<sf::Sprite>> textureAtlas;
};

#endif //RUNCRAFT_TEXTURE_HPP
