//
// Created by robcholz on 4/29/23.
//

#ifndef RUNCRAFT_PLAYER_HPP
#define RUNCRAFT_PLAYER_HPP

#include <memory>
#include "client/input/Input.hpp"
#include "world/poi/Coordinate.hpp"
#include "world/poi/Direction.hpp"

class SkinLoader {};

class Player {
protected:
private:
	using String = std::string;
	using EntityPosT = coordinate::EntityPositionT;
	using PixelPosT = coordinate::PixelPositonT;
	coordinate::Coordinate<EntityPosT> playerCoordinate{0, 0};
	coordinate::CoordinateStruct<EntityPosT> playerCoordinateOffset{0,0};
	Direction direction;

	input::KeyboardObserver moveLeft, moveRight, moveJump, moveSniff;
	String playerSkinAssetPath = "../assets/textures/player/steve.png";

	class PlayerSkin : SkinLoader {
	private:
		using sfRecti = sf::IntRect;
		using Texture = sf::Texture;
		using Sprite = sf::Sprite;
		Texture leftLeg, leftArm, leftHead;
		Texture rightLeg, rightArm, rightHead;
		Sprite legSprite, armSprite, headSprite;

		coordinate::Coordinate<EntityPosT> coordinate;
	public:
		enum class View : uint8_t { EAST, WEST };

		explicit PlayerSkin(const String& assetPath) {
			leftLeg.loadFromFile(assetPath, sfRecti{24, 52, 4, 12}); // (24,52) (27,63)
			leftArm.loadFromFile(assetPath, sfRecti{40, 52, 4, 12}); // (40,52) (43,63)
			leftHead.loadFromFile(assetPath, sfRecti{16, 8, 8, 8}); // (16,8) (23,15)
			rightLeg.loadFromFile(assetPath, sfRecti{0, 20, 4, 12}); // (0,20) (3,31)
			rightArm.loadFromFile(assetPath, sfRecti{40, 20, 4, 12}); // (40,20) (43,31)
			rightHead.loadFromFile(assetPath, sfRecti{0, 8, 8, 8}); // (0,8) (7,15)

			auto pixelToOne = (float) GameInfo.getConstExternalData()->windowState.pixelProportion / 16.0f;
			legSprite.setTexture(rightLeg);
			legSprite.scale(pixelToOne, pixelToOne);
			armSprite.setTexture(rightArm);
			armSprite.scale(pixelToOne, pixelToOne);
			headSprite.setTexture(rightHead);
			headSprite.scale(pixelToOne, pixelToOne);
		}

		PlayerSkin& viewFrom(View view) {
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
			return *this;
		}

		PlayerSkin& setPosition(EntityPosT x, EntityPosT z) {
			coordinate.setCoordinate(x, z);
			legSprite.setPosition(x, z);
			armSprite.setPosition(x, z - armSprite.getGlobalBounds().height);
			headSprite.setPosition(x - headSprite.getGlobalBounds().width / 4, z - armSprite.getGlobalBounds().height - headSprite.getGlobalBounds().height);
			return *this;
		}

		PlayerSkin& offset(EntityPosT x, EntityPosT z) {
			setPosition(coordinate.getX() + x, coordinate.getZ() + z);
			return *this;
		}

		coordinate::Coordinate<EntityPosT>& getPosition() {
			return coordinate;
		};

		void render() {
			GameInfo.getRender()->render(legSprite);
			GameInfo.getRender()->render(armSprite);
			GameInfo.getRender()->render(headSprite);
		}
	};

	PlayerSkin playerSkin{playerSkinAssetPath};

public:
	explicit Player() {
		moveLeft.attachKey(input::KeyboardKeyType::A);
		moveRight.attachKey(input::KeyboardKeyType::D);
		moveJump.attachKey(input::KeyboardKeyType::Space);
		moveSniff.attachKey(input::KeyboardKeyType::LShift);
		//setPosition(100, 0);
	}

	Player& setPosition(EntityPosT x, EntityPosT z) {
		playerCoordinate.setCoordinate(x, z);
		playerSkin.setPosition(x, z);
		return *this;
	}

	Player& offset(EntityPosT x,EntityPosT z){
		if(x<0)
			playerSkin.viewFrom(PlayerSkin::View::WEST);
		if(x>0)
			playerSkin.viewFrom(PlayerSkin::View::EAST);
		setPosition(playerCoordinate.getX()+x,playerCoordinate.getZ()+z);
		return *this;
	}

	coordinate::Coordinate<EntityPosT>* getPosition(){
		return &playerCoordinate;
	}

	Direction* getDirection(){
		return &direction;
	}

	coordinate::CoordinateStruct<EntityPosT> getOffset(){
		return playerCoordinateOffset;
	}

	void update() {
		playerCoordinateOffset.x=0;
		playerCoordinateOffset.z=0;
		if (moveLeft.isActivated()) {
			offset(-5,0);
			playerCoordinateOffset.x=-5;
		}
		if (moveRight.isActivated()) {
			offset(5,0);
			playerCoordinateOffset.x=5;
		}
		if (moveJump.isActivated()) {
			offset(0, -5);
			playerCoordinateOffset.z=-5;
		}
		if (moveSniff.isActivated()) {
			offset(0, 5);
			playerCoordinateOffset.z=5;
		}
	}

	void render() {
		playerSkin.render();
	}
};

#endif //RUNCRAFT_PLAYER_HPP
