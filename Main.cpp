#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include"VBO.h"
#include"EBO.h"
#include"VAO.h"

const int a = 50;
GLfloat* vertices = new GLfloat[(a) * (a) * 6 * 4];
GLuint* indices = new GLuint[(a) * (a) * 6];
bool state[a][a], newstate[a][a];
int num = 0;
void init() {
	float offset = 2.0f / (a);
	float colorOffset = 1.0f / a;
	srand(clock());
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < a; j++) {
			GLfloat curr_color = 1;
			state[i][j] = rand() % 2;
			newstate[i][j] = 0;
			if (state[i][j] == 1) {
				indices[num * 6 + 0] = (i * a + j) * 4;
				indices[num * 6 + 1] = (i * a + j) * 4 + 1;
				indices[num * 6 + 2] = (i * a + j) * 4 + 2;
				indices[num * 6 + 3] = (i * a + j) * 4 + 1;
				indices[num * 6 + 4] = (i * a + j) * 4 + 2;
				indices[num * 6 + 5] = (i * a + j) * 4 + 3;
				num++;
			}
			//LEFT_BOT
			vertices[(i * a + j) * 24 + 0] = -1.0f + i * offset;
			vertices[(i * a + j) * 24 + 1] = -1.0f + j * offset;
			vertices[(i * a + j) * 24 + 2] = 0.0f;
			vertices[(i * a + j) * 24 + 3] = curr_color;
			vertices[(i * a + j) * 24 + 4] = curr_color;
			vertices[(i * a + j) * 24 + 5] = curr_color;
			//RIGHT_BOT
			vertices[(i * a + j) * 24 + 6] = -1.0f + i * offset;
			vertices[(i * a + j) * 24 + 7] = -1.0f + (j + 1) * offset;
			vertices[(i * a + j) * 24 + 8] = 0.0f;
			vertices[(i * a + j) * 24 + 9] = curr_color;
			vertices[(i * a + j) * 24 + 10] = curr_color;
			vertices[(i * a + j) * 24 + 11] = curr_color;
			//LEFT_TOP
			vertices[(i * a + j) * 24 + 12] = -1.0f + (i + 1) * offset;
			vertices[(i * a + j) * 24 + 13] = -1.0f + j * offset;
			vertices[(i * a + j) * 24 + 14] = 0.0f;
			vertices[(i * a + j) * 24 + 15] = curr_color;
			vertices[(i * a + j) * 24 + 16] = curr_color;
			vertices[(i * a + j) * 24 + 17] = curr_color;
			//RIGHT_TOP
			vertices[(i * a + j) * 24 + 18] = -1.0f + (i + 1) * offset;
			vertices[(i * a + j) * 24 + 19] = -1.0f + (j + 1) * offset;
			vertices[(i * a + j) * 24 + 20] = 0.0f;
			vertices[(i * a + j) * 24 + 21] = curr_color;
			vertices[(i * a + j) * 24 + 22] = curr_color;
			vertices[(i * a + j) * 24 + 23] = curr_color;
		}
	}
}

void update() {

	memset(indices, 0, num * 6);
	num = 0;
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < a; j++) {
			int count = 0;
			newstate[i][j] = 0;
			if (i > 0)count += state[i - 1][j];
			if (i < a - 1)count += state[i + 1][j];
			if (j > 0)count += state[i][j - 1];
			if (j < a - 1)count += state[i][j + 1];
			if (i > 0 && j > 0)count += state[i - 1][j - 1];
			if (i > 0 && j < a - 1)count += state[i - 1][j + 1];
			if (i < a - 1 && j > 0)count += state[i + 1][j - 1];
			if (i < a - 1 && j < a - 1)count += state[i + 1][j + 1];
			if (state[i][j] == 1) {
				if (count <= 1 || count >= 4)newstate[i][j] = 0;
				else newstate[i][j] = 1;
			}
			else {
				if (count == 3) newstate[i][j] = 1;
			}
		}
	}
	std::swap(newstate, state);
	int count = 0;
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < a; j++) {
			if (state[i][j] == 1) {
				indices[num * 6 + 0] = (i * a + j) * 4;
				indices[num * 6 + 1] = (i * a + j) * 4 + 1;
				indices[num * 6 + 2] = (i * a + j) * 4 + 2;
				indices[num * 6 + 3] = (i * a + j) * 4 + 1;
				indices[num * 6 + 4] = (i * a + j) * 4 + 2;
				indices[num * 6 + 5] = (i * a + j) * 4 + 3;
				num++;
			}
		}
	}
}

int main() {
	//Initializing
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creating a window
	GLFWwindow* window = glfwCreateWindow(1600, 1600, "TEST", NULL, NULL);
	if (window == NULL) {
		std::cout << "CANT CREATE A WINDOW\n";
		glfwTerminate();
		return -1;
	}
	//Actually using the window
	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, 1600, 1600);




	//Creating a shader object
	Shader shaderProgram("default.vert", "default.frag");
	shaderProgram.Activate();
	//Creating a vertex array object and binding it

	init();
	VBO VBO1(vertices, sizeof(GLfloat) * (a) * (a) * 24);
	VAO VAO1;
	VAO1.Bind();
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	int prevtime_update = clock(), prevtime_buffupdate = clock(), a1 = 0, a2 = 1;
	int time_buffer = 500;
	bool is_auto = 0;
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		if (clock() - prevtime_update > time_buffer &&(is_auto || glfwGetKey(window,GLFW_KEY_R))) {
			update();
			a1++;
			prevtime_update = clock();
		}
		if (clock() - prevtime_buffupdate > 300)
		{
			if (glfwGetKey(window, (int)'-')) {
				time_buffer -= 50;
				prevtime_buffupdate = clock();
			}
			if (glfwGetKey(window, (int)'=')) {
				time_buffer += 50;
				prevtime_buffupdate = clock();
			}
			if (glfwGetKey(window, (int)'T')) {
				is_auto ^= 1;
				prevtime_buffupdate = clock();
			}
			if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
				VAO1.Delete();
				VBO1.Delete();

				shaderProgram.Delete();
				delete[] vertices;
				delete[] indices;

				glfwDestroyWindow(window);
				glfwTerminate();
				exit(0);
			}
		}
		
		

		//Creating a elements buffer object
		EBO EBO1(indices, sizeof(GLuint) * num * 6);

		
		//VAO1.Bind();
		glDrawElements(GL_TRIANGLES, num * 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		glfwPollEvents();

		EBO1.Delete();
	}
	VAO1.Delete();
	VBO1.Delete();

	shaderProgram.Delete();
	delete[] vertices;
	delete[] indices;

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
