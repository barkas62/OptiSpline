cmake_minimum_required(VERSION 3.10)

project (AppStroke)

set(APPSTROKE_C_SOURCE_DIR "../../src/c/")

set(SOURCES 
    ${APPSTROKE_C_SOURCE_DIR}AppStroke.c 
    ${APPSTROKE_C_SOURCE_DIR}AppStroke.h
    ${APPSTROKE_C_SOURCE_DIR}Basis.c
    ${APPSTROKE_C_SOURCE_DIR}Basis.h
    ${APPSTROKE_C_SOURCE_DIR}Stroke.c
    ${APPSTROKE_C_SOURCE_DIR}Stroke.h 
    ${APPSTROKE_C_SOURCE_DIR}types.h )

add_compile_options(-D DESKTOP)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
