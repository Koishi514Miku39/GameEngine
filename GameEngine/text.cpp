#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/**
 * @brief GLFW 错误回调函数，将 GLFW 内部错误输出到控制台便于诊断环境问题。
 * @param errorCode GLFW 错误码。
 * @param description 错误描述文本。
 */
void GlfwErrorCallback(int errorCode, const char* description)
{
    std::cerr << "[GLFW 错误] 代码: " << errorCode << "，描述: " << description << std::endl;
}

int main()
{
    // 错误回调须在 glfwInit 之前注册，才能捕获初始化阶段的错误
    glfwSetErrorCallback(GlfwErrorCallback);

    // GLFW 初始化：成功返回 GLFW_TRUE（非 0），失败返回 0
    if (!glfwInit())
    {
        std::cerr << "[测试失败] GLFW 初始化失败" << std::endl;
        return -1;
    }

    // 请求 OpenGL 3.3 Core Profile，与项目 glad/glfw 配置保持一致
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建 800x600 测试窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Config Test", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "[测试失败] 窗口创建失败，请检查显卡驱动是否支持 OpenGL 3.3" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 将窗口上下文设为当前线程上下文；GLAD 加载函数指针前必须先执行此步
    glfwMakeContextCurrent(window);

    // 通过 GLFW 提供地址加载器初始化 GLAD，加载全部 OpenGL 函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "[测试失败] GLAD 初始化失败（OpenGL 函数指针加载出错）" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 打印实际 OpenGL 环境信息，用于确认驱动与版本
    std::cout << "OpenGL 厂商   : " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL 渲染器 : " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL 版本   : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL 版本     : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // 渲染循环：窗口若显示为深蓝色纯色，即表示 OpenGL 渲染管线工作正常
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window); // 交换前后缓冲，提交本帧绘制结果
        glfwPollEvents();        // 处理键盘/鼠标/窗口事件
    }

    glfwTerminate(); // 销毁窗口与 GLFW 资源
    std::cout << "[测试成功] OpenGL 环境配置正常" << std::endl;
    return 0;
}
