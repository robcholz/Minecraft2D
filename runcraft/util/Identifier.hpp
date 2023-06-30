//
// Created by robcholz on 6/26/23.
//

#ifndef RUNCRAFT_IDENTIFIER_HPP
#define RUNCRAFT_IDENTIFIER_HPP

#include <algorithm>
#include <plog/Log.h>
#include "util/Utils.hpp"

class Identifier {
private:
	using String = std::string;
protected:
	// namespace, path
	static std::pair<String, String> split(const String& id, char delimiter) {
		String namespace_;
		String path;
		auto namespace_nth = utils::nthOccurrence(id, String(1, delimiter), 1);
		if (namespace_nth != -1)
			namespace_ = id.substr(0, namespace_nth);
		else
			namespace_ = defaultNamespace;
		path = id.substr(namespace_nth + 1);
		return std::make_pair(namespace_, path);
	}

public:
	enum class Category {
		SOUND,
		BLOCKSTATES,
		LANG,
		ITEM,
		ENTITY,
		TEXTURE,
		GUI
	};

	Identifier() = default;

	explicit Identifier(const String& id, Category category) {
		this->category = category;
		auto strings = split(id, ':');
		if (!isNamespaceValid(strings.first)) {
			PLOG_ERROR << "Identifier " << id << " namespace is invalid: " << strings.first;
		}
		if (!isPathValid(strings.second)) {
			PLOG_ERROR << "Identifier " << id << " path is invalid: " << strings.second;
		}
		namespace_ = strings.first;
		path = _getPath(strings.second);
		absolutePath = _getAbsolutePath();
	}

	explicit Identifier(const String& namespace_, const String& path, Category category) {
		if (!isNamespaceValid(namespace_)) {
			PLOG_ERROR << "Identifier " << namespace_ + ":" + path << " namespace is invalid: " << namespace_;
		}
		if (!isPathValid(path)) {
			PLOG_ERROR << "Identifier " << namespace_ + ":" + path << " path is invalid: " << path;
		}
		this->category = category;
		this->namespace_ = namespace_;
		this->path = _getPath(path);
	}

	Identifier(const Identifier& identifier) {
		this->namespace_ = identifier.namespace_;
		this->path = identifier.path;
		this->absolutePath = identifier.absolutePath;
		this->category = identifier.category;
	}

	~Identifier() = default;

	String getRelativePath() { return path; }

	String getAbsolutePath() { return absolutePath; }

	String getNamespace() { return namespace_; }

	Category getCategory() { return category; }

	String toString() { return namespace_ + ":" + path; }

	bool operator<(const Identifier& identifier) {
		return (this->path < identifier.path && this->namespace_ < identifier.namespace_);
	}

	bool operator>(const Identifier& identifier) {
		return (this->path > identifier.path && this->namespace_ > identifier.namespace_);
	}

	bool operator==(const Identifier& identifier) {
		return (this->path == identifier.path && this->namespace_ == identifier.namespace_);
	}

	static bool isCharValid(char c) {
		return c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c == '_' || c == ':' || c == '/' || c == '.' || c == '-';
	}

	static bool isPathValid(const String& path) {
		return std::all_of(path.begin(), path.end(), Identifier::isPathCharacterValid);
	}

	static bool isNamespaceValid(const String& namespace_) {
		return std::all_of(namespace_.begin(), namespace_.end(), Identifier::isNamespaceCharacterValid);
	}

	static bool isPathCharacterValid(char character) {
		return character == '_' || character == '-' || character >= 'a' && character <= 'z' || character >= '0' && character <= '9' || character == '/' ||
		       character == '.';
	}

	static bool isValid(const String& id) {
		auto strings = Identifier::split(id, ':');
		return Identifier::isNamespaceValid(strings.first.empty() ? defaultNamespace : strings.first) && Identifier::isPathValid(strings.second);
	}

private:
	String namespace_;
	String path;
	String absolutePath;
	Category category{};
	static inline String defaultNamespace = "runcraft";

	static inline String blockStatePath = "../assets/blockstates";
	static inline String fontPath = "../assets/font";
	static inline String guiPath = "../assets/gui";
	static inline String langPath = "../assets/lang";
	static inline String soundPath = "../assets/sounds/";
	static inline String texturePath = "../assets/textures";

	static inline String blockStateSuffix = ".json";
	static inline String fontSuffix = ".ttf";
	static inline String guiSuffix = ".png";
	static inline String langSuffix = ".json";
	static inline String soundSuffix = ".ogg";
	static inline String textureSuffix = ".png";

	static bool isNamespaceCharacterValid(char character) {
		return character == '_' || character == '-' || character >= 'a' && character <= 'z' || character >= '0' && character <= '9' || character == '.';
	}

	String _getAbsolutePath() {
		switch (category) {
			case Category::SOUND:
				return soundPath + getRelativePath() + soundSuffix;
			case Category::BLOCKSTATES:
				break;
			case Category::LANG:
				break;
			case Category::ITEM:
				break;
			case Category::ENTITY:
				break;
			case Category::TEXTURE:
				return texturePath + getRelativePath() + textureSuffix;
			case Category::GUI:
				break;
		}
		PLOG_ERROR << "You should do that";
		return "s";
	}

	static String _getPath(const String& path_) {
		String temp = path_;
		std::replace(temp.begin(), temp.end(), '.', '/');
		return temp;
	}
};

#endif //RUNCRAFT_IDENTIFIER_HPP
