#include "Application/Procedures.h"
#include <string>
#include <sstream>

int main() {
	
	std::string arguments = ".exe -conebeam3d -ipath=C:\\Users\\FURKAN.UNSAL\\Desktop\\Projektionen -ix=2048 -iy=2048 -ichannel=1 -ibytes=2 -n=1440 -fpp=16 -x=1472 -y=1472 -vs_h=128 -ps_h=2048 -fbp_dir=Source/GLSL/Compute/FBP/ -fft_dir=Source/GLSL/Compute/FFT/ -util_dir=Source/GLSL/Compute/Util/ -src_dst=730.87f -dtr_dst=669.04f -dtr_w=409.60f -v_w=213.845020f -v_h=209.196216f -log_norm_air=95 -rot=1 -opath=reconstruction -d=true -d_w=1024 -hist_norm=true";
	
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

