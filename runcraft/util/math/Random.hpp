//
// Created by robcholz on 7/1/23.
//

#ifndef RUNCRAFT_RANDOM_HPP
#define RUNCRAFT_RANDOM_HPP

#include <random>
#include <cmath>

namespace math {
	class Random {
	public:
		static auto getRange(int a, int b) {
			std::random_device dev;
			std::mt19937 rng(dev());
			std::__1::uniform_int_distribution<std::mt19937::result_type> dist6(a, b);
			return dist6(rng);
		}

		static bool randomBool() {
			return getRange(0, 1) == 1;
		}

		static bool randomBool(float probability) {
			return (getRange(1, (int) lroundf(1.f / probability)) == 1);
		}

	};
}

#endif //RUNCRAFT_RANDOM_HPP
