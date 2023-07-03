//
// Created by robcholz on 4/13/23.
//

#ifndef RUNCRAFT_POSITION_HPP
#define RUNCRAFT_POSITION_HPP

#include <memory>
#include "util/math/MathHelper.hpp"
#include "world/poi/Coordinate.hpp"
#include "world/poi/Direction.hpp"
#include "block/attributes/BlockAccess.hpp"
#include "client/GameInfo.hpp"

namespace coordinate {
	template<typename T>
	class Position {
	protected:
		using PosT = T;
		using PixelPosT = coordinate::PixelPositonT;
	private:
		coordinate::Coordinate<PosT> position{0, 0};
		coordinate::Coordinate<PixelPosT> pixelPosition{0, 0};

		void _setPosition(PosT x, PosT z) {
			auto zoom = GameInfo.getConstExternalData()->windowState.pixelProportion;
			position.setCoordinate(x, z);
			pixelPosition.setCoordinate(floor(x * zoom), -floor(z * zoom));
		}

	public:
		Position() = delete;

		explicit Position(PosT x = 0, PosT z = 0) {
			_setPosition(x, z);
		}

		explicit Position(coordinate::Coordinate<PosT>& coordinate) {
			_setPosition(coordinate.getCoordinate().x, coordinate.getCoordinate().z);
		}

		virtual void setPosition(PosT x, PosT z) {
			_setPosition(x,z);
		}

		virtual void setPosition(coordinate::Coordinate<PosT>& coordinate) {
			_setPosition(coordinate.getCoordinate().x,coordinate.getCoordinate().z);
		}

		void offsetPosition(PosT x, PosT z){
			setPosition(getPosition().getCoordinate().x+x,getPosition().getCoordinate().z+z);
		}

		void offsetPosition(coordinate::Coordinate<PosT>& coordinate){
			setPosition(getPosition().getX()+coordinate.getX(),getPosition().getZ()+coordinate.getZ());
		}

		virtual void setPixelPosition(PixelPosT x, PixelPosT z) {
			auto zoom = (float) GameInfo.getConstExternalData()->windowState.pixelProportion;
			pixelPosition.setCoordinate(x, z);
			position.setCoordinate(floor(getPixelPosition().getFloatX() / zoom), -floor(getPixelPosition().getFloatZ() / zoom));
		}

		virtual void setPixelPosition(coordinate::Coordinate<PixelPosT>& coordinate) {
			auto zoom = (float) GameInfo.getConstExternalData()->windowState.pixelProportion;
			pixelPosition.setCoordinate(coordinate.getCoordinate().x, coordinate.getCoordinate().z);
			position.setCoordinate(floor(coordinate.getFloatX() / zoom), -floor(coordinate.getFloatZ() / zoom));
		}

		void offsetPixelPosition(PixelPosT x, PixelPosT z){
			setPixelPosition(getPixelPosition().getCoordinate().x+x,getPixelPosition().getCoordinate().z-z);
		}

		void offsetPixelPosition(coordinate::Coordinate<PixelPosT>& coordinate){
			setPixelPosition(getPixelPosition().getX()+coordinate.getCoordinate().x,getPixelPosition().getCoordinate().z-coordinate.getCoordinate().z);
		}

		coordinate::Coordinate<PixelPosT>& getPixelPosition() { return pixelPosition; }

		coordinate::Coordinate<T>& getPosition() { return position; }
	};
}

namespace block {
	using BlockDirectionType = Direction::DirectionType;

	class BlockPosition : public coordinate::Position<coordinate::BlockPositionT>, public Direction {
	private:
		using BlockPosT = coordinate::BlockPositionT;
		BlockAccess* blockAccess;
	public:
		BlockPosition() = delete;

		explicit BlockPosition(BlockAccess* blockAccess, BlockPosT x, BlockPosT z, BlockDirectionType blockDirectionType = BlockDirectionType::UP)
				: Direction(blockDirectionType), Position<coordinate::BlockPositionT>(x, z) {
			this->blockAccess = blockAccess;
		}

		explicit BlockPosition(BlockAccess* blockAccess, coordinate::Coordinate<BlockPosT>& coordinate,
		                       BlockDirectionType blockDirectionType = BlockDirectionType::UP)
				: Direction(blockDirectionType), Position<coordinate::BlockPositionT>(coordinate) {
			this->blockAccess = blockAccess;
		}

		void setPosition(PosT x, PosT z) override {
			Position::setPosition(x, z);
			blockAccess->getSprite()->setPosition(getPixelPosition().getFloatX(), getPixelPosition().getFloatZ()); // negative sign here is required
			blockAccess->onPositionChange();
		}

		void setPosition(coordinate::Coordinate<PosT>& coordinate) override {
			Position::setPosition(coordinate);
			blockAccess->getSprite()->setPosition(getPixelPosition().getFloatX(), getPixelPosition().getFloatZ()); // negative sign here is required
			blockAccess->onPositionChange();
		}
	};
}

namespace entity {
	using EntityDirectionType = Direction::DirectionType;

	class EntityPosition : public coordinate::Position<coordinate::EntityPositionT>, public Direction {
	private:
		using EntityPosT = coordinate::EntityPositionT;
		coordinate::Coordinate<coordinate::BlockPositionT> blockPosition;

		void _setBlockPosition(EntityPosT x, EntityPosT z) {
			blockPosition.setCoordinate(floor(x), floor(z));
		}

		void _setBlockPosition(coordinate::Coordinate<EntityPosT>& coordinate) {
			blockPosition.setCoordinate(floor(coordinate.getCoordinate().x), floor(coordinate.getCoordinate().z));
		}

	public:
		EntityPosition() = delete;

		explicit EntityPosition(EntityPosT x=0, EntityPosT z=0, EntityDirectionType blockDirectionType = EntityDirectionType::UP)
				: Direction(blockDirectionType), Position<coordinate::EntityPositionT>(x, z) {
			_setBlockPosition(x, z);
		}

		explicit EntityPosition(coordinate::Coordinate<EntityPosT>& coordinate, EntityDirectionType blockDirectionType = EntityDirectionType::UP)
				: Direction(blockDirectionType), Position<coordinate::EntityPositionT>(coordinate) {
			_setBlockPosition(coordinate);
		}

		void setPosition(PosT x, PosT z) override {
			Position::setPosition(x, z);
			_setBlockPosition(x, z);
		}

		void setPosition(coordinate::Coordinate<PosT>& coordinate) override {
			Position::setPosition(coordinate);
			_setBlockPosition(coordinate);
		}

		 void setPixelPosition(PixelPosT x, PixelPosT z) override {
			Position::setPixelPosition(x,z);
			 _setBlockPosition(getPosition());
		}

		 void setPixelPosition(coordinate::Coordinate<PixelPosT>& coordinate) override {
			 Position::setPixelPosition(coordinate);
			 _setBlockPosition(getPosition());
		}

		coordinate::Coordinate<coordinate::BlockPositionT>& getBlockPosition() { return blockPosition; }
	};
}


#endif //RUNCRAFT_POSITION_HPP
