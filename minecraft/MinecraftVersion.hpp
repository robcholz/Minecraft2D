//
// Created by robcholz on 8/6/23.
//
#ifndef MINECRAFT_2D_MINECRAFTVERSION_HPP
#define MINECRAFT_2D_MINECRAFTVERSION_HPP


#include <string>
#include <boost/date_time.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "util/Date.hpp"


class MinecraftVersion {
private:
	using String = std::string;
public:
	MinecraftVersion() = delete;

	MinecraftVersion(const MinecraftVersion&) = default;

	~MinecraftVersion() = default;

	static void init() {
		id = boost::uuids::to_string(boost::uuids::random_generator()());
		boost::erase_all(id, "-");
		name = "Minecraft";
		modified = false;
		stable = false;
		worldVersion = 2730;
		protocolVersion = 756;
		packVersion = 8;
		buildTime = Date::getUTCBuildTime(__DATE__, __TIME__);
		releaseTarget = "1.12.1";
	}

	static String getId() {
		return id;
	}

	static String getName() {
		return name;
	}

	static String getReleaseTarget() {
		return releaseTarget;
	}

	static String getProductionName() {
		return getName() + " " + getReleaseTarget();
	}

	static int getWorldVersion() {
		return worldVersion;
	}

	static int getProtocolVersion() {
		return protocolVersion;
	}

	static int getPackVersion() {
		return packVersion;
	}

	static String getBuildTime() {
		return buildTime;
	}

	template<typename T>
	static T isStable() {
		static_assert(std::is_same<T, bool>::value || std::is_same<T, String>::value,
		              "T must be either bool or String");
		if constexpr (std::is_same<T, bool>::value)
			return stable;
		if constexpr (std::is_same<T, String>::value) {
			if (stable)
				return "release";
			return "snapshot";
		}
	}

	template<typename T>
	static T isModified() {
		static_assert(std::is_same<T, bool>::value || std::is_same<T, String>::value,
		              "T must be either bool or String");
		if constexpr (std::is_same<T, bool>::value)
			return stable;
		if constexpr (std::is_same<T, String>::value) {
			if (modified)
				return "modified";
			return "vanilla";
		}
	}

private:
	static inline String id;
	static inline String name;
	static inline bool stable;
	static inline bool modified;
	static inline int worldVersion;
	static inline int protocolVersion;
	static inline int packVersion;
	static inline String buildTime;
	static inline String releaseTarget;
};

#endif //MINECRAFT_2D_MINECRAFTVERSION_HPP
