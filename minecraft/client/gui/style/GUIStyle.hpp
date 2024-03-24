//
// Created by robcholz on 4/2/23.
//

#ifndef MINECRAFT_GUISTYLE_HPP
#define MINECRAFT_GUISTYLE_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <memory>
#include "client/gui/GUI.hpp"
#include "util/Path.hpp"

namespace initialization {
class GUIFont : public sf::Font {
 protected:
  std::string fontAssetPath = Path::fontFilePath + "/minecraft.ttf";

 public:
  GUIFont() : sf::Font() { loadFromFile(fontAssetPath); }
};
}  // namespace initialization

namespace gui_style {
sf::Color MessageColor(220, 220, 220);
static initialization::GUIFont& getMessageFont(){
  static initialization::GUIFont font;
  return font;
}
}  // namespace gui_style

#endif  // MINECRAFT_GUISTYLE_HPP
