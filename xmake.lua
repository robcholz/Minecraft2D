set_project("minecraft-2d")
add_rules("mode.debug", "mode.release")
set_languages("cxx20")
set_targetdir("bin")

add_requires("boost 1.84.0", { alias = "boost" })
add_requires("conan::sfml/2.5.1", { alias = "sfml" })
add_requires("conan::nlohmann_json/3.7.3", { alias = "json" })
add_requires("conan::plog/1.1.10", { alias = "plog" })
add_requires("conan::bitsery/5.2.2", { alias = "bitsery" })
add_requires("conan::tinyxml2/9.0.0", { alias = "xml" })

target("MemPool")
set_kind("static")
add_headerfiles("$(projectdir)/include/mempool/*.hpp")
add_headerfiles("$(projectdir)/include/mempool/*.inl")
add_headerfiles("$(projectdir)/include/mempool/*.h")
add_files("$(projectdir)/include/mempool/*.cpp")

target("MinecraftClient")
set_kind("binary")
add_includedirs("minecraft")
add_includedirs("include")
add_headerfiles("minecraft/**/*.hpp")
add_files("minecraft/**.cpp")
if is_plat("macosx") then
    add_files("minecraft/src/platform/osx/**.mm")
end
add_deps("MemPool")
add_packages("sfml")
add_packages("boost")
add_packages("json")
add_packages("plog")
add_packages("bitsery")
add_packages("xml")

--target("MinecraftClientTest")
--   set_kind("binary")
--    add_deps("MinecraftClient")
--    add_packages("conan::gtest/1.14.0")