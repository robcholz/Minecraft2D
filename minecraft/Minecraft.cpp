#include "resource/System.hpp"
#include "command/CommandLine.hpp"
#include "client/MinecraftClient.hpp"



int main(int argc,char** argv) {
	//cmd::CommandLine command_line;
	GameLogger::init();
	OS::init();
	MinecraftClient minecraft_client;
	minecraft_client.run();
	return 0;
}