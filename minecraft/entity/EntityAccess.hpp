//
// Created by robcholz on 6/15/23.
//

#ifndef MINECRAFT_ENTITYACCESS_HPP
#define MINECRAFT_ENTITYACCESS_HPP

#include "world/poi/Coordinate.hpp"
#include "world/poi/Position.hpp"

class WorldAccess;

class EntityAccess {
 public:
  virtual WorldAccess* getWorld() = 0;

  virtual coordinate::Velocity& getVelocity() = 0;

  virtual coordinate::Acceleration& getAcceleration() = 0;

  virtual coordinate::EntityPosition& getEntityPosition() = 0;

  virtual coordinate::EntityPositionT getHeight() = 0;

  virtual coordinate::EntityPositionT getWidth() = 0;

  virtual float getHealth() = 0;

  virtual float getMaxHealth() = 0;

  virtual bool isWalking() = 0;

  virtual bool onGround() = 0;

  virtual bool isDamaged() = 0;
};

#endif  // MINECRAFT_ENTITYACCESS_HPP
