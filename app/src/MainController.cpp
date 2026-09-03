//
// Created by anastasija on 27. 8. 2026..
//

#include "../include/MainController.h"

#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "spdlog/spdlog.h"

namespace app {
    class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
    public:
        void on_mouse_move(engine::platform::MousePosition position) override;
    };

    void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx * 0.1f, position.dy * 0.1f);
    }


    void MainController::initialize() {
        spdlog::info("MainController initialized!!!");
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
        engine::graphics::OpenGL::enable_depth_testing();

        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        graphics->camera()->Position = glm::vec3(0.0f, 1.0f, 0.0f);

        initialize_sea();
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }

    void MainController::update_camera() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera = graphics->camera();
        float dt = platform->dt();
        if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
        }
    }

    void MainController::update() {
        update_camera();
    }

    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainController::draw_boat() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        engine::resources::Model *boat = resources->model("boat");

        engine::resources::Shader *shader = resources->shader("lighting");

        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        shader->use();

        set_directional_light(shader);
        shader->set_vec3("view_position", graphics->camera()->Position);

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        shader->set_mat4("model", glm::mat4(1.0f));
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f));
        shader->set_mat4("model", model);

        boat->draw(shader);
    }

    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

    void MainController::initialize_sea() {
        float sea_vertices[] = {
            // position            // texture coordinates
            -1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, -1.0f, 10.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 10.0f, 10.0f,
            -1.0f, 0.0f, 1.0f, 0.0f, 10.0f
        };

        unsigned int sea_indices[] = {
            0, 2, 1,
            0, 3, 2
        };

        engine::graphics::OpenGL::initialize_sea(
            sea_vao,
            sea_vbo,
            sea_ebo,
            sea_vertices,
            sizeof(sea_vertices),
            sea_indices,
            sizeof(sea_indices)
        );
    }

    void MainController::draw_sea() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Shader *shader = resources->shader("sea");
        engine::resources::Texture *water_texture = resources->texture("dark_water");

        shader->use();

        shader->set_int("water_texture", 0);

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.15f, -3.0f));
        model = glm::scale(model, glm::vec3(100.0f, 1.0f, 100.0f));
        shader->set_mat4("model", model);

        water_texture->bind(engine::graphics::OpenGL::texture_unit(0));

        shader->set_int("water_texture", 0);

        engine::graphics::OpenGL::draw_indexed(sea_vao, 6);
    }

    void MainController::draw_lighthouse() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Model *lighthouse = resources->model("lighthouse");

        engine::resources::Shader *shader = resources->shader("lighting");

        shader->use();

        set_directional_light(shader);
        shader->set_vec3("view_position", graphics->camera()->Position);

        shader->set_mat4("projection",graphics->projection_matrix());
        shader->set_mat4("view",graphics->camera()->view_matrix());
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(8.0f, 0.0f, -15.0f));
        model = glm::rotate(model,glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model,glm::vec3(0.1f));
        shader->set_mat4("model", model);

        lighthouse->draw(shader);
    }

    void MainController::draw_skybox() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto skybox = resources->skybox("night_sky");
        auto shader = resources->shader("skybox");
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        graphics->draw_skybox(shader, skybox);

    }

    void MainController::draw() {
        draw_sea();
        draw_boat();
        draw_lighthouse();
        draw_skybox();
    }

    void MainController::set_directional_light(engine::resources::Shader *shader) {
        shader->set_vec3("directional_light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        shader->set_vec3("directional_light.ambient", glm::vec3(0.05f, 0.05f, 0.08f));
        shader->set_vec3("directional_light.diffuse", glm::vec3(0.20f, 0.25f, 0.35f));
        shader->set_vec3("directional_light.specular", glm::vec3(0.4f, 0.4f, 0.5f));
    }
} // app
