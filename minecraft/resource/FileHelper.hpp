//
// Created by robcholz on 3/11/23.
//
#pragma once

#ifndef MINECRAFT_FILEHELPER_HPP
#define MINECRAFT_FILEHELPER_HPP

#include <vector>
#include <filesystem>
#include <fstream>


class FileHelper {
private:
	using String = std::string;
public:
	explicit FileHelper(const String& directoryPath) {
		this->directoryPath = directoryPath;
	}

	~FileHelper() = default;

	bool static isExisted(const String& filename) {
		std::ifstream file;
		file.open(filename);
		if (file) return true;
		else return false;
	}

	std::vector<String>* getFilesInDirectory() {
		directories.clear();
		for (const auto& entry: std::filesystem::directory_iterator(directoryPath)) {
			directories.push_back(entry.path());
		}
		return &directories;
	}

	String getDirectoryName() {
		return directoryPath;
	}

	static bool createFolder(const String& folderName) {
		return std::filesystem::create_directory(folderName);
	}

	static bool createFolder(const String& directory, const String& folderName) {
		return std::filesystem::create_directory(directory + "/" + folderName);
	}

	String createSubFolder(const String& folderName) {
		std::filesystem::create_directory(directoryPath + "/" + folderName);
		return directoryPath + "/" + folderName;
	}

	static void createFile(const String& filename) {
		std::ofstream file(filename);
	}

	static void createFile(const String& directory, const String& filename) {
		std::ofstream file(directory + "/" + filename);
	}

	String createSubFile(const String& filename) {
		std::ofstream my_file(directoryPath + "/" + filename);
		return directoryPath + "/" + filename;
	}

	bool isDirectoryEmpty() {
		return std::filesystem::is_empty(directoryPath);
	}

	static String transferJsonPathToFilePath(const String& jsonPath) {
		String field = jsonPath.substr(0, jsonPath.find(':'));
		String type = jsonPath.substr(jsonPath.find(':') + 1, jsonPath.find('/') - jsonPath.find(':') - 1);
		String filename = jsonPath.substr(jsonPath.find('/') + 1, jsonPath.length() - jsonPath.find(':'));
		return Path::rootDirectory + "/assets/textures/" + type + "s/" + filename + ".png";
	}

	static String appendFilename(const String& filename,const String& append){
		return filename.substr(0,filename.find_last_of('.')) + append + filename.substr(filename.find_last_of('.'));
	}

private:
	std::vector<String> directories;
	String directoryPath;
};

#endif //MINECRAFT_FILEHELPER_HPP
