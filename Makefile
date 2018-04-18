

#
# 'make depend' uses makedepend to automatically generate dependencies
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

SIM ?= 0
DEBUG ?= 0
ARCH = $(shell uname -m)
ROOT = /home/mjalvar/Projects/LED/source
CVFLAGS = `pkg-config --cflags opencv` `pkg-config --libs opencv`

# define the C compiler to use
CC = g++

# define the executable file
MAIN = morpho

# define any compile-time flags
CFLAGS = -Wall -g -std=c++11
ifneq ($(ARCH),x86_64)
    CFLAGS += -O3 -DRPI
    BCMFLAGS = -l bcm2835
    ROOT = /home/pi/morpho
endif


# define any directories containing header files other than /usr/include
#
INCLUDES = -I$(ROOT)/VirtualDisplay -I$(ROOT)/MorphoEffect -I$(ROOT)/MorphoColor -I$(ROOT)/LedDisplay -I$(ROOT)/CmdParser -I$(ROOT)/GtkDisplay -I$(ROOT)/MorphoWindow -I$(ROOT)/MorphoWorker -I$(ROOT)

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
# LFLAGS = -L/home/newhall/lib  -L../lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
# LIBS = -lmylib -lm

# define the C source files
SRCS = MorphoColor/MorphoColor.cpp MorphoWorker/MorphoWorker.cpp VirtualDisplay/VirtualDisplay.cpp \
    MorphoEffect/MorphoEffect.cpp \
    MorphoEffect/LineEffect.cpp \
    MorphoEffect/ColorEffect.cpp \
    MorphoEffect/FadeInOutEffect.cpp \
    LedDisplay/LedDisplay.cpp CmdParser/CmdParser.cpp main.cpp

# define the C object files
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.cpp=.o)


ifeq ($(SIM),1)
	GTKFLAGS = `pkg-config --cflags gtkmm-3.0` `pkg-config --libs gtkmm-3.0`
	CFLAGS += -DMORPHO_GTK
	SRCS += GtkDisplay/GtkDisplay.cpp MorphoWindow/MorphoWindow.cpp
	MAIN = morpho_sim
endif

ifeq ($(DEBUG),1)
	CFLAGS += -ggdb
endif


#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean 

all:    depend $(MAIN)


$(MAIN): $(OBJS) 
		$(CC) $(CFLAGS) $(CVFLAGS) $(GTKFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS) $(BCMFLAGS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o: 
		@echo compiling $<
		$(CC) $(CFLAGS) $(CVFLAGS) $(GTKFLAGS) $(INCLUDES) -c $< $(BCMFLAGS) -o $@
# $(OBJS): 
# 		@echo compiling $<
# 		$(CC) $(CFLAGS) $(CVFLAGS) $(GTKFLAGS) $(INCLUDES) -c $<  -o $@		

clean:
		rm -f $(ROOT)/*.o $(ROOT)/*/*.o $(MAIN)*

depend: $(SRCS)
		makedepend $(INCLUDES) $^ -f makedepend.$(MAIN)
#		makedepend $(INCLUDES) $^


# DO NOT DELETE THIS LINE -- make depend needs it
include makedepend.$(MAIN)
