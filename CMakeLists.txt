cmake_minimum_required(VERSION 3.10)

# set the project name
project(VulkanTutorial)

find_package(glfw3 3.3 REQUIRED)
find_package(Vulkan REQUIRED)

# add the executable
set(SRC main.cpp vkapp.cpp)

add_executable(VulkanTutorial ${SRC})

target_link_libraries(${PROJECT_NAME} glfw vulkan)
