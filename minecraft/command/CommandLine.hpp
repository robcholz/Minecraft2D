//
// Created by robcholz on 8/6/23.
//

#ifndef MINECRAFT_2D_COMMANDLINE_HPP
#define MINECRAFT_2D_COMMANDLINE_HPP


#include <string>
#include <boost/program_options.hpp>

namespace cmd {
	namespace po = boost::program_options;

	class CommandLine {
	private:
		using String = std::string;
	public:
		CommandLine(){
			description.add_options()
				("help", "produce help message")
				("version", "print version string")
				("compression", po::value<int>(), "set compression level")
				("include-path,I", po::value<std::vector<std::string>>(), "include path")
				("input-file", po::value<std::vector<std::string>>(), "input file")
				;
		}
	private:
		po::options_description description;
	};
}

#endif //MINECRAFT_2D_COMMANDLINE_HPP
