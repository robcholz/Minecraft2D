//
// Created by robcholz on 7/1/23.
//

#ifndef MINECRAFT_RANDOM_HPP
#define MINECRAFT_RANDOM_HPP

#include <cmath>
#include <memory>
#include <random>
#include <vector>

class Random {
 public:
  static auto getRange(int a, int b) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(a, b);
    return dist6(rng);
  }

  static bool randomBool() { return getRange(0, 1) == 1; }

  static bool randomBool(float probability) {
    return (getRange(1, (int)lroundf(1.f / probability)) == 1);
  }

  template <typename T>
  static T randomElement(const std::vector<T>& vector) {
    return vector.at(getRange(0, vector.size() - 1));
  }
};

#endif  // MINECRAFT_RANDOM_HPP
