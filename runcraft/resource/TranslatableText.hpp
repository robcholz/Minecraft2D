//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_TRANSLATABLETEXT_HPP
#define RUNCRAFT_TRANSLATABLETEXT_HPP

#include <fstream>
#include <string>
#include "json.hpp"
#include "error/ErrorHandling.hpp"

using json = nlohmann::json;

namespace translatable {
	enum Category : unsigned char {
		GUI_BUTTON,
		GUI_SLIDER,
		GUI_TEXTFIELD,
		GUI_TEXTURED_BUTTON,
		BLOCK,
		TEXTURE,

		INDEX_LAST
	};
	std::string JsonMap[Category::INDEX_LAST] = {"gui.button.", "gui.slider.", "gui.textfield.", "gui.textured_button.", "gui.block.",
	                                             "gui.texture."};
}

class TranslatableText {
protected:
	inline static std::string jsonPath = "../assets/lang/en-us.json";
private:
public:
	TranslatableText() = default;

	static std::string getTranslatable(const std::string &id, translatable::Category category) {
		std::ifstream file(jsonPath);
		json lang_json = json::parse(file);
		return lang_json[translatable::JsonMap[category] + id];
	}
};

#endif //RUNCRAFT_TRANSLATABLETEXT_HPP
