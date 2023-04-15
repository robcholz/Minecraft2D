//
// Created by robcholz on 4/14/23.
//

#include <iostream>
#include "resource/FileHelper.hpp"
#include "block/BlockTextureLoader.hpp"

int main(){
	//runcraft:block/grass_block_side
	std::cout << FileHelper::transferJsonPathToFilePath("runcraft:block/grass_block_side");
	return 0;
}
