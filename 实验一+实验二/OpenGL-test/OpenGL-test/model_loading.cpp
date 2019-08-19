#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

//#include "filesystem.h"
#include "shader_m.h"
#include "camera.h"
#include "model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float x = 0.0f;
float y = 8.0f; // 8.0 1.0 0.7 2.0
float z = 0.0f;

float scale = 0.15f; // 0.2 0.3 0.3 0.3

float pos_x = 0.0f;
float pos_y = 0.0f;
float pos_z = 0.0f;
float angle_x = 0.0f;
float angle_y = 0.0f;
float angle_z = 0.0f;

float scale_old = 0.0f;
float pos_x_old = 0.0f;
float pos_y_old = 0.0f;
float pos_z_old = 0.0f;
float angle_x_old = 0.0f;
float angle_y_old = 0.0f;
float angle_z_old = 0.0f;

bool left_button_state = false;
bool right_button_state = false;
bool middle_button_state = false;
bool ctrl_state = false;
bool space_state = false;
const int space_num = 500;
float p = 0.0;
bool debug = true;
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//, GLFW_CURSOR_DISABLED

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");
	Shader lightingShader("2.1.basic_lighting.vs", "2.1.basic_lighting.fs");
	Shader lampShader("2.1.lamp.vs", "2.1.lamp.fs");
	glm::vec3 lightPos(0.0f, 2.0f, 2.0f);

    // load models
    // -----------
    Model ourModel("../nanosuit/nanosuit.obj");
	//Model ourModel("E:/data/VS/planet/planet.obj");
	//Model ourModel("E:/data/VS/rock/rock.obj");
	//Model ourModel("E:/data/VS/cyborg/cyborg.obj");
	//Model ourModel("E:/data/VS/cube/cube2.obj");
	//Model ourModel("E:/data/VS/dolphins/dolphins.obj");
	//Model ourModel("E:/data/VS/bunny/bunny.obj");
	//Model ourModel("E:/data/VS/boxwood_plant/boxwood_plant.obj");
	//Model ourModel("E:/data/VS/objHouse/objHouse.obj");
	//Model ourModel("E:/data/VS/bumpy.obj");

	// draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   
	
	/*float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	*/
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader.use();
		//lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f); 
		lightingShader.setVec3("lightColor", 1.0f, 0.0f,0.0f);
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("viewPos", camera.Position);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

        // don't forget to enable shader before setting uniforms
     /*   ourShader.use();

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);*/

		if (ctrl_state)
		{
			glm::mat4 model = glm::mat4(1.0f);
			//glm::tquat<float> q = glm::tquat<float>(glm::normalize(glm::vec3(0,0,1)), glm::normalize(glm::vec3(1,0,0))); //创建一个绕z轴旋转90度的四元数
			glm::tquat<float> q = glm::tquat<float>(glm::radians(glm::vec3(angle_x_old, angle_y_old, angle_z_old)));
			glm::mat4 move = glm::translate(glm::mat4(1.0f), glm::vec3(pos_x_old, pos_y_old, pos_z_old));
			model = move * glm::mat4_cast(q) * model;	//得到一个旋转的模型矩阵
			model = glm::scale(model, glm::vec3(scale_old, scale_old, scale_old));	// it's a bit too big for our scene, so scale it down
			model = glm::translate(model, glm::vec3(-x, -y, -z)); // translate it down so it's at the center of the scene
																  //model = glm::scale(model, glm::vec3(scale,scale,scale));	// it's a bit too big for our scene, so scale it down
			//ourShader.setMat4("model", model);
			//ourModel.Draw(ourShader);
			lightingShader.setMat4("model", model);
			ourModel.Draw(lightingShader);
		}
        // render the loaded model
        model = glm::mat4(1.0f);

		//glm::tquat<float> q = glm::tquat<float>(glm::normalize(glm::vec3(0,0,1)), glm::normalize(glm::vec3(1,0,0))); //创建一个绕z轴旋转90度的四元数
		glm::tquat<float> q = glm::tquat<float>(glm::radians(glm::vec3(angle_x,angle_y,angle_z)));
		glm::mat4 move = glm::translate(glm::mat4(1.0f), glm::vec3(pos_x,pos_y,pos_z));
		model = move * glm::mat4_cast(q) * model;	//得到一个旋转的模型矩阵
		model = glm::scale(model, glm::vec3(scale,scale,scale));	// it's a bit too big for our scene, so scale it down
		model = glm::translate(model, glm::vec3(- x, - y, - z)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(scale,scale,scale));	// it's a bit too big for our scene, so scale it down
		//ourShader.setMat4("model", model);
		//ourModel.Draw(ourShader);
		lightingShader.setMat4("model", model);
		ourModel.Draw(lightingShader);

		if (space_state)
		{
			p += float(1) / space_num;
			float p_x = pos_x_old + p*(pos_x - pos_x_old);
			float p_y = pos_y_old + p*(pos_y - pos_y_old);
			float p_z = pos_z_old + p*(pos_z - pos_z_old);
			glm::tquat<float> q = glm::slerp<float>(glm::tquat<float>(glm::radians(glm::vec3(angle_x_old, angle_y_old, angle_z_old))),
				glm::tquat<float>(glm::radians(glm::vec3(angle_x, angle_y, angle_z))),p);
			cout << q.w << q.x << q.y << q.z << endl;
			float s = scale_old + p*(scale - scale_old);

			glm::mat4 model = glm::mat4(1.0f);
			//glm::tquat<float> q = glm::tquat<float>(glm::normalize(glm::vec3(0,0,1)), glm::normalize(glm::vec3(1,0,0))); //创建一个绕z轴旋转90度的四元数
			glm::mat4 move = glm::translate(glm::mat4(1.0f), glm::vec3(p_x,p_y,p_z));
			model = move * glm::mat4_cast(q) * model;	//得到一个旋转的模型矩阵
			model = glm::scale(model, glm::vec3(s, s, s));	// it's a bit too big for our scene, so scale it down
			model = glm::translate(model, glm::vec3(-x, -y, -z)); // translate it down so it's at the center of the scene
																  //model = glm::scale(model, glm::vec3(scale,scale,scale));	// it's a bit too big for our scene, so scale it down
																  //ourShader.setMat4("model", model);
																  //ourModel.Draw(ourShader);
			lightingShader.setMat4("model", model);
			ourModel.Draw(lightingShader);
			if (p >= 1) { space_state = false; p = 0; }
		}
