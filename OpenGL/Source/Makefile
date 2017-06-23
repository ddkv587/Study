TARGET_MODULE		:= OpenGL
PROJECT_ROOT_PATH	:= $(shell pwd)

CPP_SRCDIR 			:= $(PROJECT_ROOT_PATH)
CPP_SRC 			:= $(shell find $(CPP_SRCDIR) -name "*.cpp")

RM 					:= rm -rf
ARCH 				:= /usr/bin/
CPP 				:= $(ARCH)g++

PLATFROM_MACRO 		:= OS_LINUX

CPP_TARGETDIR 		:= $(PROJECT_ROOT_PATH)/out
CPP_OBJECT_MIDDLE 	:= $(PROJECT_ROOT_PATH)/im/

CFLAGS				:= -std=c++11 -MMD -MP

CPP_APP_OBJECT 		:= $(foreach filename,$(CPP_SRC:%.cpp=%.o),$(subst $(CPP_SRCDIR),$(CPP_OBJECT_MIDDLE),$(filename)))
CPP_APP_D 			:= $(foreach filename,$(CPP_SRC:%.cpp=%.d),$(subst $(CPP_SRCDIR),$(CPP_OBJECT_MIDDLE),$(filename)))


CPP_INCLUDE			= -I$(PROJECT_ROOT_PATH)/Glad 

LIBS 				= $(shell pkg-config --libs glew) -lglfw

RPATH				+= -Wl,-rpath . 

all: $(TARGET_MODULE)

$(CPP_APP_OBJECT): $(CPP_OBJECT_MIDDLE)%.o: $(CPP_SRCDIR)%.cpp
	@echo $(notdir $<)
	@mkdir -p $(dir $@)
	@$(CPP) -I$(CPP_OBJECT_MIDDLE) $(CPP_INCLUDE) $(CFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" -c "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(TARGET_MODULE): $(CPP_APP_OBJECT)
	@echo '> Linking: $@'
	@$(CPP) -o $(TARGET_MODULE) $(CPP_APP_OBJECT) $(LIBSDIR) $(LIBS) $(RPATH)
	@mkdir -p $(CPP_TARGETDIR)
	cp -f $(TARGET_MODULE) $(CPP_TARGETDIR)/$(TARGET_MODULE)
	@-$(RM) $(TARGET_MODULE)

clean:
	-$(RM) $(CPP_APP_OBJECT) 
	-$(RM) $(CPP_APP_D)
	-$(RM) $(CPP_OBJECT_MIDDLE)/lib$(TARGET_MODULE).so
	-$(RM) $(CPP_TARGETDIR)/lib$(TARGET_MODULE).so
	-$(RM) $(PCH) $(PCH:%.gch=%.d)

.PHONY: all clean
