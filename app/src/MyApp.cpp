//
// Created by anastasija on 15.5.26..
//

#include "MyApp.h"
#include <spdlog/spdlog.h>

#include "MainController.h"

void MyApp::app_setup() {
    spdlog::info("Hello, setup!");
    auto main_controller = register_controller<app::MainController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
}
