//
// Created by robcholz on 3/31/23.
//

#ifndef RUNCRAFT_ERRORHANDLING_HPP
#define RUNCRAFT_ERRORHANDLING_HPP

#include <exception>

class NullPtrException : public std::exception {
public:
	char *what() {
		return "NullPointer Exception";
	}
};

#endif //RUNCRAFT_ERRORHANDLING_HPP
