//
// Created by anastasija on 27. 8. 2026..
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_H
#define MATF_RG_PROJECT_MAINCONTROLLER_H
#include "engine/core/Controller.hpp"
#include <engine/resources/Shader.hpp>
#include <glm/glm.hpp>
namespace app {
    class MainController : public engine::core::Controller {
        float lighthouse_angle = 0.0f;
        float lighthouse_rotation_speed = 20.0f;

        unsigned int beam_vao = 0;
        unsigned int beam_vbo = 0;
        unsigned int beam_vertex_count = 0;

        void initialize_lighthouse_beam();
        void draw_lighthouse_beam();

        glm::vec3 boat_front_lamp_position = glm::vec3(-1.15436f, -0.159531f, -5.02937f);

        bool point_light_enabled = true;
        bool p_was_down = false;

        bool boat_moving = false;
        bool b_was_down = false;
        float boat_offset = 0.0f;
        float boat_speed = 0.2f;

        glm::vec3 lighthouse_light_position = glm::vec3(7.9073563f, 6.8495054f, -14.7876625f);

        bool lighthouse_light_enabled = false;

        glm::vec3 get_lighthouse_spot_direction();

        bool t_was_down = false;
        bool event_started = false;
        float event_timer = 0.0f;

        bool directional_light_dimmed = false;

        void initialize() override;

        bool loop() override;

        void update_camera();

        void update() override;

        void begin_draw() override;

        void draw_boat();

        void end_draw() override;

        void draw_sea();

        void draw_lighthouse();

        void draw_skybox();

        void draw() override;

        void set_directional_light(engine::resources::Shader *shader);

        void set_point_light(engine::resources::Shader *shader);

        void set_spot_light(engine::resources::Shader *shader);

    public:
        std::string_view name() const override {
            return "app::MainController";
        }
    };
} // app

#endif //MATF_RG_PROJECT_MAINCONTROLLER_H
