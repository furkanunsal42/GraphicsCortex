#include "VolumeCortex.h"
#include <iostream>

int main() {

	std::filesystem::path working_directory;
	//working_directory = "C:\\Users\\unsal\\Desktop\\Kalebozan Veriler 2\\20260410145551.836\\[vg-data] 20260410145551.836\\volumedata_ctreconstructor";
	//working_directory = "C:\\Users\\unsal\\Desktop\\Kalebozan Veriler 2\\20260410132922.491_0000\\[vg-data] 20260410132922.491_0000\\volumedata_ctreconstructor";
	//working_directory = "C:\\Users\\unsal\\Desktop\\Kalebozan Veriler 2\\20260410143635.095\\[vg-data] 20260410143635.095\\volumedata_ctreconstructor";
	//working_directory = "C:\\Users\\unsal\\Desktop\\Kalebozan Veriler 2\\20260223150851.539_0000\\[vg-data] 20260223150851.539_0000\\volumedata_ctreconstructor";
	//working_directory = "C:\\Users\\unsal\\Desktop\\CTReconstruction3";
	working_directory = "C:\\Users\\unsal\\Desktop\\Kalebozan Veriler\\Demlik\\[vg-data] 20250818145043.850\\volumedata_ctreconstructor";
	
	volumecortex::init();

	VolumeInfo info;
	info.load_from_disc(working_directory);
	info.print();

	Volume solver;
	solver.read(Volume::Density, info);
	
	solver.compute_data(Volume::PorosityID);
	volumecortex::launch_sliceview_window(solver, Volume::PorosityID);

	volumecortex::release();
}