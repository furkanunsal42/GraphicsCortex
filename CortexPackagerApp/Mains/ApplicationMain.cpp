#include "Package.h"
#include <iostream>

bool verbose = true;
std::filesystem::path output_directory = "";
std::string output_filename = "ct_cache.hbv";

bool process_command(Package& package, const std::string& command) {
	size_t equal_position = command.find_first_of('=');
	
	if (equal_position == std::string::npos) {
		if (verbose) {
			std::cout << "[CortexPackager Error] following command failed because '=' is not found:" << std::endl;
			std::cout << "\t" << command << std::endl;
		}
		return false;
	}

	std::string command_type = command.substr(0, equal_position);
	std::string argument = command.substr(equal_position + 1, command.size() - 1);

	if (argument.size() > 2 && argument[0] == '"' && argument[argument.size() - 1] == '"')
		argument = argument.substr(1, argument.size() - 2);

	if (command_type == "-add") {
		bool success = package.add(std::filesystem::path(argument));
		if (!success && verbose) {
			std::cout << "[CortexPackager Error] following command was executed but failed : " << std::endl;
			std::cout << "\t" << command << std::endl;
		}
		return success;
	}
	else if (command_type == "-working_dir") {
		std::filesystem::current_path(std::filesystem::path(argument));
		return true;
	} 
	else if (command_type == "-verbose") {
		if (argument == "true") verbose = true;
		else if (argument == "false") verbose = false;
		else {
			if (verbose) {
				std::cout << "[CortexPackager Error] following command was executed but failed : " << std::endl;
				std::cout << "\t" << command << std::endl;
			}
			return false;
		}
		return true;
	}
	else if (command_type == "-output_dir") {
		output_directory = argument;
		return true;
	}
	else if (command_type == "-output_name") {
		output_filename = argument;
		return true;
	}

	if (verbose) {
		std::cout << "[CortexPackager Error] following command failed because command_type is not valid: " << std::endl;
		std::cout << "\t" << command << std::endl;
	}
	return false;
}

int main(int32_t argc, const char* argv[]) {
	
	bool success = true;
	Package p;

	for (int32_t i = 1; i < argc; i++) {
		success |= process_command(p, argv[i]);
	}

	//success |= process_command(p, "-verbose=true");
	//success |= process_command(p, "-working_dir=C:/Users/furkan.unsal/dev/GraphicsCortex/CTReconstructor");
	//success |= process_command(p, "-output_dir=C:/Users/furkan.unsal/dev/GraphicsCortex/CortexPackagerApp");
	//success |= process_command(p, "-add=C:/Users/furkan.unsal/dev/GraphicsCortex/**.comp");

	if (verbose)
		p.print_headers();
	
	p.save_to_disk(output_directory / output_filename);
	
	if (verbose)
		std::cout << "[CortexPackager Info] writing to disk finished" << std::endl;

	if (verbose && success) {
		std::cout << "[CortexPackager Info] all commands was successful" << std::endl;
		return 0;
	}
	else if (verbose && !success) {
		std::cout << "[CortexPackager Error] at least a command failed" << std::endl;
		return 1;
	}
}