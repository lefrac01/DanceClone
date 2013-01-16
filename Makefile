#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
#ifeq ($(strip $(DEVKITPPC)),)
#$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
#endif


export CC	:=	gcc
export CXX	:=	g++

#---------------------------------------------------------------------------------
%.o: %.cpp
	@echo $(notdir $<)
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) -c $< -o $@ $(ERROR_FILTER)
	
#---------------------------------------------------------------------------------
%.o: %.c
	@echo $(notdir $<)
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c $< -o $@ $(ERROR_FILTER)


#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	dance_night
BUILD		:=	build
SOURCES		:=	Source Source/Game Source/Platform Source/GUI Source/Dash Source/Platform/Linux
DATA		:=	data  
INCLUDES	:=	

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------

# -mepsilon should go here, but is giving an error ???  (floating point comparison)
#RELEASE
#CFLAGS	= -O2 -Wall $(INCLUDE)

#debug
CFLAGS	= -g -O1 -Wall $(INCLUDE)
CXXFLAGS	=	$(CFLAGS)

LDFLAGS	=	-g -Wl,-Map,$(notdir $@).map

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
#LIBS	:=	-lSDL_net -lSDL_ttf -lSDL_gfx -lSDL_mixer -lSDL_image -lSDL \
#                -ljpeg -lpng -lfreetype -lvorbisidec \
#                -lz -lasnd -lm -lmad -lfat -lwiiuse -lbte -logc -lm -lwiikeyboard

LIBS	:=	-lSDL_net -lSDL_ttf -lSDL_gfx -lSDL_mixer -lSDL_image -lSDL \
                -ljpeg -lpng -lfreetype \
                -lz -lm 

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) \
					$(sFILES:.s=.o) $(SFILES:.S=.o)

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
#export INCLUDE	:=	$(foreach dir,$(INCLUDES), -iquote $(CURDIR)/$(dir)) \
#					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
#					-I$(CURDIR)/$(BUILD) \
#					-I$(BOOST_INC)
export INCLUDE	:=	$(foreach dir,$(INCLUDES), $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD) \
					-I$(BOOST_INC) \
					-I$(SDL_INC)

					
#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
				-L/usr/lib/

export OUTPUT	:=	$(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@echo CPPFILES: $(CPPFILES) found in source dirs: $(SOURCES)
	@echo INCLUDES: $(INCLUDE)
	@echo OFILES: $(OFILES)
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
all: 
	$(MAKE) -f Makefile
#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT)
	@rm debug


#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
#$(OUTPUT): main.o
#main.o: main.cpp

$(OUTPUT): $(OFILES)
	@echo linking ... $(notdir $@)
	@echo LDFLAGS: $(LDFLAGS)
	@echo LIBPATHS: $(LIBPATHS)
	@echo LIBS: $(LIBS)
	@echo LD_LIBRARY_PATH: $(LD_LIBRARY_PATH)
	@echo ld targets: $^
	@$(LD)  $^ $(LDFLAGS) $(LIBPATHS) $(LIBS) -o $@

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
