workspace "GraphicsCortex"
	architecture "x64"

	configurations
	{
		"Debug Core"
		"Cortex Core",
		"Cortex Editor",
	}

project "GraphicsCortex"
	location "GraphicsCortex"
	kind "StaticLib"
	language "C++"

	targetdir("%{prj.name}")
	objdir("%{prj.name}/%{cfg.buildcfg}")
	
	files
	{
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp"
	}

	include 
	{
		"%{prj.name}\Source\Graphics\Rendering\new system;"
		"%{prj.name}\Source\Graphics\Textures\new system;"
		"%{prj.name}\Source\System;"
		"%{prj.name}\Source\Physics;"
		"%{prj.name}\Source\Graphics\Windowing;"
		"%{prj.name}\Source\Graphics\Textures;"
		"%{prj.name}\Source\Graphics\Text;"
		"%{prj.name}\Source\Graphics\Shaders;"
		"%{prj.name}\Source\Graphics\Rendering;"
		"%{prj.name}\Source\Graphics\Meshes;"
		"%{prj.name}\Source\Graphics\Math;"
		"%{prj.name}\Source\Graphics\Lights;"
		"%{prj.name}\Source\Graphics\GUI;"
		"%{prj.name}\Source\Graphics\Defaults;"
		"%{prj.name}\Source\Graphics\Buffers;"
		"%{prj.name}\Source\Graphics;"
		"%{prj.name}\Source;"
		"%{prj.name}\Dependencies\assimp\include;"
		"%{prj.name}\Dependencies\stb_image;"
		"%{prj.name}\Dependencies\GLEW\include;"
		"%{prj.name}\Dependencies\imgui;"
		"%{prj.name}\Dependencies\glm;"
		"%{prj.name}\Dependencies\GLFWx64\include;"
		"%{prj.name}\Dependencies\physx\include;"
		"%{prj.name}\Dependencies\physx\snippetutils;"
		"%{prj.name}\Dependencies\physx\snippetcommon;"
		"%{prj.name}\Dependencies\hiredis\include;"
		"%{prj.name}\Dependencies\freetype\include;"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			
		}

	filter "configurations:Debug Core"
		symbols "On"

	filter "configurations:Cortex Core"
		optimize "On"

	filter "configurations:Cortex Editor"
		optimize "On"

	filter {"system:windows", "configurations:Debug Core"}
		buildoptions "/MDd"
	filter {"system:windows", "configurations:Cortex Core"}
		buildoptions "/MD"
	filter {"system:windows", "configurations:Cortex Editor"}
		buildoptions "/MD"