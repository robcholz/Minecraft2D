//
// Created by robcholz on 3/11/23.
//
#pragma once

#ifndef MINECRAFT_GAMELOGGER_HPP
#define MINECRAFT_GAMELOGGER_HPP


#include <ctime>
#include <filesystem>
#include <iostream>
#include "Logger.h"
#include "Appenders/RollingFileAppender.h"
#include "Formatters/TxtFormatter.h"


class GameLogger {
private:
	using String = std::string;
	using Json = nlohmann::json;
protected:
	/**
	 * @brief get current date
	 * @return current date, in format yyyy-mm-dd
	 */
	static String getCurrentDate() {
		time_t now = time(nullptr);
		tm* t = localtime(&now);
		return std::to_string(1900 + t->tm_year) + "-" + std::to_string(1 + t->tm_mon) + "-" +
		       std::to_string(t->tm_mday);
	}

public:
	GameLogger() = delete;

	static void init() {
		load();
		if (!FileHelper::isExisted(logPath)) {
			FileHelper::createFolder(logPath);
		}

		static plog::ColorConsoleAppender<plog::TxtFormatter> console_appender;
		if (isLoggedToFile()) {
			static plog::RollingFileAppender<plog::TxtFormatter> file_appender(getLogFileName().c_str(), 8000, 1);
			plog::init(logSeverity(), &console_appender).addAppender(&file_appender);
		} else plog::init(logSeverity(), &console_appender);
	}

	GameLogger(const GameLogger&) = delete;

	~GameLogger() = delete;

	/**
	 * @brief the standard log file name
	 * @return log name, in format yyyy-mm-dd-index-log.log
	 */
	static String getLogFileName() {
		FileHelper file_helper(logPath);
		int current_daily_max, daily_max = 0, days_log;
		time_t current_date_obj = time(nullptr);
		tm* date_ptr = localtime(&current_date_obj);
		int current_days = (date_ptr->tm_year + 1900) * 360 + (date_ptr->tm_mon + 1) * 30 + date_ptr->tm_mday;
		for (String name: *file_helper.getFilesInDirectory()) {
			days_log = stoi(name.substr(name.find('-') - 4, 4)) * 360 +
			           stoi(getSplitStr(name, 1, 2)) * 30 +
			           stoi(getSplitStr(name, 2, 3));
			if (days_log == current_days) {
				// find any log file created today
				// get the largest integer in the last pos
				current_daily_max = stoi(getSplitStr(name, 3, 4));
				if (daily_max < current_daily_max)
					daily_max = current_daily_max;
			}
		}
		return logPath + "/" + getCurrentDate() + "-" + std::to_string(daily_max + 1) + "-log" + ".log";
	}

private:
	static inline String logPath = Path::rootDirectory + "/log";
	static inline Json optionsJson;

	static void load() {
		std::ifstream file(Path::optionsPath);
		optionsJson = Json::parse(file);
	}

	static bool isLoggedToFile() {
		return optionsJson["logging"]["file"];
	}

	static plog::Severity logSeverity() {
		std::string severity = optionsJson["logging"]["max_severity"];
		if (severity == "none") return plog::Severity::none;
		else if (severity == "fatal") return plog::Severity::fatal;
		else if (severity == "error") return plog::Severity::error;
		else if (severity == "warning") return plog::Severity::warning;
		else if (severity == "info") return plog::Severity::info;
		else if (severity == "debug") return plog::Severity::debug;
		else return plog::Severity::verbose;
	}

	/**
	 * @brief get the string between two '-'
	 * @param str target string
	 * @param firstOccurrence the nth occurrence of '-'
	 * @param lastOccurrence the nth+1 occurrence of '-'
	 * @return split string
	 */
	static String getSplitStr(String& str, int firstOccurrence, int lastOccurrence) {
		// get the largest integer in the last pos
		int count = 0, index_first = 0, index_last = 0;
		for (int i = 0; i < str.length() - 1; i++) {
			if (str.at(i) == '-') {
				count++;
				if (count == firstOccurrence) { index_first = i; }
				if (count == lastOccurrence) {
					index_last = i;
					break;
				}
			}
		}
		return str.substr(index_first + 1, index_last - index_first - 1);
	}
};

#endif //MINECRAFT_GAMELOGGER_HPP
