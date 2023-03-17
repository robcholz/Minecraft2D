//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_WIDGET_HPP
#define RUNCRAFT_WIDGET_HPP

#include <SFML/Graphics/Drawable.hpp>

class Widget : public GUI {
protected:
    std::string widgetAssetPath = filePath + "widgets.png";
};

#endif //RUNCRAFT_WIDGET_HPP
