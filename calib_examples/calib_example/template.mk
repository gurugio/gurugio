#
# templates for all build types
#

#
# 0. naming convention
#
OBJ_SUF=.o
LIB_PRE=lib
LIB_SUF=.a



#
# 1. define howto build
#


# obj_template: make obj files
# A label has object file names that will be linked together
# so they must be built together
# $(1) : label name
# $(2) : obj file names
define obj_template
.PHONY: build_objs
build_objs:$(BUILD_DIR) $(2)
endef


# lib_template: link objects to generate static-library file
# $(1) : label name
# $(2) : library file names
define lib_template
.PHONY: build_libs
build_libs: $(BUILD_DIR) $(2)
$(2): $(patsubst %,$(BUILD_SRC_DIR)/%,$($(1)_OBJS))
ifneq ($(SILENT_MODE),)
	@echo "Linking [$$@]"
endif
	ar rc $$@ $(patsubst %,$(BUILD_SRC_DIR)/%,$($(1)_OBJS))
endef


# build unittest and run it
# $(1): target label
# $(2): test file name
define unittest_template
ifneq ($(UNITTEST_MODE),n)
.PHONY: run_unittest
run_unittest: $(BUILD_DIR) $(BUILD_DIR)/$(1).unittest
$(BUILD_DIR)/$(1).unittest: $(2) # new execute-file -> run again
	@echo ""
	@echo "testing [$(1)]"
	@$(2) && touch $$@
	@echo ""
$(2): $(patsubst %,$(BUILD_DIR)/%$(OBJ_SUF),$(basename $($(1)_SRCS))) $(BUILD_LIB_DIR)/$(LIB_PRE)$($(1)_LIBS)
	$(CC) $(LD_FLAGS) -o $$@ $$< $(BUILD_LIB_DIR)/$(LIB_PRE)$($(1)_LIBS)
endif
endef



#
# 2. expand template to define build rules
#


# build object files
# $($(target)_SRCS) is source file names
# (patsubst %,...) make source file names into dir/obj-names
# eg) src/a.c -> build/src/a.o
$(foreach target, $(OBJ_TARGETS), $(eval $(call obj_template,$(target),$(patsubst %,$(BUILD_DIR)/%$(OBJ_SUF),$(basename $($(target)_SRCS))))))

# build library
$(foreach target, $(LIB_TARGETS), $(eval $(call lib_template,$(target),$(BUILD_DIR)/$(LIB_PRE)$($(target)_NAME)$(LIB_SUF))))

# build unittest program
$(foreach target, $(UNITTEST_TARGETS), $(eval $(call unittest_template,$(target),$(BUILD_DIR)/$($(target)_NAME))))



#
# 3. implicit rules for compile sources
#


# .c files

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o: %.c
ifneq ($(SILENT_MODE),)
	echo "Compile $(BUILD_DIR)/$(notdir $@)"
endif
	$(CC) -c $(CFLAGS) $(INCLUDE_OPT) $(DEFINE_OPT) -o $@ $<

# rules for other language files can be defined here...


clean:
	rm -rf $(BUILD_DIR)



