
#pragma once
#include <string>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


class Window {
public:

    Window(int const width , int const height , std::string_view const  m_title ) : m_display_width(width), m_display_height(height) , m_title(m_title)
    {
        GLFWInit();
        CreateWindow(m_title);
    };

    ~Window();
    static void  GLFWInit();
    void CreateWindow( std::string_view title);

    [[nodiscard]] bool isOpen() const                      { return !glfwWindowShouldClose(m_window); }
    [[nodiscard]] GLFWwindow*  get_GLFW_Window() const     { return m_window; }
    [[nodiscard]] int getWidth() const                     { return m_display_width; }
    [[nodiscard]] int getHeight() const                    { return m_display_height; }
    [[nodiscard]] float getAspectRatio() const             { return static_cast<float>(m_display_width) / static_cast<float>(m_display_height); }


private:
    int const  m_display_width ;
    int const  m_display_height ;
    std::string_view const  m_title ;
    GLFWwindow* m_window{};
};


