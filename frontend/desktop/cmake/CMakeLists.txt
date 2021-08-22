cmake_minimum_required(VERSION 3.10)

project(AppConsoleDemo)

set(APPSTROKE_C_SOURCE_DIR "../../../lib/src/c/")
set(APPSTROKE_LIB_DIR "../../../bin/linux/")

add_compile_options(-D DESKTOP)

add_library(AppStrokeC STATIC 
    ${APPSTROKE_C_SOURCE_DIR}AppStroke.c 
    ${APPSTROKE_C_SOURCE_DIR}AppStroke.h
    ${APPSTROKE_C_SOURCE_DIR}Basis.c
    ${APPSTROKE_C_SOURCE_DIR}Basis.h
    ${APPSTROKE_C_SOURCE_DIR}Stroke.c
    ${APPSTROKE_C_SOURCE_DIR}Stroke.h 
    ${APPSTROKE_C_SOURCE_DIR}types.h )


# Create the executable
add_executable(${PROJECT_NAME} 
    AppConsoleDemo.cpp
    types.h 
    AppStroke.h
    )

target_include_directories(${PROJECT_NAME} 
    PUBLIC
    ${APPSTROKE_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE AppStrokeC)
