//
// Created by robcholz on 4/13/23.
//

#ifndef RUNCRAFT_BLOCKTEXTURELOADER_HPP
#define RUNCRAFT_BLOCKTEXTURELOADER_HPP

#include <SFML/Graphics/Texture.hpp>
#include <fstream>
#include <map>
#include "json.hpp"
#include "world/poi/Position.hpp"
#include "resource/FileHelper.hpp"

namespace block{

	class BlockTextureLoader {
	protected:
		using Json = nlohmann::json;
		std::string blockStatePath = "../assets/blockstates/";
	private:
		std::map<BlockDirectionType, sf::Texture *> textureTiles;
		static inline std::map<std::string, sf::Texture *> texturePtrMap;
		Json blockStateJson;

		static sf::Texture *loadTexture(const std::string &filePath) {
			if (texturePtrMap[filePath] == nullptr) {
				auto *texture = new sf::Texture;
				texturePtrMap[filePath]=texture;
				texture->loadFromFile(filePath);
				return texture;
			} else return texturePtrMap[filePath];
		}

		void loadBlockTiles(const std::string &id) {
			std::ifstream file(blockStatePath + id + ".json");
			blockStateJson = Json::parse(file);
			textureTiles.insert({BlockDirectionType::UP, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["up"]))});
			textureTiles.insert({BlockDirectionType::DOWN, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["down"]))});
			textureTiles.insert({BlockDirectionType::IN, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["in"]))});
			textureTiles.insert({BlockDirectionType::OUT, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["out"]))});
			textureTiles.insert({BlockDirectionType::WEST, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["west"]))});
			textureTiles.insert({BlockDirectionType::EAST, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["east"]))});
		}

	public:
		BlockTextureLoader() = default;

		explicit BlockTextureLoader(const std::string &id) { loadBlockTiles(id); }

		sf::Texture *getBlockTextureTile(BlockDirectionType type) { return textureTiles[type]; }

		~BlockTextureLoader() = default;
	};
}


#endif //RUNCRAFT_BLOCKTEXTURELOADER_HPP
