#include "vkapp.h"

int main(){
    glm::mat4 matrix;
    glm::vec4 vec;
    glm::vec4 t = matrix*vec;

    vkapp app(800,600);

    return 0;
}
