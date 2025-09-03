#include "Package.h"

#ifdef PRINT_WARNINGS
#include <iostream>
#endif
#include <fstream>

#include <random>

void Package::clear()
{
	header_to_content_table.clear();
	set_default_obfuscation();
}

void Package::read_from_memory(std::string packeged_stirng)
{
	for (int32_t i = obfuscation_seeds.size() - 1; i >= 0; i--)
		_obfuscate(packeged_stirng, obfuscation_seeds[i], true);

	_read_from_concatenated_file(packeged_stirng);
}

void Package::read_from_disk(const std::filesystem::path& path)
{
	clear();

	std::ifstream file(path, std::ios::binary);
	if (!file) {
		_println("[Package Warning] Package::read_from_disk() is called but specified file couldn't found");
		return;
	}

	auto size = std::filesystem::file_size(path);
	std::string file_string(size, '\0');
	file.read(&file_string[0], size);

	read_from_memory(file_string);
}

void Package::save_to_disk(const std::filesystem::path& path) const
{
	std::string file_string = get_packaged_file();

	std::ofstream file(path, std::ios::binary);
	file.write(&file_string[0], file_string.size());
}

bool Package::does_exist(const std::string& header) const
{
	return header_to_content_table.find(header) != header_to_content_table.end();
}

Package::Package()
{
	set_default_obfuscation();
}

void Package::add(const std::string& header, const std::string& content)
{
	if (does_exist(header))
		_println("[Package Warning] Package::add_content() is called but given header already existed. It will be overwritten");

	header_to_content_table[header] = content;
}

bool Package::add(const std::filesystem::path& path)
{
	std::vector<std::filesystem::path> files = _get_all_fitting_filepaths(path);

	bool success = true;
	for (auto& file : files) {
		bool file_success = _add_file(file);
		if (!file_success) {
			success = false;
			_println("[Package Warning] _add_file failed on path : " + file.string());
		}
	}
	return success;
}

std::vector<std::filesystem::path> Package::_get_all_fitting_filepaths(std::filesystem::path path)
{
	//path = std::filesystem::absolute(path);
	std::filesystem::path parent_directory = path;

	std::filesystem::path extension = path.extension();
	std::filesystem::path file_name = path.filename();
	file_name.replace_extension("");

	bool has_extension = path.has_extension();
	bool star_in_extension = extension.string().find("*") != std::string::npos;
	bool star_in_file_name = file_name.string().find("*") != std::string::npos;
	
	if (file_name == "**" && extension == ".*")
		extension = ".**";
	if (extension == ".**" && file_name == "*")
		file_name = "**";

	if (star_in_file_name && file_name != "**" && file_name != "*") {
		_println("[Package Warning] only \"*\" and \"**\" is accepted for generic filename. specified filename: " + file_name.string() + " is not valid");
		return std::vector<std::filesystem::path>();
	}
	if (star_in_extension && extension != ".**" && extension != ".*") {
		_println("[Package Warning] only \".*\" and \".**\" is accepted for generic extension. specified extension: " + extension.string() + " is not valid");
		return std::vector<std::filesystem::path>();
	}

	if (!has_extension && (file_name == "**" || file_name == "*")) {
		extension = ".*";
		star_in_extension = true;
		parent_directory = path.parent_path();
	}
	else if (!has_extension) {
		parent_directory = path;
		file_name = "*";
		extension = ".*";
		star_in_file_name = true;
		star_in_extension = true;
	}
	else if (has_extension)
		parent_directory = path.parent_path();

	if (parent_directory == "")
		parent_directory = "./";

	enum _mode {
		_file,
		_directory,
		_directory_recursive,
	};

	_mode requested_mode = _file;


	if (file_name == "*" || extension == ".*") {
		requested_mode = _directory;
	}

	if (file_name == "**" || extension == ".**") {
		requested_mode = _directory_recursive;
	}

	std::vector<std::filesystem::path> paths;

	if (requested_mode == _file) {
		if (!std::filesystem::exists(path))
			return std::vector<std::filesystem::path>();

		paths.push_back(std::filesystem::relative(path));
	}
	else if (requested_mode == _directory) {
		
		if (!std::filesystem::exists(parent_directory))
			return std::vector<std::filesystem::path>();

		std::filesystem::directory_iterator children(parent_directory);

		for (auto& child : children) {
			if (!std::filesystem::is_regular_file(child))
				continue;
			
			std::filesystem::path child_extension = child.path().filename().extension();
			std::filesystem::path child_file_name = child.path().filename();
			child_file_name.replace_extension("");
			
			if (!star_in_file_name && child_file_name != file_name)
				continue;

			if (!star_in_extension && child_extension != extension)
				continue;

			paths.push_back(std::filesystem::relative(child.path()));
		}
	}
	else if (requested_mode == _directory_recursive) {

		if (!std::filesystem::exists(parent_directory))
			return std::vector<std::filesystem::path>();

		std::filesystem::recursive_directory_iterator children(parent_directory);

		for (auto& child : children) {
			if (!std::filesystem::is_regular_file(child))
				continue;

			std::filesystem::path child_extension = child.path().filename().extension();
			std::filesystem::path child_file_name = child.path().filename();
			child_file_name.replace_extension("");

			if (!star_in_file_name && child_file_name != file_name)
				continue;

			if (!star_in_extension && child_extension != extension)
				continue;

			paths.push_back(std::filesystem::relative(child.path()));
		}
	}

	return paths;
}

