#include "GraphicsCortex.h"

#include "Windowing/WindowSystemTest.h"
#include "Rendering/SingleMeshRenderingTest.h"
#include "Rendering/SingleMeshIndexedRenderingTest.h"
#include "Rendering/RenderPassClearTest.h"
#include "Rendering/MeshMaterialTest.h"
#include "Rendering/ShadowmapsTest.h"
#include "Rendering/CompleteRenderingTest.h"

#include "EntityComponent/EntityComponentTest.h"
#include "EntityComponent/MeshRendererTest.h"

#include "Model/ModelConstructionTest.h"
#include "Model/MeshConstructionTest.h"
#include "Model/ComplexMeshConstructionTest.h"
#include "Model/ModelLoadingTest.h"

#include "Image/ChannelSplitingTest.h"
#include "Image/HDRLoadingTest.h"

#include "Skybox/SkyboxRenderingTest.h"
#include "Skybox/EquirectangularProjectionTest.h"

#include "ImmediateRenderer/ImmediateRendererTest.h"

#include "Timer/TimerTest.h"

#include "CortexGUI/CortexGUITest_Fundamental.h"
#include "CortexGUI/CortexGUITest_Dynamic.h"
#include "CortexGUI/CortexGUITest_Relative.h"
#include "CortexGUI/CortexGUITest_Stateful.h"
#include "CortexGUI/CortexGUITest_Widgets.h"
#include "CortexGUI/CortexGUITest_Hierarchy.h"

int main() {
	
	//WindowSystemTest().run();
	//SingleMeshRenderingTest().run();
	//SingleMeshIndexedRenderingTest().run();
	//RenderPassClearTest().run();
	//EntityComponentSystemTest().run();
	//MeshMaterialTest().run();

	//ModelConstructionTest().run();
	//MeshConstructionTest().run();
	//ComplexMeshConstructionTest().run();

	//ModelLoadingTest().run();

	//MeshRendererTest().run();
	//ShadowmapsTest().run();
	//CompleteRenderingTest().run();

	//ChannelSplitingTest().run();
	//HDRLoadingTest().run();

	//SkyboxRenderingTest().run();
	//EquirectangularProjectionTest().run();

	//OpenGLResourceSystemTest().run();
	//OpenGLDeviceResourceSystemTest().run();

	//ImmediateRendererTest().run();

	//TimerTest().run();

	//CortexGUITest_Fundamental().run();
	//CortexGUITest_Dynamic().run();
	//CortexGUITest_Relative().run();
	//CortexGUITest_Stateful().run();
	CortexGUITest_Widgets().run();
	//CortexGUITest_Hierarchy().run();

}