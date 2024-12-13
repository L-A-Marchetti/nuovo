#include "module.hpp"
#include <iostream>

module::module(std::string label, std::vector<knob> knobs, std::string folder, int chan)
: label(label), knobs(knobs), chan(chan)
{
    if (!folder.empty()) parse_samples(folder);
    this->s = new sequencer();
    this->set_chunk_volume();
}

module::~module()
{
    for (auto& [key, chunk] : samples) Mix_FreeChunk(chunk);
    samples.clear();
    delete this->s;
}

std::string module::get_label() { return this->label; }

std::vector<knob>& module::get_knobs() { return this->knobs; }

sequencer* module::get_sequencer() { return this->s; }

void module::play()
{
    std::string key = build_sample_key();
    auto it = samples.find(key);
    if (it != samples.end())
    {
        if (Mix_PlayChannel(this->chan, it->second, 0) == -1)
            std::cerr << "Error reading : " << Mix_GetError() << std::endl;
    }
    else
    {
        auto default_it = samples.find("default.WAV");
        if (default_it != samples.end())
        {
            if (Mix_PlayChannel(this->chan, default_it->second, 0) == -1)
                std::cerr << "Error reading default sample : " << Mix_GetError() << std::endl;
        }
        else
        {
            std::cerr << "Unknow sample with key : " << key << std::endl;
        }
    }
}


void module::set_chunk_volume()
{
    int vol = 0;
    for (const auto& knob : this->knobs) if (knob.label == "LEVEL") vol = knob.value;
    for (auto& [key, chunk] : samples) Mix_VolumeChunk(chunk, vol);
}

void module::parse_samples(const std::string& folder)
{
    for (const auto& entry : std::filesystem::directory_iterator(folder))
    {
        if (entry.is_regular_file())
        {
            Mix_Chunk* chunk = Mix_LoadWAV(entry.path().c_str());
            if (chunk)
            {
                std::string filename = entry.path().filename().string();
                samples[filename] = chunk;
                size_t start = 0;
                while (start < filename.size())
                {
                    size_t end = filename.find("_", start);
                    if (end == std::string::npos) end = filename.size();
                    std::string segment = filename.substr(start, end - start);                    
                    size_t equal_pos = segment.find('=');
                    if (equal_pos != std::string::npos)
                    {
                        std::string key = segment.substr(0, equal_pos);
                        int value = std::stoi(segment.substr(equal_pos + 1));
                        available_values[key].insert(value);
                    }
                    start = end + 1;
                }
            }
        }
    }
}

std::string module::build_sample_key()
{
    std::string key;
    for (const auto& knob : this->knobs)
    {
        if (knob.label != "LEVEL")
        {
            const auto& values = available_values[knob.label];
            int max_available_value = *std::max_element(values.begin(), values.end());
            float normalized_value = (knob.value / 127.0f) * max_available_value;
            int closest = *values.begin();
            float min_diff = std::abs(normalized_value - closest);
            for (int v : values)
            {
                float diff = std::abs(normalized_value - v);
                if (diff < min_diff)
                {
                    min_diff = diff;
                    closest = v;
                }
            }
            key += knob.label + "=" + std::to_string(closest) + "_";
        }
    }
    if (!key.empty()) key.pop_back();  // Remove the trailing "_"
    return key + ".WAV";
}


