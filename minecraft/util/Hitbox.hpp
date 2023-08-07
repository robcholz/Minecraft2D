//
// Created by robcholz on 6/13/23.
//

#ifndef MINECRAFT_HITBOX_HPP
#define MINECRAFT_HITBOX_HPP

#include <list>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>


class Hitbox {
private:
	struct Box;
public:
	/*
    * p1---------*
    * |          |
    * |          |
    * |          |
    * *----------p2 <-point is here
    */
	Hitbox() = default;

	explicit Hitbox(int x, int y, int width, int height) {
		box.x = x;
		box.y = y;
		box.width = width;
		box.height = height;
	}

	void setHitbox(sf::Sprite* sprite) {
		box.x = floor(sprite->getGlobalBounds().left);
		box.y = floor(sprite->getGlobalBounds().top);
		box.width = floor(sprite->getGlobalBounds().width);
		box.height = floor(sprite->getGlobalBounds().height);
	}

	void setHitbox(float x, float y, float width, float height) {
		box.x = floor(x);
		box.y = floor(y);
		box.width = floor(width);
		box.height = floor(height);
	}

	bool isCollided(Hitbox& hitbox2, bool contactDetection) const {
		bool xOverlap, yOverlap;

		if (contactDetection) {
			xOverlap = contactOverlap(box.x, hitbox2.box.x, hitbox2.box.x + hitbox2.box.width) ||
			           contactOverlap(hitbox2.box.x, box.x, box.x + box.width);

			yOverlap = contactOverlap(box.y, hitbox2.box.y, hitbox2.box.y + hitbox2.box.height) ||
			           contactOverlap(hitbox2.box.y, box.y, box.y + box.height);
		} else {
			xOverlap = intersectionOverlap(box.x, hitbox2.box.x, hitbox2.box.x + hitbox2.box.width) ||
			           intersectionOverlap(hitbox2.box.x, box.x, box.x + box.width);

			yOverlap = intersectionOverlap(box.y, hitbox2.box.y, hitbox2.box.y + hitbox2.box.height) ||
			           intersectionOverlap(hitbox2.box.y, box.y, box.y + box.height);
		}
		return xOverlap && yOverlap;
	}

	static bool isCollided(Hitbox hitbox1, Hitbox hitbox2, bool contactDetection) {
		bool xOverlap, yOverlap;
		if (contactDetection) {
			xOverlap = contactOverlap(hitbox1.getHitbox().x, hitbox2.box.x, hitbox2.box.x + hitbox2.box.width) ||
			           contactOverlap(hitbox2.box.x, hitbox1.getHitbox().x, hitbox1.getHitbox().x + hitbox1.getHitbox().width);

			yOverlap = contactOverlap(hitbox1.getHitbox().y, hitbox2.box.y, hitbox2.box.y + hitbox2.box.height) ||
			           contactOverlap(hitbox2.box.y, hitbox1.getHitbox().y, hitbox1.getHitbox().y + hitbox1.getHitbox().height);
		} else {
			xOverlap = intersectionOverlap(hitbox1.getHitbox().x, hitbox2.box.x, hitbox2.box.x + hitbox2.box.width) ||
			           intersectionOverlap(hitbox2.box.x, hitbox1.getHitbox().x, hitbox1.getHitbox().x + hitbox1.getHitbox().width);

			yOverlap = intersectionOverlap(hitbox1.getHitbox().y, hitbox2.box.y, hitbox2.box.y + hitbox2.box.height) ||
			           intersectionOverlap(hitbox2.box.y, hitbox1.getHitbox().y, hitbox1.getHitbox().y + hitbox1.getHitbox().height);
		}
		return xOverlap && yOverlap;
	}

	Box& getHitbox() { return box; }

	Box& getBox() {
		return box;
	}

private:
	struct Box { int x, y, width, height; } box;

	static bool intersectionOverlap(int value, int min, int max) { return (value > min) && (value < max); }

	static bool contactOverlap(int value, int min, int max) { return (value >= min) && (value <= max); }

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
		if(!_hasHitbox){
			for (auto v:hitboxesList) {
				v->setHitbox(0,0,0,0);
			}
		}
	}

	void showHitbox(bool showHitbox) {
		_showHitbox = showHitbox;
	}

	[[nodiscard]] bool hasHitbox() const {
		return _hasHitbox;
	}

	void clearHitboxesList() {
		hitboxesList.clear();
	}

	Hitbox* getHitbox() {
		return hitboxesList.front();
	}

	bool isCollided(HitboxHandler* hitboxHandler, bool contactDetection = false) {
		if (!_hasHitbox || !hitboxHandler->_hasHitbox) return false;
		for (auto other: hitboxHandler->hitboxesList) {
			for (auto self: hitboxesList){
				if (other->isCollided(*self, contactDetection))
					return true;
			}
		}
		return false;
	}

	void renderHitbox() {
		if (_showHitbox && _hasHitbox) {
			for (auto v: hitboxesList) {
				sf::VertexArray lines(sf::LinesStrip, 2);
				lines[0].position = sf::Vector2f(v->getBox().x, v->getBox().y);
				lines[1].position = sf::Vector2f(v->getBox().x + v->getBox().width, v->getBox().y + v->getBox().height);
				RenderSystem::render(lines);
			}
		}
	}

private:
	std::list<Hitbox*> hitboxesList;
	bool _hasHitbox = true;
	bool _showHitbox = true;
};

#endif //MINECRAFT_HITBOX_HPP