/*		glBindVertexArray(cubeVAO);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lampShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
/*
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
		*/
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pos_y += SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pos_y -= SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pos_x -= SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pos_x += SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
		pos_z -= SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		pos_z += SPEED * deltaTime;
	
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		ctrl_state = true;
		pos_x_old = pos_x;
		pos_y_old = pos_y;
		pos_z_old = pos_z;
		angle_x_old = angle_x;
		angle_y_old = angle_y;
		angle_z_old = angle_z;
		scale_old = scale;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
	{
		ctrl_state = false;
		pos_x_old = pos_x;
		pos_y_old = pos_y;
		pos_z_old = pos_z;
		angle_x_old = angle_x;
		angle_y_old = angle_y;
		angle_z_old = angle_z;
		scale_old = scale;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (ctrl_state == true && space_state==false)
		{
			space_state = true;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (debug)
		{
			std::cout << "Debug" << std::endl;
			std::cout << "Pos " << pos_x << " " << pos_y << " " << pos_z << std::endl;
			std::cout << "Angle " << angle_x << " " << angle_y << " " << angle_z << std::endl;
			std::cout << "Scale " << scale << std::endl;

			debug = false;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
	{
		debug = true;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
	SCR_HEIGHT = height;
	SCR_WIDTH = width;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

	if (left_button_state)
	{
		xoffset *= 0.1f;
		yoffset *= 0.1f;
		angle_x -= yoffset;
		angle_y += xoffset;
	}
	else if (right_button_state)
	{
		angle_z -= xoffset * 0.1f;
	}
    //camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		left_button_state = true;
		//cout << "Mosue left button clicked!" << endl;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		middle_button_state = true;
		pos_x = 0.0;
		pos_y = 0.0;
		pos_z = 0.0;
		angle_x = 0.0;
		angle_y = 0.0;
		angle_z = 0.0;
		//cout << "Mosue middle button clicked!" << endl;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		right_button_state = true;
		//cout << "Mosue right button clicked!" << endl;
		break;
	default:
		return;
	}else
		if (action == GLFW_RELEASE) switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			left_button_state = false;
			//cout << "Mosue left button released!" << endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			middle_button_state = false;
			//cout << "Mosue middle button released!" << endl;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			right_button_state = false;
			//cout << "Mosue right button released!" << endl;
			break;
		default:
			return;
	}
	return;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	scale += yoffset*0.07*scale;
	if (scale <= 0)
		scale = 0;
    //camera.ProcessMouseScroll(yoffset);
}
