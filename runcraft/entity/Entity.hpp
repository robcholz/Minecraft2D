//
// Created by robcholz on 6/11/23.
//

#ifndef RUNCRAFT_ENTITY_HPP
#define RUNCRAFT_ENTITY_HPP

#include <string>
#include <limits>
#include "world/poi/Coordinate.hpp"
#include "world/poi/Direction.hpp"
#include "world/WorldAccess.hpp"
#include "world/chunk/Chunk.hpp"
#include "world/chunk/ChunkStreamAccess.hpp"

namespace entity {
	class Entity : public HitboxHandler {
	protected:
		using String = std::string;
		using EntityPosT = coordinate::EntityPositionT;
		using PixelPosT = coordinate::PixelPositonT;
		using BlockPosT = coordinate::BlockPositionT;

		EntityPosition entityPosition{0, 0};
		coordinate::Velocity velocity{0, 0};
		coordinate::Acceleration acceleration{0, 0};
		float width = 0.f;

		// length unit is block. time unit is second
		static constexpr float GRAVITY_ACCELERATION = -(1.0f / 60.f) * .8f;
		static constexpr float TERMINAL_HORIZONTAL_VELOCITY = (1.0f / 60.0f) * 2.0f; // 2B/s
		static constexpr float AIR_DRAG_COEFFICIENT = 0.3f;
		static constexpr float AIR_DRAG_HORIZONTAL_MULTIPLIER = 0.0001f;
		static constexpr float WALKING_ACCELERATION = (1.0f / 60.f) * 0.5; // x-axis: 0.5B/s-2
		static constexpr float JUMPING_VELOCITY = (1.0f / 60.0f) * 12;
		static constexpr float DYNAMIC_FRICTION_ACCELERATION = -(1.0f / 60.0f) * 0.1f; // 0.1B/s-2

		bool onGround = true, onGroundPrev = true;
		bool onGroundStateChanged = false;
		float prevZ=-1;
		float fallDistance = 0.0f;
		AudioPlayer audio{"entity/player/hurt/mdcnm"};

		virtual void onUpdate() {
			updateHitbox();
			updateVelocity();
			updateState();
			getEntityPosition().offsetPosition(getVelocity().x, getVelocity().z); // apply the velocity to the entity
		}

		virtual void onRender() = 0;

		virtual void updateHitbox() = 0;

	public:
		Entity() = delete;

		explicit Entity(WorldAccess* worldAccess) {
			this->worldAccess = worldAccess;
			audio.addAudio();
		}

		EntityPosition& getEntityPosition() {
			return entityPosition;
		}

		coordinate::Velocity& getVelocity() { return velocity; }

		void update() {
			onUpdate();
		}

		void render() {
			onRender();
		}

	private:
		WorldAccess* worldAccess = nullptr;

		BlockPosT predicateVerticalBarrierPos() {
			for (int i = chunk::ChunkGenSettings::CHUNK_HEIGHT - 1; i >= 1; i--) {
				auto chunk_stream = worldAccess->getChunkStream();
				auto block_itr_upper = chunk_stream->getBlock(coordinate::Coordinate<BlockPosT>{getEntityPosition().getBlockPosition().getX(), i});
				auto block_itr_lower = chunk_stream->getBlock(coordinate::Coordinate<BlockPosT>{getEntityPosition().getBlockPosition().getX(), i - 1});
				if (!block_itr_upper->hasHitbox() && (block_itr_lower->hasHitbox()))
					return block_itr_lower->getPosition()->getPosition().getZ();
			}
			return -1;
		}

		/**
		 * @Usage find the position of non-air blocks around entity
		 * @return
		 */
		std::pair<block::Block*, block::Block*> predicateHorizontalBarrierPos() {
			coordinate::Coordinate<BlockPosT> coordinate_right, coordinate_left;
			auto current_pos_x = getEntityPosition().getBlockPosition().getX();
			auto current_pos_z = getEntityPosition().getBlockPosition().getZ();
			coordinate_right.setCoordinate(current_pos_x + 1, current_pos_z + 1);
			coordinate_left.setCoordinate(current_pos_x - 1, current_pos_z + 1);
			auto block_right = worldAccess->getChunkStream()->getBlock(coordinate_right);
			auto block_left = worldAccess->getChunkStream()->getBlock(coordinate_left);
			coordinate_left.offset(1, 0);
			auto block_inside_entity = worldAccess->getChunkStream()->getBlock(coordinate_left);
			block::Block* block_distance_left = nullptr, * block_distance_right = nullptr;
			if (!block_inside_entity->hasHitbox()) {
				if (block_right->hasHitbox()) block_distance_right = block_right;
				if (block_left->hasHitbox()) block_distance_left = block_left;
			} else {
				block_distance_left = block_inside_entity;
				block_distance_right = block_inside_entity;
			}

			return std::make_pair(block_distance_left, block_distance_right);
		}

