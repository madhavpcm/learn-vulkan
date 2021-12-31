#include<stdexcept>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<vector>

#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>
#define GLFW_FORCE_RADIANS
#define GLFW_FORCE_DEPTH_ZERO_TO_ONE

#include<glm/vec4.hpp>
#include<glm/mat4x4.hpp>

class vkapp{
    public:
        void run();
        vkapp(int width, int height);
        ~vkapp();
    private:
        void initVulkan();
        void initWindow();
        void createInstance();
        bool checkValidationLayerSupport();

        void mainLoop();
        void cleanUp();

        GLFWwindow* m_window = nullptr;
        int m_height,m_width;
        VkInstance m_instance;
        
        const std::vector<const char*> m_validationLayers={"VK_LAYER_KHRONOS_validation"};
        #ifdef NDEBUG
        const bool enableValidationLayers = true;
        #else
        const bool m_enableValidationLayers = false;
        #endif
};
