#define GLFW_INCLUDE_NONE
#define GLM_ENABLE_EXPERIMENTAL
#include <renderer.h>
#include <camera.h>
#include <openglDebug.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::init(GLFWwindow* window)
{
	this->window = window;

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEPTH_TEST);
	glDebugMessageCallback(glDebugOutput, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	int w, h;

	glfwGetFramebufferSize(window, &w, &h);
	picker.initialise(w, h);

}

void Renderer::init_imgui(GLFWwindow* window, ImGuiIO& output)
{

	const char* glsl_version = "#version 130";

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	output = ImGui::GetIO(); (void)output;

	output.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	output.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	output.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (output.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


}

void Renderer::render(Scene& scene)
{	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
	glClearColor(0.1f, 0.3f, 0.7f, 1.0f);

	GLint stencilMask;
	glGetIntegerv(GL_STENCIL_WRITEMASK, &stencilMask);

	if (stencilMask == 0xFF)
	{
		glClear(GL_STENCIL_BUFFER_BIT);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->input(window);

	// picking phase

	picker.enable_writing();

	glClear(GL_COLOR_BUFFER_BIT);

	scene.picking_phase_render_scene();

	picker.disable_writing();

	// render normally 

	double x, y;
	glfwGetCursorPos(window, &x, &y);

	camera->pixelInfo = picker.read_pixel(x, height - y - 1);

	glStencilMask(0x00);
	scene.render_scene();


}

void Renderer::render_ui(ImGuiIO& io)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	/*if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);*/

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	//{
	//	static float f = 0.0f;
	//	static int counter = 0;

	//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//	ImGui::Checkbox("Another Window", &show_another_window);

	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//		counter++;
	//	ImGui::SameLine();
	//	ImGui::Text("counter = %d", counter);

	//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	//	ImGui::End();
	//}


	{
		//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Each frame:
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, ImGui::GetIO().DisplaySize.y - 100), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Always);

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoResize;        // Disable resizing
		window_flags |= ImGuiWindowFlags_NoMove;          // Disable manual moving
		window_flags |= ImGuiWindowFlags_NoCollapse;      // Disable collapsing
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		window_flags |= ImGuiWindowFlags_NoNavFocus;

		// Optional: remove title bar or decorations
		// window_flags |= ImGuiWindowFlags_NoTitleBar;

		ImGui::Begin("End Turn", nullptr, window_flags);
		if (ImGui::Button("Next Turn", ImVec2(280,35)))
		{
			
		}
		ImGui::End();
	}

}

void Renderer::set_camera(Camera* camera)
{
	this->camera = camera;
}
