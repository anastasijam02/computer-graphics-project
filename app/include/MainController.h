//
// Created by anastasija on 27. 8. 2026..
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_H
#define MATF_RG_PROJECT_MAINCONTROLLER_H
#include "engine/core/Controller.hpp"

#include <engine/core/Controller.hpp>

namespace app {
    class MainController : public engine::core::Controller {
        void initialize() override;

    public:
        std::string_view name() const override {
            return "app::MainController";
        }
    };
} // app

#endif //MATF_RG_PROJECT_MAINCONTROLLER_H
