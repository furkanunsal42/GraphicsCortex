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

#include "CortexGUI/CortexGUITest_Fundamental.h"
#include "CortexGUI/CortexGUITest_Dynamic.h"
#include "CortexGUI/CortexGUITest_Relative.h"
#include "CortexGUI/CortexGUITest_Stateful.h"
#include "CortexGUI/CortexGUITest_Widgets.h"
#include "CortexGUI/CortexGUITest_Hierarchy.h"

#include "SVG/SVGRenderingTest.h"

#include "DynamicMemory/DynamicMemoryTest.h"

#include "UnifiedRenderer/DynamicMemoryTest.h"
#include "UnifiedRenderer/BasicRenderTest.h"
#include "UnifiedRenderer/BasicMeshedRenderingTest.h"
#include "UnifiedRenderer/DeferredRendererTest.h"

#include "ECS/ECSTest.h"

#include "AssetImport/PBRParserTest.h"

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

	//CortexGUITest_Fundamental().run();
	//CortexGUITest_Dynamic().run();
	//CortexGUITest_Relative().run();
	//CortexGUITest_Stateful().run();
	//CortexGUITest_Widgets().run();
	//CortexGUITest_Hierarchy().run();

	//SVGRenderingTest().run();

	//DynamicMemoryTest().run();

	//UnifiedRendererAABBTest().run();
	//BasicRenderTest().run();
	//BasicMeshedRenderingTest().run();
	DeferredRendererTest().run();

	//ECSTest().run();

	//PBRParserTest().run();
}