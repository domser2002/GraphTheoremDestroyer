# Compiler and flags
CC = gcc
CFLAGS = -std=c99 -fstrict-aliasing -fstack-protector -ftrack-macro-expansion=0 \
         -Wall -Wextra -Wdouble-promotion -Wformat=2 -Wlogical-op \
         -Wstrict-aliasing -Wstrict-prototypes -Wno-override-init \
         -Werror -Wno-error=cast-function-type -Wno-format-truncation \
         -Wno-error=format-overflow= -D_POSIX_C_SOURCE=20190101L \
         -ggdb3 -DMALLOC_CHECK_=3 -Og -pthread
LFLAGS = -lm

# Paths
C_INCLUDE_PATH = gtd_fact_tree/inc:gtd_common/inc:gtd_generative_proof_machine/inc
BIN_PATH = build/bin
OBJ_PATH = build/obj

# Source files
SRC_MAIN = main.c

SRC_GTD_FACT_TREE = \
	gtd_fact_tree/src/fact_tree.c \
	gtd_fact_tree/src/fact.c \
	gtd_fact_tree/src/contradiction.c \
	gtd_fact_tree/src/implication.c \
	gtd_fact_tree/src/fact_tree_machine.c \
	gtd_fact_tree/src/function.c

SRC_GTD_COMMON = \
	gtd_common/src/common.c \
	gtd_common/src/log.c

SRC_GTD_GENERATIVE_PROOF_MACHINE = \
	gtd_generative_proof_machine/src/generative_proof_machine.c \
	gtd_generative_proof_machine/src/generative_restriction.c \
	gtd_generative_proof_machine/src/proof_tree.c \
	gtd_generative_proof_machine/src/physical_graph.c

SRC_TEST = \
	gtd_test/unit_test.c \
	gtd_fact_tree/src/test_fact_tree.c \
	gtd_generative_proof_machine/src/test_physical_graph.c \
	gtd_generative_proof_machine/src/test_generative_proof_machine.c

# Object files
OBJ_MAIN = $(OBJ_PATH)/main.o

OBJ_GTD_FACT_TREE = $(OBJ_PATH)/fact_tree.o \
                    $(OBJ_PATH)/fact.o \
                    $(OBJ_PATH)/contradiction.o \
                    $(OBJ_PATH)/implication.o \
                    $(OBJ_PATH)/fact_tree_machine.o \
                    $(OBJ_PATH)/function.o 

OBJ_GTD_COMMON = $(OBJ_PATH)/common.o \
                $(OBJ_PATH)/log.o

OBJ_GTD_PHYSICAL_GRAPH = $(OBJ_PATH)/physical_graph.o

OBJ_GTD_GENERATIVE_PROOF_MACHINE = $(OBJ_PATH)/generative_proof_machine.o \
                                   $(OBJ_PATH)/generative_restriction.o \
                                   $(OBJ_PATH)/proof_tree.o

OBJ_TEST = $(OBJ_PATH)/unit_test.o \
           $(OBJ_PATH)/test_fact_tree.o \
           $(OBJ_PATH)/test_physical_graph.o \
           $(OBJ_PATH)/test_generative_proof_machine.o

# All object files for main and test
MAIN_OBJ_FILES = $(OBJ_MAIN) $(OBJ_GTD_FACT_TREE) $(OBJ_GTD_COMMON) \
                 $(OBJ_GTD_PHYSICAL_GRAPH) $(OBJ_GTD_GENERATIVE_PROOF_MACHINE)

TEST_OBJ_FILES = $(OBJ_TEST) $(OBJ_GTD_FACT_TREE) $(OBJ_GTD_COMMON) \
                 $(OBJ_GTD_PHYSICAL_GRAPH) $(OBJ_GTD_GENERATIVE_PROOF_MACHINE)

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

$(OBJ_PATH)/fact.o: gtd_fact_tree/src/fact.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/contradiction.o: gtd_fact_tree/src/contradiction.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/implication.o: gtd_fact_tree/src/implication.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/fact_tree_machine.o: gtd_fact_tree/src/fact_tree_machine.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/function.o: gtd_fact_tree/src/function.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

# Compile gtd_common sources
$(OBJ_PATH)/common.o: gtd_common/src/common.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/log.o: gtd_common/src/log.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

# Compile gtd_physical_graph sources
$(OBJ_PATH)/physical_graph.o: gtd_generative_proof_machine/src/physical_graph.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

# Compile gtd_generative_proof_machine sources
$(OBJ_PATH)/generative_proof_machine.o: gtd_generative_proof_machine/src/generative_proof_machine.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/generative_restriction.o: gtd_generative_proof_machine/src/generative_restriction.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/proof_tree.o: gtd_generative_proof_machine/src/proof_tree.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

# Test target
test: $(BIN_PATH)/test

$(BIN_PATH)/test: $(TEST_OBJ_FILES) | mkdir_dirs
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)
	./$@

# Compile test sources
$(OBJ_PATH)/unit_test.o: gtd_test/unit_test.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/test_fact_tree.o: gtd_fact_tree/src/test_fact_tree.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/test_physical_graph.o: gtd_generative_proof_machine/src/test_physical_graph.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)/test_generative_proof_machine.o: gtd_generative_proof_machine/src/test_generative_proof_machine.c
	C_INCLUDE_PATH=$(C_INCLUDE_PATH) $(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ_PATH)/*.o $(BIN_PATH)/*

.PHONY: all test clean