bool Package::_add_file(const std::filesystem::path& path)
{
	std::string header = _convert_path_to_header(path);

	if (does_exist(header))
		_println("[Package Warning] Package::_add_file() is called but given header already existed. It will be overwritten");

	
	std::ifstream file(path, std::ios::binary);
	if (!file) return false;

	auto size = std::filesystem::file_size(path);
	std::string content(size, '\0');
	file.read(&content[0], size);

	header_to_content_table[header] = content;
	return true;
}

std::string Package::get(const char* const header) const
{
	return get(std::filesystem::path(header));
}

std::string Package::get(const std::string& header) const
{
	if (!does_exist(header))
	{
		_println("[Package Warning] Package::get() is called with an header that isn't registered yet");
		return "";
	}

	return header_to_content_table.at(header);
}

std::string Package::get(const std::filesystem::path& path) const
{
	std::string header = _convert_path_to_header(path);
	return get(header);
}

void Package::_obfuscate(std::string& str, size_t obfuscation_seed, bool inverse)
{
	size_t seed = obfuscation_seed;
	
	std::mt19937 rng(seed);
	std::uniform_int_distribution<std::mt19937::result_type> dist256(
		std::numeric_limits<uint8_t>::min(),
		std::numeric_limits<uint8_t>::max()
	);

	std::uniform_int_distribution<std::mt19937::result_type> dist_string_length(0, str.length());

	std::uniform_int_distribution<std::mt19937::result_type> dist_swap_count(
		str.length() * _obfuscation_swap_count_min_string_size_multiplier,
		str.length() * _obfuscation_swap_count_max_string_size_multiplier
	);

	size_t swap_count = dist_swap_count(rng);
	std::vector<std::pair<size_t, size_t>> swap_indicies;
	std::vector<uint8_t> character_add_field;

	swap_indicies.reserve(swap_count);
	character_add_field.reserve(str.length());

	for (size_t i = 0; i < str.length(); i++)
		character_add_field.push_back(dist256(rng));

	for (size_t i = 0; i < swap_count; i++)
		swap_indicies.push_back(std::pair<size_t, size_t>(dist_string_length(rng), dist_string_length(rng)));

	if (!inverse) {

		for (size_t i = 0; i < str.length(); i++)
			str[i] += character_add_field[i];
	
		for (size_t i = 0; i < swap_indicies.size(); i++)
			std::swap(str[swap_indicies[i].first], str[swap_indicies[i].second]);

	}
	else {
		for (size_t i = swap_indicies.size() - 1; i >= 0 && i != -1; i--)
			std::swap(str[swap_indicies[i].first], str[swap_indicies[i].second]);

		for (size_t i = 0; i < str.length(); i++)
			str[i] -= character_add_field[i];
	}
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
		offset_strings += *(((unsigned char*)&offset) + 0);
		offset_strings += *(((unsigned char*)&offset) + 1);
		offset_strings += *(((unsigned char*)&offset) + 2);
		offset_strings += *(((unsigned char*)&offset) + 3);
		offset_strings += *(((unsigned char*)&offset) + 4);
		offset_strings += *(((unsigned char*)&offset) + 5);
		offset_strings += *(((unsigned char*)&offset) + 6);
		offset_strings += *(((unsigned char*)&offset) + 7);
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
		for (int32_t byte_i = 0; byte_i < sizeof(T); byte_i++) {
			uint8_t unsigned_byte = file[file_offset_in_bytes + byte_i];
			value |= ((T)unsigned_byte) << (byte_i * 8);
		}

		return value;
	}
}

