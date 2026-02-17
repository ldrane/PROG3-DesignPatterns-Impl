#include <iostream>
#include <memory>
#include <string>

// === INTERFACES ABSTRACTAS ===

class Window {
public:
    virtual ~Window() = default;
    virtual void draw() = 0;
};

class Button {
public:
    virtual ~Button() = default;
    virtual void draw() = 0;
};

class Shader {
public:
    virtual ~Shader() = default;
    virtual void compile() = 0;
};


// === IMPLEMENTACION OpenGL ===

class OpenGLWindow : public Window {
public:
    void draw() override {
        std::cout << "Dibujando ventana con OpenGL" << std::endl;
    }
};

class OpenGLButton : public Button {
public:
    void draw() override {
        std::cout << "Dibujando boton con OpenGL" << std::endl;
    }
};

class OpenGLShader : public Shader {
public:
    void compile() override {
        std::cout << "Compilando shader con OpenGL" << std::endl;
    }
};

// === IMPLEMENTACION Vulkan ===

class VulkanWindow : public Window {
public:
    void draw() override {
        std::cout << "Dibujando ventana con Vulkan" << std::endl;
    }
};

class VulkanButton : public Button {
public:
    void draw() override {
        std::cout << "Dibujando boton con Vulkan" << std::endl;
    }
};

class VulkanShader : public Shader {
public:
    void compile() override {
        std::cout << "Compilando shader con Vulkan" << std::endl;
    }
};

// ==== ABSTRACT FACTORY ====

class RenderFactory {
public:
    virtual ~RenderFactory() = default;
    virtual std::unique_ptr<Window> createWindow() = 0;
    virtual std::unique_ptr<Button> createButton() = 0;
    virtual std::unique_ptr<Shader> createShader() = 0;
};

class OpenGLFactory : public RenderFactory {
public:
    std::unique_ptr<Window> createWindow() override {
        return std::make_unique<OpenGLWindow>();
    }

    std::unique_ptr<Button> createButton() override {
        return std::make_unique<OpenGLButton>();
    }

    std::unique_ptr<Shader> createShader() override {
        return std::make_unique<OpenGLShader>();
    }
};

class VulkanFactory : public RenderFactory {
public:
    std::unique_ptr<Window> createWindow() override {
        return std::make_unique<VulkanWindow>();
    }

    std::unique_ptr<Button> createButton() override {
        return std::make_unique<VulkanButton>();
    }

    std::unique_ptr<Shader> createShader() override {
        return std::make_unique<VulkanShader>();
    }
};

int main() {
    std::unique_ptr<RenderFactory> factory = std::make_unique<OpenGLFactory>();

    auto window = factory->createWindow();
    auto button = factory->createButton();

    window->draw();
    button->draw();

    factory = std::make_unique<VulkanFactory>();
    auto shader = factory->createShader();
    shader->compile();

    return 0;
}