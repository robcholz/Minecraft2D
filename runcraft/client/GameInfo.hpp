//
// Created by robcholz on 3/11/23.
//
#pragma once

#ifndef RUNCRAFT_GAMEINFO_HPP
#define RUNCRAFT_GAMEINFO_HPP


#include <fstream>
#include "Log.h"
#include "client/render/RenderSystem.hpp"
#include "json.hpp"

namespace internal_data {
	typedef unsigned char SoundVolume;
	enum Biome : unsigned short {
		SNOWY_PLAINS,
		PLAINS,
		FOREST,
		DESERT,
		BIRCH_FOREST,
		JUNGLE
	};
	struct SoundLevel {
		SoundVolume masterVolume;
		SoundVolume music, jukeBoxOrNoteblocks;
		SoundVolume weather, blocks;
		SoundVolume hostileCreatures, friendlyCreatures;
		SoundVolume players, ambientAndEnvironment;
	};

	struct Internal {
		Biome biome;
		SoundLevel soundLevel;
	};
}

namespace external_data {
	struct General {
		std::string gameName;
		std::string lang;
		std::string lastOpenedDate;
	};

	struct PeripheralState {
		sf::Vector2i mouseAbsolutePosition{};
		sf::Vector2i mouseRelativeToGameWindowPos{};
		double mouseWheelScrollDelta{};
		bool isButtonPressedLeft{};
		bool isButtonPressedRight{};
		bool isButtonPressedMiddle{};
		bool touchBegan{}, touchMoved{}, touchEnded{};

	};

	struct WindowState {
		RenderSystem* rendererPtr = nullptr;
		sf::View* camera = nullptr;
		typedef short ZoomT;
		/**
		 * @range [50,100]
		 */
		ZoomT zoomSize = 50;
		ZoomT pixelToBlock = 30; // the screen pixel of one block(16 pixels) // one block take up pixelProportion pixels
		ZoomT pixelProportion = (ZoomT) (pixelToBlock * 2);
		float actualPixelToOnePixel = (float)pixelProportion / 16;
		bool gainedFocus{}, lostFocus{};
		bool resized{};

		[[nodiscard]]
		unsigned int getScreenWidth() const { return rendererPtr->getWindowConfig().screenWidth; }

		[[nodiscard]]
		unsigned int getScreenHeight() const { return rendererPtr->getWindowConfig().screenHeight; }

		[[nodiscard]]
		RenderSystem* getRender() const { return rendererPtr; }
	};

	struct Logging {
		using Json = nlohmann::json;
		Json optionsJson;

		void load() {
			std::ifstream file(Path::optionsPath);
			optionsJson = Json::parse(file);
		}

		bool isLoggedToFile() { return optionsJson["logging"]["file"]; }

		plog::Severity logSeverity() {
			std::string severity = optionsJson["logging"]["max_severity"];
			if (severity == "none") return plog::Severity::none;
			else if (severity == "fatal") return plog::Severity::fatal;
			else if (severity == "error") return plog::Severity::error;
			else if (severity == "warning") return plog::Severity::warning;
			else if (severity == "info") return plog::Severity::info;
			else if (severity == "debug") return plog::Severity::debug;
			else return plog::Severity::verbose;
		}
	};

	struct External {
		General general;
		WindowState windowState;
		PeripheralState peripheralState;
		Logging logger;
	};
}

class GameInfo {
private:
	internal_data::Internal internalData{};
	external_data::External externalData{};
public:
	GameInfo() {
		externalData.logger.load();
	}

	void setRenderer(RenderSystem* renderer) {
		externalData.windowState.rendererPtr = renderer;
		externalData.windowState.camera = renderer->getCamera();
		PLOG_DEBUG << "Successfully set the renderer";
	}

	internal_data::Internal* getInternalData() { return &internalData; }

	const internal_data::Internal* getConstInternalData() { return &internalData; }

	external_data::External* getExternalData() { return &externalData; }

	const external_data::External* getConstExternalData() { return &externalData; }

	[[nodiscard]] RenderSystem* getRender() const { return externalData.windowState.rendererPtr; }
};

GameInfo GameInfo;

#endif //RUNCRAFT_GAMEINFO_HPP
