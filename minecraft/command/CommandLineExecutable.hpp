//
// Created by robcholz on 8/6/23.
//

#ifndef MINECRAFT_2D_COMMANDLINEEXECUTABLE_HPP
#define MINECRAFT_2D_COMMANDLINEEXECUTABLE_HPP

#include <boost/program_options.hpp>
#include <string>

namespace cmd {
namespace po = boost::program_options;

class CommandLineExecutable {
 private:
  using String = std::string;

 public:
  CommandLineExecutable() {
    description.add_options()("help", "produce help message")(
        "version", "print version string")("compression", po::value<int>(),
                                           "set compression level")(
        "include-path,I", po::value<std::vector<std::string>>(),
        "include path")("input-file", po::value<std::vector<std::string>>(),
                        "input file");
  }

 private:
  po::options_description description;
};
}  // namespace cmd

#endif  // MINECRAFT_2D_COMMANDLINEEXECUTABLE_HPP
