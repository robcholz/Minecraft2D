//
// Created by robcholz on 4/29/23.
//

#ifndef RUNCRAFT_PLAYER_HPP
#define RUNCRAFT_PLAYER_HPP

#include <memory>
#include "client/input/Input.hpp"
#include "block/attributes/Block.hpp"
#include "entity/Entity.hpp"

class Player : public entity::Entity {
protected:
	void onUpdate() override {
		entity::Entity::onUpdate();
		updateSkin();
		_setPixelPosition(getEntityPosition().getPixelPosition().getX(), getEntityPosition().getPixelPosition().getZ());
	}

	void updateHitbox() override {
		legHitbox.setHitbox(&legSprite);
		armHitbox.setHitbox(&armSprite);
		headHitbox.setHitbox(&headSprite);
	}

public:
	enum class View : uint8_t { EAST, WEST };

	explicit Player(WorldAccess* worldAccess) : Entity(worldAccess) {
		moveLeft.attachKey(input::KeyboardKeyType::A);
		moveRight.attachKey(input::KeyboardKeyType::D);
		moveJump.attachKey(input::KeyboardKeyType::Space);
		moveSniff.attachKey(input::KeyboardKeyType::LShift);
		initSkin();
		addHitbox(&legHitbox);
		addHitbox(&armHitbox);
		addHitbox(&headHitbox);
	}

	void onRender() override {
		GameInfo.getRender()->render(legSprite);
		GameInfo.getRender()->render(armSprite);
		GameInfo.getRender()->render(headSprite);
		renderHitbox();
	}

private:
	input::KeyboardObserver moveLeft, moveRight, moveJump, moveSniff;
	String playerSkinAssetPath = "../assets/textures/player/steve.png";

	using sfRecti = sf::IntRect;
	using Texture = sf::Texture;
	using Sprite = sf::Sprite;
	Texture leftLeg, leftArm, leftHead;
	Texture rightLeg, rightArm, rightHead;
	Sprite legSprite, armSprite, headSprite;
	Hitbox legHitbox{}, armHitbox{}, headHitbox{};
	View _view;

	void initSkin() {
		leftLeg.loadFromFile(playerSkinAssetPath, sfRecti{24, 52, 4, 12}); // (24,52) (27,63)
		leftArm.loadFromFile(playerSkinAssetPath, sfRecti{40, 52, 4, 12}); // (40,52) (43,63)
		leftHead.loadFromFile(playerSkinAssetPath, sfRecti{16, 8, 8, 8}); // (16,8) (23,15)
		rightLeg.loadFromFile(playerSkinAssetPath, sfRecti{0, 20, 4, 12}); // (0,20) (3,31)
		rightArm.loadFromFile(playerSkinAssetPath, sfRecti{40, 20, 4, 12}); // (40,20) (43,31)
		rightHead.loadFromFile(playerSkinAssetPath, sfRecti{0, 8, 8, 8}); // (0,8) (7,15)

		auto pixelToOne = (float) GameInfo.getConstExternalData()->windowState.pixelProportion / 16.0f;
		legSprite.setTexture(rightLeg);
		legSprite.scale(pixelToOne, pixelToOne);
		armSprite.setTexture(rightArm);
		armSprite.scale(pixelToOne, pixelToOne);
		headSprite.setTexture(rightHead);
		headSprite.scale(pixelToOne, pixelToOne);
		width = legSprite.getGlobalBounds().width;
	}

	void viewFrom(View view) {
		switch (view) {
			case View::EAST:
				legSprite.setTexture(rightLeg);
				armSprite.setTexture(rightArm);
				headSprite.setTexture(rightHead);
				break;
			case View::WEST:
				legSprite.setTexture(leftLeg);
				armSprite.setTexture(leftArm);
				headSprite.setTexture(leftHead);
				break;
		}
		_view = view;
	}

	void _setPixelPosition(PixelPosT x, PixelPosT z) {
		auto x_f = (float) x;
		auto z_f = (float) z - legSprite.getGlobalBounds().height;
		legSprite.setPosition(x_f, z_f);
		armSprite.setPosition(x_f, z_f - armSprite.getGlobalBounds().height);
		headSprite.setPosition(x_f - headSprite.getGlobalBounds().width / 4, z_f - armSprite.getGlobalBounds().height - headSprite.getGlobalBounds().height);
	}

	void updateSkin() {
		acceleration.x = 0;
		acceleration.z = 0;
		if (moveLeft.isActivated()) {
			viewFrom(View::WEST);
			acceleration.x = -WALKING_ACCELERATION;
			getEntityPosition().setDirection(Direction::DirectionType::WEST);
		}
		if (moveRight.isActivated()) {
			viewFrom(View::EAST);
			acceleration.x = WALKING_ACCELERATION;
			getEntityPosition().setDirection(Direction::DirectionType::EAST);
		}
		if (moveJump.isActivated()) {
			if (onGround)
				velocity.z = JUMPING_VELOCITY;
		}
		if (moveSniff.isActivated()) {
		}
	}
};

#endif //RUNCRAFT_PLAYER_HPP
