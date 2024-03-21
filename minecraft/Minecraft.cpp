#include "client/MinecraftClient.hpp"
#include "command/CommandLineExecutable.hpp"
#include "resource/System.hpp"

int main(int argc, char** argv) {
  // cmd::CommandLine command_line;
  GameLogger::init();
  OS::init();
  MinecraftClient minecraft_client;
  minecraft_client.run();
  return 0;
}