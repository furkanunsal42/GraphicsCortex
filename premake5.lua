workspace "GraphicsCortex"
	architecture "x64"

	configurations
	{
		"Release",
	}

	startproject "CortexEditor"


project "CortexPackager"
	location "CortexPackager"
	kind "StaticLib"
	language "C++"

	out_dir = "%{cfg.system}-%{cfg.buildcfg}-%{cfg.architecture}";

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")
	
	files
	{
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
	}

	includedirs 
	{
	}

	libdirs
	{
	}
		
	links
	{
		"kernel32.lib",
		"user32.lib",
		"gdi32.lib",
		"winspool.lib",
		"comdlg32.lib",
		"advapi32.lib",
		"shell32.lib",
		"ole32.lib",
		"oleaut32.lib",
		"uuid.lib",
		"odbc32.lib",
		"odbccp32.lib"
	}

	buildoptions { "/sdl"}--, "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	filter {"system:windows", "configurations:Debug"}
		buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }
	
		

project "CortexPackagerApp"
	location "CortexPackagerApp"
	kind "ConsoleApp"
	language "C++"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")

	files
	{
		"%{prj.name}/Mains/ApplicationMain.cpp", 
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",
		"%{prj.name}/Tests",
		"CortexPackager/Source",
	}

	links
	{
		"CortexPackager",
	}

	buildoptions { "/sdl" } -- , "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	filter {"system:windows", "configurations:Debug"}
		buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }
	
