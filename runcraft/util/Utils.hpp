//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_UTILS_HPP
#define RUNCRAFT_UTILS_HPP

#include <string>
#include <cmath>

namespace utils {
	bool approximatelyEqual(float a, float b, float epsilon) {
		return fabs(a - b) <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
	}

	bool essentiallyEqual(float a, float b, float epsilon) {
		return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
	}

	bool definitelyGreaterThan(float a, float b, float epsilon) {
		return (a - b) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
	}

	bool definitelyLessThan(float a, float b, float epsilon) {
		return (b - a) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
	}

	template<typename T>
	T negative(T v) {
		if (v > 0)
			v = -v;
		return v;
	}

	template<typename T>
	T positive(T v) {
		if (v < 0)
			v = -v;
		return v;
	}

	/**
	 * @return return true when v is positive or zero
	 */
	template<typename T>
	bool sign(T v) {
		return (v > 0);
	}

	float constrain(float lower, float upper, float v) {
		if (v > upper)
			return upper;
		if (v < lower)
			return lower;
		return v;
	}

	int iround(float v) {
		return (int) round(v);
	}

	int nthOccurrence(const std::string& str, const std::string& findMe, int nth) {
		size_t pos = 0;
		int cnt = 0;
		while (cnt != nth) {
			pos += 1;
			pos = str.find(findMe, pos);
			if (pos == std::string::npos)
				return -1;
			cnt++;
		}
		return static_cast<int>(pos);
	}

	template<typename Iter>
	Iter nthOccurrence(Iter first, Iter last, Iter first_, Iter last_, unsigned nth) {
		Iter it = std::search(first, last, first_, last_);
		if (nth == 0) return it;
		if (it == last) return it;
		return nthOccurrence(it + std::distance(first_, last_), last, first_, last_, nth - 1);
	}

	template<typename T>
	T reverseSign(T v) {
		return -v;
	}
}

#endif //RUNCRAFT_UTILS_HPP
