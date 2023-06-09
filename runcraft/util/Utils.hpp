//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_UTILS_HPP
#define RUNCRAFT_UTILS_HPP

#include <string>

namespace utils{
	size_t nthOccurrence(const std::string& str, const std::string& findMe, int nth) {
		size_t pos = 0;
		int cnt = 0;
		while (cnt != nth) {
			pos += 1;
			pos = str.find(findMe, pos);
			if (pos == std::string::npos)
				return -1;
			cnt++;
		}
		return pos;
	}

	template<typename Iter>
	Iter nthOccurrence(Iter first, Iter last, Iter first_, Iter last_, unsigned nth) {
		Iter it = std::search(first, last, first_, last_);
		if (nth == 0) return it;
		if (it == last) return it;
		return nthOccurrence(it + std::distance(first_, last_), last,
		                     first_, last_, nth - 1);
	}
}

#endif //RUNCRAFT_UTILS_HPP
