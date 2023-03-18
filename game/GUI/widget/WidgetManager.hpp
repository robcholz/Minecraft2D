//
// Created by robcholz on 3/17/23.
//

#ifndef RUNCRAFT_WIDGETMANAGER_HPP
#define RUNCRAFT_WIDGETMANAGER_HPP

#include <vector>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../GUI.hpp"

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

class WidgetManager {
private:
    std::vector<Widget *> widgetsList;

public:
    WidgetManager()=default;

    WidgetManager &addWidget(Widget *widget) {
        widgetsList.push_back(widget);
        return *this;
    }

    void render(sf::Vector2i mousePosition,bool isPressed){
        for (auto &widgetObj: widgetsList) {
            widgetObj->listen(mousePosition, isPressed);
            widgetObj->render();
        }
    }
};

#endif //RUNCRAFT_WIDGETMANAGER_HPP
