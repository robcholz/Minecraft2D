//
// Created by robcholz on 6/30/23.
//

#ifndef RUNCRAFT_RUNCRAFTCLIENTACCESS_HPP
#define RUNCRAFT_RUNCRAFTCLIENTACCESS_HPP

#include "sound/SoundManager.hpp"

class RuncraftClientAccess {
public:
	virtual SoundManager* getSoundManager() = 0;
};

#endif //RUNCRAFT_RUNCRAFTCLIENTACCESS_HPP
