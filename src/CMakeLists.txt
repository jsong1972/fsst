set(FSST_LINK_THIRD_PARTYLIBS
    fmt-header-only
	spdlog_header_only)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_subdirectory(main)

set(FSST_LINK_LIBS
	fmt-header-only
    spdlog_header_only)

add_library(fsst STATIC ${ALL_OBJECT_FILES})
target_link_libraries(fsst ${FSST_LINK_LIBS})
target_include_directories(fsst PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

