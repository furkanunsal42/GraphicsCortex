#include "GraphicsCortex.h"

#include "Windowing/WindowSystemTest.h"
#include "Rendering/SingleMeshRenderingTest.h"
#include "Rendering/SingleMeshIndexedRenderingTest.h"
#include "Rendering/RenderPassClearTest.h"
#include "EntityComponent/EntityComponentTest.h"
#include "Model/ModelConstructionTest.h"

int main() {
	
	//WindowSystemTest().run();
	//SingleMeshRenderingTest().run();
	SingleMeshIndexedRenderingTest().run();
	//RenderPassClearTest().run();
	//EntityComponentSystemTest().run();

	//ModelConstructionTest().run();,
}