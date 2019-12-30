OS=$(shell $(CXX) -dumpmachine)
SOEXT?=so
BINEXT?=out
ifeq (,$(findstring mingw,$(OS)))
SOEXT?=dll
BINEXT?=exe
endif
ifeq (,$(findstring djgpp,$(OS)))
SOEXT?=dll
BINEXT?=exe
endif
ifeq ($(PREFIX),)
PREFIX := /usr/local
endif
CC?=gcc
CXX?=g++
AR?=ar
BUILDDIR=./bin/build
BIN=./bin/a.$(BINEXT)
SRCDIR=src
SRC:=$(shell find $(SRCDIR) -name "*.cpp" -o -name "*.c")
OBJS=$(addprefix $(BUILDDIR)/,$(patsubst %.c,%.o ,$(patsubst %.cpp,%.o,$(SRC))))
CXXFLAGS+=\
	-I./include\
	-std=c++11
CFLAGS+=\
	-I./include
LDFLAGS+=\
	-L./lib
all:\
	$(BIN)
$(BIN):$(OBJS)
	@printf "generating $@...\n"
	$(CXX)\
		$(CXXFLAGS)\
		$(OBJS)\
		$(LDFLAGS)\
		-o $(BIN)
$(BUILDDIR)/%.o: %.cpp
	@printf "generating $@...\n"
	@mkdir -p $(@D)
	$(CXX)\
		$(CXXFLAGS)\
		-c $<\
		$(LDFLAGS)\
		-o $@
$(BUILDDIR)/%.o: %.c
	@printf "generating $@...\n"
	@mkdir -p $(@D)
	$(CC)\
		$(CFLAGS)\
		-c $<\
		$(LDFLAGS)\
		-o $@

.phony:\
	run\
	clean\
	lsvar
clean:
	@rm -r ./bin
run: $(BIN)
	$(BIN)
lsvar:
	@echo $(BIN)
	@echo $(OBJS)
	@echo $(SRC)
	@echo $(SRCDIR)
	@echo $(PREFIX)




