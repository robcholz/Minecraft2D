//
// Created by robcholz on 4/13/23.
//

#ifndef MINECRAFT_BLOCKTEXTURELOADER_HPP
#define MINECRAFT_BLOCKTEXTURELOADER_HPP

#include <SFML/Graphics/Texture.hpp>
#include <fstream>
#include <map>
#include "json.hpp"
#include "util/Identifier.hpp"
#include "world/poi/Position.hpp"
#include "resource/FileHelper.hpp"

namespace block {
	class TextureLoader {
	private:
		using Json = nlohmann::json;
		using String = std::string;
	public:
		TextureLoader(const TextureLoader&) = delete;

		TextureLoader& operator=(const TextureLoader&) = delete;

		static TextureLoader& getInstance() {
			static TextureLoader instance;
			return instance;
		}

		sf::Texture* getBlockTextureTile(Identifier& identifier, coordinate::DirectionType type) {
			auto pair = std::pair{identifier.toString(), type};
			if (textureTiles.contains(pair))
				return textureTiles[pair];
			return loadBlockTiles(identifier, type);
		}

	private:
		std::map<std::pair<String, coordinate::DirectionType>, sf::Texture*> textureTiles;
		Json blockStateJson;
		std::map<coordinate::DirectionType, String> directionMap = {{coordinate::DirectionType::UP, "up"},
		                                                            {coordinate::DirectionType::DOWN, "down"},
		                                                            {coordinate::DirectionType::IN, "in"},
		                                                            {coordinate::DirectionType::OUT, "out"},
		                                                            {coordinate::DirectionType::WEST, "west"},
		                                                            {coordinate::DirectionType::EAST, "east"}};

		TextureLoader() = default;

		~TextureLoader(){
			for(auto& texture:textureTiles){
				delete texture.second;
			}
		}

		sf::Texture* loadBlockTiles(Identifier& identifier, coordinate::DirectionType direction) {
			std::ifstream file(identifier.getAbsolutePath());
			blockStateJson = Json::parse(file);
			auto texture=new sf::Texture;
			texture->loadFromFile(FileHelper::transferJsonPathToFilePath(blockStateJson[directionMap[direction]]));
			textureTiles.insert({{identifier.toString(),direction}, texture});
			return texture;
		}
	};
}


#endif //MINECRAFT_BLOCKTEXTURELOADER_HPP
