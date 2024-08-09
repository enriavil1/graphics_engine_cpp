#include <cstdio>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <unistd.h>

static void glfw_sleep(int milliseconds) { usleep(milliseconds * 1000); }

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW failed with error %d: %s\n", error, description);
}

int main() {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    return 1;
  }

  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Required on Mac
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create window with graphics context
  GLFWwindow *window = glfwCreateWindow(
      1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);

  if (window == nullptr) {
    std::cerr << "Failed to initialize window" << std::endl;
    return -1;
  }

  glfwMakeContextCurrent(window);
  // Enable vsync
  glfwSwapInterval(1);

  // set up dear imgui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();

  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // our clearing color
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  auto show_another_window = false;

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
      glfw_sleep(10);
      continue;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
      static float f = 0.0f;
      static int counter = 0;

      // Create a window called "Hello, world!" and
      // append into it.
      ImGui::Begin("Hello, world!");

      // Display some text (you can use
      // a format strings too)
      ImGui::Text("This is some useful text.");
      ImGui::Checkbox("Another Window", &show_another_window);

      // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

      // Edit 2 floats representing a color
      ImGui::ColorEdit3("clear color", reinterpret_cast<float *>(&clear_color));

      // Buttons return true when clicked (most
      if (ImGui::Button("Button")) {
        counter++;
      }

      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    if (show_another_window) {
      // Pass a pointer to our bool variable (the
      // window will have a closing button that will
      // clear the bool when clicked)

      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me"))
        show_another_window = false;
      ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
