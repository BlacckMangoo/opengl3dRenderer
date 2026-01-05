#include "Renderer/Window.h"
#include <iostream>


Window::~Window() {
    std::cout << "Window destructor called!" << std::endl;
    glfwDestroyWindow(m_window);
    glfwTerminate();
    std::cout << "Window destructor finished!" << std::endl;
}


void  Window::GLFWInit() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl ;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::CreateWindow(const std::string_view  title) {
    m_window = glfwCreateWindow( m_display_width, m_display_height, title.data(), nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync

    // Set key callback to handle ESC key
    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    });

    // Set window close callback
    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        std::cout << "Window close requested!" << std::endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    });

    // Initialize GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();

    }


}


