cmake_minimum_required(VERSION 3.20)

message(STATUS "## ${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt")

set(TARGET_NAME libXCSoar)
# add_subdirectory(Dialogs)
include(CMakeSource.cmake)

if (${XCSOAR_LIB_TYPE} STREQUAL "WITHOUT")
  set(XCSOAR_LIB_TYPE STATIC)
endif()

# add_executable(${TARGET_NAME}
add_library(${TARGET_NAME} ${XCSOAR_LIB_TYPE}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${SCRIPT_FILES}
)

set_target_properties(${TARGET_NAME} PROPERTIES FOLDER _Main)

target_link_libraries(${TARGET_NAME} PUBLIC Dialogs Math WeGlide) # Data)
# add_dependencies(${TARGET_NAME} Dialogs Math Data)
add_dependencies(${TARGET_NAME} util Data)
