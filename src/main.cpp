#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    // 初始化GLFW
    // -------------------------
    glfwInit();
    // 配置GLFW （选项的名称，值）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 3.3版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //core profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //mac os x系统

    // 创建一个窗口对象 （宽，高，窗口的名称，..., ...）
    // -------------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 高度GLFW希望每次窗口调整大小的时候调用这个函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
    // 初始化GLAD来管理OpenGL的函数指针
    // -------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // vertex shader
    // ----------------------
    unsigned int vertexShader; // 储存
    vertexShader = glCreateShader(GL_VERTEX_SHADER); //创建着色器
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //把源码附加到着色器上（要编译的着色器，源码字符数，源码，NULL）
    glCompileShader(vertexShader); //编译
    //检查是否编译成功
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragement shader
    // ------------------------
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //检查是否编译成功
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 多个着色器合并
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); //链接
    //检查error
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    //删除
    glDeleteShader(vertexShader); 
    glDeleteShader(fragmentShader);

    // 顶点输入
    // ------------------------
    // 1. vertices for a single triangle
    // float vertices[] = { 
    //     -0.5f, -0.5f, 0.0f,
    //      0.5f, -0.5f, 0.0f,
    //      0.0f,  0.5f, 0.0f
    // };

    // 2. vertices for two triangles that form a rectangle
    // float vertices[] = {
    //     0.5f, 0.5f, 0.0f,   // 右上角
    //     0.5f, -0.5f, 0.0f,  // 右下角
    //     -0.5f, -0.5f, 0.0f, // 左下角
    //     -0.5f, 0.5f, 0.0f   // 左上角
    // };

    // 3. vertices for two triangles that next to each other
    float vertices[] = {
        -0.9f, -0.5f, 0.0f,
        -0.45f, 0.5f, 0.0f,
        0.0f, -0.5f, -0.0f,
        0.45f, 0.5f, 0.0f,
        0.9f, -0.5f, 0.0f
    };

    // 2.
    // unsigned int indices[] = {
    //     // 注意索引从0开始! 
    //     // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    //     // 这样可以由下标代表顶点组合成矩形
    //     0, 1, 3, // 第一个三角形
    //     1, 2, 3  // 第二个三角形
    // };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 4
    };

    // VBO: vertex buffer object, VAO:顶点数组对象(Vertex Array Object, VAO)，Element Buffer Object，EBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); //生成一个带有缓冲ID的VBO对象
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO); 

    glBindBuffer(GL_ARRAY_BUFFER, VBO);  //把新创建的缓冲绑定到GL_ARRAY_BUFFER上
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //把定点数据复制到缓冲的内存中（目标缓冲的类型，传输数据的大小，实际数据，显卡如何管理数据）
    // GL_STATIC_DRAW ：数据不会或几乎不会改变。
    // GL_DYNAMIC_DRAW：数据会被改变很多。
    // GL_STREAM_DRAW ：数据每次绘制时都会改变。
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 告诉OpenGL如何解析定点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //（location=0, vec3, 数据类型，是否希望数据标准化，步长Stride连续的顶点属性组之间的间隔，起始位置偏移量）
    glEnableVertexAttribArray(0); //启用顶点属性（顶点属性位置）

    // VBO清零
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // VAO清零
    glBindVertexArray(0); 

    // 线框模式Wireframe Mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //默认模式

    //渲染循环，不希望只绘制一个图像后我们的应用程序就立即退出并关闭窗口
    while(!glfwWindowShouldClose(window))
    {   
        // 输入
        // -----------------------
        processInput(window);

        // 渲染
        // ----------------------
        // 清空颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清空屏幕所用的颜色，状态设置函数
        glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲后，颜色将会被上面设置的所填充，状态使用函数

        // 绘制
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3); // （绘制的类型，顶点数组的起始index，多少个顶点）
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // (绘制的模式， 顶点个数， 索引类型，偏移量)
        // -------------------------
        glfwSwapBuffers(window); //触发事件
        glfwPollEvents(); //交换颜色缓冲
    }

    //删除VAO，VBO和shader
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    //删除之前分配的所有资源
    glfwTerminate();
    return 0;
}

// 实现输入控制
void processInput(GLFWwindow *window)
{   
    // 是否按下Esc键
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// 窗口回调函数，每次窗口大小被调整的时候被调用（FLGWwindow， 新维度，新维度）
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 渲染窗口的尺寸大小-视口Viewport （左下角位置，左下角位置，宽度，高度）
    glViewport(0, 0, width, height);
}   