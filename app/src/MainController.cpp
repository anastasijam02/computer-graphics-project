//
// Created by anastasija on 27. 8. 2026..
//

#include "../include/MainController.h"

#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "spdlog/spdlog.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>

namespace app {
    class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
    public:
        void on_mouse_move(engine::platform::MousePosition position) override;
    };

    void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx * 0.1f, position.dy * 0.1f);
    }


    void MainController::initialize_lighthouse_beam(){
        const float length = 15.0f;
        const float half_width = 1.3f;

        float vertices[] = {
            // vrh kod lampe
            0.0f,  0.0f, 0.0f,

           // kraj snopa
           -half_width, 0.0f, length,

            half_width, 0.0f, length
       };

        beam_vertex_count = 3;

        engine::graphics::OpenGL::initialize_lamp(
            beam_vao,
            beam_vbo,
            vertices,
            sizeof(vertices)
        );
    }

    void MainController::draw_lighthouse_beam() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        auto shader = resources->shader("beam");

        shader->use();

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::vec3 spot_direction = get_lighthouse_spot_direction();

        //lokalni koord.sistem za beam
        //lokalna z osa pokazuje u smeru svetlosti
        glm::vec3 initial_up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(initial_up, spot_direction));
        glm::vec3 up = glm::normalize(glm::cross(spot_direction, right));


        // matrica rotacije
        glm::mat4 rotation = glm::mat4(1.0f);
        rotation[0] = glm::vec4(right, 0.0f);
        rotation[1] = glm::vec4(up, 0.0f);
        rotation[2] = glm::vec4(spot_direction, 0.0f);

        glm::mat4 base_model = glm::mat4(1.0f);

        base_model = glm::translate(base_model, lighthouse_light_position);
        base_model = base_model * rotation;


        int beam_planes = 4;
        for (int i = 0; i < beam_planes; i++){
            float angle = glm::radians(45.0f * i);
            glm::mat4 model = base_model;
            model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

            shader->set_mat4("model", model);

            engine::graphics::OpenGL::draw_lamp(beam_vao, beam_vertex_count);
        }

    }

    void MainController::initialize_lamp(){
        float lamp_vertices[] = {
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,

             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,


            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,


            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,


             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,

             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,


            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,

             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,


            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f
        };

        engine::graphics::OpenGL::initialize_lamp(
            lamp_vao,
            lamp_vbo,
            lamp_vertices,
            sizeof(lamp_vertices)
        );
    }

    void MainController::initialize() {
        spdlog::info("MainController initialized!!!");
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
        engine::graphics::OpenGL::enable_depth_testing();

        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        graphics->camera()->Position = glm::vec3(0.0f, 1.0f, 0.0f);

        initialize_sea();
        initialize_lamp();
        initialize_lighthouse_beam();
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
        set_point_light(shader);
        set_spot_light(shader);

        shader->set_vec3("view_position", graphics->camera()->Position);

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

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

        //point light sa broda
        shader->set_vec3("point_light.position", boat_front_lamp_position);
        shader->set_vec3("point_light.diffuse", glm::vec3(3.0f, 1.4f, 0.3f));
        shader->set_float("point_light.constant", 1.0f);
        shader->set_float("point_light.linear",0.7f);
        shader->set_float("point_light.quadratic", 1.2f);

        set_spot_light(shader);

        shader->set_int("water_texture", 0);

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.15f, -3.0f));
        model = glm::scale(model, glm::vec3(100.0f, 1.0f, 100.0f));
        shader->set_mat4("model", model);

        water_texture->bind(engine::graphics::OpenGL::texture_unit(0));

        engine::graphics::OpenGL::draw_indexed(sea_vao, 6);
    }

    void MainController::draw_lighthouse() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Model *lighthouse = resources->model("lighthouse");

        engine::resources::Shader *shader = resources->shader("lighting");

        shader->use();

        set_directional_light(shader);
        shader->set_vec3("directional_light.ambient", glm::vec3(0.20f, 0.20f, 0.30f));

        set_spot_light(shader);

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

    void MainController::draw_lamp(){
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        auto shader = resources->shader("lamp");

        shader->use();

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, boat_front_lamp_position);
        model = glm::scale(model, glm::vec3(0.04f));
        shader->set_mat4("model", model);

        engine::graphics::OpenGL::draw_lamp(lamp_vao,36);

        glm::mat4 lighthouse_lamp_model = glm::mat4(1.0f);
        lighthouse_lamp_model = glm::translate(lighthouse_lamp_model, lighthouse_light_position);
        lighthouse_lamp_model = glm::scale(lighthouse_lamp_model, glm::vec3(0.08f));
        shader->set_mat4("model", lighthouse_lamp_model);

        engine::graphics::OpenGL::disable_depth_testing();
        engine::graphics::OpenGL::draw_lamp(lamp_vao, 36);
        engine::graphics::OpenGL::enable_depth_testing();

    }

    glm::vec3 MainController::get_lighthouse_spot_direction() {
        return glm::normalize(lighthouse_spot_target - lighthouse_light_position);
    }


    void MainController::draw() {
        draw_sea();
        draw_boat();
        draw_lighthouse();
        draw_skybox();

        engine::graphics::OpenGL::enable_blending();
        draw_lighthouse_beam();
        engine::graphics::OpenGL::disable_blending();
        draw_lamp();
    }

    void MainController::set_directional_light(engine::resources::Shader *shader) {
        shader->set_vec3("directional_light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        shader->set_vec3("directional_light.ambient", glm::vec3(0.05f, 0.05f, 0.08f));
        shader->set_vec3("directional_light.diffuse", glm::vec3(0.20f, 0.25f, 0.35f));
        shader->set_vec3("directional_light.specular", glm::vec3(0.4f, 0.4f, 0.5f));
    }

    void MainController::set_point_light(engine::resources::Shader *shader) {
        shader->set_vec3("point_light.position", boat_front_lamp_position);
        shader->set_vec3("point_light.ambient", glm::vec3(0.01f, 0.005f, 0.001f));
        shader->set_vec3("point_light.diffuse", glm::vec3(1.5f, 0.7f, 0.15f));
        shader->set_vec3("point_light.specular", glm::vec3(0.8f, 0.35f, 0.08f));

        shader->set_float("point_light.constant", 1.0f);
        shader->set_float("point_light.linear", 2.0f);
        shader->set_float("point_light.quadratic", 4.0f);
    }

    void MainController::set_spot_light(engine::resources::Shader *shader) {
        shader->set_vec3("spot_light.position", lighthouse_light_position);

        glm::vec3 spot_direction = get_lighthouse_spot_direction();
        shader->set_vec3("spot_light.direction", spot_direction);

        shader->set_float("spot_light.cutOff", glm::cos(glm::radians(4.0f)));
        shader->set_float("spot_light.outerCutOff", glm::cos(glm::radians(7.0f)));

        shader->set_vec3("spot_light.ambient", glm::vec3(0.0f));
        shader->set_vec3("spot_light.diffuse",glm::vec3(4.0f, 3.0f, 1.2f));
        shader->set_vec3("spot_light.specular", glm::vec3(1.5f, 1.4f, 1.0f));

        shader->set_float("spot_light.constant", 1.0f);
        shader->set_float("spot_light.linear", 0.045f);
        shader->set_float("spot_light.quadratic", 0.0075f);
    }
} // app
