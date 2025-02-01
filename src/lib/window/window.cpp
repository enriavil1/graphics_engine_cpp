
#include "imgui.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <unistd.h>

#include "../../include/Engine3D/Engine3D.hpp"
#include "../../include/window/window.hpp"

bool Window::m_is_running = false;

ImVec4 Window::m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

GLFWwindow *Window::m_window;

int Window::m_width = 1280;
int Window::m_height = 720;

void Window::set_window_width(int width) { Window::m_width = width; }
void Window::set_window_height(int height) { Window::m_height = height; }
void Window::set_window_clear_color(const ImVec4 &clear_color) {
  Window::m_clear_color = clear_color;
}

void Window::glfw_sleep(int milliseconds) { usleep(milliseconds * 1000); }
void Window::glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW failed with error %d: %s\n", error, description);
}

bool Window::initialize(const char *window_title) {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    return false;
  }

  // Mac specific versioning
  const char *glsl_version = "#version 150";

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Required on Mac
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create window with graphics context
  Window::m_window = glfwCreateWindow(Window::m_width, Window::m_height,
                                      window_title, nullptr, nullptr);

  if (Window::m_window == nullptr) {
    std::cerr << "Failed to initialize window" << std::endl;
    return false;
  }

  glfwMakeContextCurrent(Window::m_window);
  // Enable vsync
  glfwSwapInterval(1);

  // set up dear imgui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();

  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(Window::m_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  return true;
}

void Window::process_frame() {}

void Window::process_events() {}

void Window::run() {
  auto show_another_window = false;
  auto &io = ImGui::GetIO();

  engine3D::Engine engine;

  double theta = 0.0;

  while (!glfwWindowShouldClose(m_window)) {
    glfwPollEvents();
    if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0) {
      glfw_sleep(10);
      continue;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();

    {
      ImGui::Begin("Stats");

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    {
      ImGui::Begin("Draw Port");
      auto dt = 1.0f / (io.Framerate * 1.0f);
      theta += 1 * dt;

      engine.project(theta);

      ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(m_clear_color.x * m_clear_color.w,
                 m_clear_color.y * m_clear_color.w,
                 m_clear_color.z * m_clear_color.w, m_clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(m_window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(m_window);
  glfwTerminate();
}
