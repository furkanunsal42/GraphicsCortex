#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) clear_errors();\
	x;\
	ASSERT(log_errors(#x, __LINE__, __FILE__));

bool log_errors(const char* function, int line, const char* file) {
	bool error_safe = true;
	while (unsigned int error = glGetError()) {
		std::string error_name = std::to_string(error);
		switch (error) {
		case 0x0500: error_name = "GL_INVALID_ENUM"; break;
		case 0x0501: error_name = "GL_INVALID_VALUE"; break;
		case 0x0502: error_name = "GL_INVALID_OPERATION"; break;
		case 0x0503: error_name = "GL_STACK_OVERFLOW"; break;
		case 0x0504: error_name = "GL_STACK_UNDERFLOW"; break;
		case 0x0505: error_name = "GL_OUT_OF_MEMORY"; break;
		}
		std::cout << "[OpenGL ERROR] on " << function << " line: " << line << " file: " << file << "\nError:" << error_name << '\n';
		error_safe = false;
	}
	return error_safe;
}

void clear_errors() {
	while (unsigned int error = glGetError()) {
		std::cout << "there was a previous error: " << error << '\n';
	}
}

struct Shader {
	std::string vertex_shader;
	std::string fragment_shader;
};

Shader read_shader(const std::string target_file) {
	Shader shader;
	std::string type = "";
	std::ifstream file(target_file);
	std::string line; 

	while (std::getline(file, line)) {
		if (line.find("#<vertex shader>") != std::string::npos) {
			type = "vertex";
			continue;
		}
		else if (line.find("#<fragment shader>") != std::string::npos) {
			type = "fragment";
			continue;
		}
		if (type == "vertex") {
			shader.vertex_shader += line + '\n';
		}
		else if (type == "fragment") {
			shader.fragment_shader += line + '\n';
		}
	}
	return shader;
}

unsigned int compile_shader(unsigned int type, const std::string shader_source) {
	unsigned int shader = glCreateShader(type);
	const char* shader_code_pointer = &shader_source[0];
	GLCall(glShaderSource(shader, 1, &shader_code_pointer, nullptr));
	
	std::string type_string = std::to_string(type);
	if (type == GL_VERTEX_SHADER)
		type_string = "vertex shader";
	else if (type == GL_FRAGMENT_SHADER)
		type_string = "fragment shader";

	std::cout << "compiling the shader of type " << type_string << '\n';
	
	GLCall(glCompileShader(shader));

	int compile_status;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status));
	if (compile_status != GL_TRUE) {
		char* compile_error_msg = (char*)malloc(1024 * sizeof(char));
		GLCall(glGetShaderInfoLog(shader, 1024, nullptr, compile_error_msg));
		std::cout << "shader compile error with massage: " << compile_error_msg << '\n';
	}
	
	return shader;
}

unsigned int compile_program(const std::string vertex_shader_source, const std::string fragment_shader_source) {
	unsigned int program = glCreateProgram();
	unsigned int vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
	unsigned int fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

	GLCall(glAttachShader(program, vertex_shader));
	GLCall(glAttachShader(program, fragment_shader));
	GLCall(glLinkProgram(program));
	
	GLCall(glDeleteShader(vertex_shader));
	GLCall(glDeleteShader(fragment_shader));

	return program;
}

int main() {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(640, 480, "Opengl_Window", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewInit();
	float verticies[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};
	unsigned int vertex_array;
	GLCall(glGenVertexArrays(1, &vertex_array));
	GLCall(glBindVertexArray(vertex_array));
	
	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), verticies, GL_STATIC_DRAW));
	
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

	unsigned int triangles[] = {
		0, 1, 2,
		0, 3, 2
	};
	unsigned int index_buffer;
	GLCall(glGenBuffers(1, &index_buffer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), triangles, GL_STATIC_DRAW));

	Shader shader_file = read_shader("Shader.shdr");

	unsigned int program = compile_program(shader_file.vertex_shader, shader_file.fragment_shader);
	GLCall(glUseProgram(program));
	
	unsigned int unform_color = glGetUniformLocation(program, "u_color");
	float r = 0.2f;
	float change = 0.005f;
	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();

		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

		r += change;
		if (r > 1){ 
			r = 1;
			change *= -1;
		}
		else if (r < 0) {
			r = 0;
			change *= -1;
		}

		GLCall(glUniform4f(unform_color, r, 0.25f + 0.5f * cos(5*(1.6f + r*6.28f)), 0.25f + 0.5f * sin(5*(r * 6.28f)), 1.0f));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}