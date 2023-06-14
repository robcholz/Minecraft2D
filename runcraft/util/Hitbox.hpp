//
// Created by robcholz on 6/13/23.
//

#ifndef RUNCRAFT_HITBOX_HPP
#define RUNCRAFT_HITBOX_HPP

#include <list>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "client/GameInfo.hpp"

class Hitbox {
private:
	struct Box { int x, y, width, height; } box;

	static bool valueInRange(int value, int min, int max) { return (value >= min) && (value <= max); }

public:
	/*
    * p1---------*
    * |          |
    * |          |
    * |          |
    * *----------p2 <-point is here
    */
	Hitbox() = default;

	void setHitbox(sf::Sprite* sprite) {
		box.x = floor(sprite->getGlobalBounds().left);
		box.y = floor(sprite->getGlobalBounds().top);
		box.width = floor(sprite->getGlobalBounds().width);
		box.height = floor(sprite->getGlobalBounds().height);
	}

	bool isCollided(Hitbox& hitbox2) const {
		bool xOverlap = valueInRange(box.x, hitbox2.box.x, hitbox2.box.x + hitbox2.box.width) ||
		                valueInRange(hitbox2.box.x, box.x, box.x + box.width);

		bool yOverlap = valueInRange(box.y, hitbox2.box.y, hitbox2.box.y + hitbox2.box.height) ||
		                valueInRange(hitbox2.box.y, box.y, box.y + box.height);

		return xOverlap && yOverlap;
	}

	Hitbox& getHitbox() { return *this; }

	Box& getBox() {
		return box;
	}
};

class HitboxHandler {
protected:
	HitboxHandler& addHitbox(Hitbox* hitbox) {
		if (_hasHitbox)
			hitboxesList.push_back(hitbox);
		return *this;
	}

public:
	void enableHitbox(bool hasHitbox) {
		_hasHitbox = hasHitbox;
	}

	void showHitbox(bool showHitbox) {
		_showHitbox = showHitbox;
	}

	[[nodiscard]] bool hasHitbox() const {
		return _hasHitbox;
	}

	bool isCollided(HitboxHandler* hitboxHandler) {
		if (!_hasHitbox || !hitboxHandler->_hasHitbox) return false;
		for (auto other: hitboxHandler->hitboxesList) {
			for (auto self: hitboxesList)
				if (other->isCollided(*self))
					return true;
		}
		return false;
	}

	void renderHitbox() {
		if (_showHitbox) {
			for (auto v: hitboxesList) {
				sf::VertexArray lines(sf::LinesStrip, 2);
				lines[0].position = sf::Vector2f(v->getBox().x, v->getBox().y);
				lines[1].position = sf::Vector2f(v->getBox().x+v->getBox().width, v->getBox().y+v->getBox().height);
				GameInfo.getRender()->render(lines);
			}
		}
	}

private:
	std::list<Hitbox*> hitboxesList;
	bool _hasHitbox = true;
	bool _showHitbox = true;
};

#endif //RUNCRAFT_HITBOX_HPP
