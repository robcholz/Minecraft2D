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
#include <string>


class GameLogger {
private:
    std::string logPath = "../log";
protected:
    std::string getLogFileName() {
        FileHelper fileHelper(logPath);
        auto filename = fileHelper.getDirectory();
        if (fileHelper.isDirEmpty() || isdigit(*(filename[0].at(filename->size() - 1).c_str()))) {
            return getCurrentDate() + "-0";
        }
        int maxDigit = atoi((filename[0].at(filename[0].size() - 1)).c_str());
        for (int i = 0; i < fileHelper.getDirectory()->size(); i++) {
            if (maxDigit < atoi(&filename->at(i).at(filename->size() - 1))) {
                maxDigit = atoi(&filename->at(i).at(filename->size() - 1));
            }
        }
        return getCurrentDate() + "-" + std::to_string(maxDigit + 1);
    }

    std::string getCurrentDate() {
        time_t now = time(nullptr);
        tm *t = localtime(&now);
        return std::to_string(1900 + t->tm_year) + "-" + std::to_string(1 + t->tm_mon) + "-" +
               std::to_string(t->tm_mday);
    }

public:
    GameLogger() {
        if (!FileHelper::fIsExisted(logPath)) {
            FileHelper::createFolder(logPath);
        }
    }

};

#endif //RUNCRAFT_GAMELOGGER_HPP
