# The MIT License (MIT)

# Copyright (c) 2016 нabijaczleweli

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


include configMakefile


LDAR := $(LNCXXAR) $(shell pkg-config fuse --libs-only-l --libs-only-L)
INCAR := $(foreach l,tclap cpptoml,-isystemext/$(l)/include)
VERAR := $(foreach l,CTFFS,-D$(l)_VERSION='$($(l)_VERSION)')
SOURCES := $(wildcard $(SRCDIR)*.cpp)


.PHONY : clean all exe

all : exe

clean :
	rm -rf $(OUTDIR)

exe :  $(OUTDIR)ctffs$(EXE)

$(OUTDIR)ctffs$(EXE) : $(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,$(OBJ),$(SOURCES)))
	$(CXX) $(CXXAR) -o$@ $^ $(PIC) $(LDAR)


$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) $(VERAR) -D_FILE_OFFSET_BITS=$(BITNESS) -DFUSE_USE_VERSION=26 -c -o$@ $^
