set(SOURCE_FILES_PLIST_LIB
        base64.hpp
        Plist.cpp
        Plist.hpp
        PlistDate.cpp
        PlistDate.hpp
        pugiconfig.hpp
        pugixml.cpp
        pugixml.hpp
        )

add_library(Plist STATIC ${SOURCE_FILES_PLIST_LIB})

#boost
find_package(Boost 1.81.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(Plist ${Boost_LIBRARIES})