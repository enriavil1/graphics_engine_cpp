#include "GLFW/glfw3.h"
#include "imgui.h"

#include <OpenGL/gl.h>
#include <cstdio>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <memory>
#include <ostream>
#include <unistd.h>
#include <vector>

#include "../../include/window/drawPort.hpp"
#include "../../include/window/fileDialog.hpp"
#include "../../include/window/mainWindow.hpp"
#include "../../include/window/statsPort.hpp"

GLFWwindow *MainWindow::mp_window = nullptr;
auto MainWindow::m_theta = 0.0;

const auto MainWindow::mp_width = 1280;
const auto MainWindow::mp_height = 720;
const auto MainWindow::mp_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void MainWindow::glfw_sleep(int milliseconds) { usleep(milliseconds * 1000); }
void MainWindow::glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW failed with error %d: %s\n", error, description);
}

GLFWwindow *MainWindow::getWindow() { return MainWindow::mp_window; }

bool MainWindow::initialize() {
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
  MainWindow::mp_window =
      glfwCreateWindow(MainWindow::mp_width, MainWindow::mp_height,
                       "Main Window", nullptr, nullptr);

  if (MainWindow::mp_window == nullptr) {
    std::cerr << "Failed to initialize window" << std::endl;
    return false;
  }

  glfwMakeContextCurrent(MainWindow::mp_window);
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
  ImGui_ImplGlfw_InitForOpenGL(MainWindow::mp_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  glEnable(GL_CULL_FACE);

  return true;
}

void MainWindow::process_frame() {}

void MainWindow::process_events() {}

void MainWindow::run() {
  auto &io = ImGui::GetIO();

  auto stats_port = std::make_shared<StatsPort>("Stats");
  auto draw_port = std::make_shared<DrawPort>("Draw Port");

  std::vector<std::shared_ptr<Window>> windows = {draw_port, stats_port};

  while (!glfwWindowShouldClose(mp_window)) {
    glfwPollEvents();
    if (glfwGetWindowAttrib(mp_window, GLFW_ICONIFIED) != 0) {
      glfw_sleep(10);
      continue;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();

    if (ImGui::BeginMainMenuBar()) {

      if (ImGui::IsKeyDown(ImGuiKey_ModCtrl) && ImGui::IsKeyDown(ImGuiKey_O)) {
        FileDialog::m_selecting_files = true;
      }

      if (ImGui::BeginMenu("File")) {

        ImGui::MenuItem("Open...", "(Ctr/Cmd)+O",
                        &FileDialog::m_selecting_files);
        ImGui::EndMenu();
      }

      for (auto &window : windows) {
        window->initialize();
      }

      ImGui::EndMainMenuBar();
    }

    for (auto &window : windows) {
      if (window->get_is_running())
        window->run();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(mp_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(mp_clear_color.x * mp_clear_color.w,
                 mp_clear_color.y * mp_clear_color.w,
                 mp_clear_color.z * mp_clear_color.w, mp_clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(mp_window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(mp_window);
  glfwTerminate();
}
