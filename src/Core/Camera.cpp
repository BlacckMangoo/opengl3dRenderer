#include "../../include/App/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
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
    glfwSetWindowUserPointer(window, camera);
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));

        if (cam->firstMouse) {
            cam->lastX = static_cast<float>(xpos);
            cam->lastY = static_cast<float>(ypos);
            cam->firstMouse = false;
        }

        const float x_offset = static_cast<float>(xpos) - cam->lastX;
        const float y_offset = cam->lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

        cam->lastX = static_cast<float>(xpos);
        cam->lastY = static_cast<float>(ypos);

        if (cam->isRotating) {
            cam->ProcessMouseMovement(x_offset, y_offset);
        } else if (cam->isPanning) {
            cam->ProcessMousePan(x_offset, y_offset);
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow* wind, double xOffset, const double yOffset) {
        auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(wind));
        cam->ProcessMouseScroll(static_cast<float>(yOffset));
    });


}

void Camera::ProcessMouseMovement(float xOffset, float yOffset) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // Constrain pitch
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    UpdateCameraVectors();
}

void Camera::ProcessMousePan(float xOffset, float yOffset) {
    xOffset *= panSensitivity;
    yOffset *= panSensitivity;

    position -= right * xOffset;
    position += up * yOffset;
}

void Camera::ProcessMouseScroll(const float yoffset) {
    fov -= yoffset * scrollSensitivity;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

glm::mat4 Camera::GetProjectionMatrix(const float aspectRatio) const {
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