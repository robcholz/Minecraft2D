//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_GUISTYLE_HPP
#define RUNCRAFT_GUISTYLE_HPP

#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include "client/gui/GUI.hpp"
#include "util/Path.hpp"

namespace initialization {
	class GUIFont : public sf::Font {
	protected:
		std::string fontAssetPath = Path::fontFilePath + "/runcraft.ttf";
	public:
		GUIFont() : sf::Font() {
			loadFromFile(fontAssetPath);
		}
	};
}

namespace gui_style {
	sf::Color MessageColor(220, 220, 220);
	sf::Color Yellow(255,255,0);
	initialization::GUIFont MessageFont;
}

#endif //RUNCRAFT_GUISTYLE_HPP
