//
// Created by robcholz on 3/17/23.
//
#pragma once

#ifndef MINECRAFT_GUI_HPP
#define MINECRAFT_GUI_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include "client/Layout.hpp"

class GUI {
 public:
  virtual void update() = 0;

  virtual void render() = 0;

  virtual void onWindowResize() {}

 private:
};

#endif  // MINECRAFT_GUI_HPP
