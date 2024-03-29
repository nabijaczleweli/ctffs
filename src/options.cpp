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


#include "options.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <tclap/CmdLine.h>


class config_paths_constraint : public TCLAP::Constraint<std::string> {
	std::string description() const { return "file exists"; }

	std::string shortID() const { return "config-paths"; }

	bool check(const std::string & value) const { return static_cast<bool>(std::ifstream(value)); }
};


options::options(int argc, const char * const * argv) {
	try {
		TCLAP::CmdLine command_line("ctffs -- flat filesystem for text files", ' ', CTFFS_VERSION);

		config_paths_constraint cp_c;
		TCLAP::UnlabeledMultiArg<std::string> cp("config-paths", "Files specifying filesystem configurations", true, &cp_c, command_line);
		command_line.parse(argc, argv);

		config_paths = cp.getValue();
	} catch(const TCLAP::ArgException & ae) {
		std::cerr << ae.what() << '\n';
	}
}
