#pragma once
#include <cstdlib>
#include <iosfwd>
#include <vector>
#include "glm/vec3.hpp"
#include <fstream>
#include <thread>
#include <atomic>


inline void PlotPoints(const std::vector<glm::vec2>& points) {
    static std::atomic<bool> hasPlotted{false};
    if (hasPlotted.exchange(true)) return; // Only plot once per session

    const auto dataPath = "C:/Users/lenovo/CLionProjects/XEngine/data.csv";
    std::ofstream out(dataPath);
    for (const auto point : points) {
        out << point.x << " " << point.y << "\n";
    }
    // Run plotting in a separate thread to avoid blocking
    std::thread([](){
        std::system(R"(C:\Users\lenovo\CLionProjects\XEngine\.venv\Scripts\python.exe C:\Users\lenovo\CLionProjects\XEngine\scripts\plot.py C:\Users\lenovo\CLionProjects\XEngine\data.csv)");
    }).detach();
}

inline void RenderMatrix4AsLatex(const glm::mat4& matrix) {
    static std::atomic<int> matrixId{0};
    int id = matrixId++;
    std::string dataPath = "C:/Users/lenovo/CLionProjects/XEngine/matrix_" + std::to_string(id) + ".csv";
    std::ofstream out(dataPath);
    out << "# MatrixID: " << id << "\n";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            out << matrix[i][j];
            if (j < 3) out << " ";
        }
        out << "\n";
    }
    std::thread([dataPath]() {
        std::string cmd = R"(C:\Users\lenovo\CLionProjects\XEngine\.venv\Scripts\python.exe C:\Users\lenovo\CLionProjects\XEngine\scripts\matrixLatex.py )" + dataPath;
        std::system(cmd.c_str());
    }).detach();
}

inline void RenderMatrix3AsLatex(const glm::mat3& matrix) {
    static std::atomic<int> matrixId{0};
    int id = matrixId++;
    std::string dataPath = "C:/Users/lenovo/CLionProjects/XEngine/matrix3_" + std::to_string(id) + ".csv";
    std::ofstream out(dataPath);
    out << "# MatrixID: " << id << "\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            out << matrix[i][j];
            if (j < 2) out << " ";
        }
        out << "\n";
    }
    std::thread([dataPath]() {
        std::string cmd = R"(C:\Users\lenovo\CLionProjects\XEngine\.venv\Scripts\python.exe C:\Users\lenovo\CLionProjects\XEngine\scripts\matrixLatex.py )" + dataPath;
        std::system(cmd.c_str());
    }).detach();
}
