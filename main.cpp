/*
Kelompok 8:
- Muhammad Rafli Syaputra 	2005381
- Nyoman Ari Satyadharma  	2000831
- Wildan Septi Ramadhan 	2009085
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <camera.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

// variabel global
glm::mat4 model         = glm::mat4(1.0f);
glm::vec3 movement      = glm::vec3(0.0f);
glm::vec3 scaling       = glm::vec3(0.0f);
float rotation          = 0.0f;
int current3D = 0, currentObject = 0;

// kamera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// speed translasi
int speed = 1;

// warna background
float redColor = 0.35f;
float greenColor = 0.95f;
float blueColor = 0.94f;

int main(){
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Belajar OpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertex.vs", "fragment.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    // vertex untuk kubus
    float kubus[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // posisi kubus di world space
    glm::vec3 posisiKubus[] = {
        glm::vec3(0.0f, -1.0f, -1.0f),
        glm::vec3(0.0f, -1.0f, -3.0f),
        glm::vec3(0.0f, -1.0f, -5.0f),
        glm::vec3(0.0f, -1.0f, -7.0f),
        glm::vec3(0.0f, -1.0f, -9.0f),
        glm::vec3(0.0f, -1.0f, -11.0f),
        glm::vec3(0.0f, -1.0f, -13.0f),
        glm::vec3(0.0f, -1.0f, -15.0f),
        glm::vec3(0.0f, -1.5f, -30.0f)
    };
    // rotasi kubus
    float rotasiKubus[] = {
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f
    };
    // scaling kubus
    glm::vec3 ukuranKubus[] = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(100.0f, 0.0f, 100.0f)
    };

    // vertex untuk limas segitiga
    float limas[] = {
        //alas
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //kiri
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //kanan
         0.0f, -0.5f, -0.5f,  0.5f, 1.0f, //atas

        //depan
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //kiri
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //kanan
         0.0f,  0.5f,  0.0f,  0.5f, 1.0f, //atas

        //kiri
         0.0f, -0.5f, -0.5f,  0.0f, 0.0f, //kiri
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //kanan
         0.0f,  0.5f,  0.0f,  0.5f, 1.0f, //atas

        //kanan
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //kiri
         0.0f, -0.5f, -0.5f,  1.0f, 0.0f, //kanan
         0.0f,  0.5f,  0.0f,  0.5f, 1.0f //atas
    };
    // posisi limas segitiga di world space
    glm::vec3 posisiLimas[] = {
        glm::vec3(-2.0f, -1.0f, -1.0f),
        glm::vec3(-2.0f, -1.0f, -3.0f),
        glm::vec3(-2.0f, -1.0f, -5.0f),
        glm::vec3(-2.0f, -1.0f, -7.0f),
        glm::vec3(-2.0f, -1.0f, -9.0f),
        glm::vec3(-2.0f, -1.0f, -11.0f),
        glm::vec3(-2.0f, -1.0f, -13.0f),
        glm::vec3(-2.0f, -1.0f, -15.0f)
    };
    // rotasi limas segitiga
    float rotasiLimas[] = {
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f
    };
    // scaling limas segitiga
    glm::vec3 ukuranLimas[] = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };

    // vertex untuk prisma segitiga
    float prisma[] = {
        //alas
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        //depan
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //kiri
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //kanan
         0.0f,  0.5f,  0.5f,  0.5f, 1.0f, //atas

        //belakang
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //kiri
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, //kanan
         0.0f,  0.5f, -0.5f,  0.5f, 1.0f, //atas

        //kiri
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //kiri bawah
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //kanan bawah
         0.0f,  0.5f,  0.5f,  1.0f, 1.0f, //kanan atas
         0.0f,  0.5f,  0.5f,  1.0f, 1.0f, //kanan atas
         0.0f,  0.5f, -0.5f,  0.0f, 1.0f, //kiri atas
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //kiri bawah

        //kanan
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, //kanan bawah
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //kiri bawah
         0.0f,  0.5f,  0.5f,  0.0f, 1.0f, //kiri atas
         0.0f,  0.5f,  0.5f,  0.0f, 1.0f, //kiri atas
         0.0f,  0.5f, -0.5f,  1.0f, 1.0f, //kanan atas
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, //kanan bawah
    };
    // posisi prisma segitiga di world space
    glm::vec3 posisiPrisma[] = {
        glm::vec3(2.0f, -1.0f, -1.0f),
        glm::vec3(2.0f, -1.0f, -3.0f),
        glm::vec3(2.0f, -1.0f, -5.0f),
        glm::vec3(2.0f, -1.0f, -7.0f),
        glm::vec3(2.0f, -1.0f, -9.0f),
        glm::vec3(2.0f, -1.0f, -11.0f),
        glm::vec3(2.0f, -1.0f, -13.0f),
        glm::vec3(2.0f, -1.0f, -15.0f)
    };
    // rotasi prisma segitiga
    float rotasiPrisma[] = {
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f
    };
    // scaling prisma segitiga
    glm::vec3 ukuranPrisma[] = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };

    // vertex untuk piramida
    float piramida[] = {
        //alas
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        //depan
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //kiri
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //kanan
         0.0f,  0.5f,  0.0f,  0.5f, 1.0f, //atas

        //belakang
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //kiri
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, //kanan
         0.0f,  0.5f,  0.0f,  0.5f, 1.0f, //atas

        //kiri
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //kiri
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //kanan
         0.0f,  0.5f,  0.0f,  0.5f, 1.0f, //atas

        //kanan
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //kiri
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, //kanan
         0.0f,  0.5f,  0.0f,  0.5f, 1.0f, //atas
    };
    // posisi piramida di world space
    glm::vec3 posisiPiramida[] = {
        glm::vec3(4.0f, -1.0f, -1.0f),
        glm::vec3(4.0f, -1.0f, -3.0f),
        glm::vec3(4.0f, -1.0f, -5.0f),
        glm::vec3(4.0f, -1.0f, -7.0f),
        glm::vec3(4.0f, -1.0f, -9.0f),
        glm::vec3(4.0f, -1.0f, -11.0f),
        glm::vec3(4.0f, -1.0f, -13.0f),
        glm::vec3(4.0f, -1.0f, -15.0f)
    };
    // rotasi piramida
    float rotasiPiramida[] = {
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f
    };
    // scaling piramida
    glm::vec3 ukuranPiramida[] = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };

    // vertex untuk diamond
    float diamond[] = {
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
         0.0f,  0.5f,  0.0f,    0.5f, 1.0f,

        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
         0.0f,  0.5f,  0.0f,    0.5f, 1.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
         0.0f,  0.5f,  0.0f,    0.5f, 1.0f,

         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
         0.0f,  0.5f,  0.0f,    0.5f, 1.0f,

         -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
         0.5f,  -0.5f, -0.5f,    1.0f, 0.0f,
         0.0f,  -1.5f,  0.0f,    0.5f, 1.0f,

        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
         0.0f, -1.5f,  0.0f,    0.5f, 1.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
         0.0f, -1.5f,  0.0f,    0.5f, 1.0f,

         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
         0.0f, -1.5f,  0.0f,    0.5f, 1.0f
    };
    // posisi piramida di world space
    glm::vec3 posisiDiamond[] = {
        glm::vec3(6.0f, 0.0f, -1.0f),
        glm::vec3(6.0f, 0.0f, -3.0f),
        glm::vec3(6.0f, 0.0f, -5.0f),
        glm::vec3(6.0f, 0.0f, -7.0f),
        glm::vec3(6.0f, 0.0f, -9.0f),
        glm::vec3(6.0f, 0.0f, -11.0f),
        glm::vec3(6.0f, 0.0f, -13.0f),
        glm::vec3(6.0f, 0.0f, -15.0f)
    };
    // rotasi piramida
    float rotasiDiamond[] = {
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f
    };
    // scaling piramida
    glm::vec3 ukuranDiamond[] = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };

    // setup kubus
    unsigned int VBO_kubus, VAO_kubus;
    glGenVertexArrays(1, &VAO_kubus);
    glGenBuffers(1, &VBO_kubus);

    glBindVertexArray(VAO_kubus);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_kubus);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kubus), kubus, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // setup limas segitiga
    unsigned int VBO_limas, VAO_limas;
    glGenVertexArrays(1, &VAO_limas);
    glGenBuffers(1, &VBO_limas);

    glBindVertexArray(VAO_limas);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_limas);
    glBufferData(GL_ARRAY_BUFFER, sizeof(limas), limas, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // setup prisma segitiga
    unsigned int VBO_prisma, VAO_prisma;
    glGenVertexArrays(1, &VAO_prisma);
    glGenBuffers(1, &VBO_prisma);

    glBindVertexArray(VAO_prisma);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_prisma);
    glBufferData(GL_ARRAY_BUFFER, sizeof(prisma), prisma, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // setup prisma segitiga
    unsigned int VBO_piramida, VAO_piramida;
    glGenVertexArrays(1, &VAO_piramida);
    glGenBuffers(1, &VBO_piramida);

    glBindVertexArray(VAO_piramida);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_piramida);
    glBufferData(GL_ARRAY_BUFFER, sizeof(piramida), piramida, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // setup prisma segitiga
    unsigned int VBO_diamond, VAO_diamond;
    glGenVertexArrays(1, &VAO_diamond);
    glGenBuffers(1, &VBO_diamond);

    glBindVertexArray(VAO_diamond);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_diamond);
    glBufferData(GL_ARRAY_BUFFER, sizeof(diamond), diamond, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture
    // -------------------------
    unsigned int texture1, texture2, texture3, texture4, texture5, texture6, texture7, texture8;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("woodMinecraft.png", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("diamondMinecraft.png", &width, &height, &nrChannels, 0);
    if (data){
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 3
    // ---------
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("brickMinecraft.png", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 4
    // ---------
    glGenTextures(1, &texture4);
    glBindTexture(GL_TEXTURE_2D, texture4);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("stoneMinecraft.png", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 5
    // ---------
    glGenTextures(1, &texture5);
    glBindTexture(GL_TEXTURE_2D, texture5);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("grassMinecraft.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 6
    // ---------
    glGenTextures(1, &texture6);
    glBindTexture(GL_TEXTURE_2D, texture6);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("leafMinecraft.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 7
    // ---------
    glGenTextures(1, &texture7);
    glBindTexture(GL_TEXTURE_2D, texture7);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("treeMinecraft.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 8
    // ---------
    glGenTextures(1, &texture8);
    glBindTexture(GL_TEXTURE_2D, texture8);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("goldMinecraft.png", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)){
        // input
        // -----

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glfwSetKeyCallback(window, key_callback);

        // render
        // ------
        glClearColor(redColor, greenColor, blueColor, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

         // bind textures on corresponding texture units

        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        /*
        // create transformations
        glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection    = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // pass transformation matrices to the shader
        ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("view", view);
        */

        // render kubus
        glBindVertexArray(VAO_kubus);
        for (unsigned int i = 0; i < 9; i++){
            // calculate the model matrix for each object and pass it to shader before drawing
            model = glm::mat4(1.0f);
            model = glm::translate(model, posisiKubus[i]);
            model = glm::rotate(model, glm::radians(rotasiKubus[i]), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, ukuranKubus[i]);
            ourShader.setMat4("model", model);

            if(i < 4){
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture1);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture1);
            }
            if(i >= 4 && i < 8){
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture7);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture7);
            }
            if(i == 8){
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture5);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture5);
            }

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // render limas segitiga
        glBindVertexArray(VAO_limas);
        for (unsigned int i = 0; i < 8; i++){
            // calculate the model matrix for each object and pass it to shader before drawing
            model = glm::mat4(1.0f);
            model = glm::translate(model, posisiLimas[i]);
            model = glm::rotate(model, glm::radians(rotasiLimas[i]), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, ukuranLimas[i]);
            ourShader.setMat4("model", model);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture4);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture4);

            glDrawArrays(GL_TRIANGLES, 0, 12);
        }

        // render prisma segitiga
        glBindVertexArray(VAO_prisma);
        for (unsigned int i = 0; i < 8; i++){
            // calculate the model matrix for each object and pass it to shader before drawing
            model = glm::mat4(1.0f);
            model = glm::translate(model, posisiPrisma[i]);
            model = glm::rotate(model, glm::radians(rotasiPrisma[i]), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, ukuranPrisma[i]);
            ourShader.setMat4("model", model);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture3);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture3);

            glDrawArrays(GL_TRIANGLES, 0, 24);
        }

        // render piramida
        glBindVertexArray(VAO_piramida);
        for (unsigned int i = 0; i < 8; i++){
            // calculate the model matrix for each object and pass it to shader before drawing
            model = glm::mat4(1.0f);
            model = glm::translate(model, posisiPiramida[i]);
            model = glm::rotate(model, glm::radians(rotasiPiramida[i]), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, ukuranPiramida[i]);
            ourShader.setMat4("model", model);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture6);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture6);

            glDrawArrays(GL_TRIANGLES, 0, 18);
        }

        // render diamond
        glBindVertexArray(VAO_diamond);
        for (unsigned int i = 0; i < 8; i++){
            // calculate the model matrix for each object and pass it to shader before drawing
            model = glm::mat4(1.0f);
            model = glm::translate(model, posisiDiamond[i]);
            model = glm::rotate(model, glm::radians(rotasiDiamond[i]), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, ukuranDiamond[i]);
            ourShader.setMat4("model", model);

            if(i < 4){
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture2);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture2);
            }
            if(i >= 4 && i < 8){
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture8);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture8);
            }

            glDrawArrays(GL_TRIANGLES, 0, 24);
        }

        // proses kontrol objek
        if(current3D == 0){
            // translasi
            posisiKubus[currentObject] = posisiKubus[currentObject] + movement;
            model = glm::translate(model, posisiKubus[currentObject]);

            // rotasi
            rotasiKubus[currentObject] = rotasiKubus[currentObject] + rotation;

            // scaling
            ukuranKubus[currentObject] = ukuranKubus[currentObject] + scaling;

            movement = glm::vec3(0.0f);
            rotation = 0.0f;
            scaling = glm::vec3(0.0f);
        }
        else if(current3D == 1){
            // translasi
            posisiLimas[currentObject] = posisiLimas[currentObject] + movement;
            model = glm::translate(model, posisiLimas[currentObject]);

            // rotasi
            rotasiLimas[currentObject] = rotasiLimas[currentObject] + rotation;

            // scaling
            ukuranLimas[currentObject] = ukuranLimas[currentObject] + scaling;

            movement = glm::vec3(0.0f);
            rotation = 0.0f;
            scaling = glm::vec3(0.0f);
        }
        else if(current3D == 2){
            // translasi
            posisiPrisma[currentObject] = posisiPrisma[currentObject] + movement;
            model = glm::translate(model, posisiPrisma[currentObject]);

            // rotasi
            rotasiPrisma[currentObject] = rotasiPrisma[currentObject] + rotation;

            // scaling
            ukuranPrisma[currentObject] = ukuranPrisma[currentObject] + scaling;

            movement = glm::vec3(0.0f);
            rotation = 0.0f;
            scaling = glm::vec3(0.0f);
        }
        else if(current3D == 3){
            // translasi
            posisiPiramida[currentObject] = posisiPiramida[currentObject] + movement;
            model = glm::translate(model, posisiPiramida[currentObject]);

            // rotasi
            rotasiPiramida[currentObject] = rotasiPiramida[currentObject] + rotation;

            // scaling
            ukuranPiramida[currentObject] = ukuranPiramida[currentObject] + scaling;

            movement = glm::vec3(0.0f);
            rotation = 0.0f;
            scaling = glm::vec3(0.0f);
        }
        else if(current3D == 4){
            // translasi
            posisiDiamond[currentObject] = posisiDiamond[currentObject] + movement;
            model = glm::translate(model, posisiDiamond[currentObject]);

            // rotasi
            rotasiDiamond[currentObject] = rotasiDiamond[currentObject] + rotation;

            // scaling
            ukuranDiamond[currentObject] = ukuranDiamond[currentObject] + scaling;

            movement = glm::vec3(0.0f);
            rotation = 0.0f;
            scaling = glm::vec3(0.0f);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    glDeleteVertexArrays(1, &VAO_kubus);
    glDeleteBuffers(1, &VBO_kubus);

    glDeleteVertexArrays(1, &VAO_limas);
    glDeleteBuffers(1, &VBO_limas);

    glDeleteVertexArrays(1, &VAO_prisma);
    glDeleteBuffers(1, &VBO_prisma);

    glDeleteVertexArrays(1, &VAO_piramida);
    glDeleteBuffers(1, &VBO_piramida);

    glDeleteVertexArrays(1, &VAO_diamond);
    glDeleteBuffers(1, &VBO_diamond);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window){
    /*
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    */

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    // Background color
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS){
        redColor = redColor + 0.001f;
        if(redColor >= 1.0f){
            redColor = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS){
        redColor = redColor - 0.001f;
        if(redColor <= 0.0f){
            redColor = 0.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS){
        greenColor = greenColor + 0.001f;
        if(greenColor >= 1.0f){
            greenColor = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS){
        greenColor = greenColor - 0.001f;
        if(greenColor <= 0.0f){
            greenColor = 0.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS){
        blueColor = blueColor + 0.001f;
        if(blueColor >= 1.0f){
            blueColor = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS){
        blueColor = blueColor - 0.001f;
        if(blueColor <= 0.0f){
            blueColor = 0.0f;
        }
    }
}

// Proses input dari keyboard
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)){
        glfwSetWindowShouldClose(window, true);
    }

    // Speed translasi
    if ((key == GLFW_KEY_EQUAL) && (action == GLFW_PRESS)){
        speed++;
        if(speed >= 5){
            speed = 5;
        }
    }
    else if ((key == GLFW_KEY_MINUS) && (action == GLFW_PRESS)){
        speed--;
        if(speed <= 1){
            speed = 1;
        }
    }

    // Translasi
    if ((key == GLFW_KEY_UP) && (action == GLFW_PRESS)){
        movement = glm::vec3(0.0f, (0.1f * speed), 0.0f);
    }
    else if ((key == GLFW_KEY_DOWN) && (action == GLFW_PRESS)){
        movement = glm::vec3(0.0f, (-0.1f * speed), 0.0f);
    }
    else if ((key == GLFW_KEY_LEFT) && (action == GLFW_PRESS)){
        movement = glm::vec3((-0.1f * speed), 0.0f, 0.0f);
    }
    else if ((key == GLFW_KEY_RIGHT) && (action == GLFW_PRESS)){
        movement = glm::vec3((0.1f * speed), 0.0f, 0.0f);
    }
    else if ((key == GLFW_KEY_M) && (action == GLFW_PRESS)){
        movement = glm::vec3(0.0f, 0.0f, (0.1f * speed));
    }
    else if ((key == GLFW_KEY_N) && (action == GLFW_PRESS)){
        movement = glm::vec3(0.0f, 0.0f, (-0.1f * speed));
    }

    // Rotasi
    if ((key == GLFW_KEY_R) && (action == GLFW_PRESS)){
        rotation = -10.0f;
    }
    else if ((key == GLFW_KEY_T) && (action == GLFW_PRESS)){
        rotation = 10.0f;
    }

    // Scaling
    if ((key == GLFW_KEY_F) && (action == GLFW_PRESS)){
        scaling = glm::vec3(0.2f, 0.2f, 0.2f);
    }
    else if ((key == GLFW_KEY_G) && (action == GLFW_PRESS)){
        scaling = glm::vec3(-0.2f, -0.2f, -0.2f);
    }

    // Pilih bentuk 3D
    if ((key == GLFW_KEY_K) && (action == GLFW_PRESS)){
        current3D = 0;
    }
    else if ((key == GLFW_KEY_L) && (action == GLFW_PRESS)){
        current3D = 1;
    }
    else if ((key == GLFW_KEY_P) && (action == GLFW_PRESS)){
        current3D = 2;
    }
    else if ((key == GLFW_KEY_O) && (action == GLFW_PRESS)){
        current3D = 3;
    }
    else if ((key == GLFW_KEY_I) && (action == GLFW_PRESS)){
        current3D = 4;
    }

    // Pilih objek
    if ((key == GLFW_KEY_1) && (action == GLFW_PRESS)){
        currentObject = 0;
    }
    else if ((key == GLFW_KEY_2) && (action == GLFW_PRESS)){
        currentObject = 1;
    }
    else if ((key == GLFW_KEY_3) && (action == GLFW_PRESS)){
        currentObject = 2;
    }
    else if ((key == GLFW_KEY_4) && (action == GLFW_PRESS)){
        currentObject = 3;
    }
    else if ((key == GLFW_KEY_5) && (action == GLFW_PRESS)){
        currentObject = 4;
    }
    else if ((key == GLFW_KEY_6) && (action == GLFW_PRESS)){
        currentObject = 5;
    }
    else if ((key == GLFW_KEY_7) && (action == GLFW_PRESS)){
        currentObject = 6;
    }
    else if ((key == GLFW_KEY_8) && (action == GLFW_PRESS)){
        currentObject = 7;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
