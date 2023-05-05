//
// Created by robcholz on 4/13/23.
//

#ifndef RUNCRAFT_BLOCKTEXTURELOADER_HPP
#define RUNCRAFT_BLOCKTEXTURELOADER_HPP

#include <SFML/Graphics/Texture.hpp>
#include <fstream>
#include <map>
#include "json.hpp"
#include "block/BlockPos.hpp"
#include "resource/FileHelper.hpp"
#include "BlockPos.hpp"

namespace block{

	class BlockTextureLoader {
	protected:
		using Json = nlohmann::json;
		std::string blockStatePath = "../assets/blockstates/";
	private:
		std::map<BlockDirectionType, sf::Texture *> textureTiles;
		std::map<std::string, sf::Texture *> texturePtrMap;
		Json blockStateJson;

		sf::Texture *loadTexture(const std::string &filePath) {
			if (texturePtrMap[filePath] == nullptr) {
				auto *texture = new sf::Texture;
				texturePtrMap[filePath]=texture;
				texture->loadFromFile(filePath);
				return texture;
			} else return texturePtrMap[filePath];
		}

	public:
		BlockTextureLoader() = default;

		explicit BlockTextureLoader(const std::string &id) { loadBlockTiles(id); }

		void loadBlockTiles(const std::string &id) {
			std::ifstream file(blockStatePath + id + ".json");
			blockStateJson = Json::parse(file);
			textureTiles.insert({UP, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["up"]))});
			textureTiles.insert({DOWN, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["down"]))});
			textureTiles.insert({IN, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["in"]))});
			textureTiles.insert({OUT, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["out"]))});
			textureTiles.insert({NORTH, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["north"]))});
			textureTiles.insert({SOUTH, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["south"]))});
		}

		sf::Texture *getBlockTextureTile(BlockDirectionType type) { return textureTiles[type]; }

		~BlockTextureLoader() = default;

	};
}


#endif //RUNCRAFT_BLOCKTEXTURELOADER_HPP