//
// Created by robcholz on 6/11/23.
//

#ifndef MINECRAFT_ENTITY_HPP
#define MINECRAFT_ENTITY_HPP

#include <string>
#include <limits>
#include "EntityAccess.hpp"
#include "world/poi/Direction.hpp"
#include "world/WorldAccess.hpp"
#include "world/chunk/Chunk.hpp"
#include "world/chunk/ChunkStreamAccess.hpp"

namespace entity {
	class Entity : public EntityAccess, public HitboxHandler {
	private:
		using String = std::string;
		using EntityPosT = coordinate::EntityPositionT;
		using PixelPosT = coordinate::PixelPositonT;
		using BlockPosT = coordinate::BlockPositionT;
		using EntityPos = coordinate::EntityPosition;
		using BlockPos = coordinate::BlockPosition;
	protected:
		EntityPos entityPosition{0, 0};
		coordinate::Velocity velocity{0, 0};
		coordinate::Acceleration acceleration{0, 0};
		Hitbox entityHitbox{};
		HitboxHandler entityBottomBlocksHitboxHandler;
		HitboxHandler entityRightBlocksHitboxHandler;
		HitboxHandler entityLeftBlocksHitboxHandler;

		// length unit is block. time unit is second
		static constexpr float GRAVITY_ACCELERATION = -(1.0f / 60.f) * .8f;
		static constexpr float TERMINAL_HORIZONTAL_VELOCITY = (1.0f / 60.0f) * 2.0f; // 2B/s
		static constexpr float AIR_DRAG_COEFFICIENT = 0.3f;
		static constexpr float AIR_DRAG_HORIZONTAL_MULTIPLIER = 0.0001f;
		static constexpr float WALKING_ACCELERATION = (1.0f / 60.f) * 0.5; // x-axis: 0.5B/s-2
		static constexpr float JUMPING_VELOCITY = (1.0f / 60.0f) * 12;
		static constexpr float DYNAMIC_FRICTION_ACCELERATION = -(1.0f / 60.0f) * 0.1f; // 0.1B/s-2

		virtual void onUpdate() {
			updateHitbox();
			updateVelocity();
			updateFallDistance();
			updateDamage();
			getEntityPosition().offset(getVelocity().x, getVelocity().z); // apply the velocity to the entity
		}

		virtual void onRender() = 0;

		virtual void updateHitbox() = 0;

	public:
		Entity() = delete;

		explicit Entity(WorldAccess* worldAccess) {
			this->worldAccess = worldAccess;
			addHitbox(&entityHitbox);
		}

		virtual ~Entity() = default;

		WorldAccess* getWorld() override {
			return worldAccess;
		}

		EntityPos& getEntityPosition() override {
			return entityPosition;
		}

		coordinate::Velocity& getVelocity() override { return velocity; }

		EntityPosT getHeight() override {
			auto zoom = (EntityPosT) RenderSystem::Settings::pixelProportion;
			return (EntityPosT) entityHitbox.getHitbox().height / zoom;
		}

		EntityPosT getWidth() override {
			auto zoom = (EntityPosT) RenderSystem::Settings::pixelProportion;
			return (EntityPosT) entityHitbox.getHitbox().width / zoom;
		}

		float getHealth() override {
			return health;
		}

		bool isWalking() override {
			return velocity.x != 0;
		}

		bool onGround() override {
			return grounded;
		}

		bool isDamaged() override {
			return damaged;
		}

		void update() {
			onUpdate();
		}

		void render() {
			onRender();
		}

	private:
		WorldAccess* worldAccess = nullptr;
		float health = 20.f;
		float last_health = health;
		bool damaged = false;
		bool grounded = true;
		bool prevGrounded = false;
		float fallDistance = 0.f;
		float prevZ = 0.f;

		BlockPosT predicateVerticalBarrierPos() {
			for (int i = getEntityPosition().getZ<coordinate::BlockPos>() - 1; i >= 1; i--) {
				auto chunk_stream = worldAccess->getChunkStream();
				auto x = getEntityPosition().get<coordinate::BlockPos>().x;
				auto block_pos_upper = BlockPos(x, i);
				auto block_pos_lower = BlockPos(x, i - 1);
				auto block_itr_upper = chunk_stream->getBlock(block_pos_upper);
				auto block_itr_lower = chunk_stream->getBlock(block_pos_lower);
				if ((!block_itr_upper->hasHitbox() || block_itr_upper->isError()) && (block_itr_lower->hasHitbox()))
					return block_itr_lower->getPosition().get().z;
			}
			return -1;
		}

