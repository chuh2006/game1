cmake_minimum_required(VERSION 3.10)

# Set the project name
project(play)

# Add the executable
add_executable(play main.cpp game.cpp player.cpp enemy.cpp)
