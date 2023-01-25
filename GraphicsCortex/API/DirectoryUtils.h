#pragma once

#include <string>

std::string compute_directory(const std::string& filename);			// "C:\folder\folder\file.cpp" -> "C:\folder\folder\"
std::string compute_filename(const std::string& filename);			// "C:\folder\folder\file.cpp" -> "file.cpp"
std::string compute_filename_typeless(const std::string& filename);	// "C:\folder\folder\file.cpp" -> "file"
std::string compute_filetype(const std::string& filename);			// "C:\folder\folder\file.cpp" -> ".cpp" , "C:\folder\folder\file" -> ""
bool check_file_exist(const std::string& filename);
