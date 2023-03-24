//
// Created by robcholz on 3/24/23.
//

#ifndef RUNCRAFT_WIDGET_HPP
#define RUNCRAFT_WIDGET_HPP

#include "../GUI.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <vector>

class Widget : public GUI {
protected:
    struct RenderAble {
        sf::Drawable *drawable;
        sf::Text *text;
    } renderAble{};

    std::string widgetAssetPath = filePath + "widgets.png";
public:
    Widget() = default;
    virtual RenderAble *getWidgetRenderAble() = 0;
    virtual void listen(sf::Vector2i mousePos, bool isPressed)=0;
};

#endif //RUNCRAFT_WIDGET_HPP
