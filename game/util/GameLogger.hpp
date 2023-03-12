//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_GAMELOGGER_HPP
#define RUNCRAFT_GAMELOGGER_HPP

#include "ctime"
#include "Logger.h"
#include <filesystem>
#include <iostream>
#include "FileHelper.hpp"
#include "Appenders/RollingFileAppender.h"
#include "Formatters/TxtFormatter.h"
#include "Appenders/ColorConsoleAppender.h"
#include <string>


class GameLogger {
private:
    std::string logPath = "../log";
protected:
    std::string getCurrentDate() {
        time_t now = time(nullptr);
        tm *t = localtime(&now);
        return std::to_string(1900 + t->tm_year) + "-" + std::to_string(1 + t->tm_mon) + "-" +
               std::to_string(t->tm_mday);
    }

public:
    GameLogger() {
        static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(
                getLogFileName().c_str(), 8000, 1);
        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::init(plog::debug, &fileAppender).addAppender(&consoleAppender);
    }

    GameLogger(const std::string &path) {
        logPath = path;
        if (!FileHelper::fIsExisted(logPath)) {
            FileHelper::createFolder(logPath);
        }
    }

    std::string getLogFileName() {
        FileHelper fileHelper(logPath);
        auto filename = fileHelper.getDirectory();
        int current_max = 0, max = 0;
        for (std::string name: *filename) {
            // get the largest integer in the last pos
            int count = 0;
            int index_front = 0;
            int index_back = 0;
            for (int i = name.length() - 1; i >= 0; i--) {
                if (name.at(i) == '-') {
                    if (count == 0) {
                        index_front = i;
                        count++;
                    }
                    if ((count == 1) && (i != index_front)) {
                        index_back = i;
                        current_max = stoi(name.substr(index_back + 1, index_front));
                        i = -1;
                    }
                }
            }
            if (max < current_max)
                max = current_max;
        }
        return logPath + "/" + getCurrentDate() + "-" + std::to_string(max + 1) + "-log" + ".log";
    }
}GameLogger;

#endif //RUNCRAFT_GAMELOGGER_HPP
