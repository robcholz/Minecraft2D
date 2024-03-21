//
// Created by robcholz on 4/2/23.
//

#ifndef MINECRAFT_TRANSLATABLETEXT_HPP
#define MINECRAFT_TRANSLATABLETEXT_HPP

#include <codecvt>
#include <fstream>
#include <locale>
#include <string>

#include <nlohmann/json.hpp>
#include "error/ErrorHandling.hpp"
#include "util/Identifier.hpp"

class TranslatableText {
 private:
  using Json = nlohmann::json;

 public:
  TranslatableText() = delete;

  TranslatableText(const TranslatableText& other) = delete;

  ~TranslatableText() = delete;

  static std::wstring getTranslatable(Identifier& identifier) {
    std::ifstream file(Path::currentLangPath);
    Json lang_json = Json::parse(file);
    std::string literal = lang_json[identifier.getDotPath()];
    return convert(literal);  // TODO
  }

  static std::wstring convert(const std::string& input) {
    std::wstring result;
    // Calculate the required length of the resulting wide string
    size_t requiredLength = mbstowcs(nullptr, input.c_str(), 0);
    if (requiredLength == static_cast<size_t>(-1)) {
      // Error handling for conversion failure
      return L"";
    }

    result.resize(requiredLength);
    // Perform the actual conversion
    if (mbstowcs(&result[0], input.c_str(), requiredLength) ==
        static_cast<size_t>(-1)) {
      // Error handling for conversion failure
      return L"";
    }
    return result;
  }
};

#endif  // MINECRAFT_TRANSLATABLETEXT_HPP
