#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct  Transform {

    Transform() {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
    };

    explicit Transform(const glm::vec3 pos ,
               const glm::vec3 rot   ,
               const glm::vec3 scl  )
        : position(pos) , rotation(rot) , scale(scl) {
    }

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation  = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale   = glm::vec3(1.0f, 1.0f, 1.0f);


   [[nodiscard]] glm::mat4 GetTransformMatrix() const {
       glm::mat4 m(1.0f);
       m = glm::translate(m, position);
       m = glm::rotate(m, rotation.x, glm::vec3(1,0,0));
       m = glm::rotate(m, rotation.y, glm::vec3(0,1,0));
       m = glm::rotate(m, rotation.z, glm::vec3(0,0,1));
       m = glm::scale(m, scale);
       return m;
   }

    [[nodiscard]] Transform GetTransform() const {
       return Transform(position, rotation, scale);
    }

     void SetPosition(const glm::vec3 &pos) {
       position = pos;
   }

    void SetRotation(const glm::vec3 &rot) {
       rotation = rot;
   } ;

    void SetScale(const glm::vec3 &scl) {
       scale = scl;
   };


};