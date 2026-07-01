#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <chrono>

class RecursiveProgressBar {
public:
	typedef std::chrono::system_clock::duration		duration;
	typedef std::chrono::system_clock::time_point	time_point;
	static constexpr time_point invalid_time = time_point::min();

	void		start();
	void		finish();
	void		publish_checkpoint();

	duration	get_ellapsed_time();
	duration	get_eta();
	float		get_estimated_remaining_work_percentage();

	void		clear();

private:

	struct Work {
		std::string name = "work";
		std::vector<time_point> checkpoints;
		bool is_finished = false;
	};

	std::vector<Work> work_stack;
};