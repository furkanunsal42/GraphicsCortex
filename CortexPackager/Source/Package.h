#pragma once

#define PRINT_WARNINGS

#include <string>
#include <filesystem>
#include <map>
#include <memory>

class Package {
public:

	Package();

	constexpr static size_t invalid_index = std::numeric_limits<size_t>::max();

	void add(const std::string& header, const std::string& content);
	bool add(const std::filesystem::path& path);

	const std::string& get(const char* const header) const;
	const std::string& get(const std::string& header) const;
	const std::string& get(const std::filesystem::path& path) const;

	bool does_exist(const std::filesystem::path& path, std::string* out_match = nullptr) const;
	bool does_exist(const std::string& header) const;
	void clear();

	void read_from_memory(std::string packeged_stirng);
	bool read_from_disk(const std::filesystem::path& path);
	void save_to_disk(const std::filesystem::path& path) const;
	std::string get_packaged_file(bool obfuscate = true) const;

	void clear_obfuscation_seeds();
	void push_obfuscation_seed(size_t seed);
	void set_default_obfuscation();

	std::vector<size_t>& get_obfuscation_seeds();
	std::map<std::string, std::string>& get_header_to_content_table();

	bool is_weak_matching_accepted();
	void set_weak_matching_policy(bool accepted);

	// debug
	void print_headers();
	void print_content();
	void print_concatenated_file();
	void print_obfuscated_file();

	// loading package
	static bool load_package(const std::filesystem::path& package_path);
	static void unload_package();
	static bool is_package_loaded();
	static std::unique_ptr<Package> loaded_package;

private:

	static constexpr int32_t _obfuscation_swap_count_min_string_size_multiplier = 4;
	static constexpr int32_t _obfuscation_swap_count_max_string_size_multiplier = 5;

	static std::vector<std::filesystem::path> _get_all_fitting_filepaths(std::filesystem::path path);
	static void _obfuscate(std::string& str, size_t obfuscation_seed, bool inverse = false);
	
	bool _add_file(const std::filesystem::path& path);

	std::string _get_concatenated_file() const;
	void _read_from_concatenated_file(const std::string& file);

	static void _println(const std::string& message, const std::string& endline = "\n");
	static std::string _convert_path_to_header(const std::filesystem::path& path);
	static std::string _convert_path_to_header_weak(const std::filesystem::path& path);
	
	std::vector<size_t> obfuscation_seeds;
	std::map<std::string, std::string> header_to_content_table;
	bool accept_weak_matching_paths = true;
};