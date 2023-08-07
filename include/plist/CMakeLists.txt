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

ADD_LIBRARY(PlistLib STATIC ${SOURCE_FILES_PLIST_LIB})

#boost
find_package(Boost 1.81.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(PlistLib ${Boost_LIBRARIES})