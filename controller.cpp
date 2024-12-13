#include "controller.hpp"

controller::controller() {}

controller::~controller() {}

void controller::set_volume()
{
    Mix_Volume(-1, this->volume.value);
}