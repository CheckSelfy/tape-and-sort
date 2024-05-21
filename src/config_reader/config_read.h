#pragma once

#include <fstream>
#include <sstream>
#include "../tape/delay_tape.h"

void process_key_value(std::string const& key, std::string const& value, delays& d) {
    if (key == "read_time") {
        d.read_time = std::stoi(value);
    } else if (key == "write_time") {
        d.put_time = std::stoi(value);
    } else if (key == "move_time") {
        d.move_time = std::stoi(value);
    } else if (key == "move_to_start_time") {
        d.move_to_start_time = std::stoi(value);
    } else {
        throw std::runtime_error("Unknown option \"" + key = "\"");
    }
}

delays config_read(std::string const& file_name) {
    std::ifstream fin = std::ifstream(file_name, std::ios::in);

    delays d{10, 10, 10, 10};

    if (!fin.is_open()) {
        std::cout << "Couldn't open config file (config.txt), using standard values" << std::endl;
        return d;
    }


    std::string line;
    while (std::getline(fin, line)) {
        std::istringstream line_stream = std::istringstream(line);
        std::string key;
        if (std::getline(line_stream, key, '=')) {
            std::string value;
            if (std::getline(line_stream, value)) {
                process_key_value(key, value, d);
            } else {
                throw std::runtime_error("No option provided for \"" + key + "\"");
            }
        } else {
            throw std::runtime_error("No key provided at line \"" + line + "\"");
        }
    }

    return d;
}
