//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_FILEHELPER_HPP
#define RUNCRAFT_FILEHELPER_HPP

#include <vector>
#include <filesystem>
#include <fstream>
#include <dirent.h>

#pragma once

class FileHelper {
private:
    std::vector<std::string> directories;

    std::string directoryPath;
public:
    explicit FileHelper(const std::string &directoryPath) {
        this->directoryPath = directoryPath;
    }

    ~FileHelper()=default;

    bool static fIsExisted(const std::string &filename) {
        std::ifstream file;
        file.open(filename);
        if (file) return true;
        else return false;
    }

    bool isExisted(const std::string &filename) {
        std::ifstream file;
        file.open(directoryPath + filename);
        if (file) return true;
        else return false;
    }

    std::vector<std::string> *getDirectory() {
        directories.clear();
        for (const auto &entry: std::filesystem::directory_iterator(directoryPath))
            directories.push_back(entry.path());
        return &directories;
    }

    bool static createFolder(const std::string &folderName) {
        return mkdir(folderName.c_str(), 0777);
    }

    void createFile(const std::string &filename) {
        std::ofstream my_file(directoryPath + "/" + filename);
    }

    bool isDirEmpty() {
        return std::filesystem::is_empty(directoryPath);
    }
};

#endif //RUNCRAFT_FILEHELPER_HPP
