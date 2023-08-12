//
// Created by robcholz on 6/26/23.
//

#ifndef MINECRAFT_IDENTIFIER_HPP
#define MINECRAFT_IDENTIFIER_HPP

#include <algorithm>
#include <plog/Log.h>
#include "util/Utils.hpp"
#include "Path.hpp"

class Identifier {
private:
	using String = std::string;
protected:
	// namespace, relativePath
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
		BLOCK,
		LANG,
		ITEM,
		ENTITY,
		TEXTURE,
		GUI
	};

	const Category DEFAULT_CATEGORY = static_cast<const Category>(-1);
	const String DEFAULT_NAMESPACE = "minecraft";
	const String DEFAULT_PATH = "UNDEFINED";

	Identifier() {
		this->category = DEFAULT_CATEGORY;
		this->namespace_ = DEFAULT_NAMESPACE;
		this->relativePath = DEFAULT_PATH;
		this->path = _getPath(DEFAULT_PATH);
		this->absolutePath = _getAbsolutePath();
	}

	explicit Identifier(const String& id, Category category) {
		this->category = category;
		auto strings = split(id, ':');
		if (!isNamespaceValid(strings.first)) {
			PLOG_ERROR << "Identifier " << id << " namespace is invalid: " << strings.first;
		}
		if (!isPathValid(strings.second)) {
			PLOG_ERROR << "Identifier " << id << " relativePath is invalid: " << strings.second;
		}
		namespace_ = strings.first;
		relativePath = _getRelativePath(strings.second);
		path = _getPath(strings.second);
		absolutePath = _getAbsolutePath();
	}

	explicit Identifier(const String& namespace_, const String& path, Category category) {
		if (!isNamespaceValid(namespace_)) {
			PLOG_ERROR << "Identifier " << namespace_ + ":" + path << " namespace is invalid: " << namespace_;
		}
		if (!isPathValid(path)) {
			PLOG_ERROR << "Identifier " << namespace_ + ":" + path << " relativePath is invalid: " << path;
		}
		this->category = category;
		this->namespace_ = namespace_;
		this->relativePath = _getRelativePath(path);
		this->path = _getPath(path);
	}

	Identifier(const Identifier& identifier) {
		this->namespace_ = identifier.namespace_;
		this->relativePath = identifier.relativePath;
		this->absolutePath = identifier.absolutePath;
		this->path = identifier.path;
		this->category = identifier.category;
	}

	~Identifier() = default;

	/**
	 * @example for id "minecraft:block.air_block", this returns "block.air_block"
	 * @return a specific path used to represent a given resource
	 */
	String getDotPath() { return path; }

	/**
	 * @return the path of the resource relative to ../assets
	 */
	String getRelativePath() { return relativePath; }

	/**
	 * @return The system path pointing to the resource.
	 */
	String getAbsolutePath() { return absolutePath; }

	/**
	 * @example for id "minecraft:block.air_block", this returns "minecraft"
	 * @return the namespace the resource belongs to.
	 */
	String getNamespace() { return namespace_; }

	/**
	 * @return the category the resource belongs to
	 */
	Category getCategory() { return category; }

	/**
	 * @example for id "minecraft:block.air_block", this returns "minecraft:air_block"
	 * @return a short id used in game
	 */
	String toString() { return namespace_ + ":" + relativePath; }

	bool operator<(const Identifier& identifier) const {
		return (this->relativePath < identifier.relativePath && this->namespace_ < identifier.namespace_);
	}

	bool operator>(const Identifier& identifier) const {
		return (this->relativePath > identifier.relativePath && this->namespace_ > identifier.namespace_);
	}

	bool operator==(const Identifier& identifier) const {
		return (this->relativePath == identifier.relativePath && this->namespace_ == identifier.namespace_);
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
	String relativePath;
	String absolutePath;
	String path;
	Category category{};
	static inline String defaultNamespace = "minecraft";

	static bool isNamespaceCharacterValid(char character) {
		return character == '_' || character == '-' || character >= 'a' && character <= 'z' || character >= '0' && character <= '9' || character == '.';
	}

	String _getAbsolutePath() {
		switch (category) {
			case Category::SOUND:
				return Path::soundPath + getRelativePath() + Path::soundSuffix;
			case Category::BLOCK:
				return Path::blockStatePath + getRelativePath() + Path::blockStateSuffix;
			case Category::LANG:
				return Path::langPath + getRelativePath() + Path::langSuffix;
				break;
			case Category::ITEM:
				break;
			case Category::ENTITY:
				break;
			case Category::TEXTURE:
				return Path::texturePath + getRelativePath() + Path::textureSuffix;
			case Category::GUI:
				return Path::guiPath + getRelativePath() + Path::guiSuffix;
		}
		PLOG_ERROR << "You should do that";
		return "s";
	}

	static String _getRelativePath(const String& path_) {
		String temp = path_;
		std::replace(temp.begin(), temp.end(), '.', '/');
		return temp;
	}

	String _getPath(const String& path_) {
		String temp = path_;
		String prefix_;
		switch (category) {
			case Category::SOUND:
				prefix_ = "sound";
				break;
			case Category::BLOCK:
				prefix_ = "blockstates";
				break;
			case Category::LANG:
				prefix_ = "lang";
				break;
			case Category::ITEM:
				prefix_ = "item";
				break;
			case Category::ENTITY:
				prefix_ = "entity";
				break;
			case Category::TEXTURE:
				prefix_ = "textures";
				break;
			case Category::GUI:
				prefix_ = "gui";
				break;
		}
		std::replace(temp.begin(), temp.end(), '/', '.');
		return prefix_ + "." + temp;
	}
};

#endif //MINECRAFT_IDENTIFIER_HPP
