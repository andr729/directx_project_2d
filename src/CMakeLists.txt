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
	"/Wall /std:c++20 /DUNICODE /TP /Zc:__cplusplus /EHs /MT /O2")

set (SOURCE_FILES main.cpp physics.cpp button.cpp draw_tools.cpp brush.cpp app.cpp global_state.cpp shop_scene.cpp upgrade_bar.cpp game_scene.cpp menu_scene.cpp random.cpp win_scene.cpp)

add_executable (app ${SOURCE_FILES})
target_link_libraries(app ${DIRECT2D} ${DWRITE})

