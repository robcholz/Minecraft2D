//
// Created by robcholz on 4/2/23.
//

#ifndef MINECRAFT_TRANSLATABLETEXT_HPP
#define MINECRAFT_TRANSLATABLETEXT_HPP

#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include "json.hpp"
#include "util/Identifier.hpp"
#include "error/ErrorHandling.hpp"


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
		return convert(literal);
	}

	static std::wstring convert(const std::string& input)
	{
		try
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			return converter.from_bytes(input);
		}
		catch(std::range_error& e)
		{
			size_t length = input.length();
			std::wstring result;
			result.reserve(length);
			for(size_t i = 0; i < length; i++)
			{
				result.push_back(input[i] & 0xFF);
			}
			return result;
		}
	}
};

#endif //MINECRAFT_TRANSLATABLETEXT_HPP
