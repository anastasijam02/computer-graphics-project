//
// Created by anastasija on 27. 8. 2026..
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_H
#define MATF_RG_PROJECT_MAINCONTROLLER_H
#include "engine/core/Controller.hpp"

#include <engine/core/Controller.hpp>

namespace app {
    class MainController : public engine::core::Controller {
        unsigned int sea_vao;
        unsigned int sea_vbo;
        unsigned int sea_ebo;

        void initialize() override;

        bool loop() override;

        void update_camera();

        void update() override;

        void begin_draw() override;

        void draw_boat();

        void end_draw() override;

        void initialize_sea();

        void draw_sea();

        void draw_lighthouse();

        void draw_skybox();

        void draw() override;

    public:
        std::string_view name() const override {
            return "app::MainController";
        }
    };
} // app

#endif //MATF_RG_PROJECT_MAINCONTROLLER_H
