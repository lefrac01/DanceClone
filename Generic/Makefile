#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

# Directories.
OBJ_DIR			:= obj
LIB_DIR			:= lib
BIN_DIR			:= bin
#SRC_DIR			:= src src/video src/audio
SRC_DIR			:= src
INCLUDE_DIR		:= include
TEST_OBJ_DIR		:= $(OBJ_DIR)/test
TEST_SRC_DIR		:= src
INSTALL_HEADER_DIR	?= $(LIBOGC_INC)
INSTALL_LIB_DIR		?= $(LIBOGC_LIB)

# Tools.
PIPE_TO_SED := 2>&1 | sed "s/:\([0-9]*\):/\(\1\) :/"

# Library source files.
#SRCS	:= $(wildcard $(SRC_DIR)/*.c)
SRCS    := $(foreach dir,$(SOURCES),$(notdir $(filter-out $(dir)/glmovie.c $(dir)/glmovie-tile.c $(dir)/gtv.c $(dir)/plaympeg.c,$(wildcard $(dir)/*.c))))

# Library object files.
OBJS	:= $(subst $(SRC_DIR),$(OBJ_DIR),$(SRCS:.c=.o))

# What's a full build?
all: $(LIB_DIR)/libsmpeg.a headers

# How to delete the intermediate files.
clean:
	@echo Cleaning $(LIB_DIR)/libsmpeg.a $(OBJ_DIR)
	@rm -f $(LIB_DIR)/libsmpeg.a $(OBJS) $(TEST_OBJS)

# How to build a library.
$(LIB_DIR)/libsmpeg.a: $(OBJS)
	@echo Archiving $@
	@-mkdir -p $(dir $@)
	@powerpc-eabi-ar crs $@ $(OBJS)
	@echo ----
	
install: all
	@mkdir -p $(INSTALL_HEADER_DIR) $(INSTALL_LIB_DIR)
	@mkdir -p $(INSTALL_HEADER_DIR)/SDL
	@cp -frv $(LIB_DIR)/*.* $(INSTALL_LIB_DIR)
	@cp -frv include/*.* $(INSTALL_HEADER_DIR)/SDL

# How to copy the header file
SOURCES		:=	src src/video src/audio
FILTERED_HEADERS:=	$(foreach dir,$(SOURCES),$(filter-out $(dir)/glmovie.h $(dir)/gtv.h,$(wildcard $(dir)/*.h)))

headers: 
	@echo Copy filtered headers to $(INCLUDE_DIR)
	@-mkdir -p $(INCLUDE_DIR)
#	@cp $(wildcard $(SRC_DIR)/*.h) $(INCLUDE_DIR)
	@cp $(FILTERED_HEADERS) $(INCLUDE_DIR)



# Compilation flags.
COMMON_FLAGS	:= -g -O3 -mrvl -Wall $(MACHDEP)
INCLUDES		:= -Iinclude -I$(LIBOGC_INC) -I$(LIBOGC_INC)/SDL -I$(SRC_DIR)/../../SDL/include
DEFINES			:= -DGEKKO -DDEBUG_ERROR -DDEBUG_TIMERS -DDEBUG_THREADS -DDEBUG_BUILD -DDEBUG_CONVERT \
			   -DDISABLE_VIDEO_CALLBACK_THREAD -DNOCONTROLS
CFLAGS			:= $(COMMON_FLAGS) $(INCLUDES) $(DEFINES) 


# How to compile C file (SDL library).
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo Compiling $<
	@-mkdir -p $(dir $@)
	@powerpc-eabi-gcc $(CFLAGS) -c $< -o $@ $(PIPE_TO_SED)
