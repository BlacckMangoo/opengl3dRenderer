#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera();

    void ProcessInput(GLFWwindow* window, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessMousePan(float xoffset, float yoffset);
    void ProcessMouseScroll(float yoffset);

    // Set up mouse callbacks - call this once during initialization
    static void SetupMouseCallbacks(GLFWwindow* window, Camera* camera);

    // Get matrices
    [[nodiscard]] glm::mat4 GetProjectionMatrix(float aspectRatio) const;
    [[nodiscard]] glm::mat4 GetViewMatrix() const;

    // Getters
     [[nodiscard]] glm::vec3 GetPosition() const { return position; }
     [[nodiscard]] glm::vec3 GetFront() const { return front; }
     [[nodiscard]] glm::vec3 GetUp() const { return up; }

    // Mouse state
    bool firstMouse = true;
    float lastX = 400.0f;
    float lastY = 300.0f;
    bool isRotating = false;
    bool isPanning = false;

private:
    // Camera properties
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right{};
    glm::vec3 worldUp;

    // Euler angles
    float yaw;
    float pitch;
    float roll;

    // Camera settings
    float fov;
    float nearPlane;
    float farPlane;
    float movementSpeed;
    float rotationSpeed;
    float mouseSensitivity;
    float panSensitivity;
    float scrollSensitivity;


    void UpdateCameraVectors();
};