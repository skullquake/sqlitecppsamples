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
SRC:=$(shell find $(SRCDIR) -name '*.cpp')
OBJS:=$(addprefix $(BUILDDIR)/,$(SRC:%.cpp=%.o))
CXXFLAGS+=\
	-I./include
LDFLAGS+=\
	-L./lib\
	-std=c++11\
	-lSQLiteCpp\
	-lsqlite3
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
.phony:\
	run\
	clean\
	lsvar
clean:
	@rm -r ./bin
run:
	$(BIN)
lsvar:
	@echo $(BIN)
	@echo $(OBJS)
	@echo $(SRC)
	@echo $(SRCDIR)
	@echo $(PREFIX)




