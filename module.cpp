#include "module.hpp"

module::module(std::string label, std::vector<std::string> knobs)
: label(label), knobs(knobs) { return; }

module::~module() {}

std::string module::get_label() { return this->label; }

std::vector<std::string> module::get_knobs() { return this->knobs; }
