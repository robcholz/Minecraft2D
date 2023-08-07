//
// Created by robcholz on 3/11/23.
//

#ifndef MINECRAFT_UTILS_HPP
#define MINECRAFT_UTILS_HPP

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

	template<typename T>
	std::string setPrecision(T number, short decimalPlace) {
		static_assert(std::is_floating_point<T>::value, "T must be either a floating point or integer type");
		char numberString[10];
		auto arg = "%." + std::to_string(decimalPlace) + "f";
		snprintf(numberString, sizeof(numberString), arg.c_str(), number);
		return numberString;
	}

	std::pair<std::string, int> execCommand(const std::string& command) {
		const char* cmd = command.c_str();
		std::array<char, 128> buffer{};
		std::string result;
		int return_code = -1;
		auto p_close_wrapper = [&return_code](FILE* cmd) { return_code = pclose(cmd); };
		{
			const std::unique_ptr<FILE, decltype(p_close_wrapper)> pipe(popen(cmd, "r"), p_close_wrapper);
			if (pipe) {
				while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
					result += buffer.data();
				}
			}
			// remove the newline char from result
			result.erase(std::remove(result.begin(), result.end(), '\n'), result.cend());
		}
		return make_pair(result, return_code);
	}
}

#endif //MINECRAFT_UTILS_HPP
