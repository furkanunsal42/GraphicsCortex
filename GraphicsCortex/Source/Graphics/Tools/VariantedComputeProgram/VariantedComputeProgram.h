#pragma once

#include "ComputeProgram.h"

#include <unordered_map>
#include <memory>

class VariantedComputeProgram;

class VariantDefinitions {
public:
	typedef std::vector<std::pair<std::string, std::string>>::iterator iterator;
	typedef std::vector<std::pair<std::string, std::string>>::const_iterator const_iterator;

	std::string& operator[](const std::string& macro_name);
	std::string& at(const std::string& macro_name);
	const std::string& at(const std::string& macro_name) const;
	
	const_iterator find(const std::string& macro_name) const;

	const_iterator begin() const;
	const_iterator end() const;

	void clear();
	size_t size() const;

	friend bool operator==(const VariantDefinitions& A, const VariantDefinitions& B);

private:
	friend VariantedComputeProgram;
	std::vector<std::pair<std::string, std::string>> definitions;
};

namespace std
{
	template<class A, class B>
	struct hash<pair<A, B>> {
		size_t operator() (const pair<A, B>& p) const {
			return std::rotl(hash<A>{}(p.first), 1) ^
				hash<B>{}(p.second);
		}
	};

	template<typename A, typename B>
	inline bool operator==(const std::pair<A, B>& lhs, const std::pair<A, B>& rhs) {
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}
}


struct Hash_VariantDefinitions {
	size_t operator()(const VariantDefinitions& obj) const {
		size_t seed = obj.size();
		for (auto x : obj) {
			// Hash combine formula from Boost
			seed ^= std::hash<std::pair<std::string, std::string>>{}(x)+0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};

class VariantedComputeProgram {
public:

	VariantedComputeProgram(size_t maximum_variant_count = 0);
	VariantedComputeProgram(Shader shader, size_t maximum_variant_count = 0);

	void set_shader(Shader shader);

	void begin_variant();
	void variant_define(const std::string& macro_name, const std::string& macro_value);
	std::shared_ptr<ComputeProgram> get_current_variant();
	
	void release_current_variant();
	void release_all_variants();

private:

	std::unordered_map<VariantDefinitions, std::shared_ptr<ComputeProgram>, Hash_VariantDefinitions> programs;
	VariantDefinitions current_macros;
	std::unique_ptr<Shader> shader = nullptr;
	size_t maximum_variant_count = 0;
};