void Package::_read_from_concatenated_file(const std::string& file)
{
	clear();

	size_t entry_count = _parse_large_int<size_t>(file, 0);
	std::vector<size_t> header_offsets;

	for (int32_t i = 0; i < entry_count * 2; i++) {
		size_t value = _parse_large_int<size_t>(file, sizeof(size_t) * (1 + i));
		header_offsets.push_back(value);
	}

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

		add(header, content);
	}
}

void Package::_println(const std::string& message, const std::string& endline)
{
	#ifdef PRINT_WARNINGS
	std::cout << message << endline;
	#endif
}

std::string Package::_convert_path_to_header(const std::filesystem::path& path)
{
	return path.generic_string();
}

std::string Package::get_packaged_file(bool obfuscate) const
{
	std::string concatenated_file = _get_concatenated_file();

	if (obfuscate) {
		for (size_t seed : obfuscation_seeds)
			_obfuscate(concatenated_file, seed, false);
	}

	return concatenated_file;
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
	push_obfuscation_seed(128937986851);
}

std::vector<size_t>& Package::get_obfuscation_seeds()
{
	return obfuscation_seeds;
}

std::map<std::string, std::string>& Package::get_header_to_content_table()
{
	return header_to_content_table;
}

void Package::print_headers(){
	std::cout << "[Package Info] Loaded Headers: " << std::endl;
	std::cout << "////////////////////////////////" << std::endl;
	for (auto& entry : header_to_content_table)
		std::cout << entry.first << std::endl;
	std::cout << "////////////////////////////////" << std::endl;
}

void Package::print_content(){
	std::cout << "[Package Info] Loaded Content: " << std::endl;

	for (auto& entry : header_to_content_table) {
		std::cout << "////////////////////////////////" << std::endl;
		std::cout << "	--HEADER--	" << std::endl;
		std::cout << entry.first << std::endl;
		std::cout << "	--CONTENT--	" << std::endl;
		std::cout << entry.second << std::endl;
		std::cout << "////////////////////////////////" << std::endl;
	}
}

void Package::print_concatenated_file(){
	std::cout << "[Package Info] Concatenated File: " << std::endl;
	std::cout << "////////////////////////////////" << std::endl;
	std::string concatenated_file = get_packaged_file(false);
	for (char& c : concatenated_file)
		std::cout << c;
	std::cout << "////////////////////////////////" << std::endl;
}

void Package::print_obfuscated_file(){
	std::cout << "[Package Info] Concatenated File: " << std::endl;
	std::cout << "////////////////////////////////" << std::endl;
	std::string obfuscated_file = get_packaged_file(true);
	for (char& c : obfuscated_file)
		std::cout << c;
	std::cout << "////////////////////////////////" << std::endl;
}
