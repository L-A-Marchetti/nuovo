#include "sequencer.hpp"

sequencer::sequencer()
{
	int size = 83;
	this->buttons =
	{
		{.label="1", .size=size},
		{.label="2", .size=size},
		{.label="3", .size=size},
                {.label="4", .size=size},
		{.label="5", .size=size},
                {.label="6", .size=size},
		{.label="7", .size=size},
                {.label="8", .size=size},
		{.label="9", .size=size},
                {.label="10", .size=size},
		{.label="11", .size=size},
                {.label="12", .size=size},
		{.label="13", .size=size},
                {.label="14", .size=size},
		{.label="15", .size=size},
                {.label="16", .size=size},
	};
	return;
}

sequencer::~sequencer() {}
