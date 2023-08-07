//
// Created by robcholz on 4/29/23.
//

#ifndef MINECRAFT_PLAYERENTITY_HPP
#define MINECRAFT_PLAYERENTITY_HPP

#include <memory>
#include "client/input/Input.hpp"
#include "block/attributes/Block.hpp"
#include "entity/Entity.hpp"
#include "block/attributes/Blocks.hpp"

namespace entity {

	class PlayerEntity : public Entity {
	private:
		using String = std::string;
		using BlockPosition = coordinate::BlockPosition;
		using PixelPosition = coordinate::PixelPosition;
		using sfRecti = sf::IntRect;
		using Texture = sf::Texture;
		using Sprite = sf::Sprite;
	protected:
		void updateHitbox() override {
			entityHitbox.setHitbox(headSprite.getGlobalBounds().left, headSprite.getGlobalBounds().top, headSprite.getGlobalBounds().width,
			                       headSprite.getGlobalBounds().height + armSprite.getGlobalBounds().height +
			                       legSprite.getGlobalBounds().height);
		}

		void onUpdate() override {
			Entity::onUpdate();
			setSkinTexturePixelPosition(Entity::getEntityPosition().get<coordinate::PixelPos>());
			receiveUpdateFromInput();

			if (getWorld()->getChunkStream()->getBlock(getEntityPosition().get<coordinate::BlockPos>())->getID().tempID == "grass_block") {
				if (Entity::isWalking() && Entity::onGround())
					this->minecraftClientAccess->getSoundManager()->addSound(SoundEvents::getInstance().STEP_SOUND_GRASS);
			}

		}

		void onRender() override {
			RenderSystem::render(legSprite);
			RenderSystem::render(armSprite);
			RenderSystem::render(headSprite);
		}

	public:
		enum class View : uint8_t {
			EAST,
			WEST
		};

		explicit PlayerEntity(MinecraftClientAccess* minecraftClientAccess, WorldAccess* worldAccess) : Entity(worldAccess) {
			this->minecraftClientAccess = minecraftClientAccess;
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
		Texture leftLeg, leftArm, leftHead;
		Texture rightLeg, rightArm, rightHead;
		Sprite legSprite, armSprite, headSprite;
		View _view{};
		MinecraftClientAccess* minecraftClientAccess = nullptr;

		void initSkin() {
			leftLeg.loadFromFile(playerSkinAssetPath, sfRecti{24, 52, 4, 12}); // (24,52) (27,63)
			leftArm.loadFromFile(playerSkinAssetPath, sfRecti{40, 52, 4, 12}); // (40,52) (43,63)
			leftHead.loadFromFile(playerSkinAssetPath, sfRecti{16, 8, 8, 8}); // (16,8) (23,15)
			rightLeg.loadFromFile(playerSkinAssetPath, sfRecti{0, 20, 4, 12}); // (0,20) (3,31)
			rightArm.loadFromFile(playerSkinAssetPath, sfRecti{40, 20, 4, 12}); // (40,20) (43,31)
			rightHead.loadFromFile(playerSkinAssetPath, sfRecti{0, 8, 8, 8}); // (0,8) (7,15)

			auto pixelToOne = (float) RenderSystem::Settings::pixelProportion / 16.0f;
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

		void setSkinTexturePixelPosition(coordinate::PixelPos position) {
			auto x_f = (float) position.x;
			auto z_f = (float) position.z - legSprite.getGlobalBounds().height;
			legSprite.setPosition(x_f, z_f);
			armSprite.setPosition(x_f, z_f - armSprite.getGlobalBounds().height);
			headSprite.setPosition(x_f - headSprite.getGlobalBounds().width / 4,
			                       z_f - armSprite.getGlobalBounds().height - headSprite.getGlobalBounds().height);
		}

		void receiveUpdateFromInput() {
			acceleration.x = 0;
			acceleration.z = 0;
			if (moveLeft.isActivated()) {
				viewFrom(View::WEST);
				acceleration.x = -WALKING_ACCELERATION;
				getEntityPosition().setDirection(Direction::DirectionType::WEST);
				//entity::Entity::getEntityPosition().offset<coordinate::PixelPos>(-5,0);
			}
			if (moveRight.isActivated()) {
				viewFrom(View::EAST);
				acceleration.x = WALKING_ACCELERATION;
				getEntityPosition().setDirection(Direction::DirectionType::EAST);
				//entity::Entity::getEntityPosition().offset<coordinate::PixelPos>(5,0);
			}
			if (moveJump.isActivated()) {
				if (onGround())
					velocity.z = JUMPING_VELOCITY;
				//entity::Entity::getEntityPosition().offset<coordinate::PixelPos>(0,-5);
			}
			if (moveSniff.isActivated()) {
				//entity::Entity::getEntityPosition().offset<coordinate::PixelPos>(0,5);
			}
		}
	};
}

#endif //MINECRAFT_PLAYERENTITY_HPP
