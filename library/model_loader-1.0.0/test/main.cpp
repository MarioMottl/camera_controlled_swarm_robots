#include <iostream>
#include <sstream>
#include <string.h>
#include "Model.h"

int main()
{
    gl::Model model;
    gl::model_error_t error = model.load("models/", "test_model.obj", gl::Model::SWAP_YZ | gl::Model::MESH_COMBINED | gl::Model::IGNORE_VT);

    std::cout << gl::Model::strerror(error) << std::endl;

    std::cout << model.vertex_data().size() << std::endl;
    std::cout << model.texture_coord_data().size() << std::endl;
    std::cout << model.normal_data().size() << std::endl;

    for(auto mat : model.get_materials())
    {
        std::cout << mat.second.diffuse_texture << std::endl;
    }

    for(size_t i=0; i<model.vertex_data().size(); i++)
    {
        //std::cout << "x: " << model.vertex_data().at(i) << std::endl;
    }
    return 0;
}
