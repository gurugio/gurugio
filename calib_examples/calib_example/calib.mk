

# making calib
# There values are used by every Makefile
# so that they are defined here.
CALIB_SRCS = sys_info.c build_info.c
CALIB_OBJS = $(CALIB_SRCS:.c=.o)



#
# build option
#

# BUILD_MODE setup
# eg) make build_mode=debug
AVAILABLE_BUILD_MODE = debug release
DEFAULT_BUILD_MODE = debug

ifneq ($(DEFAULT_BUILD_MODE),)
 ifeq ($(origin build_mode), undefined)
  BUILD_MODE = $(DEFAULT_BUILD_MODE)
 else
  BUILD_MODE = $(filter $(build_mode), $(AVAILABLE_BUILD_MODE))
  ifneq ($(BUILD_MODE), $(build_mode))
   $(error Unknown build mode: $(build_mode))
  endif
 endif
endif

# COMPILE_BIT setup
# eg) make compile_bit=64
# AVAILABLE_COMPILE_BIT, DEFAULT_COMPILE_BIT are setup at config.mk
AVAILABLE_COMPILE_BIT=64 32
DEFAULT_COMPILE_BIT=64

ifneq ($(DEFAULT_COMPILE_BIT),)
 ifeq ($(origin compile_bit), undefined)
  COMPILE_BIT = $(DEFAULT_COMPILE_BIT)
 else
  COMPILE_BIT = $(filter $(compile_bit), $(AVAILABLE_COMPILE_BIT))
  ifneq ($(COMPILE_BIT), $(compile_bit))
   $(error Unknown compile bit: $(compile_bit))
  endif
 endif
else
 $(error No setting for compile bit: $(DEFAULT_COMPILE_BIT))
endif

# UNITTEST_MODE setup
# eg) make unittest_mode=y
AVAILABLE_UNITTEST_MODE = y n
DEFAULT_UNITTEST_MODE = y

ifneq ($(DEFAULT_UNITTEST_MODE),)
 ifeq ($(origin unittest_mode), undefined)
  UNITTEST_MODE = $(DEFAULT_UNITTEST_MODE)
 else
  UNITTEST_MODE = $(filter $(unittest_mode), $(AVAILABLE_UNITTEST_MODE))
  ifneq ($(UNITTEST_MODE), $(unittest_mode))
   $(error Unknown unittest mode: $(unittest_mode))
  endif
 endif
endif

#
# Silent mode
#
ifneq ($(findstring s,$(filter-out -%,$(MAKEFLAGS))),)
 SILENT_MODE = 1
 ifneq ($(MAKELEVEL),0)
  ifeq ($(findstring w,$(filter-out -%,$(MAKEFLAGS))),)
   MAKEFLAGS := w$(MAKEFLAGS)
  endif
 endif
endif


#
# directory setting
#
ROOT_DIR := /home/gurugio/caoskernel/branches/calib_examples/calib_example
LIB_DIR = $(ROOT_DIR)/lib
SRC_DIR = $(ROOT_DIR)/src
TEST_DIR = $(ROOT_DIR)/test

VARIANT = $(BUILD_MODE)-$(COMPILE_BIT)
BUILD_ROOT = $(ROOT_DIR)/build/$(VARIANT)

# dir to store build targets
CUR_DIR = $(shell pwd)
# CALIB_HOME=$(ROOT_DIR)
# WORK_DIR = $(subst $(CALIB_HOME),,$(CUR_DIR))
WORK_DIR = $(subst $(ROOT_DIR),,$(CUR_DIR))
BUILD_DIR = $(BUILD_ROOT)$(WORK_DIR)# WORK_DIR starts with /
BUILD_SRC_DIR = $(BUILD_ROOT)/src
BUILD_LIB_DIR = $(BUILD_ROOT)/lib
BUILD_TEST_DIR = $(BUILD_ROOT)/test

SUB_DIRS = src lib test


#
# compile option
#
CC=gcc
LD=ld

INCLUDES += $(ROOT_DIR)/include

DEFINES += CALIB_CFG_BUILD_MODE=\"$(BUILD_MODE)\"

ifeq ($(BUILD_MODE), debug)
 DEFINES += CALIB_CFG_BUILD_MODE_DEBUG
else
 ifeq ($(BUILD_MODE), release)
  DEFINES += CALIB_CFG_BUILD_MODE_RELEASE
 endif
endif

DEFINES += CALIB_CFG_COMPILE_BIT=$(COMPILE_BIT)
DEFINES += CALIB_CFG_COMPILE_BIT_$(COMPILE_BIT)

INCLUDE_OPT = $(addprefix -I,$(INCLUDES))
DEFINE_OPT = $(addprefix -D,$(DEFINES))

CFLAGS += -Wextra -Wall

ifeq ($(BUILD_MODE), debug)
 CFLAGS += -g
else
 CFLAGS += -O2
endif

ifeq ($(COMPILE_BIT), 64)
 CFLAGS += -m64
else
 CFLAGS += -m32
endif

ifeq ($(COMPILE_BIT), 64)
 LD_FLAGS += -m64
else
 LD_FLAGS += -m32
endif

