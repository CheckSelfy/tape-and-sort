#pragma once

#include <fstream>
#include <sstream>
#include "../tape/delay_tape.h"

void process_key_value(std::string const& key, std::string const& value, delays& d);

delays config_read(std::string const& file_name);