		static float exertFriction(float v, float acceleration) {
			if (v > 0)v += acceleration;
			if (v < 0)v -= acceleration;
			if (abs(v) < -acceleration) v = 0;
			return v;
		}

		void exertGravity() {
			auto current_pos = getEntityPosition().getPosition().getZ();
			auto barrier_block_pos = (EntityPosT) predicateVerticalBarrierPos();
			if (barrier_block_pos != -1 && current_pos > barrier_block_pos) {
				if(prevZ==-1)
					prevZ=current_pos;
				onGround = false;
				acceleration.z = GRAVITY_ACCELERATION - acceleration.z / AIR_DRAG_COEFFICIENT;// a=g-a/k
			} // on air
		}

		void exertHorizontalAirFriction(float friction_const) {
			if (velocity.x > 0)velocity.x -= velocity.x * friction_const;
			if (velocity.x < 0)velocity.x -= velocity.x * friction_const;
		}

		void entityPositionConstrain() {
			auto predicted_velocity_x = acceleration.x + velocity.x;
			auto predicted_velocity_z = acceleration.z + velocity.z;
			auto current_pixel_pos_x = getEntityPosition().getPixelPosition().getX();
			auto current_pos_x = getEntityPosition().getPosition().getX();
			auto current_pos_z = getEntityPosition().getPosition().getZ();
			auto barrier_block_pos_x = predicateHorizontalBarrierPos();
			auto barrier_block_pos_z = (EntityPosT) predicateVerticalBarrierPos();
			if ((current_pos_z + predicted_velocity_z <= barrier_block_pos_z) && velocity.z < 0) {
				if(prevZ!=-1)
					fallDistance=prevZ-current_pos_z;
				onGround = true;
				acceleration.z = 0;
				velocity.z = 0;
				getEntityPosition().setPosition(current_pos_x, barrier_block_pos_z);
			} // z-axis constrain
			//PLOG_DEBUG << barrier_block_pos_x.first << " " << barrier_block_pos_x.second;
			if (barrier_block_pos_x.first != nullptr) {
				if ((float) current_pixel_pos_x <=
				    barrier_block_pos_x.first->getSprite()->getGlobalBounds().left + barrier_block_pos_x.first->getSprite()->getGlobalBounds().width &&
				    getEntityPosition().isFacing(Direction::DirectionType::WEST)) {
					acceleration.x = 0;
					velocity.x = 0;
					getEntityPosition().setPosition((float) barrier_block_pos_x.first->getPosition()->getPosition().getX() + 1, current_pos_z);
				}
			}
			if (barrier_block_pos_x.second != nullptr) {
				if ((float) current_pixel_pos_x + width >= barrier_block_pos_x.second->getSprite()->getGlobalBounds().left &&
				    getEntityPosition().isFacing(Direction::DirectionType::EAST)) {
					acceleration.x = 0;
					velocity.x = 0;
					//getEntityPosition().setPosition((float) barrier_block_pos_x.second, current_pos_z);
				}
			}
		}


		void updateVelocity() {
			velocity.x = exertFriction(velocity.x, DYNAMIC_FRICTION_ACCELERATION);
			exertHorizontalAirFriction(AIR_DRAG_HORIZONTAL_MULTIPLIER);
			velocity.x = utils::constrain(-TERMINAL_HORIZONTAL_VELOCITY, TERMINAL_HORIZONTAL_VELOCITY, velocity.x);
			exertGravity();
			entityPositionConstrain();
			//PLOG_DEBUG<<getEntityPosition().getBlockPosition().getX();

			velocity.x += acceleration.x;
			velocity.z += acceleration.z;
		}

		void updateState() {
			onGroundStateChanged = onGroundPrev != onGround;
			if (onGroundStateChanged&&onGround) {
				if(fallDistance>3){
					audio.play();
				}
			}
			onGroundPrev = onGround;
		}
	};
}

#endif //RUNCRAFT_ENTITY_HPP
