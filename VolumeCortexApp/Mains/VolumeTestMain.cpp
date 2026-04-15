#include "VolumeCortex.h"
#include <iostream>

int main() {

	std::filesystem::path working_directory;
	working_directory = "C:\\Users\\unsal\\Desktop\\Kalebozan Veriler 2\\20260410145551.836\\[vg-data] 20260410145551.836\\volumedata_ctreconstructor";
	
	volumecortex::init();

	VolumeInfo info;
	info.load_from_disc(working_directory);
	info.print();

	Volume solver;
	solver.read(Volume::Density, info);

	volumecortex::launch_sliceview_window(solver);

	volumecortex::release();
}