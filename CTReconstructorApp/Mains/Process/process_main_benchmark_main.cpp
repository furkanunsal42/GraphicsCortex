#include "Application/Procedures.h"
#include <string>
#include <sstream>

void reconstruction_test(std::string& arguments) {

	std::stringstream ss(arguments);
	std::vector<std::string> argv_vector;
	while (!ss.eof()) {
		std::string line;
		ss >> line;
		argv_vector.push_back(line);
	}

	char** argv = new char* [argv_vector.size()];
	for (int i = 0; i < argv_vector.size(); i++)
		argv[i] = const_cast<char*>(argv_vector[i].c_str());

	reconstruct(argv_vector.size(), argv);
}

int main() {

	std::vector<int> resolutions = { 2800, 3072 };
	std::vector<int> volume_segment_sizes = { 64 };
	std::vector<int> projection_segment_sizes = { 2800 };

	for (int current_resolution : resolutions) {
		for (int current_projection_segment_size : projection_segment_sizes) {
			for (int current_volume_segment_size : volume_segment_sizes) {

				if (current_volume_segment_size >= current_resolution) {
					current_volume_segment_size = current_resolution;
				}

				if (current_projection_segment_size >= current_resolution) {
					current_projection_segment_size = current_resolution;
				}

				std::string arguments = ".exe -conebeam3d -ipath=C:\\Users\\FURKAN.UNSAL\\Desktop\\Projektionen -ix=2048 -iy=2048 -ichannel=1 -ibytes=2 -n=1440 -fpp=16 \
					-x="	+ std::to_string(current_resolution)				+ " \
					-y="	+ std::to_string(current_resolution)				+ " \
					-vs_h=" + std::to_string(current_volume_segment_size)		+ " \
					-ps_h=" + std::to_string(current_projection_segment_size)	+ " \
					-fbp_dir=Source/GLSL/Compute/FBP/ -fft_dir=Source/GLSL/Compute/FFT/ -util_dir=Source/GLSL/Compute/Util/ -src_dst=730.87f -dtr_dst=669.04f -dtr_w=409.60f -v_w=213.845020f -v_h=209.196216f -log_norm_air=95 -rot=1 -opath=reconstruction -d=false -d_w=1024 -hist_norm=true -d_p=false";

				auto begin = std::chrono::system_clock::now();

				reconstruction_test(arguments);

				auto end = std::chrono::system_clock::now();

				std::cout << "resolution:" << current_resolution << " projection_segment: " << current_projection_segment_size << " volume_segment:" << current_volume_segment_size << " took " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;
				
				double video_memory_volume = (double)current_resolution * current_volume_segment_size * current_resolution * 2;// *(current_volume_segment_size == current_resolution ? 1 : 2);
				double video_memory_projection = (double)current_resolution * current_projection_segment_size * 1440 * 2;//  *(current_projection_segment_size == current_resolution ? 1 : 2);
				
				double total_vram = video_memory_projection + video_memory_volume;
				double total_memory = ((double)current_resolution * current_resolution * current_resolution + (double)current_resolution * current_resolution * 1440) * 2;
				total_memory = total_memory - total_vram;

				std::cout << "total_memory: " <<  total_memory / (1024.0 * 1024.0 * 1024.0) << "gb" << std::endl;
				std::cout << "video memory: " << (video_memory_projection + video_memory_volume) / (1024.0 * 1024.0 * 1024.0) << "gb" << std::endl;
			
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
		}
	}


}