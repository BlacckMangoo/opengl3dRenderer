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
     [[nodiscard]] glm::vec3 GetPosition() const { return position; }
     [[nodiscard]] glm::vec3 GetFront() const { return front; }
     [[nodiscard]] glm::vec3 GetUp() const { return up; }

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Camera settings
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float movementSpeed  = 5.0f;
    float rotationSpeed = 0.1f;
    float mouseSensitivity = 0.1f;
    float panSensitivity = 0.1f;
    float scrollSensitivity = 1.0f;


private:
    bool firstMouse = true;
    float lastX = 400.0f;
    float lastY = 300.0f;
    bool isRotating = false;
    bool isPanning = false;


    // Euler angles
    float yaw = -90.0f;
    float pitch = 0.0f;
    float roll = 0.0f;




    void UpdateCameraVectors();
};