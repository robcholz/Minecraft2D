//
// Created by robcholz on 4/29/23.
//

#ifndef RUNCRAFT_PLAYER_HPP
#define RUNCRAFT_PLAYER_HPP

#include "world/poi/WorldCoordinate.hpp"

class Player{
private:
	coordinate::WorldCoordinate playerCoordinate{0,0};
	sf::Texture skin;
public:
	explicit Player(){
		skin.loadFromFile("../assets/textures/player/Steve.png");
	}
	Player &move(coordinate::WorldPositionT x,coordinate::WorldPositionT z){
		playerCoordinate.offset(x,z);
	}

	void update(){

	}
};

#endif //RUNCRAFT_PLAYER_HPP
