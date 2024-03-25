workspace "EarthGL"
    configurations { "Debug", "Release" }

    language "C++"
    cppdialect "C++20"
    architecture "x64"

    filter "configurations:Debug"
        defines "_DEBUG"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines "_RELEASE"
        symbols "Off"
        optimize "Speed"

    outputDir = "%{cfg.buildcfg}/"

project "EarthGL"
    location "%{prj.name}"
    kind "ConsoleApp"

    -- Output directories
    targetdir ("_Output/Bin/" .. outputDir .. "%{prj.name}")
    objdir    ("_Output/Obj/" .. outputDir .. "%{prj.name}")

    files {
        "%{prj.location}/**.hpp",
        "%{prj.location}/**.cpp",
        "%{prj.location}/**.glsl",
    }

    includedirs {
        "Vendor/glew/include",
        "Vendor/glfw/include",
    }

    libdirs {
        "Vendor/glew/lib/Release/x64",
        "Vendor/glfw/lib-vc2022",
    }

    links {
        "opengl32",
        "glew32",
        "glfw3",
    }

    postbuildcommands {
        "{COPY} %{wks.location}/Vendor/glew/bin/Release/x64/glew32.dll %{cfg.targetdir}"
    }

    filter "system:windows"
        defines "_PLATFORM_WIN32"
        systemversion "latest"
