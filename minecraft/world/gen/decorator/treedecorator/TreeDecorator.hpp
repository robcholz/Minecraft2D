//
// Created by robcholz on 8/22/23.
//

#ifndef MINECRAFT_2D_TREEDECORATOR_HPP
#define MINECRAFT_2D_TREEDECORATOR_HPP

// abstract class
class TreeDecorator {
 protected:
  virtual void placeVine() {}

 public:
  virtual void generate() {}
};

#endif  // MINECRAFT_2D_TREEDECORATOR_HPP
