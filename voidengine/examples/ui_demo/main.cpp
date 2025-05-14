#include "window/Window.h"
#include "ui/UIManager.h"
#include "ui/Panel.h"
#include "ui/Button.h"
#include "ui/Text.h"
#include <iostream>
#include <functional>

int main() {
    try {
        voidengine::window::Window window(800, 600, "VoidEngine UI Demo");
        
        auto uiManager = window.getUIManager();
        
        auto panel = uiManager->createPanel("mainPanel", 
                                           glm::vec2(50, 50), 
                                           glm::vec2(300, 400),
                                           glm::vec4(0.2f, 0.2f, 0.3f, 0.8f));
        
        auto titleText = uiManager->createText("titleText", 
                                              glm::vec2(200, 70), 
                                              "UI Demo",
                                              24.0f,
                                              glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        titleText->setAlignment(voidengine::ui::TextAlignment::CENTER);
        
        auto button1 = uiManager->createButton("button1", 
                                              glm::vec2(100, 150), 
                                              glm::vec2(200, 40),
                                              "Click Me",
                                              []() {
                                                  std::cout << "Button 1 clicked!" << std::endl;
                                              });
        
        auto button2 = uiManager->createButton("button2", 
                                              glm::vec2(100, 200), 
                                              glm::vec2(200, 40),
                                              "Disabled Button");
        button2->setEnabled(false);
        
        auto button3 = uiManager->createButton("button3", 
                                              glm::vec2(100, 250), 
                                              glm::vec2(200, 40),
                                              "Exit",
                                              [&window]() {
                                                  glfwSetWindowShouldClose(window.getNativeWindow(), GLFW_TRUE);
                                              });
        
        auto infoText = uiManager->createText("infoText", 
                                             glm::vec2(200, 350), 
                                             "Move your mouse over buttons\n"
                                             "to see hover effects.\n"
                                             "Click buttons to trigger actions.",
                                             12.0f,
                                             glm::vec4(0.8f, 0.8f, 1.0f, 1.0f));
        infoText->setAlignment(voidengine::ui::TextAlignment::CENTER);
        
        panel->addComponent(titleText);
        panel->addComponent(button1);
        panel->addComponent(button2);
        panel->addComponent(button3);
        panel->addComponent(infoText);
        
        uiManager->initialize();
        
        while (!window.shouldClose()) {
            window.clear(0.1f, 0.1f, 0.2f, 1.0f);
            window.swapBuffers();
            window.pollEvents();
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 