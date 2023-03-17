//
// Created by robcholz on 3/17/23.
//

#ifndef RUNCRAFT_WIDGETMANAGER_HPP
#define RUNCRAFT_WIDGETMANAGER_HPP

#include <vector>
#include "Widget.hpp"

class WidgetManager {
private:
    std::vector<Widget *> widgetsList;
public:
    WidgetManager() = default;

    WidgetManager &addWidget(Widget *widget) {
        widgetsList.push_back(widget);
    }
};

#endif //RUNCRAFT_WIDGETMANAGER_HPP
