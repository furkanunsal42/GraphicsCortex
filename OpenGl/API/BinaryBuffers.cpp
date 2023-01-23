#include "BinaryBuffers.h"
#include "DirectoryUtils.h"

File::File(const std::string& filename, bool overwrite, bool append){
	std::ios::openmode mode = std::fstream::in | std::fstream::out | std::fstream::binary;
	if (append) mode = mode | std::fstream::app;
	else if (overwrite) mode = mode | std::fstream::trunc;
	_file = std::fstream(filename, mode);
}

File::~File() {
	_file.close();
}

void File::writebuffer(const void* buffer_begin, size_t buffer_size) {
	const char* const memory = (const char* const)buffer_begin;
	_file.write(memory, buffer_size);
}
std::string File::read() {
	std::string input;
	_file >> input;
	return input;
}


void save_buffer_to_disc(const std::string& filename, const void* buffer_begin, size_t buffer_size, bool append) {
	std::ios::openmode mode = std::ios::binary;
	if (append) mode = mode | std::ios::app;
	else mode = mode | std::ios::out;
	
	std::ofstream file(filename, mode);
	const char* const memory = (const char* const)buffer_begin;
	file.write(memory, buffer_size);
	file.close();
}

