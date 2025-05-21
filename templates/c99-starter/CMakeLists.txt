cmake_minimum_required(VERSION 3.16)
project(PROJECTNAME C)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    src/PROJECTNAME.c
)

# Specify C standard as a compile feature (no manual -std flags)
target_compile_features(${PROJECT_NAME} PRIVATE c_std_99)

# Include header files
target_include_directories(${PROJECT_NAME} PRIVATE
   "${PROJECT_SOURCE_DIR}/include"
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        #-Werror        # Treat warnings as errors
    )
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
        #/WX            # Treat warnings as errors
    )
endif()