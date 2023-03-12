//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_GAMEINFO_HPP
#define RUNCRAFT_GAMEINFO_HPP

#include "Render.hpp"
#include "util/GameLogger.hpp"
#include <Log.h>
#include "Initializers/RollingFileInitializer.h"
#include "Appenders/ColorConsoleAppender.h"

class GameInfo {
private:
    Render *renderInstance;
public:
    GameInfo() = default;

    GameInfo &setRenderer(Render *renderer) {
        renderInstance = renderer;

        PLOG_DEBUG<<"Successfully set the renderer";
        return *this;
    }

    [[nodiscard]] unsigned int getScreenWidth() const { return renderInstance->getScreenWidth(); }

    [[nodiscard]] unsigned int getScreenHeight() const { return renderInstance->getScreenHeight(); }

    Render *getRender() { return renderInstance; }
};

GameInfo GameInfo;

#endif //RUNCRAFT_GAMEINFO_HPP
