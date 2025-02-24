#include "GraphicsCortex.h"

#include "Windowing/WindowSystemTest.h"
#include "Rendering/SingleMeshRenderingTest.h"
#include "Rendering/SingleMeshIndexedRenderingTest.h"
#include "Rendering/RenderPassClearTest.h"
#include "EntityComponent/EntityComponentTest.h"
#include "Model/ModelConstructionTest.h"
#include "Model/MeshConstructionTest.h"
#include "Model/ComplexMeshConstructionTest.h"


int main() {
	
	//WindowSystemTest().run();
	//SingleMeshRenderingTest().run();
	//SingleMeshIndexedRenderingTest().run();
	//RenderPassClearTest().run();
	//EntityComponentSystemTest().run();

	//ModelConstructionTest().run();
	//MeshConstructionTest().run();
	ComplexMeshConstructionTest().run();
}