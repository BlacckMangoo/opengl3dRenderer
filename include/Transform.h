#pragma once
#include <glm/glm.hpp>

struct  Transform {

    explicit Transform(const glm::vec3 pos  = glm::vec3(0.0f,0.0f,0.0f) ,
               const glm::vec3 rot  = glm::vec3(0.0f,0.0f,0.0f) ,
               const glm::vec3 scl  = glm::vec3(1.0f,1.0f,1.0f) )
        : position(pos) , rotation(rot) , scale(scl) {

    }

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation  = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale   = glm::vec3(1.0f, 1.0f, 1.0f);


   [[nodiscard]] glm::mat4 GetTransform() const {
       glm::mat4 m(1.0f);
       m = glm::translate(m, position);
       m = glm::rotate(m, rotation.x, glm::vec3(1,0,0));
       m = glm::rotate(m, rotation.y, glm::vec3(0,1,0));
       m = glm::rotate(m, rotation.z, glm::vec3(0,0,1));
       m = glm::scale(m, scale);
       return m;
   }
};