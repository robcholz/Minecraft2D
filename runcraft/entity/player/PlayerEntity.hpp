//
// Created by robcholz on 4/29/23.
//

#ifndef RUNCRAFT_PLAYERENTITY_HPP
#define RUNCRAFT_PLAYERENTITY_HPP

#include <memory>
#include "client/input/Input.hpp"
#include "block/attributes/Block.hpp"
#include "entity/Entity.hpp"
#include "block/attributes/Blocks.hpp"


class PlayerEntity : public entity::Entity {
protected:
	void updateHitbox() override {
		entityHitbox.setHitbox(headSprite.getGlobalBounds().left, headSprite.getGlobalBounds().top, headSprite.getGlobalBounds().width,
		                       headSprite.getGlobalBounds().height + armSprite.getGlobalBounds().height +
		                       legSprite.getGlobalBounds().height);
	}

	void onUpdate() override {
		entity::Entity::onUpdate();
		_setPixelPosition(getEntityPosition().getPixelPosition().getX(), getEntityPosition().getPixelPosition().getZ());
		updateSkin();

		if (getWorld()->getChunkStream()->getBlock(getEntityPosition().getBlockPosition())->getID().id == "grass_block") {
			if (entity::Entity::isWalking() && entity::Entity::onGround())
				this->runcraftClientAccess->getSoundManager()->addSound(SoundEvents::getInstance().STEP_SOUND_GRASS);
		}

	}

	void onRender() override {
		GameInfo.getRender()->render(legSprite);
		GameInfo.getRender()->render(armSprite);
		GameInfo.getRender()->render(headSprite);
	}

public:
	enum class View : uint8_t { EAST, WEST };

	explicit PlayerEntity(RuncraftClientAccess* runcraftClientAccess, WorldAccess* worldAccess) : Entity(worldAccess) {
		this->runcraftClientAccess = runcraftClientAccess;
		moveLeft.attachKey(input::KeyboardKeyType::A);
		moveRight.attachKey(input::KeyboardKeyType::D);
		moveJump.attachKey(input::KeyboardKeyType::Space);
		moveSniff.attachKey(input::KeyboardKeyType::LShift);
		initSkin();
	}

	~PlayerEntity() override = default;

	float getMaxHealth() override {
		return 20.f;
	}

private:
	input::KeyboardObserver moveLeft, moveRight, moveJump, moveSniff;
	String playerSkinAssetPath = Path::currentPlayerPath;

	using sfRecti = sf::IntRect;
	using Texture = sf::Texture;
	using Sprite = sf::Sprite;
	Texture leftLeg, leftArm, leftHead;
	Texture rightLeg, rightArm, rightHead;
	Sprite legSprite, armSprite, headSprite;
	View _view{};
	RuncraftClientAccess* runcraftClientAccess;

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
			//entity::Entity::getEntityPosition().getPixelPosition().offset(-5,0);
		}
		if (moveRight.isActivated()) {
			viewFrom(View::EAST);
			acceleration.x = WALKING_ACCELERATION;
			getEntityPosition().setDirection(Direction::DirectionType::EAST);
			//entity::Entity::getEntityPosition().getPixelPosition().offset(5,0);
		}
		if (moveJump.isActivated()) {

			if (onGround())
				velocity.z = JUMPING_VELOCITY;

			//entity::Entity::getEntityPosition().getPixelPosition().offset(0,-5);
		}
		if (moveSniff.isActivated()) {
			//entity::Entity::getEntityPosition().getPixelPosition().offset(0,5);
		}
	}
};

#endif //RUNCRAFT_PLAYERENTITY_HPP
