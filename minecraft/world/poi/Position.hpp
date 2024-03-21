//
// Created by robcholz on 4/13/23.
//

#ifndef MINECRAFT_POSITION_HPP
#define MINECRAFT_POSITION_HPP

#include <type_traits>
#include "util/math/MathHelper.hpp"
#include "world/poi/Coordinate.hpp"
#include "world/poi/Direction.hpp"

namespace coordinate {
/**
 * @brief stores the direction position and converts the position of the targets
 * @tparam DefaultPosT accepts coordinate::EntityPos, coordinate::BlockPos, and
 * coordinate::PixelPos only
 */
template <typename DefaultPosT>
class Position {
 private:
  PixelPos pixelPosition{0, 0};
  PixelPos pixelPositionOffset{0, 0};
  Direction direction{Direction::DirectionType::OUT};

 public:
  Position() = default;

  explicit Position(
      typename DefaultPosT::type x,
      typename DefaultPosT::type z,
      Direction::DirectionType edirection = Direction::DirectionType::OUT) {
    pixelPosition = convertToPixelPos<DefaultPosT>(DefaultPosT(x, z));
    pixelPositionOffset.x = 0;
    pixelPositionOffset.z = 0;
    direction.setDirection(edirection);
  }

  explicit Position(DefaultPosT& position, Direction& direction) {
    pixelPosition = convertToPixelPos(position);
    setDirection(direction);
  }

  /**
   *
   * @tparam SrcPosT struct that will be converted from
   * @tparam DesPosT struct that will be converted to
   * @param position struct that will be converted from
   * @return struct that will be converted to
   */
  template <typename SrcPosT, typename DesPosT>
  static DesPosT convertTo(SrcPosT position) {
    static_assert(std::is_same<SrcPosT, EntityPos>::value ||
                      std::is_same<SrcPosT, BlockPos>::value ||
                      std::is_same<SrcPosT, PixelPos>::value,
                  "convertTo() only accepts parameters: EntityPos, BlockPos, "
                  "and PixelPos as source types");
    static_assert(std::is_same<DesPosT, EntityPos>::value ||
                      std::is_same<DesPosT, BlockPos>::value ||
                      std::is_same<DesPosT, PixelPos>::value,
                  "convertTo() only accepts parameters: EntityPos, BlockPos, "
                  "and PixelPos as destination types");
    if constexpr (std::is_same<SrcPosT, BlockPos>::value &&
                  std::is_same<PixelPos, DesPosT>::value) {
      auto zoom = RenderSystem::Settings::pixelProportion;
      return PixelPos(position.x * zoom, -position.z * zoom);
    }
    if constexpr (std::is_same<EntityPos, SrcPosT>::value &&
                  std::is_same<PixelPos, DesPosT>::value) {
      auto zoom = RenderSystem::Settings::pixelProportion;
      return PixelPos(math::floor(position.x * zoom),
                      -math::floor(position.z * zoom));
    }
    if constexpr (std::is_same<PixelPos, SrcPosT>::value &&
                  std::is_same<BlockPos, DesPosT>::value) {
      auto zoom = (float)RenderSystem::Settings::pixelProportion;
      return BlockPos(math::floor(position.x / zoom),
                      -math::floor(position.z / zoom));
    }
    if constexpr (std::is_same<PixelPos, SrcPosT>::value &&
                  std::is_same<EntityPos, DesPosT>::value) {
      auto zoom = (float)RenderSystem::Settings::pixelProportion;
      return EntityPos(position.x / zoom, -position.z / zoom);
    }
    if constexpr (std::is_same<SrcPosT, DesPosT>::value) {
      return position;
    }
    return DesPosT(static_cast<typename DesPosT::type>(position.x),
                   static_cast<typename DesPosT::type>(position.z));
  }

  template <typename SrcPosT>
  static PixelPos convertToPixelPos(SrcPosT position) {
    return convertTo<SrcPosT, PixelPos>(position);
  }

  template <typename PosT = DefaultPosT>
  void set(PosT position) {
    pixelPosition = convertToPixelPos(position);
  }

  template <typename PosT = DefaultPosT>
  void set(typename PosT::type x, typename PosT::type z) {
    pixelPosition = convertToPixelPos(PosT(x, z));
  }

  void setDirection(Direction& edirection) { this->direction = edirection; }

  void setDirection(Direction::DirectionType directionType) {
    this->direction.setDirection(directionType);
  }

  template <typename PosT = DefaultPosT>
  void offset(PosT& offsetPosition) {
    pixelPositionOffset = convertToPixelPos(offsetPosition);
    pixelPosition.x += pixelPositionOffset.x;
    pixelPosition.z += pixelPositionOffset.z;
  }

  template <typename PosT = DefaultPosT>
  void offset(typename PosT::type x, typename PosT::type z) {
    pixelPositionOffset = convertToPixelPos(PosT(x, z));
    pixelPosition.x += pixelPositionOffset.x;
    pixelPosition.z += pixelPositionOffset.z;
  }

  template <typename PosT = DefaultPosT>
  PosT get() const {
    return convertTo<PixelPos, PosT>(pixelPosition);
  }

  template <typename PosT = DefaultPosT>
  typename PosT::type getX() const {
    return convertTo<PixelPos, PosT>(pixelPosition).x;
  }

  template <typename PosT = DefaultPosT>
  typename PosT::type getZ() const {
    return convertTo<PixelPos, PosT>(pixelPosition).z;
  }

  template <typename PosT = DefaultPosT>
  PosT getOffset() const {
    return convertTo<PixelPos, PosT>(pixelPositionOffset);
  }

  Direction& getDirection() { return direction; }
};

using BlockPosition = Position<BlockPos>;
using EntityPosition = Position<EntityPos>;
using PixelPosition = Position<PixelPos>;
using DirectionType = Direction::DirectionType;
}  // namespace coordinate

#endif  // MINECRAFT_POSITION_HPP
