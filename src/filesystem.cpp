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
#include <cstring>
#include <iostream>


static std::string naem = "ctffs";
static char * argv[2]   = {&naem[0], nullptr};
static fuse_args arg{1, argv, false};


filesystem::filesystem(const fs_config & config) : cfg(config), ops{}, channel{}, fs{} {
	ops.init = [](fuse_conn_info *) -> void * {
		std::cerr << "init";
		return nullptr;
	};
	ops.getattr = [](const char * path, FUSE_STAT * stbuf) {
		std::cerr << "getattr(" << path << ")\n";
		int res = 0;

		std::memset(stbuf, 0, sizeof(FUSE_STAT));
		if(std::strcmp(path, "/") == 0) {
			stbuf->st_mode  = S_IFDIR | 0755;
			stbuf->st_nlink = 2;
		} else if(std::strcmp(path + 1, "capitalism") == 0) {
			stbuf->st_mode  = S_IFREG | 0444;
			stbuf->st_nlink = 1;
			stbuf->st_size  = std::strlen("capitalism");
		} else
			res = -ENOENT;

		return res;
	};
	ops.readdir = [](const char * path, void * buf, fuse_fill_dir_t filler, FUSE_OFF_T, fuse_file_info *) {
		std::cerr << "readdir(" << path << ")\n";
		if(std::strcmp(path, "/") != 0)
			return -ENOENT;

		filler(buf, ".", nullptr, 0);
		filler(buf, "..", nullptr, 0);
		filler(buf, "capitalism", nullptr, 0);

		return 0;
	};
	ops.open = [](const char * path, fuse_file_info * fi) {
		std::cerr << "open(" << path << ")\n";
		if(std::strcmp(path + 1, "capitalism") != 0)
			return -ENOENT;

		if((fi->flags & 3) != O_RDONLY)
			return -EACCES;

		return 0;
	};
	ops.read = [](const char * path, char * buf, std::size_t size, FUSE_OFF_T offset, fuse_file_info *) {
		std::cerr << "read(" << path << ")\n";
		if(std::strcmp(path + 1, "capitalism") != 0)
			return -ENOENT;

		std::size_t len = std::strlen("capitalism");
		if(offset < (int)len) {
			if(offset + size > len)
				size = len - offset;
			std::memcpy(buf, "capitalism" + offset, size);
		} else
			size = 0;

		return (int)size;
	};

	channel = fuse_mount(cfg.mount_point.c_str(), &arg);
	fs      = fuse_new(channel, &arg, &ops, sizeof(ops), this);
}

filesystem::~filesystem() {
	fuse_unmount(cfg.mount_point.c_str(), channel);
	fuse_destroy(fs);
}
