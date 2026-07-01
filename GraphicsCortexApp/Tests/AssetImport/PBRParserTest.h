#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "Asset/PBRParser.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include <assimp/postprocess.h>

class PBRParserTest : public TestBench {
public:

	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(0, 0);
		default_window_desc->f_swap_interval = 0;
		default_window_desc->w_scale_framebuffer_size = false;
		default_window_desc->w_scale_window_size = false;

		default_init();

        std::filesystem::path asset_path = "../GraphicsCortex/Models/Sponza/scene.gltf";

        Assimp::Importer* importer = new Assimp::Importer();
        const aiScene* scene = ((Assimp::Importer*)importer)->ReadFile(asset_path.string(),
            aiProcess_Triangulate |
            aiProcess_SortByPType |
            aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
            aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
            aiProcess_ValidateDataStructure | // perform a full validation of the loader's output
            aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
            aiProcess_RemoveRedundantMaterials | // remove redundant materials
            //aiProcess_FindDegenerates         | // remove degenerated polygons from the import
            //aiProcess_FindInvalidData         | // detect invalid model data, such as invalid normal vectors
            aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
            aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
            aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
            aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
            aiProcess_OptimizeMeshes | // join small meshes, if possible;
            aiProcess_SplitByBoneCount            // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
        );

		MaterialParser parser;
        for (int32_t i = 0; i < scene->mNumMaterials; i++) {
            ParsedMaterial material_info = parser.parse(scene->mMaterials[i], asset_path.parent_path());
            std::cout << material_info << std::endl;
        }
		
		std::cin.get();
		return true;
	}
};