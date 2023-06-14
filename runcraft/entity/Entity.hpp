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
		Hitbox entityHitbox{};
		HitboxHandler entityBottomBlocksHitboxHandler;
		HitboxHandler entityRightBlocksHitboxHandler;
		HitboxHandler entityLeftBlocksHitboxHandler;

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
		float prevZ = -1;
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
			addHitbox(&entityHitbox);
			audio.addAudio();
		}

		EntityPosition& getEntityPosition() {
			return entityPosition;
		}

		coordinate::Velocity& getVelocity() { return velocity; }

		EntityPosT getHeight() {
			auto zoom = (EntityPosT) GameInfo.getConstExternalData()->windowState.pixelProportion;
			return (EntityPosT) entityHitbox.getHitbox().height / zoom;
		}

		EntityPosT getWidth() {
			auto zoom = (EntityPosT) GameInfo.getConstExternalData()->windowState.pixelProportion;
			return (EntityPosT) entityHitbox.getHitbox().width / zoom;
		}

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
				if (prevZ == -1)
					prevZ = current_pos;
				onGround = false;
				acceleration.z = GRAVITY_ACCELERATION - acceleration.z / AIR_DRAG_COEFFICIENT;// a=g-a/k
			} // on air
		}

		void exertHorizontalAirFriction(float friction_const) {
			if (velocity.x > 0)velocity.x -= velocity.x * friction_const;
			if (velocity.x < 0)velocity.x -= velocity.x * friction_const;
		}

		void handleCollision() {
			// step. fill bottom, left, right containers
			entityBottomBlocksHitboxHandler.clearHitboxesList();
			entityLeftBlocksHitboxHandler.clearHitboxesList();
			entityRightBlocksHitboxHandler.clearHitboxesList();

			auto predicted_velocity_x = acceleration.x + velocity.x;
			auto predicted_velocity_z = acceleration.z + velocity.z;

			auto entity_stand_pos_x = getEntityPosition().getPosition().getX();
			auto entity_stand_pos_z = getEntityPosition().getPosition().getZ();

			auto entity_stand_block_pos_x = getEntityPosition().getBlockPosition().getX();
			auto entity_stand_block_pos_z = getEntityPosition().getBlockPosition().getZ();

			auto bottom_block_range_lower_x = entity_stand_block_pos_x;
			auto bottom_block_range_upper_x = bottom_block_range_lower_x + (BlockPosT) std::ceil(getWidth()) - 1;

			auto left_block_range_lower_z = entity_stand_block_pos_z + 1;
			auto left_block_range_upper_z = entity_stand_block_pos_z + (BlockPosT) std::ceil(getHeight());

			auto right_block_range_lower_z = entity_stand_block_pos_z + 1;
			auto right_block_range_upper_z = entity_stand_block_pos_z + (BlockPosT) std::ceil(getHeight());

			auto getBlock_ = [&](BlockPosT x, BlockPosT z) {
				return worldAccess->getChunkStream()->getBlock(coordinate::Coordinate<BlockPosT>(x, z));
			};

			// find the position of a block with hitbox that has the largest z position; this block must be lower than the pos of entity
			auto predicateVerticalBarrierPos_ = [&](BlockPosT blockPosX) {
				for (int i = chunk::ChunkGenSettings::CHUNK_HEIGHT - 1; i >= 1; i--) {
					auto block_itr_upper = getBlock_(blockPosX, i);
					auto block_itr_lower = getBlock_(blockPosX, i - 1);
					if (!block_itr_upper->hasHitbox() && (block_itr_lower->hasHitbox())) {
						if (block_itr_lower->getPosition()->getPosition().getFloatZ() < entity_stand_pos_z)
							return block_itr_lower->getPosition()->getPosition().getZ();
					} // has hitbox
				}
				return -1;
			};

			auto isEntityCollidedToBottom = [&]() {
				auto maximum_x = 0;
				auto maximum_z = -1;
				for (auto x = bottom_block_range_lower_x; x <= bottom_block_range_upper_x; ++x) {
					auto barrier_z = predicateVerticalBarrierPos_(x);
					if (barrier_z > maximum_z) {
						maximum_z = barrier_z;
						maximum_x = x;
					}
				}/*now we found the barrier block that has the max z position*/
				bool isCollided = false;
				int z_pos = -1;
				if (maximum_z != -1) {
					//PLOG_DEBUG << "Bottom Range: " << x << " " << entity_stand_block_pos_z;
					// set entity position to the barrier pos for a moment...
					auto hitbox = entityHitbox.getHitbox();
					auto blockHitbox = getBlock_(maximum_x, maximum_z)->getHitbox();
					auto entityFutureHitbox = Hitbox(hitbox.x, blockHitbox->getHitbox().y - hitbox.height / 2, hitbox.width, hitbox.height);
					isCollided = Hitbox::isCollided(Hitbox(*blockHitbox), entityFutureHitbox, true);
					z_pos = maximum_z;
				} // such a block exists; add it to the collision detection list and handle it.
				return std::make_pair(isCollided, z_pos);
			};

			for (auto z_left = left_block_range_lower_z; z_left <= left_block_range_upper_z; ++z_left) {
				//TODO in line 230 x should plus 1; why x stayed constant here??
				auto block = getBlock_(bottom_block_range_lower_x, z_left);
				if (block->isAir())
					block->getHitbox()->setHitbox(0, 0, 0, 0);
				entityLeftBlocksHitboxHandler.addHitbox(block->getHitbox());
			}

			for (auto z_right = right_block_range_lower_z; z_right <= right_block_range_upper_z; ++z_right) {
				auto block = getBlock_(bottom_block_range_upper_x + 1, z_right); //TODO pretty wried!
				if (block->isAir())
					block->getHitbox()->setHitbox(0, 0, 0, 0);
				entityRightBlocksHitboxHandler.addHitbox(block->getHitbox());
			}

			auto isEntityCollidedToLeft = isCollided(&entityLeftBlocksHitboxHandler, true);
			auto isEntityCollidedToRight = isCollided(&entityRightBlocksHitboxHandler, true);

			if (isEntityCollidedToBottom().first &&
			    (entity_stand_pos_z + predicted_velocity_z <= (EntityPosT) isEntityCollidedToBottom().second && velocity.z < 0)) {
				// entity is falling; entity will go through the block that has a hitbox; entity will have a collision with that block;
				onGround = true;
				acceleration.z = 0;
				velocity.z = 0;
				getEntityPosition().setPosition(entity_stand_pos_x, (EntityPosT) isEntityCollidedToBottom().second);
			}

			//PLOG_DEBUG<<isEntityCollidedToLeft<<" "<<getEntityPosition().getPixelPosition().getIntX();
			if (isEntityCollidedToLeft && getEntityPosition().isFacing(Direction::DirectionType::WEST)) {
				acceleration.x = 0;
				velocity.x = 0;
				// TODO in line 260 setEntityPosition() is commented, why not do so in this line????? strange.
				getEntityPosition().setPosition(getEntityPosition().getBlockPosition().getFloatX() + 1, getEntityPosition().getPosition().getZ());
			}
			if (isEntityCollidedToRight && getEntityPosition().isFacing(Direction::DirectionType::EAST)) {
				acceleration.x = 0;
				velocity.x = 0;
				//auto block=getBlock_(entity_stand_block_pos_x + 1, entity_stand_block_pos_z + 1)
				// TODO whyyyyyyyy
				//getEntityPosition().setPixelPosition(block->getHitbox()->getHitbox().x - getHitbox()->getHitbox().width,getEntityPosition().getPixelPosition().getZ());
			}
		}


		void updateVelocity() {
			velocity.x = exertFriction(velocity.x, DYNAMIC_FRICTION_ACCELERATION);
			exertHorizontalAirFriction(AIR_DRAG_HORIZONTAL_MULTIPLIER);
			velocity.x = utils::constrain(-TERMINAL_HORIZONTAL_VELOCITY, TERMINAL_HORIZONTAL_VELOCITY, velocity.x);
			exertGravity();
			handleCollision();
			velocity.x += acceleration.x;
			velocity.z += acceleration.z;
		}

		void updateState() {
		}
	};
}

#endif //RUNCRAFT_ENTITY_HPP
