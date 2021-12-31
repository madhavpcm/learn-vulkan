#include "vkapp.h"
vkapp::vkapp(int width, int height)
:m_width(width),m_height(height){

    initWindow();
    initVulkan();
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
    createInstance();
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

    uint32_t glfwExtCount =0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtCount);
    icInfo.enabledExtensionCount = glfwExtCount;
    icInfo.ppEnabledExtensionNames = glfwExtensions;

    if (vkCreateInstance(&icInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    };

    if(m_enableValidationLayers){
        icInfo.enabledLayerCount = static_cast<uint32_t> (m_validationLayers.size());
        icInfo.ppEnabledLayerNames = m_validationLayers.data();
    }else{
        icInfo.enabledLayerCount =0 ;
    }
}

