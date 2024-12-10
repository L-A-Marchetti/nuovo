#ifndef MODULE_HPP
#define MODULE_HPP

#include <string>
#include <vector>

class module
{
	private:
		std::string label;
		std::vector<std::string> knobs;
	public:
		module(std::string label, std::vector<std::string> knobs);
		~module();
		std::string get_label();
		std::vector<std::string> get_knobs();
};

#endif //MODULE_HPP
