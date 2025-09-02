#include "Package.h"

#ifdef PRINT_WARNINGS
#include <iostream>
#endif
#include <fstream>

void Package::clear()
{
	header_to_content_table.clear();
}

bool Package::does_exist(const std::string& header) const
{
	return header_to_content_table.find(header) != header_to_content_table.end();
}

void Package::add_content(const std::string& header, const std::string& content)
{
	if (does_exist(header))
		_println("[Package Warning] Package::add_content() is called but given header already existed. It will be overwritten");

	header_to_content_table[header] = content;
}

bool Package::add_content(const std::filesystem::path& path) 
{
	std::string header = _convert_path_to_header(path);

	if (does_exist(header))
		_println("[Package Warning] Package::add_content() is called but given header already existed. It will be overwritten");

	
	auto size = std::filesystem::file_size(path);
	std::string content(size, '\0');
	std::ifstream file(path, std::ios::binary);
	file.read(&content[0], size);

	header_to_content_table[header] = content;
}

std::string Package::get_content(const char* const header) const
{
	return get_content(std::filesystem::path(header));
}

std::string Package::get_content(const std::string& header) const
{
	if (!does_exist(header))
	{
		_println("[Package Warning] Package::get_content() is called with an header that isn't registered yet");
		return "";
	}

	return header_to_content_table.at(header);
}

std::string Package::get_content(const std::filesystem::path& path) const
{
	std::string header = _convert_path_to_header(path);
	return get_content(header);
}

std::string Package::_obfuscate(const std::string& str)
{
	return std::string();
}

std::string Package::_get_concatenated_file() const {
	
	size_t entry_count = header_to_content_table.size();
	
	std::vector<size_t> header_offsets;
	std::string header_names_string;
	
	header_offsets.resize(1 + entry_count*2);
	header_offsets[0] = entry_count;

	size_t offset_numbers_byte_length = sizeof(size_t) * (1 + entry_count * 2);
	
	std::string content_string;

	size_t header_names_total_length = 0;
	for (auto& entry : header_to_content_table)
		header_names_total_length += entry.first.size();

	int32_t i = 1;
	for (auto& entry : header_to_content_table) {
		header_names_string += entry.first;
		content_string += entry.second;
		header_offsets[-1 + i*2 + 0] = offset_numbers_byte_length + header_names_string.size();
		header_offsets[-1 + i*2 + 1] = offset_numbers_byte_length + header_names_total_length + content_string.size();
		i++;
	}

	std::string offset_strings;
	for (int32_t i = 0; i < header_offsets.size(); i++) {
		size_t offset = header_offsets[i];
		offset_strings += *(((char*)&offset) + 0);
		offset_strings += *(((char*)&offset) + 1);
		offset_strings += *(((char*)&offset) + 2);
		offset_strings += *(((char*)&offset) + 3);
		offset_strings += *(((char*)&offset) + 4);
		offset_strings += *(((char*)&offset) + 5);
		offset_strings += *(((char*)&offset) + 6);
		offset_strings += *(((char*)&offset) + 7);
	}

	std::string merged_string = offset_strings + header_names_string + content_string;

	return merged_string;
}

namespace {
	template<typename T>
	T _parse_large_int(const std::string& file, size_t file_offset_in_bytes) {
		if (file_offset_in_bytes + 7 >= file.size())
			return Package::invalid_index;

		T value = 0;
		for (int32_t byte_i = 0; byte_i < sizeof(T); byte_i++)
			value |= (T)file[file_offset_in_bytes + byte_i] << (byte_i * 8);

		return value;
	}
}

void Package::_read_from_concatenated_file(const std::string& file)
{
	clear();

	size_t entry_count = _parse_large_int<size_t>(file, 0);
	std::vector<size_t> header_offsets;

	for (int32_t i = 0; i < entry_count*2; i++)
		header_offsets.push_back(_parse_large_int<size_t>(file, sizeof(size_t) * (1 + i)));

	if (header_offsets.size() == 0)
		return;

	size_t first_header_begin = sizeof(size_t) * (1 + entry_count * 2);
	size_t first_content_begin = header_offsets[header_offsets.size() - 2];

	for (int32_t entry = 0; entry < entry_count; entry++) {

		size_t header_begin = entry == 0 ? first_header_begin : header_offsets[(entry - 1) * 2 + 0];
		size_t header_end = header_offsets[(entry) * 2 + 0];

		size_t content_begin = entry == 0 ? first_content_begin : header_offsets[(entry - 1) * 2 + 1];
		size_t content_end = header_offsets[(entry) * 2 + 1];

		std::string header = file.substr(header_begin, header_end - header_begin);
		std::string content = file.substr(content_begin, content_end - content_begin);

		add_content(header, content);
	}
}

void Package::_println(const std::string& message, const std::string& endline) const
{
	#ifdef PRINT_WARNINGS
	std::cout << message << endline;
	#endif
}

std::string Package::_convert_path_to_header(const std::filesystem::path& path)
{
	return path.generic_string();
}

std::string Package::get_packaged_file() const
{
	std::string concatenated_file = _get_concatenated_file();

}

void Package::clear_obfuscation_seeds()
{
	obfuscation_seeds.clear();
}

void Package::push_obfuscation_seed(size_t seed)
{
	obfuscation_seeds.push_back(seed);
}

void Package::set_default_obfuscation()
{
	push_obfuscation_seed(123);
}
