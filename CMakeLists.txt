# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

project ("RTv1")

set(TARGET "RTv1")


 set(SOURCES 
    src/rtv1.cpp
    src/main.cpp)


add_executable(${TARGET} ${SOURCES} "includes/scene.h")

target_include_directories(${TARGET} PUBLIC includes)
 set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Добавьте источник в исполняемый файл этого проекта.
