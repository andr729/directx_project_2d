cmake_minimum_required (VERSION 3.8)

project ("DirectX2D")

find_library(DIRECT2D d2d1)
if (NOT DIRECT2D)
    message(FATAL_ERROR "Could not find Direct2D.")
endif ()
find_library(DWRITE dwrite)
if (NOT DWRITE)
    message(FATAL_ERROR "Could not find DWRITE.")
endif ()

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# include_directories(src)

set (CMAKE_WIN32_EXECUTABLE "True")
set (CMAKE_CXX_FLAGS 
	"/Wall /std:c++20 /DUNICODE /TP /Zc:__cplusplus /EHs /MT")

set (SOURCE_FILES main.hpp main.cpp base.hpp game_state.hpp global_state.hpp physics.hpp physics.cpp upgrade_bar.hpp shop_scene.hpp button.hpp button.cpp draw_tools.hpp draw_tools.cpp rad_brush.cpp rad_brush.hpp)

add_executable (app ${SOURCE_FILES})
target_link_libraries(app ${DIRECT2D} ${DWRITE})


