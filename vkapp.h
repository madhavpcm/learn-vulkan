#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_FORCE_RADIANS
#define GLFW_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
struct QueueFamilyIndices
{
        std::optional<uint32_t> graphicsFamily;

        bool isComplete()
        {
                return graphicsFamily.has_value();
        }
};
class vkapp
{
public:
        void run();
        vkapp(int width, int height);
        ~vkapp();

private:
        void initVulkan();
        void initWindow();
        void createInstance();
        void createDebugInstance();
        bool checkValidationLayerSupport();
        std::vector<const char *> getRequiredExtensions();
        void setupDebugMessenger();
        void dbgMessengerConstructor(VkDebugUtilsMessengerCreateInfoEXT &v);
        void pickDevice();
        bool isDeviceSupported(VkPhysicalDevice device);
        bool isDeviceSuitable(VkPhysicalDevice device);
        void mainLoop();
        void cleanUp();
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);
        int rateDeviceSuitability(VkPhysicalDevice device);
        void pickPhysicalDevice();
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);

        GLFWwindow *m_window = nullptr;
        int m_height, m_width;
        VkInstance m_instance;
        VkDebugUtilsMessengerEXT m_dbgmessenger;

        const std::vector<const char *> m_validationLayers = {"VK_LAYER_KHRONOS_validation"};
#ifdef NDEBUG
        const bool m_enableValidationLayers = true;
#else
        const bool m_enableValidationLayers = false;
#endif
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT msgType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT *pCallBacksData,
                                                            void *pUserData);
};