		/**
		 * @Usage find the position of non-air blocks around entity
		 * @return
		 */
		std::pair<block::Block*, block::Block*> predicateHorizontalBarrierPos() {
			BlockPos coordinate_right, coordinate_left;
			auto current_pos_x = getEntityPosition().get<coordinate::BlockPos>().x;
			auto current_pos_z = getEntityPosition().get<coordinate::BlockPos>().z;
			coordinate_right.set(current_pos_x + 1, current_pos_z + 1);
			coordinate_left.set(current_pos_x - 1, current_pos_z + 1);
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
			auto current_pos = getEntityPosition().get().z;
			auto barrier_block_pos = (EntityPosT) predicateVerticalBarrierPos();
			if (barrier_block_pos != -1 && current_pos > barrier_block_pos) {
				if (prevZ == -1)
					prevZ = current_pos;
				grounded = false;
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

			auto entity_stand_pos_x = getEntityPosition().get().x;
			auto entity_stand_pos_z = getEntityPosition().get().z;

			auto entity_stand_block_pos_x = getEntityPosition().get<coordinate::BlockPos>().x;
			auto entity_stand_block_pos_z = getEntityPosition().get<coordinate::BlockPos>().z;

			auto bottom_block_range_lower_x = entity_stand_block_pos_x;
			auto bottom_block_range_upper_x = bottom_block_range_lower_x + (BlockPosT) std::ceil(getWidth()) - 1;

			auto left_block_range_lower_z = entity_stand_block_pos_z + 1;
			auto left_block_range_upper_z = entity_stand_block_pos_z + (BlockPosT) std::ceil(getHeight());

			auto right_block_range_lower_z = entity_stand_block_pos_z + 1;
			auto right_block_range_upper_z = entity_stand_block_pos_z + (BlockPosT) std::ceil(getHeight());

			auto getBlock_ = [&](BlockPosT x, BlockPosT z) {
				return worldAccess->getChunkStream()->getBlock(BlockPos(x, z));
			};

			// find the position of a block with hitbox that has the largest z position; this block must be lower than the pos of entity
			auto predicateVerticalBarrierPos_ = [&](BlockPosT blockPosX) {
				for (int i = chunk::ChunkGenSettings::CHUNK_HEIGHT - 1; i >= 1; i--) {
					auto block_itr_upper = getBlock_(blockPosX, i);
					auto block_itr_lower = getBlock_(blockPosX, i - 1);
					if (!block_itr_upper->hasHitbox() && (block_itr_lower->hasHitbox())) {
						if ((float)block_itr_lower->getPosition().get().z < entity_stand_pos_z)
							return block_itr_lower->getPosition().get().z;
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
				auto block = getBlock_(bottom_block_range_lower_x - 1, z_left);
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
			    ((float) entity_stand_pos_z + predicted_velocity_z <= (EntityPosT) isEntityCollidedToBottom().second && velocity.z < 0)) {
				// entity is falling; entity will go through the block that has a hitbox; entity will have a collision with that block;
				grounded = true;
				acceleration.z = 0;
				velocity.z = 0;
				getEntityPosition().set((float) entity_stand_pos_x, (EntityPosT) isEntityCollidedToBottom().second);
			}

			//PLOG_DEBUG<<isEntityCollidedToLeft<<" "<<getEntityPosition().getPixelPosition().getIntX();
			if (isEntityCollidedToLeft && getEntityPosition().getDirection().isFacing(Direction::DirectionType::WEST)) {
				acceleration.x = 0;
				velocity.x = 0;
				// TODO in line 260 setEntityPosition() is commented, why not do so in this line????? strange.
				//getEntityPosition().setPosition(getEntityPosition().getBlockPosition().getFloatX() + 1, getEntityPosition().getPosition().getZ());
			}
			if (isEntityCollidedToRight && getEntityPosition().getDirection().isFacing(Direction::DirectionType::EAST)) {
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

		void updateFallDistance() {
			auto z_pos = getEntityPosition().get().z;
			if (grounded && !prevGrounded) {
				fallDistance = prevZ - z_pos;
				prevZ = z_pos;
			} else
				fallDistance = 0.f;
			prevGrounded = grounded;
		}

		void updateDamage() {
			if (fallDistance >= 3)
				health -= floor(((int) fallDistance - 3) / 2); // falling damage
			damaged = (last_health > health);
			last_health = health;
		}
	};
}

#endif //MINECRAFT_ENTITY_HPP
