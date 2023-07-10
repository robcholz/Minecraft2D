//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_FILEHELPER_HPP
#define RUNCRAFT_FILEHELPER_HPP

#include <vector>
#include <filesystem>
#include <fstream>

#pragma once

class FileHelper {
private:
	std::vector<std::string> directories;
	std::string directoryPath;
public:
	explicit FileHelper(const std::string &directoryPath) {
		this->directoryPath = directoryPath;
	}

	~FileHelper() = default;

	bool static isExisted(const std::string &filename) {
		std::ifstream file;
		file.open(filename);
		if (file) return true;
		else return false;
	}

	std::vector<std::string>* getFilesInDirectory() {
		directories.clear();
		for (const auto &entry: std::filesystem::directory_iterator(directoryPath)) {
			directories.push_back(entry.path());
		}
		return &directories;
	}

	std::string getDirectoryName(){
		return directoryPath;
	}

	static bool createFolder(const std::string &folderName) {
		return std::filesystem::create_directory(folderName);
	}

	static bool createFolder(const std::string &directory, const std::string &folderName) {
		return std::filesystem::create_directory(directory + "/" + folderName);
	}

	std::string createSubFolder(const std::string &folderName) {
		std::filesystem::create_directory(directoryPath + "/" + folderName);
		return directoryPath + "/" + folderName;
	}

	static void createFile(const std::string &filename) {
		std::ofstream file(filename);
	}

	static void createFile(const std::string &directory, const std::string &filename) {
		std::ofstream file(directory + "/" + filename);
	}

	std::string createSubFile(const std::string &filename) {
		std::ofstream my_file(directoryPath + "/" + filename);
		return directoryPath + "/" + filename;
	}

	bool isDirectoryEmpty() {
		return std::filesystem::is_empty(directoryPath);
	}

	static std::string transferJsonPathToFilePath(const std::string &jsonPath) {
		std::string field = jsonPath.substr(0, jsonPath.find(':'));
		std::string type = jsonPath.substr(jsonPath.find(':') + 1, jsonPath.find('/') - jsonPath.find(':') - 1);
		std::string filename = jsonPath.substr(jsonPath.find('/') + 1, jsonPath.length() - jsonPath.find(':'));
		return "../assets/textures/" + type + "s/" + filename + ".png";
	}
};

#endif //RUNCRAFT_FILEHELPER_HPP
