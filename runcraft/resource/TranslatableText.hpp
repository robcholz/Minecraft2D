//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_TRANSLATABLETEXT_HPP
#define RUNCRAFT_TRANSLATABLETEXT_HPP

#include <fstream>
#include <string>
#include "json.hpp"
#include "error/ErrorHandling.hpp"

using Json = nlohmann::json;

namespace translatable {
	enum CategoryType : unsigned char {
		GUI,
		GAME,

		CATEGORY_INDEX
	};
	enum GUICategory : unsigned char {
		GUI_BUTTON,
		GUI_SLIDER,
		GUI_TEXTFIELD,

		GUI_INDEX_LAST
	};
	enum GameCategory : unsigned short {
		GAME_BLOCK,
		GAME_ITEM,
		GAME_MOB,
		GAME_PLAYER,
		GAME_SUBTITLE,

		GAME_INDEX_LAST
	};
	std::string GUIJsonMap[GUICategory::GUI_INDEX_LAST] = {{"gui.button.", "gui.slider.", "gui.textfield."};
	std::string GameJsonMap[GameCategory::GAME_INDEX_LAST] = {"runcraft.block.", "runcraft.item.", "runcraft.mob.", "runcraft.player.", "runcraft.subtitle."};
	std::string CategoryArrayMap[CategoryType::CATEGORY_INDEX] = {*GUIJsonMap, *GameJsonMap};
}

class TranslatableText {
protected:
	inline static std::string langPath = "../assets/lang/en-us.json";
private:
public:
	TranslatableText() = default;

	static std::string getTranslatable(const std::string &id, translatable::GUICategory category) {
		std::ifstream file(langPath);
		Json lang_json = Json::parse(file);
		return lang_json[translatable::GUIJsonMap[category] + id];
	}

	static std::string getTranslatable(const std::string &id, translatable::GUICategory category,translatable::CategoryType categoryType) {
		std::ifstream file(langPath);
		Json lang_json = Json::parse(file);
		switch (categoryType) {
			case translatable::CategoryType::GUI:


		}
		return lang_json[translatable::GUIJsonMap[category] + id];
	}
};

#endif //RUNCRAFT_TRANSLATABLETEXT_HPP
