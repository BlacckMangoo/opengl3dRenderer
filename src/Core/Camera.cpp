#include "Core/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
    : position(0.0f, 0.0f, 10.0f)
    , front(0.0f, 0.0f, -1.0f)
    , up(0.0f, 1.0f, 0.0f)
    , worldUp(0.0f, 1.0f, 0.0f)
    , yaw(-90.0f)
    , pitch(0.0f)
    , roll(0.0f)
    , fov(45.0f)
    , nearPlane(0.1f)
    , farPlane(1000.0f)
    , movementSpeed(2.5f)
    , rotationSpeed(45.0f)
    , mouseSensitivity(0.1f)
    , panSensitivity(0.003f)
    , scrollSensitivity(2.0f)
{
    UpdateCameraVectors();
}

void Camera::ProcessInput(GLFWwindow* window, float deltaTime) {
    const float velocity = movementSpeed * deltaTime;

    // WASD for movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += right * velocity;

    // Check mouse button states
    isRotating = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    isPanning = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
}

void Camera::SetupMouseCallbacks(GLFWwindow* window, Camera* camera) {
    // Store camera pointer in window user pointer for callback access
    glfwSetWindowUserPointer(window, camera);

    // Cursor position callback for camera rotation and panning
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

        if (camera->firstMouse) {
            camera->lastX = static_cast<float>(xpos);
            camera->lastY = static_cast<float>(ypos);
            camera->firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos) - camera->lastX;
        float yoffset = camera->lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

        camera->lastX = static_cast<float>(xpos);
        camera->lastY = static_cast<float>(ypos);

        if (camera->isRotating) {
            camera->ProcessMouseMovement(xoffset, yoffset);
        } else if (camera->isPanning) {
            camera->ProcessMousePan(xoffset, yoffset);
        }
    });

    // Scroll callback for zoom
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        camera->ProcessMouseScroll(static_cast<float>(yoffset));
    });
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    UpdateCameraVectors();
}

void Camera::ProcessMousePan(float xoffset, float yoffset) {
    xoffset *= panSensitivity;
    yoffset *= panSensitivity;

    position -= right * xoffset;
    position += up * yoffset;
}

void Camera::ProcessMouseScroll(float yoffset) {
    fov -= yoffset * scrollSensitivity;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::UpdateCameraVectors() {
    // Calculate the new front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // Recalculate the right and up vectors
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}