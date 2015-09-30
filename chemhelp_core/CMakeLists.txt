aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES)
add_library(chemhelp_core SHARED ${SOURCE_FILES})
target_include_directories(chemhelp_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})