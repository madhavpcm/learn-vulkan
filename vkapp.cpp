#include "vkapp.h"
vkapp::vkapp(int width, int height)
:m_width(width),m_height(height){

    initWindow();
    initVulkan();
    setupDebugMessenger();
}
vkapp::~vkapp(){
    cleanUp();
}
void vkapp::run(){

}
void vkapp::mainLoop(){
    glfwPollEvents();
}

void vkapp::cleanUp(){
    if(m_enableValidationLayers){
        DestroyDebugUtilsMessengerEXT(m_instance,m_dbgmessenger,nullptr);
    }
    vkDestroyInstance(m_instance,nullptr);

    glfwDestroyWindow(m_window);
    glfwTerminate();

}
bool vkapp::checkValidationLayerSupport(){
    uint32_t layerCount=0;
    vkEnumerateInstanceLayerProperties(&layerCount,nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount,availableLayers.data());

    for(const char* layer: m_validationLayers){
        bool flag= false;
        for(const auto& layerProperties: availableLayers){
            if(!strcmp(layer, layerProperties.layerName)){
                flag= true;
                break;
            }
        }
        if(!flag)
            return false;
    }
    return true;
}
void vkapp::initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
    GLFWwindow* m_window = glfwCreateWindow(m_width,m_height,"Vulkan", nullptr,nullptr);

    uint32_t extCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
    std::cout << extCount << " extensions supported.\n";
}
void vkapp::initVulkan(){
#ifdef NDEBUG
    createDebugInstance();
#else
    createInstance();
#endif
}
void vkapp::createInstance(){
    if(m_enableValidationLayers && !checkValidationLayerSupport()){
        throw std::runtime_error("No/ some of the requested validation layers are available");
    }
    
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VulkanTutorial";
    appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.pEngineName = "NIL";
    appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo icInfo{}; 
    icInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    icInfo.pApplicationInfo = &appInfo;

    if(m_enableValidationLayers){
        icInfo.enabledLayerCount = static_cast<uint32_t> (m_validationLayers.size());
        icInfo.ppEnabledLayerNames = m_validationLayers.data();
    }else{
        icInfo.enabledLayerCount =0 ;
    }

    std::vector<const char*> glfwExtensions = getRequiredExtensions();
    icInfo.enabledExtensionCount = static_cast<uint32_t> (glfwExtensions.size());
    icInfo.ppEnabledExtensionNames = glfwExtensions.data();

    if (vkCreateInstance(&icInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    };

}
void vkapp::createDebugInstance(){
    if(m_enableValidationLayers && !checkValidationLayerSupport()){
        throw std::runtime_error("No/ some of the requested validation layers are available");
    }
    
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VulkanTutorial";
    appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.pEngineName = "NIL";
    appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo icInfo{}; 
    icInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    icInfo.pApplicationInfo = &appInfo;

    VkDebugUtilsMessengerCreateInfoEXT dbgInfo{};
    if(m_enableValidationLayers){
        icInfo.enabledLayerCount = static_cast<uint32_t> (m_validationLayers.size());
        icInfo.ppEnabledLayerNames = m_validationLayers.data();

        dbgMessengerConstructor(dbgInfo);
        icInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&dbgInfo;
    }else{
        icInfo.enabledLayerCount =0 ;
    }

    std::vector<const char*> glfwExtensions = getRequiredExtensions();
    icInfo.enabledExtensionCount = static_cast<uint32_t> (glfwExtensions.size());
    icInfo.ppEnabledExtensionNames = glfwExtensions.data();

    if (vkCreateInstance(&icInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    };

}
std::vector<const char*> vkapp::getRequiredExtensions(){
    uint32_t glfwExtCount =0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtCount);
    std::vector<const char*> vecGlfwExtensions(glfwExtensions, glfwExtensions+glfwExtCount);
    if(m_enableValidationLayers){
        vecGlfwExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return vecGlfwExtensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vkapp::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT msgType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT *pCallBacksData,
                                                    void *pUserData){
    int x;                                                        
    std::cerr<< "validation layer :" << pCallBacksData->pMessage<< std::endl;
    return VK_FALSE;
}
void vkapp::setupDebugMessenger(){
    if(!m_enableValidationLayers)
        return;

    VkDebugUtilsMessengerCreateInfoEXT msgInfo{};

    dbgMessengerConstructor(msgInfo);
    if (CreateDebugUtilsMessengerEXT(m_instance, &msgInfo, nullptr, &m_dbgmessenger) != VK_SUCCESS) {
        throw std::runtime_error("Debug Messenger Could not be created");
    }
}
void vkapp::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}
VkResult vkapp::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger){
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
void vkapp::dbgMessengerConstructor(VkDebugUtilsMessengerCreateInfoEXT& v){
    v.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    v.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    v.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    v.pfnUserCallback = debugCallback;
    v.pUserData = nullptr; //optionally send classes for debugl
}

void vkapp::pickDevice(){
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

    if(!deviceCount){
        throw std::runtime_error("No Vulkan GPU found");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    for(const VkPhysicalDevice& device : devices){
        if(isDeviceSupported(device)){
            physicalDevice = device;
            break;
        }
    }
    if(physicalDevice == VK_NULL_HANDLE){
        throw std::runtime_error("No suitable gpu found!");
    }


}
bool vkapp::isDeviceSupported(VkPhysicalDevice device){
    return true;
}