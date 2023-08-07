//
// Created by robcholz on 8/6/23.
//
#pragma once

#ifndef MINECRAFT_2D_SYSTEM_HPP
#define MINECRAFT_2D_SYSTEM_HPP


#include <string>
#include <map>
#include <type_traits>
#include <thread>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <plist/Plist.hpp>
#include "util/Utils.hpp"
#include "plog/Log.h"


#ifdef _WIN32

#elif _WIN64

#elif __APPLE__ || __MACH__

#include <sys/utsname.h>
#include <sys/proc_info.h>
#include <sys/types.h>

#elif __linux__

#elif __FreeBSD__

#elif __unix || __unix__

#else
// none
#endif


class OS {
private:
	using String = std::string;
public:
	static void init() {
		processor_count = std::thread::hardware_concurrency();
#ifdef _WIN32
		os_name = "Windows 32-bit";
#elif _WIN64
		os_name =  "Windows 64-bit";
#elif __APPLE__ || __MACH__
		struct utsname uts{};
		std::map<String, boost::any> dict;
		uname(&uts);
		Plist::readPlist("/System/Library/CoreServices/SystemVersion.plist", dict);
		os_name = boost::any_cast<String>(dict["ProductName"]);
		os_version = boost::any_cast<String>(dict["ProductVersion"]);
		os_architecture = String(uts.machine);
		cpu_model_name = utils::execCommand("sysctl -n machdep.cpu.brand_string").first;
		gpu_model_name = getExecCommandString("system_profiler SPDisplaysDataType | grep Chipset", "Chipset Model:");
		screen_info = getExecCommandString("system_profiler SPDisplaysDataType | grep Resolution", "Resolution:");
		pid = getpid();
		RAM_total = getExecCommandNumeric<long>("sysctl -n hw.memsize", "") / 1024 / 1024;
#elif __linux__
		os_name = "Linux";
#elif __FreeBSD__
		os_name = "FreeBSD";
#elif __unix || __unix__
		os_name = "Unix";
#else
		os_name = "Uknown";
		os_version = "Uknown";
		os_architecture = "Uknown";
		pid = -1;
#endif
	}

	static String getOSName() {
		return os_name;
	}

	static String getOSVersion() {
		return os_version;
	}

	static String getOSArch() {
		return os_architecture;
	}

	static String getCPUModelName() {
		return cpu_model_name;
	}

	static String getGPUModelName() {
		return gpu_model_name;
	}

	static String getScreenInfo() {
		return screen_info;
	}

	template<typename T>
	static T getPID() {
		static_assert(std::is_same<T, signed int>::value || std::is_same<T, String>::value, "T must be either signed int or String");
		if constexpr (std::is_same<T, signed int>::value)
			return pid;
		if constexpr (std::is_same<T, String>::value) {
			static String str = std::to_string(pid);
			return str;
		}
	}

	static unsigned int getTotalRAM() {
		return RAM_total;
	}

	static unsigned int getProcessorCount() {
		return processor_count;
	}

	template<typename T>
	static T getProcessCPUUsagePercent() {
		static_assert(std::is_same<T, String>::value || std::is_same<T, int>::value,
		              "T must be either String or int");
		if constexpr (std::is_same<T, String>::value)
			return getExecCommandString("ps -p " + getPID<String>() + " -o %cpu", "%CPU");
		if constexpr (std::is_same<T, int>::value)
			return getExecCommandNumeric<int>("ps -p " + getPID<String>() + " -o %cpu", "%CPU");
		return -1;
	}

	static int getGPUUsage() {
		return -1;
		//TODO to implement
	}

	static int getProcessMemoryUsage() {
		float mem_percent = getExecCommandNumeric<float>("ps -p " + getPID<String>() + " -o %mem", "%MEM") / 100;
		auto mem_resident = (int) (getExecCommandNumeric<float>("ps -p " + getPID<String>() + " -o rss", "RSS") / 1024 / 1024);
		return (int) (mem_percent * (float) getTotalRAM()) + mem_resident;
	}

	static int getProcessMemoryUsagePercent() {
		return getExecCommandNumeric<int>("ps -p " + getPID<String>() + " -o %mem", "%MEM");
	}

	static String getProcessRunningTime() {
		return getExecCommandString("ps -p " + getPID<String>() + " -o etime", "ELAPSED");
	}

private:
	static inline String os_name;
	static inline String os_version;
	static inline String os_architecture;

	static inline String cpu_model_name;
	static inline String gpu_model_name;
	static inline String screen_info;

	// the unit is MB
	static inline unsigned int RAM_total;
	static inline unsigned int processor_count;

	static inline signed int pid;

	static void removeString(String& str, const String& remove) {
		boost::erase_all(str, remove);
		boost::trim(str);
	}

	static String getExecCommandString(const String& cmd, const String& remove) {
		auto str = utils::execCommand(cmd).first;
		removeString(str, remove);
		return str;
	}

	template<typename T>
	static T getExecCommandNumeric(const String& cmd, const String& remove) {
		auto str = getExecCommandString(cmd, remove);
		char* end_ptr;
		T integer;
		if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value)
			integer = (T) std::strtof(str.c_str(), &end_ptr);
		else
			integer = (T) std::strtol(str.c_str(), &end_ptr, 10);
		if (end_ptr != str || *end_ptr == '\0')
			return integer;
		PLOG_ERROR << "Failed to convert to number. Invalid String:" << str << " Output:" << integer;
		return -1;
	}
};


#endif //MINECRAFT_2D_SYSTEM_HPP
