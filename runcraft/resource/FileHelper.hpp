//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_FILEHELPER_HPP
#define RUNCRAFT_FILEHELPER_HPP

#include <vector>
#include <filesystem>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>

#pragma once

class FileHelper {
private:
	std::__1::vector<std::string> directories;
	std::string directoryPath;
public:
	explicit FileHelper(const std::string &directoryPath) {
		this->directoryPath = directoryPath;
	}

	~FileHelper() = default;

	bool static isFileExisted(const std::string &filename) {
		std::ifstream file;
		file.open(filename);
		if (file) return true;
		else return false;
	}

	std::__1::vector<std::string> *getFilesInDirectory() {
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

	bool isDirectoryEmpty() {
		return std::filesystem::is_empty(directoryPath);
	}

	std::string static transferJsonPathToFilePath(const std::string &jsonPath) {
		std::string field = jsonPath.substr(0, jsonPath.find(':'));
		std::string type = jsonPath.substr(jsonPath.find(':') + 1, jsonPath.find('/') - jsonPath.find(':') - 1);
		std::string filename = jsonPath.substr(jsonPath.find('/') + 1, jsonPath.length() - jsonPath.find(':'));
		return "../assets/textures/" + type + "s/" + filename + ".png";
	}
};

#endif //RUNCRAFT_FILEHELPER_HPP
