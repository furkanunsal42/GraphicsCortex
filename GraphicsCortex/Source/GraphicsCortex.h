#pragma once

#include "Config.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Debuger.h"
#include "Monitor.h"
#include "Window.h"
#include "Library.h"
#include "Clipboard.h"

#include "Model.h"
#include "Asset/Asset.h"
#include "Buffer.h"
#include "VertexAttributeBuffer.h"
#include "Mesh.h"
#include "UniformBuffer.h"
#include "AsyncBuffer.h"
#include "ShaderCompiler.h"
#include "ComputeProgram.h"
#include "Image.h"
#include "TextureBase.h"
#include "Texture1D.h"
#include "Texture2D.h"
#include "Texture2DArray.h"
#include "TextureCubeMap.h"
#include "Texture3D.h"
#include "Renderbuffer.h"
#include "BindlessTextureBank.h"

//#include "BindlessMaterial.h"
#include "Framebuffer.h"
#include "Camera.h"
//#include "Scene.h"
//#include "Default_Assets.h"
#include "Default_Programs.h"
//#include "SkyBox.h"

#include "Font.h"
#include "Text.h"

#include "RenderParameters.h"
#include "PrimitiveRenderer.h"

#include "Rendering/RenderPipeline.h"
#include "RenderPasses/RenderPass_Clear.h"
#include "RenderPasses/RenderPass_Forward.h"
#include "RenderPasses/RenderPass_Skybox.h"
#include "RenderPasses/RenderPass_Shadowmaps.h"
//#include "RenderPasses/RenderPass_GBuffer.h"
//#include "RenderPasses/RenderPass_Deferred.h"

//#include "AssetImporter.h"
#include "DirectoryUtils.h"
#include "NTree.h"
#include "BinaryBuffers.h"
//#include "Gui.h"
//#include "GuiWidget.h"
//#include "DefaultWidgets.h"
//#include "Font.h"
//#include "Text.h"
#include "RamCache.h"

#include "Component/Component.h"
#include "Component/Entity.h"
#include "Component/Scene.h"
#include "Components/Component_Mesh.h"
#include "Components/Component_MeshRenderer.h"
#include "Components/Component_Material.h"

#include "PhysicsContext.h"
#include "PhysicsGeometry.h"
#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "PhysicsLink.h"
#include "PhysicsVehicle.h"
//#include "Object.h