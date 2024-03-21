//
// Created by robcholz on 8/6/23.
//
#pragma once

#ifndef MINECRAFT_2D_SYSTEM_HPP
#define MINECRAFT_2D_SYSTEM_HPP

#include <plog/Log.h>
#include <tinyxml2.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/format.hpp>
#include <map>
#include <string>
#include <thread>
#include <type_traits>
#include "util/Utils.hpp"

class OS {
 private:
  using String = std::string;

 public:
  static void init();

  static String getOSName() { return os_name; }

  static String getOSVersion() { return os_version; }

  static String getOSArch() { return os_architecture; }

  static String getCPUModelName() { return cpu_model_name; }

  static String getGPUModelName() { return gpu_model_name; }

  static String getScreenInfo() { return screen_info; }

  template <typename T>
  static T getPID() {
    static_assert(
        std::is_same<T, signed int>::value || std::is_same<T, String>::value,
        "T must be either signed int or String");
    if constexpr (std::is_same<T, signed int>::value)
      return pid;
    if constexpr (std::is_same<T, String>::value) {
      static String str = std::to_string(pid);
      return str;
    }
  }

  static unsigned int getTotalRAM() { return RAM_total; }

  static unsigned int getProcessorCount() { return processor_count; }

  static uint8_t getProcessCPUUsagePercent();

  static int getGPUUsage() {
    return -1;
    // TODO to implement
  }

  static uint64_t getProcessMemoryUsage();

  static uint8_t getProcessMemoryUsagePercent() {
    return getProcessMemoryUsage() / getTotalRAM();
  }

  static String getProcessRunningTime() {
    return getExecCommandString("ps -p " + getPID<String>() + " -o etime",
                                "ELAPSED");
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

  static void loadPlistFile(tinyxml2::XMLDocument& doc,
                            std::map<String, String>& dictMap) {
    if (doc.LoadFile("/System/Library/CoreServices/SystemVersion.plist") !=
        tinyxml2::XML_SUCCESS) {
      PLOG_FATAL << "Cannot load System Version Configuration Files";
      dictMap["ProductName"] = "Unknown Mac";
      dictMap["ProductVersion"] = "Unknown Mac Version";
      return;
    }
    tinyxml2::XMLNode* root = doc.FirstChildElement("plist");
    if (!root) {
      PLOG_FATAL << "Should never get there!";
      std::exit(1);
    }
    tinyxml2::XMLElement* dict = root->FirstChildElement("dict");
    if (!dict) {
      PLOG_FATAL << "Should never get there!";
      std::exit(1);
    }
    tinyxml2::XMLElement* key = dict->FirstChildElement("key");
    while (key) {
      const char* keyName = key->GetText();
      tinyxml2::XMLElement* value = key->NextSiblingElement("string");
      if (value) {
        const char* valueText = value->GetText();
        if (keyName && valueText) {
          dictMap[String(keyName)] = String(valueText);
        }
      }
      key = value->NextSiblingElement("key");
    }
  }

  static void removeString(String& str, const String& remove) {
    boost::erase_all(str, remove);
    boost::trim(str);
  }

  static String getExecCommandString(const String& cmd, const String& remove) {
    auto str = utils::execCommand(cmd).first;
    removeString(str, remove);
    return str;
  }

  template <typename T>
  static inline T getExecCommandNumeric(const String& cmd,
                                        const String& remove) {
    auto str = getExecCommandString(cmd, remove);
    char* end_ptr;
    T integer;
    if constexpr (std::is_same<T, float>::value ||
                  std::is_same<T, double>::value)
      integer = (T)std::strtof(str.c_str(), &end_ptr);
    else
      integer = (T)std::strtol(str.c_str(), &end_ptr, 10);
    if (end_ptr != str || *end_ptr == '\0')
      return integer;
    PLOG_ERROR << "Failed to convert to number. Invalid String:" << str
               << " Output:" << integer;
    return -1;
  }
};

#endif  // MINECRAFT_2D_SYSTEM_HPP
