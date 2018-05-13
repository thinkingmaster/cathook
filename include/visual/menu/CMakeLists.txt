target_sources(cathook PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/GUI.h")

add_subdirectory(ncc)

target_include_directories(cathook PRIVATE "${CMAKE_CURRENT_LIST_DIR}")