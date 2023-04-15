//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_GAMEINFO_HPP
#define RUNCRAFT_GAMEINFO_HPP

#pragma once

#include <fstream>
#include "Log.h"
#include "client/render/Render.hpp"
#include "json.hpp"

namespace game_data {
	enum Biome : unsigned short {
		SNOWY_PLAINS,
		PLAINS,
		FOREST,
		DESERT,
		BIRCH_FOREST,
		JUNGLE
	};

	typedef unsigned char SoundVolume;
	struct SoundLevel {
		SoundVolume masterVolume;
		SoundVolume music, jukeBoxOrNoteblocks;
		SoundVolume weather, blocks;
		SoundVolume hostileCreatures, friendlyCreatures;
		SoundVolume players, ambientAndEnvironment;
	};
	struct GameGlobalData {
		[[maybe_unused]] Biome biome;
		SoundLevel soundLevel;
	};
	struct GameUniversalInfo {
		using String = std::string;
		String gameName;
		String lang;
		String lastOpenedDate;
	};
}

struct InputState {
	sf::Vector2i mousePosition;
	bool isPressed{};
};

struct Options {
	using Json = nlohmann::json;
	using String = std::string;
	using LogSeverity = plog::Severity;
	String optionsJsonPath = "../assets/options.json";
	Json optionsJson;

	void load() {
		std::ifstream file(optionsJsonPath);
		optionsJson = Options::Json::parse(file);
	}

	bool isLoggedToFile() { return optionsJson["logging"]["file"]; }

	LogSeverity logSeverity() {
		String severity = optionsJson["logging"]["max_severity"];
		if (severity == "none") return LogSeverity::none;
		else if(severity=="fatal") return LogSeverity::fatal;
		else if(severity=="error") return LogSeverity::error;
		else if(severity=="warning") return LogSeverity::warning;
		else if(severity=="info") return LogSeverity::info;
		else if(severity=="debug") return LogSeverity::debug;
		else return LogSeverity::verbose;
	}
};

class GameInfo {
private:
	Render *renderInstance{};
	InputState inputState;
public:
	game_data::GameGlobalData gameGlobalData{};
	game_data::GameUniversalInfo gameUniversalOptions{};

	Options options;

	GameInfo() {
		options.load();
	}

	GameInfo &setRenderer(Render *renderer) {
		renderInstance = renderer;

		PLOG_DEBUG << "Successfully set the renderer";
		return *this;
	}

	void listen(sf::Vector2i mousePosition, bool isPressed) {
		this->inputState.isPressed = isPressed;
		this->inputState.mousePosition = mousePosition;
	}

	InputState *getInputState() { return &inputState; }

	[[nodiscard]] unsigned int getScreenWidth() const { return renderInstance->getScreenWidth(); }

	[[nodiscard]] unsigned int getScreenHeight() const { return renderInstance->getScreenHeight(); }

	Render *getRender() { return renderInstance; }
};

GameInfo GameInfo;

#endif //RUNCRAFT_GAMEINFO_HPP
