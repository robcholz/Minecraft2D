//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_GUISTYLE_HPP
#define RUNCRAFT_GUISTYLE_HPP

#include <memory>
#include <SFML/Graphics/Color.hpp>

namespace initialization {
	class GUIFont : public sf::Font, public GUI {
	protected:
		std::string fontAssetPath = fontFilePath + "runcraft.ttf";
	private:
		void render() override {}

	public:
		GUIFont() : sf::Font() {
			loadFromFile(fontAssetPath);
		}
	};
}

namespace gui_style {
	sf::Color MessageColor(220, 220, 220, 255);
	initialization::GUIFont MessageFont;
}

#endif //RUNCRAFT_GUISTYLE_HPP
