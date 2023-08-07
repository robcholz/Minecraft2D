//
// Created by robcholz on 8/7/23.
//
#ifndef MINECRAFT_2D_DATE_HPP
#define MINECRAFT_2D_DATE_HPP

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>


class Date {
private:
	using String = std::string;
public:
	static String getUTCBuildTime(const String& dateStr,const String& timeStr) {
		// month map
		std::map<std::string, int> monthMap = {{"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4}, {"May", 5}, {"Jun", 6},
		                                       {"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}};
		std::string monthStr = dateStr.substr(0, 3);
		int month = monthMap[monthStr];
		// Extract the day and year from the date string
		int day = std::stoi(dateStr.substr(4));
		int year = std::stoi(dateStr.substr(dateStr.length() - 4));
		// Extract the hour, minute, and second from the time string
		int hour = std::stoi(timeStr.substr(0, 2));
		int minute = std::stoi(timeStr.substr(3, 2));
		int second = std::stoi(timeStr.substr(6, 2));
		// Create a boost::posix_time::ptime object with the converted values
		boost::posix_time::ptime currentTime(boost::gregorian::date(year, month, day),
		                                     boost::posix_time::time_duration(hour, minute, second));
		// Convert the local time to UTC
		boost::posix_time::ptime utcTime =
				boost::date_time::local_adjustor<boost::posix_time::ptime, 0, boost::posix_time::no_dst>::utc_to_local(currentTime);
		// Format the UTC time into a single string
		std::string utcTimeString = boost::posix_time::to_iso_extended_string(utcTime);
		return utcTimeString;
	}

};

#endif //MINECRAFT_2D_DATE_HPP
