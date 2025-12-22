#pragma once
#include <imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <Core/Camera.h>

#include "SceneGraph .h"

inline void ImguiInit(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    const ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::GetIO().FontGlobalScale = 1.5f;
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ScrollbarRounding = 0;

}

inline void ImGuiBeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

inline void ImGuiEndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

inline void CameraPropertiesWindow( Camera& camera) {
    ImGui::Begin("Camera Properties");

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
        ImGui::DragFloat3("Front", &camera.front.x, 0.1f);
        ImGui::DragFloat3("Up", &camera.up.x, 0.1f);
    }

    if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragFloat("FOV", &camera.fov, 0.1f, 1.0f, 179.0f);
        ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.01f, 1000.0f);
        ImGui::DragFloat("Far Plane", &camera.farPlane, 1.0f, 0.1f, 10000.0f);
        ImGui::DragFloat("Sensitivity", &camera.mouseSensitivity, 0.01f, 0.001f, 10.0f);
        ImGui::DragFloat("Speed", &camera.movementSpeed, 0.1f, 0.1f, 100.0f);
    }

    ImGui::End();
}

// Transform Window for ImGui
inline void TransformWindow(Transform& transform) {

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
        ImGui::DragFloat3("Rotation", &transform.rotation.x, 0.1f);
        ImGui::DragFloat3("Scale", &transform.scale.x, 0.1f);
    }

}

inline void MaterialPropsWindow(Material& material) {

    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::ColorEdit4("Base Color Factor", &material.baseColorFactor.x);
        ImGui::DragFloat("Metallic Factor", &material.metallicFactor, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Roughness Factor", &material.roughnessFactor, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("AO Factor", &material.aoFactor, 0.01f, 0.0f, 1.0f);
        ImGui::ColorEdit3("Emissive Factor", &material.emissiveFactor.x);
    }

}

// Scene graph related functions

inline void SceneGraphWindow(const Scene& scene) {
    ImGui::Begin("Scene Graph");
    ImGui::Text("Scene Objects");
    ImGui::Separator();

    for (const auto& object : scene.objects) {
        std::string objName = "Object " + std::to_string(object.ID);
        if (const bool isSelected = (scene.editor->selectedObjectIndex == object.ID); ImGui::Selectable(objName.c_str(), isSelected)) {
           scene.editor->SetSelectedObjectIndex(object.ID);
        }
    }
    ImGui::End();

}

