cmake_minimum_required(VERSION 3.10)
project(StiffChaos)

# Estándar de C y banderas de optimización
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall")

# Incluir la carpeta de cabeceras
include_directories(include)

# Definir el ejecutable uniendo las piezas
add_executable(test_robertson 
    tests/test_robertson.c 
    solver/ros3.c 
    models/robertson.c
)

# Enlazar la librería matemática (el famoso -lm)
target_link_libraries(test_robertson m)
