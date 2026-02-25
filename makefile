####################################################
#                                                  #
#  Men became scientific because they expected     #
#  law in nature, and they expected law in nature  #
#  because they believed in a Legislator.          #
#                                                  #
#      ~ C.S. Lewis                                #
#                                                  #
####################################################

# Specify output binary name:
TARGET			:= minheap



INCLUDES		:= include

LIBS			:=
LIBDIRS			:=

SOURCES			:= src
BUILD			:= build

export INCLUDE	:= $(foreach dir, $(INCLUDES), -I$(dir))

SRCS			:= $(foreach f, $(SOURCES), $(wildcard $(f)/*.c))

FILES			:= $(CFILES:.c=)

export OBJS		:= $(foreach f, $(SRCS), $(addprefix $(BUILD)/, $(f:.c=.o)))
DEPENDS			:= $(wildcard ../$(BUILD)/*.d)

LDFLAGS			:=



VPATH			 = $(foreach dir, $(SOURCES), ../$(dir)) $(foreach dir, $(INCLUDES), $(dir))

OPTFLAGS		:= -Ofast -ffast-math \
				   -ffunction-sections \
				   -fmerge-all-constants -Wl,--gc-sections \
				   -flto -fomit-frame-pointer \

DEBUGOPTFLAGS	:= -g

NO_WARNINGS		:= -Wno-unused-result

CFLAGS		:= $(INCLUDE) $(OPTFLAGS) $(NO_WARNINGS) -std=c17 -MMD -MP -Wall



.PHONY: all dev clean



all: $(TARGET)

-include $(DEPENDS)

$(OBJS): $(BUILD)/%.o : %.c | $(BUILD) $(BUILD)/src
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -MMD -MF $(@:.o=.d) -c -o $@

$(TARGET): $(OBJS) | $(BUILD)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(BUILD) $(BUILD)/src:
	mkdir -p $@

clean:
	@echo clean ...
	
	@rm -f $(TARGET)
	
	@rm -rf $(BUILD)

run:
	@./$(TARGET)
