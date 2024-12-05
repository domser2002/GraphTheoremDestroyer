# Compiler and Flags
CC = gcc
CFLAGS = -std=c99 -fstrict-aliasing -fstack-protector \
         -ftrack-macro-expansion=0 -Wall -Wextra \
         -Wdouble-promotion -Wformat=2 -Wlogical-op \
         -Wstrict-aliasing -Wstrict-prototypes -Wno-override-init \
         -Werror -Wno-error=cast-function-type -Wno-format-truncation \
         -Wno-error=format-overflow= -D_POSIX_C_SOURCE=20190101L \
         -ggdb3 -DMALLOC_CHECK_=3 -Og -pthread

# Libraries
LFLAGS = -lm

# Include Directories
INCLUDES = \
    -I gtd_fact_tree/inc \
    -I gtd_log/inc \
    -I gtd_common/inc \
    -I gtd_proof_machine/inc \
    -I gtd_physical_graph/inc \
    -I gtd_generative_proof_machine/inc

# Append includes to CFLAGS
CFLAGS += $(INCLUDES)

# Paths
BIN_PATH = build/bin
OBJ_PATH = build/obj

# Source and Object Files
MAIN_OBJ_FILES = main.o graph.o fact_tree.o fact.o contradiction.o \
                 implication.o log.o common.o machine.o physical_graph.o \
                 generative_proof_machine.o fact_tree_machine.o proof_tree.o

TEST_OBJ_FILES = unit_test.o test_fact_tree.o fact.o contradiction.o \
                 implication.o common.o log.o test_physical_graph.o \
                 physical_graph.o test_generative_proof_machine.o \
                 generative_proof_machine.o generative_restriction.o proof_tree.o

MAIN_OBJ_FILES_WITH_PATH = $(addprefix $(OBJ_PATH)/, $(MAIN_OBJ_FILES))
TEST_OBJ_FILES_WITH_PATH = $(addprefix $(OBJ_PATH)/, $(TEST_OBJ_FILES))

# Targets
all: mkdir_setup fact_tree log common proof_machine physical_graph generative_proof_machine proof_tree main

# Create necessary directories
mkdir_setup:
	@mkdir -p $(BIN_PATH)
	@mkdir -p $(OBJ_PATH)

# Compile Fact Tree Module
fact_tree: mkdir_setup
	$(CC) $(CFLAGS) -c gtd_fact_tree/src/fact_tree.c -o $(OBJ_PATH)/fact_tree.o
	$(CC) $(CFLAGS) -c gtd_fact_tree/src/fact.c -o $(OBJ_PATH)/fact.o
	$(CC) $(CFLAGS) -c gtd_fact_tree/src/contradiction.c -o $(OBJ_PATH)/contradiction.o
	$(CC) $(CFLAGS) -c gtd_fact_tree/src/implication.c -o $(OBJ_PATH)/implication.o
	$(CC) $(CFLAGS) -c gtd_fact_tree/src/fact_tree_machine.c -o $(OBJ_PATH)/fact_tree_machine.o

# Compile Log Module
log: mkdir_setup
	$(CC) $(CFLAGS) -c gtd_common/src/common.c -o $(OBJ_PATH)/common.o
	$(CC) $(CFLAGS) -c gtd_common/src/log.c -o $(OBJ_PATH)/log.o

# Placeholder for Common Module (Add additional steps if necessary)
common: mkdir_setup
	# Add any additional common build steps here if necessary

# Compile Proof Machine Module
proof_machine: mkdir_setup
	$(CC) $(CFLAGS) -c gtd_proof_machine/src/proof_machine.c -o $(OBJ_PATH)/proof_machine.o

# Compile Physical Graph Module
physical_graph: mkdir_setup
	$(CC) $(CFLAGS) -c gtd_physical_graph/src/physical_graph.c -o $(OBJ_PATH)/physical_graph.o

# Compile Generative Proof Machine Module
generative_proof_machine: mkdir_setup
	$(CC) $(CFLAGS) -c gtd_generative_proof_machine/src/generative_proof_machine.c -o $(OBJ_PATH)/generative_proof_machine.o
	$(CC) $(CFLAGS) -c gtd_generative_proof_machine/src/generative_restriction.c -o $(OBJ_PATH)/generative_restriction.o
	$(CC) $(CFLAGS) -c gtd_generative_proof_machine/src/proof_tree.c -o $(OBJ_PATH)/proof_tree.o

# Placeholder for Proof Tree (Add additional steps if necessary)
proof_tree: generative_proof_machine
	# Add any additional proof_tree build steps here if necessary

# Compile Main Executable
main: mkdir_setup
	$(CC) $(CFLAGS) -c main.c -o $(OBJ_PATH)/main.o
	$(CC) $(CFLAGS) -o $(BIN_PATH)/main $(MAIN_OBJ_FILES_WITH_PATH) $(OBJ_PATH)/generative_restriction.o $(LFLAGS)

# Compile and Run Tests
test: all
	$(CC) $(CFLAGS) -c gtd_test/unit_test.c -o $(OBJ_PATH)/unit_test.o
	$(CC) $(CFLAGS) -o $(BIN_PATH)/test $(TEST_OBJ_FILES_WITH_PATH) $(LFLAGS)
	./$(BIN_PATH)/test

# Compile Fact Tree Tests
fact_tree_test: mkdir_setup
	$(CC) $(CFLAGS) -c gtd_fact_tree/src/test_fact_tree.c -o $(OBJ_PATH)/test_fact_tree.o

# Compile Physical Graph Tests
physical_graph_test: mkdir_setup
	$(CC) $(CFLAGS) -c gtd_physical_graph/src/test_physical_graph.c -o $(OBJ_PATH)/test_physical_graph.o

# Compile Generative Proof Machine Tests
generative_proof_machine_test: mkdir_setup
	$(CC) $(CFLAGS) -c gtd_generative_proof_machine/src/test_generative_proof_machine.c -o $(OBJ_PATH)/test_generative_proof_machine.o

# Clean Build Directories
clean:
	rm -rf $(BIN_PATH)/* || true
	rm -rf $(OBJ_PATH)/* || true
