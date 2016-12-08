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


#include "fs_config.hpp"
#include <cpptoml.h>

using namespace std::literals;


#define TRY_GET(toml, T, name, var_to_set)                                       \
	do {                                                                           \
		auto temp = (toml)->get_as<T>(name);                                         \
		if(temp)                                                                     \
			(var_to_set) = *temp;                                                      \
		else                                                                         \
			return std::make_tuple(fs_config{}, 1, "\""s + name + "\" doesn't exist"); \
	} while(false)


std::tuple<fs_config, int, std::string> fs_config::from(std::istream & from) {
	fs_config retval;
	std::shared_ptr<cpptoml::table> cfg;
	try {
		cfg = cpptoml::parser(from).parse();
	} catch(...) {
		return std::make_tuple(std::move(retval), 2, "not valid TOML");
	}

	TRY_GET(cfg, std::string, "mount_point", retval.mount_point);
	TRY_GET(cfg, std::string, "database", retval.db_location);

	return std::make_tuple(std::move(retval), 0, ""s);
}
