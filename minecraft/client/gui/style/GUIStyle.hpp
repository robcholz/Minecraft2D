//
// Created by robcholz on 4/2/23.
//

#ifndef MINECRAFT_GUISTYLE_HPP
#define MINECRAFT_GUISTYLE_HPP

#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include "client/gui/GUI.hpp"
#include "util/Path.hpp"

namespace initialization {
	class GUIFont : public sf::Font {
	protected:
		std::string fontAssetPath = Path::fontFilePath + "/minecraft.ttf";
	public:
		GUIFont() : sf::Font() {
			loadFromFile(fontAssetPath);
		}
	};
}

namespace gui_style {
	sf::Color MessageColor(220, 220, 220);
	initialization::GUIFont MessageFont;
}

#endif //MINECRAFT_GUISTYLE_HPP
