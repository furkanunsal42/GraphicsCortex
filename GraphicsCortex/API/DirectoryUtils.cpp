#pragma once
#include <string>
#include <fstream>

std::string compute_directory(const std::string& filename) { // "C:\folder\folder\file" -> "C:\folder\folder\"
	std::string dir = filename;
	for (int i = dir.size() - 1; i >= 0; i--) {
		if (dir[i] != '/' && dir[i] != '\\')
			dir.pop_back();
		else
			break;
	}
	return dir;
}

std::string compute_filename(const std::string& filename) { // "C:\folder\folder\file.cpp" -> "file.cpp"
	std::string name = "";
	for (int i = filename.size() - 1; i >= 0; i--) {
		if (filename[i] != '/' && filename[i] != '\\')
			name = filename[i] + name;
		else
			break;
	}
	return name;
}

std::string compute_filetype(const std::string& filename) { // "C:\folder\folder\file" -> "" , "C:\folder\folder\file.cpp" -> ".cpp"
	std::string type;
	for (int i = filename.length()-1; i >= 0; i--) {
		type = filename[i] + type;
		if (filename[i] == '.') return type;
		else if (filename[i] == '\\' || filename[i] == '/') return "";
	}
	return type;
}

std::string compute_filename_typeless(const std::string& filename) { // "C:\folder\folder\file.cpp" -> "file"
	std::string type = compute_filetype(filename);
	std::string name;
	for (int i = filename.length() - 1 - type.length(); i >= 0; i--) {
		if (filename[i] == '\\' || filename[i] == '/') return name;
		name = filename[i] + name;
	}
	return name;
}

bool check_file_exist(const std::string& filename) {
	std::ifstream file;
	file.open(filename, std::ios::in);
	if (file) {
		file.close();
		return true;
	}
	else return false;
}