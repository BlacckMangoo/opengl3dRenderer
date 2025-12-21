#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera();

    void ProcessInput(GLFWwindow* window, float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset);
    void ProcessMousePan(float xOffset, float yOffset);
    void ProcessMouseScroll(float yoffset);

    static void SetupMouseCallbacks(GLFWwindow* window, Camera* camera); // call once

    // Get matrices
    [[nodiscard]] glm::mat4 GetProjectionMatrix(float aspectRatio) const;
    [[nodiscard]] glm::mat4 GetViewMatrix() const;

    // Getters
     [[nodiscard]] glm::vec3 GetPosition() const { return position; }
     [[nodiscard]] glm::vec3 GetFront() const { return front; }
     [[nodiscard]] glm::vec3 GetUp() const { return up; }

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right{};
    glm::vec3 worldUp;

    // Camera settings
    float fov;
    float nearPlane;
    float farPlane;
    float movementSpeed;
    float rotationSpeed;
    float mouseSensitivity;
    float panSensitivity;
    float scrollSensitivity;


private:
    bool firstMouse = true;
    float lastX = 400.0f;
    float lastY = 300.0f;
    bool isRotating = false;
    bool isPanning = false;


    // Euler angles
    float yaw;
    float pitch;
    float roll;




    void UpdateCameraVectors();
};