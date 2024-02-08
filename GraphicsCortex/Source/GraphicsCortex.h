#pragma once

#include "Config.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include "CortexMath.h"
#include "Debuger.h"
#include "Frame.h"
#include "Model.h"
#include "Buffer.h"
#include "ShaderCompiler.h"
#include "Image.h"
#include "Texture.h"

#include "TextureBase.h"
#include "Texture2D.h"

#include "Graphic.h"
#include "Camera.h"
#include "Scene.h"
#include "Default_Assets.h"
#include "Default_Programs.h"
#include "FrameBuffer.h"
#include "CubeMap.h"
#include "UnifromQueue.h"
#include "UniformUpdater.h"
#include "ModelTextureTable.h"
#include "AssetImporter.h"
#include "DirectoryUtils.h"
#include "BinaryBuffers.h"
#include "Gui.h"
#include "_templated_overloads.h"
#include "Default_Gui_Widgets.h"
#include "Font.h"
#include "Text.h"
#include "RamCache.h"
#include "RenderPipeline.h"
#include "CustomRenderPipelines.h"

#include "PhysicsContext.h"
#include "PhysicsGeometry.h"
#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "PhysicsLink.h"
#include "PhysicsVehicle.h"

#include "Object.h"



