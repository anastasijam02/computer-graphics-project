
#include <engine/resources/Model.hpp>
#include <engine/resources/Shader.hpp>

namespace engine::resources {

void Model::draw(const Shader *shader) {
    shader->use();
    for (auto &mesh: m_meshes) {
        mesh.draw(shader);
    }
}
void Model::destroy() {
    for (auto &mesh: m_meshes) {
        mesh.destroy();
    }
}

    void Model::draw_mesh(const Shader *shader, const std::string &mesh_name){
        shader->use();

        for(auto &mesh : m_meshes){
            if(mesh.name() == mesh_name){
                mesh.draw(shader);
            }
        }
    }

    void Model::draw_except_mesh(const Shader *shader, const std::string &mesh_name){
        shader->use();

        for(auto &mesh : m_meshes){
            if(mesh.name() != mesh_name){
                mesh.draw(shader);
            }
        }
    }


}// namespace engine::resources
