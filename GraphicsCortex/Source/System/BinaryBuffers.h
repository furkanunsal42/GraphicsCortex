#pragma once

#include <fstream>
#include <vector>

class File {
public:
	File(const std::string& filename, bool overwrite = true, bool append = false);
	~File();

	template<typename T>
	void write(const T& variable) {
		const char* const memory = (const char* const)&variable;
		_file.write(memory, sizeof(variable));
	}
	void writebuffer(const void* buffer_begin, size_t buffer_size);
	
	template<typename T>
	void writebuffer(const std::vector<T> vector) {
		writebuffer(vector.begin(), vector.end() - vector.begin());
	}

	std::string read();
	template<typename T>
	void read(T& variable) {
		char* memory = (char*)&variable;
		_file.read(memory, sizeof(variable));
	}
	template<typename T>
	void readbuffer(T* buffer_begin, size_t buffer_size) {
		char* memory = (char*)buffer_begin;
		_file.read(memory, buffer_size);
	}

	std::fstream _file;
private:
};

void save_buffer_to_disc(const std::string& filename, const void* buffer_begin, size_t buffer_size, bool append = false);

template<typename T>
void read_buffer_from_disc(const std::string& filename, T& variable) {
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	char* memory = (char*)&variable;
	file.read(memory, sizeof(variable));
	file.close();
}

template<typename T>
void read_buffer_from_disc(const std::string& filename, T* buffer_begin, size_t buffer_size) {
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	char* memory = (char*)(buffer_begin);
	file.read(memory, buffer_size);
	file.close();
}
