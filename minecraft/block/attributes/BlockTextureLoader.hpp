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
	class BlockTextureLoader {
	private:
		using Json = nlohmann::json;
		using String=std::string;
	public:
		BlockTextureLoader() = default;

		explicit BlockTextureLoader(Identifier& identifier) { loadBlockTiles(identifier); }

		sf::Texture* getBlockTextureTile(coordinate::DirectionType type) { return textureTiles[type]; }

		~BlockTextureLoader() = default;

	private:
		std::map<coordinate::DirectionType, sf::Texture*> textureTiles;
		static inline std::map<String, sf::Texture*> texturePtrMap;
		Json blockStateJson;

		static sf::Texture* loadTexture(const String& filePath) {
			if (texturePtrMap[filePath] == nullptr) {
				auto* texture = new sf::Texture;
				texturePtrMap[filePath] = texture;
				texture->loadFromFile(filePath);
				return texture;
			} else return texturePtrMap[filePath];
		}

		void loadBlockTiles(Identifier& identifier) {
			std::ifstream file(identifier.getAbsolutePath());
			blockStateJson = Json::parse(file);
			textureTiles.insert({coordinate::DirectionType::UP, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["up"]))});
			textureTiles.insert({coordinate::DirectionType::DOWN, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["down"]))});
			textureTiles.insert({coordinate::DirectionType::IN, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["in"]))});
			textureTiles.insert({coordinate::DirectionType::OUT, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["out"]))});
			textureTiles.insert({coordinate::DirectionType::WEST, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["west"]))});
			textureTiles.insert({coordinate::DirectionType::EAST, loadTexture(FileHelper::transferJsonPathToFilePath(blockStateJson["east"]))});
		}
	};
}


#endif //MINECRAFT_BLOCKTEXTURELOADER_HPP
