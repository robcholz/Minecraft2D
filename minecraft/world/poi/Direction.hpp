//
// Created by robcholz on 6/8/23.
//

#ifndef MINECRAFT_DIRECTION_HPP
#define MINECRAFT_DIRECTION_HPP

#include <cstdint>

class Direction {
 public:
  using DirectionT = uint8_t;

  enum class DirectionType : uint8_t { UP, DOWN, WEST, EAST, IN, OUT };

  explicit Direction(DirectionType blockDirection = DirectionType::UP) {
    direction = blockDirection;
  }

  DirectionType getDirection() { return direction; }

  void setDirection(DirectionType blockDirection) {
    direction = blockDirection;
  }

  Direction& up() {
    direction = DirectionType::UP;
    return *this;
  }

  Direction& down() {
    direction = DirectionType::DOWN;
    return *this;
  }

  Direction& west() {
    direction = DirectionType::WEST;
    return *this;
  }

  Direction& east() {
    direction = DirectionType::EAST;
    return *this;
  }

  Direction& in() {
    direction = DirectionType::IN;
    return *this;
  }

  Direction& out() {
    direction = DirectionType::OUT;
    return *this;
  }

  bool isFacing(DirectionType faceDirection) {
    return (this->direction == faceDirection);
  }

 private:
  DirectionType direction;
};

#endif  // MINECRAFT_DIRECTION_HPP
