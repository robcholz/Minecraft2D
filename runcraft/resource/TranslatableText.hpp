//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_TRANSLATABLETEXT_HPP
#define RUNCRAFT_TRANSLATABLETEXT_HPP

#include <fstream>
#include <string>
#include <util/Identifier.hpp>
#include "json.hpp"
#include "error/ErrorHandling.hpp"


class TranslatableText {
private:
	using Json = nlohmann::json;
	using String = std::string;
public:
	TranslatableText() = default;

	TranslatableText(const TranslatableText& other) = delete;

	~TranslatableText() = default;

	static String getTranslatable(Identifier& identifier) {
		std::ifstream file(Path::currentLangPath);
		Json lang_json = Json::parse(file);
		return lang_json[identifier.getPath()];
	}
};

#endif //RUNCRAFT_TRANSLATABLETEXT_HPP
