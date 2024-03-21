//
// Created by robcholz on 6/13/23.
//

#ifndef MINECRAFT_HITBOX_HPP
#define MINECRAFT_HITBOX_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <list>

class Hitbox {
 public:
  /*
   * p1---------*
   * |          |
   * |          |
   * |          |
   * *----------p2 <-point is here
   */
  Hitbox() = default;

  explicit Hitbox(float left, float top, float width, float height) {
    box.left = left;
    box.top = top;
    box.width = width;
    box.height = height;
  }

  Hitbox(const Hitbox& copy) { this->box = copy.box; }

  void setHitbox(sf::Sprite* sprite) { box = sprite->getGlobalBounds(); }

  void setHitbox(float left, float top, float width, float height) {
    box.left = left;
    box.top = top;
    box.width = width;
    box.height = height;
  }

  bool isCollided(Hitbox& hitbox2) {
    return this->getBox().intersects(hitbox2.getBox());
  }

  static bool isCollided(Hitbox& hitbox1, Hitbox& hitbox2) {
    return hitbox1.getBox().intersects(hitbox2.getBox());
  }

  sf::FloatRect& getBox() { return box; }

 private:
  sf::FloatRect box;
};

class HitboxHandler {
 public:
  HitboxHandler& addHitbox(Hitbox* hitbox) {
    if (_hasHitbox)
      hitboxesList.push_back(hitbox);
    return *this;
  }

  void enableHitbox(bool hasHitbox) {
    _hasHitbox = hasHitbox;
    if (!_hasHitbox) {
      for (auto v : hitboxesList) {
        v->setHitbox(0, 0, 0, 0);
      }
    }
  }

  void showHitbox(bool showHitbox) { _showHitbox = showHitbox; }

  [[nodiscard]] bool hasHitbox() const { return _hasHitbox; }

  void clearHitboxesList() { hitboxesList.clear(); }

  Hitbox* getHitbox() { return hitboxesList.front(); }

  bool isCollided(HitboxHandler* hitboxHandler) {
    if (!_hasHitbox || !hitboxHandler->_hasHitbox)
      return false;
    for (auto other : hitboxHandler->hitboxesList) {
      for (auto self : hitboxesList) {
        if (other->isCollided(*self))
          return true;
      }
    }
    return false;
  }

  void renderHitbox() {
    if (_showHitbox && _hasHitbox) {
      for (auto v : hitboxesList) {
        sf::VertexArray lines(sf::LinesStrip, 2);
        lines[0].position = sf::Vector2f(v->getBox().left, v->getBox().top);
        lines[1].position = sf::Vector2f(v->getBox().left + v->getBox().width,
                                         v->getBox().top + v->getBox().height);
        RenderSystem::render(lines);
      }
    }
  }

 private:
  std::list<Hitbox*> hitboxesList;
  bool _hasHitbox = true;
  bool _showHitbox = true;
};

#endif  // MINECRAFT_HITBOX_HPP
