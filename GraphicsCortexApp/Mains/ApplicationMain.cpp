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

#include "CortexGUI/CortexGUITest_Button.h"
#include "CortexGUI/CortexGUITest_Stack.h"
#include "CortexGUI/CortexGUITest_Grid.h"
#include "CortexGUI/CortexGUITest_TextInput.h"

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

	//CortexGUITest_Button().run();
	//CortexGUITest_Stack().run();
	//CortexGUITest_Grid().run();
	CortexGUITest_TextInput().run();
}