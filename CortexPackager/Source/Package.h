#pragma once

#define PRINT_WARNINGS

#include <string>
#include <filesystem>
#include <map>

class Package {
public:

	constexpr static size_t invalid_index = std::numeric_limits<size_t>::max();

	void add_content(const std::string& header, const std::string& content);
	bool add_content(const std::filesystem::path& path);

	std::string get_content(const char* const header) const;
	std::string get_content(const std::string& header) const;
	std::string get_content(const std::filesystem::path& path) const;

	bool does_exist(const std::string& header) const;
	void clear();

	void read_from_memory(const std::string& packeged_stirng);
	void read_from_disk(const std::filesystem::path& path);
	void save_to_disk(const std::filesystem::path& path) const;
	std::string get_packaged_file() const;

	void clear_obfuscation_seeds();
	void push_obfuscation_seed(size_t seed);
	void set_default_obfuscation();

//private:

	std::string _obfuscate(const std::string& str);
	std::string _obfuscate_inverse(const std::string& str);

	std::string _get_concatenated_file() const;
	void _read_from_concatenated_file(const std::string& file);

	void _println(const std::string& message, const std::string& endline = "\n") const;
	static std::string _convert_path_to_header(const std::filesystem::path& path);
	
	std::vector<size_t> obfuscation_seeds;
	std::map<std::string, std::string> header_to_content_table;
};