# Compiler and flags
CC = gcc
CFLAGS = -std=c99 -fstrict-aliasing -fstack-protector -ftrack-macro-expansion=0 \
         -Wall -Wextra -Wdouble-promotion -Wformat=2 -Wlogical-op \
         -Wstrict-aliasing -Wstrict-prototypes -Wno-override-init \
         -Werror -Wno-error=cast-function-type -Wno-format-truncation \
         -Wno-error=format-overflow= -D_POSIX_C_SOURCE=20190101L \
         -ggdb3 -DMALLOC_CHECK_=3 -Og -pthread
LFLAGS = -lm -pthread

# Paths
C_INCLUDE_PATH = external/parson:gtd_fact_tree/inc:gtd_common/inc:gtd_generative/inc:gtd_main_module/inc
BIN_PATH = build/bin
OBJ_PATH = build/obj

# Source files
SRC_MAIN = main.c

SRC_GTD_FACT_TREE = \
	gtd_fact_tree/src/fact_tree.c \
	gtd_fact_tree/src/contradiction.c \
	gtd_fact_tree/src/implication.c \
	gtd_fact_tree/src/fact_tree_machine.c \
	gtd_fact_tree/src/fact_tree_main.c

SRC_GTD_COMMON = \
	gtd_common/src/common.c \
	gtd_common/src/log.c \
	gtd_common/src/fact.c \
	gtd_common/src/function.c \
	gtd_common/src/module_args.c 

SRC_GTD_GENERATIVE = \
	gtd_generative/src/generative_proof_machine.c \
	gtd_generative/src/generative_restriction.c \
	gtd_generative/src/proof_tree.c \
	gtd_generative/src/physical_graph.c \
	gtd_generative/src/generative_main.c

SRC_GTD_MAIN = \
	gtd_main_module/src/json_parser.c \
	gtd_main_module/src/module.c

SRC_TEST = \
	gtd_test/unit_test.c \
	gtd_fact_tree/src/test_fact_tree.c \
	gtd_generative/src/test_physical_graph.c \
	gtd_generative/src/test_generative_proof_machine.c

SRC_PARSON = external/parson/parson.c

# Object files
OBJ_MAIN = $(OBJ_PATH)/main.o

OBJ_GTD_FACT_TREE = $(OBJ_PATH)/fact_tree.o \
                    $(OBJ_PATH)/contradiction.o \
                    $(OBJ_PATH)/implication.o \
                    $(OBJ_PATH)/fact_tree_machine.o \
                    $(OBJ_PATH)/fact_tree_main.o

OBJ_GTD_COMMON = $(OBJ_PATH)/common.o \
                $(OBJ_PATH)/log.o \
                    $(OBJ_PATH)/fact.o \
                    $(OBJ_PATH)/function.o \
					$(OBJ_PATH)/module_args.o 

OBJ_GTD_GENERATIVE = $(OBJ_PATH)/generative_proof_machine.o \
                                   $(OBJ_PATH)/generative_restriction.o \
                                   $(OBJ_PATH)/proof_tree.o \
								   $(OBJ_PATH)/physical_graph.o \
                                   $(OBJ_PATH)/generative_main.o

OBJ_GTD_MAIN = \
	$(OBJ_PATH)/json_parser.o \
	$(OBJ_PATH)/module.o

OBJ_PARSON = $(OBJ_PATH)/parson.o

OBJ_TEST = $(OBJ_PATH)/unit_test.o \
           $(OBJ_PATH)/test_fact_tree.o \
           $(OBJ_PATH)/test_physical_graph.o \
           $(OBJ_PATH)/test_generative_proof_machine.o 

# All object files for main and test
MAIN_OBJ_FILES = $(OBJ_MAIN) $(OBJ_GTD_FACT_TREE) $(OBJ_GTD_COMMON) \
                 $(OBJ_GTD_GENERATIVE) $(OBJ_GTD_MAIN) $(OBJ_PARSON)

TEST_OBJ_FILES = $(OBJ_TEST) $(OBJ_GTD_FACT_TREE) $(OBJ_GTD_COMMON) \
                 $(OBJ_GTD_GENERATIVE) $(OBJ_PARSON)

# Targets
all: $(BIN_PATH)/main

# Create necessary directories
mkdir_dirs:
	mkdir -p $(BIN_PATH) $(OBJ_PATH)

# Link the main executable
$(BIN_PATH)/main: $(MAIN_OBJ_FILES) | mkdir_dirs
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

# Compile main.c
$(OBJ_PATH)/main.o: main.c | mkdir_dirs
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

# Compile gtd_fact_tree sources
$(OBJ_PATH)/fact_tree.o: gtd_fact_tree/src/fact_tree.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/contradiction.o: gtd_fact_tree/src/contradiction.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/implication.o: gtd_fact_tree/src/implication.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/fact_tree_machine.o: gtd_fact_tree/src/fact_tree_machine.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/fact_tree_main.o: gtd_fact_tree/src/fact_tree_main.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

# Compile gtd_common sources
$(OBJ_PATH)/common.o: gtd_common/src/common.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/log.o: gtd_common/src/log.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/fact.o: gtd_common/src/fact.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/function.o: gtd_common/src/function.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/module_args.o: gtd_common/src/module_args.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

# Compile gtd_generative sources
$(OBJ_PATH)/generative_proof_machine.o: gtd_generative/src/generative_proof_machine.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/generative_restriction.o: gtd_generative/src/generative_restriction.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/proof_tree.o: gtd_generative/src/proof_tree.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/physical_graph.o: gtd_generative/src/physical_graph.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/generative_main.o: gtd_generative/src/generative_main.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

# Compile gtd_main_module sources
$(OBJ_PATH)/json_parser.o: gtd_main_module/src/json_parser.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/module.o: gtd_main_module/src/module.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

# Compile external
$(OBJ_PATH)/parson.o: external/parson/parson.c | mkdir_dirs
	$(CC) $(CFLAGS) -c $< -o $@

# Test target
test: $(BIN_PATH)/test

$(BIN_PATH)/test: $(TEST_OBJ_FILES) | mkdir_dirs
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)
	-./$@

# Compile test sources
$(OBJ_PATH)/unit_test.o: gtd_test/unit_test.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/test_fact_tree.o: gtd_fact_tree/src/test_fact_tree.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/test_physical_graph.o: gtd_generative/src/test_physical_graph.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/test_generative_proof_machine.o: gtd_generative/src/test_generative_proof_machine.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ_PATH)/*.o $(BIN_PATH)/*

.PHONY: all test clean
