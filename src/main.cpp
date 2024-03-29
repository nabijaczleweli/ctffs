// The MIT License (MIT)

// Copyright (c) 2016 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "filesystem.hpp"
#include "fs_config.hpp"
#include "options.hpp"
#include <fstream>
#include <iostream>
#include <vector>


int main(int argc, const char ** argv) {
	options opts(argc, argv);

	std::cout << "config_paths = {";
	for(auto && cp : opts.config_paths)
		std::cout << cp << ", ";
	std::cout << "}\n";

	std::vector<fs_config> configs;
	configs.reserve(opts.config_paths.size());
	for(auto && cp : opts.config_paths) {
		std::ifstream cfg_f(cp);
		auto cfg = fs_config::from(cfg_f);

		if(std::get<int>(cfg) == 0) {
			configs.emplace_back(std::move(std::get<fs_config>(cfg)));
		} else {
			std::cerr << "Failed to parse config file \"" << cp << "\": " << std::get<std::string>(cfg) << '\n';
			return std::get<int>(cfg);
		}
	}

	std::cout << "configs = {\n";
	for(auto && cfg : configs)
		std::cout << "  {\n"
		          << "    mount_point = \"" << cfg.mount_point << "\"\n"  //
		          << "    db_location = \"" << cfg.db_location << "\"\n"  //
		          << "  }, \n";
	std::cout << "}\n";

	filesystem fs(configs[0]);
	std::cout << "Enter to unmount...";
	char _;
	std::cin.get(_);
}
