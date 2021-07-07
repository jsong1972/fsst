set(FSST_MAIN_FILES
    fsst.cpp)

add_library_unity(fsst_main OBJECT ${FSST_MAIN_FILES})		
target_include_directories(fsst_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(fsst_main ${FSST_LINK_THIRD_PARTYLIBS})

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:fsst_main>
    PARENT_SCOPE)
	