project "GraphicsCortex"
	location "GraphicsCortex"
	kind "StaticLib"
	language "C++"

	out_dir = "%{cfg.system}-%{cfg.buildcfg}-%{cfg.architecture}";

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")
	
	files
	{
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Dependencies/stb_image/**.h",
		"%{prj.name}/Dependencies/stb_image/**.cpp",
		"%{prj.name}/Dependencies/physx/snippetcommon/**.h",
		"%{prj.name}/Dependencies/physx/snippetcommon/**.cpp",
		"%{prj.name}/Dependencies/imgui/**.h",
		"%{prj.name}/Dependencies/imgui/**.cpp",
		"%{prj.name}/Dependencies/nanosvg/**.h",
		"%{prj.name}/Dependencies/nanosvg/**.cpp",
	}

	removefiles  
	{
		"%{prj.name}/Dependencies/physx/snippetcommon/ClassicMain.cpp",
		"%{prj.name}/Dependencies/imgui/imgui_demo.cpp",
	}

	includedirs 
	{
		"CortexPackager/Source",
		"%{prj.name}/Source/Graphics/RenderPasses",
		"%{prj.name}/Source/System",
		"%{prj.name}/Source/Physics",
		"%{prj.name}/Source/Graphics/Windowing",
		"%{prj.name}/Source/Graphics/Textures",
		"%{prj.name}/Source/Graphics/Text",
		"%{prj.name}/Source/Graphics/Shaders",
		"%{prj.name}/Source/Graphics/Rendering",
		"%{prj.name}/Source/Graphics/Meshes",
		"%{prj.name}/Source/Graphics/Math",
		"%{prj.name}/Source/Graphics/Lights",
		"%{prj.name}/Source/Graphics/GUI",
		"%{prj.name}/Source/Graphics/Defaults",
		"%{prj.name}/Source/Graphics/Buffers",
		"%{prj.name}/Source/Graphics",
		"%{prj.name}/Source",
		"%{prj.name}/Dependencies/libtiff/include",
		"%{prj.name}/Dependencies/assimp/include",
		"%{prj.name}/Dependencies/stb_image",
		"%{prj.name}/Dependencies/GLEW/include",
		"%{prj.name}/Dependencies/imgui",
		"%{prj.name}/Dependencies/glm",
		"%{prj.name}/Dependencies/GLFWx64/include",
		"%{prj.name}/Dependencies/physx/include",
		"%{prj.name}/Dependencies/physx/snippetutils",
		"%{prj.name}/Dependencies/physx/snippetcommon",
		"%{prj.name}/Dependencies/freetype/include",
		"%{prj.name}/Dependencies/nanosvg/"
	}

	libdirs
	{
		"%{prj.name}/Dependencies/libtiff/lib",
		"%{prj.name}/Dependencies/assimp/lib",
		"%{prj.name}/Dependencies/stb_image",
		"%{prj.name}/Dependencies/glm",
		"%{prj.name}/Dependencies/GLEW/lib/Release/x64",
		"%{prj.name}/Dependencies/GLFWx64/lib-vc2022",
		"%{prj.name}/Dependencies/physx/lib_checked",
		"%{prj.name}/Dependencies/freetype",
		"%{prj.name}/Dependencies/nanosvg",
	}
		
	links
	{
		"CortexPackager",
		"tiff.lib",
		"freetype.lib",
		"SnippetUtils_static_64.lib",
		"SnippetRender_static_64.lib",
		"SimulationController_static_64.lib",
		"SceneQuery_static_64.lib",
		"PhysXVehicle_static_64.lib",
		"PhysXTask_static_64.lib",
		"PhysXPvdSDK_static_64.lib",
		"PhysXFoundation_64.lib",
		"PhysXExtensions_static_64.lib",
		"PhysXCooking_64.lib",
		"PhysXCommon_64.lib",
		"PhysXCharacterKinematic_static_64.lib",
		"PhysX_64.lib",
		"LowLevelDynamics_static_64.lib",
		"LowLevelAABB_static_64.lib",
		"LowLevel_static_64.lib",
		"assimp-vc143-mt.lib",
		"zlibstatic.lib",
		"IrrXML.lib",
		"opengl32.lib",
		"glew32s.lib",
		"glfw3.lib",
		"kernel32.lib",
		"user32.lib",
		"gdi32.lib",
		"winspool.lib",
		"comdlg32.lib",
		"advapi32.lib",
		"shell32.lib",
		"ole32.lib",
		"oleaut32.lib",
		"uuid.lib",
		"odbc32.lib",
		"odbccp32.lib"
	}

	buildoptions { "/sdl", "/openmp", "/D _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING"}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }
	

project "GraphicsCortexApp"
	location "GraphicsCortexApp"
	kind "ConsoleApp"
	language "C++"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")

	files
	{
		"%{prj.name}/Mains/ApplicationMain.cpp", 
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Tests/**.h",
		"%{prj.name}/Tests/**.cpp",
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",
		"%{prj.name}/Tests",
		"CortexPackager/Source",
		"GraphicsCortex/Source/Graphics/RenderPasses",
		"GraphicsCortex/Source/System",
		"GraphicsCortex/Source/Physics",
		"GraphicsCortex/Source/Graphics/Windowing",
		"GraphicsCortex/Source/Graphics/Textures",
		"GraphicsCortex/Source/Graphics/Text",
		"GraphicsCortex/Source/Graphics/Shaders",
		"GraphicsCortex/Source/Graphics/Rendering",
		"GraphicsCortex/Source/Graphics/Meshes",
		"GraphicsCortex/Source/Graphics/Math",
		"GraphicsCortex/Source/Graphics/Lights",
		"GraphicsCortex/Source/Graphics/GUI",
		"GraphicsCortex/Source/Graphics/Defaults",
		"GraphicsCortex/Source/Graphics/Buffers",
		"GraphicsCortex/Source/Graphics",
		"GraphicsCortex/Source",
		"GraphicsCortex/Dependencies/libtiff/include",
		"GraphicsCortex/Dependencies/assimp/include",
		"GraphicsCortex/Dependencies/stb_image",
		"GraphicsCortex/Dependencies/GLEW/include",
		"GraphicsCortex/Dependencies/imgui",
		"GraphicsCortex/Dependencies/glm",
		"GraphicsCortex/Dependencies/GLFWx64/include",
		"GraphicsCortex/Dependencies/physx/include",
		"GraphicsCortex/Dependencies/physx/snippetutils",
		"GraphicsCortex/Dependencies/physx/snippetcommon",
		"GraphicsCortex/Dependencies/freetype/include",
		"GraphicsCortex/Dependencies/nanosvg"
	}

	links
	{
		"GraphicsCortex",
	}

	buildoptions { "/sdl", "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }

	postbuildcommands 
	{
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
	}

project "CortexEditor"
	location "CortexEditor"
	kind "ConsoleApp"
	language "C++"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")

	files
	{
		"%{prj.name}/Mains/ApplicationMain.cpp", 
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",

		"CortexPackager/Source",
		"GraphicsCortex/Source/Graphics/Rendering/new system",
		"GraphicsCortex/Source/Graphics/Textures/new system",
		"GraphicsCortex/Source/System",
		"GraphicsCortex/Source/Physics",
		"GraphicsCortex/Source/Graphics/Windowing",
		"GraphicsCortex/Source/Graphics/Textures",
		"GraphicsCortex/Source/Graphics/Text",
		"GraphicsCortex/Source/Graphics/Shaders",
		"GraphicsCortex/Source/Graphics/Rendering",
		"GraphicsCortex/Source/Graphics/Meshes",
		"GraphicsCortex/Source/Graphics/Math",
		"GraphicsCortex/Source/Graphics/Lights",
		"GraphicsCortex/Source/Graphics/GUI",
		"GraphicsCortex/Source/Graphics/Defaults",
		"GraphicsCortex/Source/Graphics/Buffers",
		"GraphicsCortex/Source/Graphics",
		"GraphicsCortex/Source",
		"GraphicsCortex/Dependencies/libtiff/include",
		"GraphicsCortex/Dependencies/assimp/include",
		"GraphicsCortex/Dependencies/stb_image",
		"GraphicsCortex/Dependencies/GLEW/include",
		"GraphicsCortex/Dependencies/imgui",
		"GraphicsCortex/Dependencies/glm",
		"GraphicsCortex/Dependencies/GLFWx64/include",
		"GraphicsCortex/Dependencies/physx/include",
		"GraphicsCortex/Dependencies/physx/snippetutils",
		"GraphicsCortex/Dependencies/physx/snippetcommon",
		"GraphicsCortex/Dependencies/freetype/include",
		"GraphicsCortex/Dependencies/nanosvg"
	}

	links
	{
		"GraphicsCortex",
	}

	buildoptions { "/sdl", "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }

	postbuildcommands 
	{
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
	}
	
	
project "GraphicsCortexEditor"
	location "GraphicsCortexEditor"
	kind "ConsoleApp"
	language "C++"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")

	files
	{
		"%{prj.name}/Mains/ApplicationMain.cpp", 
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",

		"CortexPackager/Source",
		"GraphicsCortex/Source/Graphics/Rendering/new system",
		"GraphicsCortex/Source/Graphics/Textures/new system",
		"GraphicsCortex/Source/System",
		"GraphicsCortex/Source/Physics",
		"GraphicsCortex/Source/Graphics/Windowing",
		"GraphicsCortex/Source/Graphics/Textures",
		"GraphicsCortex/Source/Graphics/Text",
		"GraphicsCortex/Source/Graphics/Shaders",
		"GraphicsCortex/Source/Graphics/Rendering",
		"GraphicsCortex/Source/Graphics/Meshes",
		"GraphicsCortex/Source/Graphics/Math",
		"GraphicsCortex/Source/Graphics/Lights",
		"GraphicsCortex/Source/Graphics/GUI",
		"GraphicsCortex/Source/Graphics/Defaults",
		"GraphicsCortex/Source/Graphics/Buffers",
		"GraphicsCortex/Source/Graphics",
		"GraphicsCortex/Source",
		"GraphicsCortex/Dependencies/libtiff/include",
		"GraphicsCortex/Dependencies/assimp/include",
		"GraphicsCortex/Dependencies/stb_image",
		"GraphicsCortex/Dependencies/GLEW/include",
		"GraphicsCortex/Dependencies/imgui",
		"GraphicsCortex/Dependencies/glm",
		"GraphicsCortex/Dependencies/GLFWx64/include",
		"GraphicsCortex/Dependencies/physx/include",
		"GraphicsCortex/Dependencies/physx/snippetutils",
		"GraphicsCortex/Dependencies/physx/snippetcommon",
		"GraphicsCortex/Dependencies/freetype/include",
		"GraphicsCortex/Dependencies/nanosvg"
	}

	links
	{
		"GraphicsCortex",
	}

	buildoptions { "/sdl", "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }

	postbuildcommands 
	{
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
	}
	

	project "ImageCortex"
	location "ImageCortex"
	kind "ConsoleApp"
	language "C++"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",
		"CortexPackager/Source",
		"GraphicsCortex/Source/Graphics/RenderPasses",
		"GraphicsCortex/Source/System",
		"GraphicsCortex/Source/Physics",
		"GraphicsCortex/Source/Graphics/Windowing",
		"GraphicsCortex/Source/Graphics/Textures",
		"GraphicsCortex/Source/Graphics/Text",
		"GraphicsCortex/Source/Graphics/Shaders",
		"GraphicsCortex/Source/Graphics/Rendering",
		"GraphicsCortex/Source/Graphics/Meshes",
		"GraphicsCortex/Source/Graphics/Math",
		"GraphicsCortex/Source/Graphics/Lights",
		"GraphicsCortex/Source/Graphics/GUI",
		"GraphicsCortex/Source/Graphics/Defaults",
		"GraphicsCortex/Source/Graphics/Buffers",
		"GraphicsCortex/Source/Graphics",
		"GraphicsCortex/Source",
		"GraphicsCortex/Dependencies/libtiff/include",
		"GraphicsCortex/Dependencies/assimp/include",
		"GraphicsCortex/Dependencies/stb_image",
		"GraphicsCortex/Dependencies/GLEW/include",
		"GraphicsCortex/Dependencies/imgui",
		"GraphicsCortex/Dependencies/glm",
		"GraphicsCortex/Dependencies/GLFWx64/include",
		"GraphicsCortex/Dependencies/physx/include",
		"GraphicsCortex/Dependencies/physx/snippetutils",
		"GraphicsCortex/Dependencies/physx/snippetcommon",
		"GraphicsCortex/Dependencies/freetype/include",
		"GraphicsCortex/Dependencies/nanosvg"
	}

	links
	{
		"GraphicsCortex",
	}

	buildoptions { "/sdl", "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }

	postbuildcommands 
	{
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
	}

project "CTReconstructor"
	location "CTReconstructor"
	kind "SharedLib"
	language "C++"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")

	files
	{
		"%{prj.name}/Dependencies/half_precision/include/**.h",
		"%{prj.name}/Dependencies/half_precision/include/**.hpp",
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",
		"%{prj.name}/Dependencies/half_precision/include",
		"CortexPackager/Source",
		"GraphicsCortex/Source/Graphics/RenderPasses",
		"GraphicsCortex/Source/System",
		"GraphicsCortex/Source/Physics",
		"GraphicsCortex/Source/Graphics/Windowing",
		"GraphicsCortex/Source/Graphics/Textures",
		"GraphicsCortex/Source/Graphics/Text",
		"GraphicsCortex/Source/Graphics/Shaders",
		"GraphicsCortex/Source/Graphics/Rendering",
		"GraphicsCortex/Source/Graphics/Meshes",
		"GraphicsCortex/Source/Graphics/Math",
		"GraphicsCortex/Source/Graphics/Lights",
		"GraphicsCortex/Source/Graphics/GUI",
		"GraphicsCortex/Source/Graphics/Defaults",
		"GraphicsCortex/Source/Graphics/Buffers",
		"GraphicsCortex/Source/Graphics",
		"GraphicsCortex/Source",
		"GraphicsCortex/Dependencies/libtiff/include",
		"GraphicsCortex/Dependencies/assimp/include",
		"GraphicsCortex/Dependencies/stb_image",
		"GraphicsCortex/Dependencies/GLEW/include",
		"GraphicsCortex/Dependencies/imgui",
		"GraphicsCortex/Dependencies/glm",
		"GraphicsCortex/Dependencies/GLFWx64/include",
		"GraphicsCortex/Dependencies/physx/include",
		"GraphicsCortex/Dependencies/physx/snippetutils",
		"GraphicsCortex/Dependencies/physx/snippetcommon",
		"GraphicsCortex/Dependencies/freetype/include",
		"GraphicsCortex/Dependencies/nanosvg"
	}

	links
	{
		"GraphicsCortex",
	}

	buildoptions { "/sdl", "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }

	postbuildcommands 
	{
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
	}


	
project "CTReconstructorApp"
	location "CTReconstructorApp"
	kind "ConsoleApp"
	language "C++"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")

	files
	{
		"CTReconstructor/Dependencies/half_precision/include/**.h",
		"CTReconstructor/Dependencies/half_precision/include/**.hpp",
		"CTReconstructor/Source/**.h",
		"CTReconstructor/Source/**.cpp",
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",
		"CTReconstructor/Source",
		"CTReconstructor/Dependencies/half_precision/include",
		"CortexPackager/Source",
		"GraphicsCortex/Source/Graphics/RenderPasses",
		"GraphicsCortex/Source/System",
		"GraphicsCortex/Source/Physics",
		"GraphicsCortex/Source/Graphics/Windowing",
		"GraphicsCortex/Source/Graphics/Textures",
		"GraphicsCortex/Source/Graphics/Text",
		"GraphicsCortex/Source/Graphics/Shaders",
		"GraphicsCortex/Source/Graphics/Rendering",
		"GraphicsCortex/Source/Graphics/Meshes",
		"GraphicsCortex/Source/Graphics/Math",
		"GraphicsCortex/Source/Graphics/Lights",
		"GraphicsCortex/Source/Graphics/GUI",
		"GraphicsCortex/Source/Graphics/Defaults",
		"GraphicsCortex/Source/Graphics/Buffers",
		"GraphicsCortex/Source/Graphics",
		"GraphicsCortex/Source",
		"GraphicsCortex/Dependencies/libtiff/include",
		"GraphicsCortex/Dependencies/assimp/include",
		"GraphicsCortex/Dependencies/stb_image",
		"GraphicsCortex/Dependencies/GLEW/include",
		"GraphicsCortex/Dependencies/imgui",
		"GraphicsCortex/Dependencies/glm",
		"GraphicsCortex/Dependencies/GLFWx64/include",
		"GraphicsCortex/Dependencies/physx/include",
		"GraphicsCortex/Dependencies/physx/snippetutils",
		"GraphicsCortex/Dependencies/physx/snippetcommon",
		"GraphicsCortex/Dependencies/freetype/include",
		"GraphicsCortex/Dependencies/nanosvg"
	}

	links
	{
		"GraphicsCortex",
		--"CTReconstructor",
	}

	buildoptions { "/sdl", "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }

	postbuildcommands 
	{
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
		--("{COPY} %{wks.location}build/" .. out_dir .. "CTReconstructor/*.dll %{cfg.buildtarget.relpath.directory}"),
	}

	
project "CTAnalyzerNeo"
	location "CTAnalyzerNeo"
	kind "ConsoleApp"
	language "C++"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")

	files
	{
		"CTReconstructor/Dependencies/half_precision/include/**.h",
		"CTReconstructor/Dependencies/half_precision/include/**.hpp",
		"CTReconstructor/Source/**.h",
		"CTReconstructor/Source/**.cpp",
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",
		"CTReconstructor/Source",
		"CTReconstructor/Dependencies/half_precision/include",
		"CortexPackager/Source",
		"GraphicsCortex/Source/Graphics/RenderPasses",
		"GraphicsCortex/Source/System",
		"GraphicsCortex/Source/Physics",
		"GraphicsCortex/Source/Graphics/Windowing",
		"GraphicsCortex/Source/Graphics/Textures",
		"GraphicsCortex/Source/Graphics/Text",
		"GraphicsCortex/Source/Graphics/Shaders",
		"GraphicsCortex/Source/Graphics/Rendering",
		"GraphicsCortex/Source/Graphics/Meshes",
		"GraphicsCortex/Source/Graphics/Math",
		"GraphicsCortex/Source/Graphics/Lights",
		"GraphicsCortex/Source/Graphics/GUI",
		"GraphicsCortex/Source/Graphics/Defaults",
		"GraphicsCortex/Source/Graphics/Buffers",
		"GraphicsCortex/Source/Graphics",
		"GraphicsCortex/Source",
		"GraphicsCortex/Dependencies/libtiff/include",
		"GraphicsCortex/Dependencies/assimp/include",
		"GraphicsCortex/Dependencies/stb_image",
		"GraphicsCortex/Dependencies/GLEW/include",
		"GraphicsCortex/Dependencies/imgui",
		"GraphicsCortex/Dependencies/glm",
		"GraphicsCortex/Dependencies/GLFWx64/include",
		"GraphicsCortex/Dependencies/physx/include",
		"GraphicsCortex/Dependencies/physx/snippetutils",
		"GraphicsCortex/Dependencies/physx/snippetcommon",
		"GraphicsCortex/Dependencies/freetype/include",
		"GraphicsCortex/Dependencies/nanosvg"
	}

	links
	{
		"GraphicsCortex",
		--"CTReconstructor",
	}

	buildoptions { "/sdl", "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }

	postbuildcommands 
	{
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
		--("{COPY} %{wks.location}build/" .. out_dir .. "CTReconstructor/*.dll %{cfg.buildtarget.relpath.directory}"),
	}


project "ipclib"
	location "ipclib"
	kind "SharedLib"
	language "C++"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",
	}

	links
	{
	}

	buildoptions { "/sdl", "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }

	postbuildcommands 
	{
		--("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
		--("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
	}

	
project "ipclibcs"
	location "ipclibcs"
	kind "ConsoleApp"
	language "C#"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")
	
	dotnetframework ("8.0")
	csversion("12")

	files
	{
		"%{prj.name}/Source/**.cs", 
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",
	}

	links
	{
	}

	buildoptions { 
		--"/sdl", 
		--"/openmp"
		 }

	filter "system:windows"
		--cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			--"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			--"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	--filter {"system:windows", "configurations:Release"}
		--buildoptions { "/MD" }

	postbuildcommands 
	{
		--("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
		--("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
	}


	
project "BoltzmannFluids"
location "BoltzmannFluids"
kind "ConsoleApp"
language "C++"

targetdir("build/" .. out_dir .. "%{prj.name}")
objdir("build-obj/" .. out_dir .. "%{prj.name}")

files
{
	"%{prj.name}/Source/**.h", 
	"%{prj.name}/Source/**.cpp",
}

removefiles  
{

}

includedirs 
{
	"%{prj.name}/Source",
	"GraphicsCortex/Source/Graphics/RenderPasses",
	"GraphicsCortex/Source/System",
	"GraphicsCortex/Source/Physics",
	"GraphicsCortex/Source/Graphics/Windowing",
	"GraphicsCortex/Source/Graphics/Textures",
	"GraphicsCortex/Source/Graphics/Text",
	"GraphicsCortex/Source/Graphics/Shaders",
	"GraphicsCortex/Source/Graphics/Rendering",
	"GraphicsCortex/Source/Graphics/Meshes",
	"GraphicsCortex/Source/Graphics/Math",
	"GraphicsCortex/Source/Graphics/Lights",
	"GraphicsCortex/Source/Graphics/GUI",
	"GraphicsCortex/Source/Graphics/Defaults",
	"GraphicsCortex/Source/Graphics/Buffers",
	"GraphicsCortex/Source/Graphics",
	"GraphicsCortex/Source",
	"GraphicsCortex/Dependencies/libtiff/include",
	"GraphicsCortex/Dependencies/assimp/include",
	"GraphicsCortex/Dependencies/stb_image",
	"GraphicsCortex/Dependencies/GLEW/include",
	"GraphicsCortex/Dependencies/imgui",
	"GraphicsCortex/Dependencies/glm",
	"GraphicsCortex/Dependencies/GLFWx64/include",
	"GraphicsCortex/Dependencies/physx/include",
	"GraphicsCortex/Dependencies/physx/snippetutils",
	"GraphicsCortex/Dependencies/physx/snippetcommon",
	"GraphicsCortex/Dependencies/freetype/include",
		"GraphicsCortex/Dependencies/nanosvg"
}

links
{
	"GraphicsCortex",
}

buildoptions { "/sdl", "/openmp" }

filter "system:windows"
	cppdialect "C++20"
	staticruntime "On"
	systemversion "latest"

filter "configurations:Debug"
	symbols "On"
	defines
	{
		"GLEW_STATIC",
		"WIN32",
		"_DEBUG",
		"_CONSOLE"
	}

filter "configurations:Release"
	optimize "On"
	defines 
	{
		"GLEW_STATIC",
		"WIN32",
		"NDEBUG",
		"_CONSOLE"
	}

--filter {"system:windows", "configurations:Debug"}
--	buildoptions { "/MDd" }
filter {"system:windows", "configurations:Release"}
	buildoptions { "/MD" }

postbuildcommands 
{
	("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
	("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
}


project "RubiksCube"
	location "RubiksCube"
	kind "ConsoleApp"
	language "C++"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")

	files
	{
		"%{prj.name}/Mains/ApplicationMain.cpp", 
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Tests/**.h",
		"%{prj.name}/Tests/**.cpp",
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",
		"%{prj.name}/Tests",
		"CortexPackager/Source",
		"GraphicsCortex/Source/Graphics/RenderPasses",
		"GraphicsCortex/Source/System",
		"GraphicsCortex/Source/Physics",
		"GraphicsCortex/Source/Graphics/Windowing",
		"GraphicsCortex/Source/Graphics/Textures",
		"GraphicsCortex/Source/Graphics/Text",
		"GraphicsCortex/Source/Graphics/Shaders",
		"GraphicsCortex/Source/Graphics/Rendering",
		"GraphicsCortex/Source/Graphics/Meshes",
		"GraphicsCortex/Source/Graphics/Math",
		"GraphicsCortex/Source/Graphics/Lights",
		"GraphicsCortex/Source/Graphics/GUI",
		"GraphicsCortex/Source/Graphics/Defaults",
		"GraphicsCortex/Source/Graphics/Buffers",
		"GraphicsCortex/Source/Graphics",
		"GraphicsCortex/Source",
		"GraphicsCortex/Dependencies/libtiff/include",
		"GraphicsCortex/Dependencies/assimp/include",
		"GraphicsCortex/Dependencies/stb_image",
		"GraphicsCortex/Dependencies/GLEW/include",
		"GraphicsCortex/Dependencies/imgui",
		"GraphicsCortex/Dependencies/glm",
		"GraphicsCortex/Dependencies/GLFWx64/include",
		"GraphicsCortex/Dependencies/physx/include",
		"GraphicsCortex/Dependencies/physx/snippetutils",
		"GraphicsCortex/Dependencies/physx/snippetcommon",
		"GraphicsCortex/Dependencies/freetype/include",
		"GraphicsCortex/Dependencies/nanosvg"
	}

	links
	{
		"GraphicsCortex",
	}

	buildoptions { "/sdl", "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }

	postbuildcommands 
	{
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
	}


project "Gozdiscoptics"
	location "Gozdiscoptics"
	kind "ConsoleApp"
	language "C++"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")

	files
	{
		"%{prj.name}/Mains/ApplicationMain.cpp", 
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",
		"%{prj.name}/Tests",
		"CortexPackager/Source",
		"GraphicsCortex/Source/Graphics/RenderPasses",
		"GraphicsCortex/Source/System",
		"GraphicsCortex/Source/Physics",
		"GraphicsCortex/Source/Graphics/Windowing",
		"GraphicsCortex/Source/Graphics/Textures",
		"GraphicsCortex/Source/Graphics/Text",
		"GraphicsCortex/Source/Graphics/Shaders",
		"GraphicsCortex/Source/Graphics/Rendering",
		"GraphicsCortex/Source/Graphics/Meshes",
		"GraphicsCortex/Source/Graphics/Math",
		"GraphicsCortex/Source/Graphics/Lights",
		"GraphicsCortex/Source/Graphics/GUI",
		"GraphicsCortex/Source/Graphics/Defaults",
		"GraphicsCortex/Source/Graphics/Buffers",
		"GraphicsCortex/Source/Graphics",
		"GraphicsCortex/Source",
		"GraphicsCortex/Dependencies/libtiff/include",
		"GraphicsCortex/Dependencies/assimp/include",
		"GraphicsCortex/Dependencies/stb_image",
		"GraphicsCortex/Dependencies/GLEW/include",
		"GraphicsCortex/Dependencies/imgui",
		"GraphicsCortex/Dependencies/glm",
		"GraphicsCortex/Dependencies/GLFWx64/include",
		"GraphicsCortex/Dependencies/physx/include",
		"GraphicsCortex/Dependencies/physx/snippetutils",
		"GraphicsCortex/Dependencies/physx/snippetcommon",
		"GraphicsCortex/Dependencies/freetype/include",
		"GraphicsCortex/Dependencies/nanosvg"
	}

	links
	{
		"GraphicsCortex",
	}

	buildoptions { "/sdl", "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }

	postbuildcommands 
	{
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
	}

project "GozdiscopticsMax"
	location "GozdiscopticsMax"

	kind "ConsoleApp"
	language "C++"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")

	files
	{
		"%{prj.name}/Mains/ApplicationMain.cpp", 
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",
		"%{prj.name}/Tests",
		"CortexPackager/Source",
		"GraphicsCortex/Source/Graphics/RenderPasses",
		"GraphicsCortex/Source/System",
		"GraphicsCortex/Source/Physics",
		"GraphicsCortex/Source/Graphics/Windowing",
		"GraphicsCortex/Source/Graphics/Textures",
		"GraphicsCortex/Source/Graphics/Text",
		"GraphicsCortex/Source/Graphics/Shaders",
		"GraphicsCortex/Source/Graphics/Rendering",
		"GraphicsCortex/Source/Graphics/Meshes",
		"GraphicsCortex/Source/Graphics/Math",
		"GraphicsCortex/Source/Graphics/Lights",
		"GraphicsCortex/Source/Graphics/GUI",
		"GraphicsCortex/Source/Graphics/Defaults",
		"GraphicsCortex/Source/Graphics/Buffers",
		"GraphicsCortex/Source/Graphics",
		"GraphicsCortex/Source",
		"GraphicsCortex/Dependencies/libtiff/include",
		"GraphicsCortex/Dependencies/assimp/include",
		"GraphicsCortex/Dependencies/stb_image",
		"GraphicsCortex/Dependencies/GLEW/include",
		"GraphicsCortex/Dependencies/imgui",
		"GraphicsCortex/Dependencies/glm",
		"GraphicsCortex/Dependencies/GLFWx64/include",
		"GraphicsCortex/Dependencies/physx/include",
		"GraphicsCortex/Dependencies/physx/snippetutils",
		"GraphicsCortex/Dependencies/physx/snippetcommon",
		"GraphicsCortex/Dependencies/freetype/include",
		"GraphicsCortex/Dependencies/nanosvg"
	}

	links
	{
		"GraphicsCortex",
	}

	buildoptions { "/sdl", "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }

	postbuildcommands 
	{
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
	}


project "KSP3"
	location "KSP3"


	kind "ConsoleApp"
	language "C++"

	targetdir("build/" .. out_dir .. "%{prj.name}")
	objdir("build-obj/" .. out_dir .. "%{prj.name}")

	files
	{
		"%{prj.name}/Mains/ApplicationMain.cpp", 
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp",
	}

	removefiles  
	{

	}

	includedirs 
	{
		"%{prj.name}/Source",
		"%{prj.name}/Tests",
		"CortexPackager/Source",
		"GraphicsCortex/Source/Graphics/RenderPasses",
		"GraphicsCortex/Source/System",
		"GraphicsCortex/Source/Physics",
		"GraphicsCortex/Source/Graphics/Windowing",
		"GraphicsCortex/Source/Graphics/Textures",
		"GraphicsCortex/Source/Graphics/Text",
		"GraphicsCortex/Source/Graphics/Shaders",
		"GraphicsCortex/Source/Graphics/Rendering",
		"GraphicsCortex/Source/Graphics/Meshes",
		"GraphicsCortex/Source/Graphics/Math",
		"GraphicsCortex/Source/Graphics/Lights",
		"GraphicsCortex/Source/Graphics/GUI",
		"GraphicsCortex/Source/Graphics/Defaults",
		"GraphicsCortex/Source/Graphics/Buffers",
		"GraphicsCortex/Source/Graphics",
		"GraphicsCortex/Source",
		"GraphicsCortex/Dependencies/libtiff/include",
		"GraphicsCortex/Dependencies/assimp/include",
		"GraphicsCortex/Dependencies/stb_image",
		"GraphicsCortex/Dependencies/GLEW/include",
		"GraphicsCortex/Dependencies/imgui",
		"GraphicsCortex/Dependencies/glm",
		"GraphicsCortex/Dependencies/GLFWx64/include",
		"GraphicsCortex/Dependencies/physx/include",
		"GraphicsCortex/Dependencies/physx/snippetutils",
		"GraphicsCortex/Dependencies/physx/snippetcommon",
		"GraphicsCortex/Dependencies/freetype/include",
		"GraphicsCortex/Dependencies/nanosvg"
	}

	links
	{
		"GraphicsCortex",
	}

	buildoptions { "/sdl", "/openmp" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		defines
		{
			"GLEW_STATIC",
			"WIN32",
			"_DEBUG",
			"_CONSOLE"
		}

	filter "configurations:Release"
		optimize "On"
		defines 
		{
			"GLEW_STATIC",
			"WIN32",
			"NDEBUG",
			"_CONSOLE"
		}

	--filter {"system:windows", "configurations:Debug"}
	--	buildoptions { "/MDd" }
	filter {"system:windows", "configurations:Release"}
		buildoptions { "/MD" }

	postbuildcommands 
	{
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/physx/lib_checked/*.dll %{cfg.buildtarget.relpath.directory}"),
		("{COPY} %{wks.location}GraphicsCortex/Dependencies/libtiff/lib/*.dll %{cfg.buildtarget.relpath.directory}")
	}
