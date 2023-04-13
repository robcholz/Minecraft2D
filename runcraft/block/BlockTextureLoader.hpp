//
// Created by robcholz on 4/13/23.
//

#ifndef RUNCRAFT_BLOCKTEXTURELOADER_HPP
#define RUNCRAFT_BLOCKTEXTURELOADER_HPP

#include <SFML/Graphics/Texture.hpp>
#include <fstream>
#include <map>
#include "json.hpp"

using Json = nlohmann::json;

class BlockTextureLoader {
protected:
	std::string blockStatesPath = "../assets/textures/blockstates/";
private:
	sf::Texture *textureUp, *textureDown, *textureSouth, *textureNorth, *textureIn, *textureOut;
	std::map<std::string,std::string> texturePathMap;
	Json blockStateJson;

	void loadTexture(std::string &direction){

	}
public:
	explicit BlockTextureLoader(std::string &id) {
		std::ifstream file(blockStatesPath + id + ".json");
		blockStateJson = Json::parse(file);
		for (int i = 0; i < blockStatesPath.size(); i++) {
			texturePathMap.insert(blockStateJson,"")
			//TODO: FUCK
		}
	}

};

#endif //RUNCRAFT_BLOCKTEXTURELOADER_HPP
