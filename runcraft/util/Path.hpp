//
// Created by robcholz on 7/11/23.
//

#ifndef RUNCRAFT_PATH_HPP
#define RUNCRAFT_PATH_HPP

#include <string>
#include <boost/filesystem.hpp>
#include <boost/dll/runtime_symbol_info.hpp>


class Path {
private:
	using String = std::string;
protected:
	static inline String getRoot() {
		boost::filesystem::path program_location(boost::system::error_code& ec);
		auto raw_dir = boost::dll::program_location().string();
		raw_dir = raw_dir.substr(0, raw_dir.find_last_of("\\/"));
		raw_dir = raw_dir.substr(0, raw_dir.find_last_of("\\/"));
		return raw_dir;
	}

public:
	static inline String rootDirectory = getRoot();

	static inline String guiFilePath = rootDirectory + "/assets/textures/gui";
	static inline String widgetPath = rootDirectory + "/assets/textures/gui/widgets.png";
	static inline String currentLangPath = rootDirectory + "/assets/lang/en-us.json";
	static inline String optionsPath = rootDirectory + "/assets/options.json";
	static inline String currentPlayerPath = rootDirectory + "/assets/textures/player/steve.png";

	static inline String directory = rootDirectory + "/assets/";
	static inline String blockStatePath = rootDirectory + "/assets/blockstates/";
	static inline String fontPath = rootDirectory + "/assets/font/";
	static inline String guiPath = rootDirectory + "/assets/textures/gui/";
	static inline String langPath = rootDirectory + "/assets/lang/";
	static inline String soundPath = rootDirectory + "/assets/sounds/";
	static inline String texturePath = rootDirectory + "/assets/textures/";
	static inline String fontFilePath = rootDirectory + "/assets/font/";

	static inline String blockStateSuffix = ".json";
	static inline String fontSuffix = ".ttf";
	static inline String guiSuffix = ".png";
	static inline String langSuffix = ".json";
	static inline String soundSuffix = ".ogg";
	static inline String textureSuffix = ".png";
};

#endif //RUNCRAFT_PATH_HPP
