#include "GraphicsCortex.h"

#include "Windowing/WindowSystemTest.h"
#include "Rendering/SingleMeshRenderingTest.h"
#include "Rendering/SingleMeshIndexedRenderingTest.h"
#include "Rendering/RenderPassClearTest.h"
#include "Rendering/MeshMaterialTest.h"

#include "EntityComponent/EntityComponentTest.h"
#include "EntityComponent/MeshRendererTest.h"

#include "Model/ModelConstructionTest.h"
#include "Model/MeshConstructionTest.h"
#include "Model/ComplexMeshConstructionTest.h"
#include "Model/ModelLoadingTest.h"

#include "Image/ChannelSplitingTest.h"
#include "Image/HDRLoadingTest.h"

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

	//ChannelSplitingTest().run();
	HDRLoadingTest().run();
}