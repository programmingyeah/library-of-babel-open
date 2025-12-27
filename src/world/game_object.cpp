#include "game_object.hpp"
#include "mesh.hpp"
#include <iostream>

void GameObject::Draw(unsigned int shaderProgram) {
    model->Draw(shaderProgram);
}