CC = gcc
CFLAGS = -std=c99 -fstrict-aliasing -fstack-protector -ftrack-macro-expansion=0 -Wall -Wextra -Wdouble-promotion -Wformat=2 -Wlogical-op -Wstrict-aliasing -Wstrict-prototypes -Wno-override-init -Werror -Wno-error=cast-function-type -Wno-format-truncation -Wno-error=format-overflow= -D_POSIX_C_SOURCE=20190101L -ggdb3 -DMALLOC_CHECK_=3 -Og
LFLAGS = -lm
C_INCLUDE_PATH = gtd_fact_tree/inc:gtd_log/inc:gtd_common/inc:gtd_proof_machine/inc:gtd_physical_graph/inc:gtd_generative_proof_machine/inc
BIN_PATH = build/bin
OBJ_PATH = build/obj
MAIN_OBJ_FILES = main.o graph.o fact.o contradiction.o implication.o log.o common.o machine.o physical_graph.o generative_proof_machine.o
TEST_OBJ_FILES = unit_test.o test_fact_tree.o fact.o contradiction.o implication.o common.o log.o test_physical_graph.o physical_graph.o
MAIN_OBJ_FILES_WITH_PATH = $(addprefix ${OBJ_PATH}/, ${MAIN_OBJ_FILES})
TEST_OBJ_FILES_WITH_PATH = $(addprefix ${OBJ_PATH}/, ${TEST_OBJ_FILES})

all: fact_tree log common proof_machine physical_graph generative_proof_machine
	mkdir ${BIN_PATH} || true
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c main.c -o ${OBJ_PATH}/main.o
	${CC} ${CFLAGS} -o ${BIN_PATH}/main ${MAIN_OBJ_FILES_WITH_PATH} ${LFLAGS}

fact_tree:
	mkdir build || true
	mkdir ${OBJ_PATH} || true
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_fact_tree/src/graph.c -o ${OBJ_PATH}/graph.o
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_fact_tree/src/fact.c -o ${OBJ_PATH}/fact.o
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_fact_tree/src/contradiction.c -o ${OBJ_PATH}/contradiction.o
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_fact_tree/src/implication.c -o ${OBJ_PATH}/implication.o

log:
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_log/src/log.c -o ${OBJ_PATH}/log.o

common:
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_common/src/common.c -o ${OBJ_PATH}/common.o

proof_machine:
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_proof_machine/src/machine.c -o ${OBJ_PATH}/machine.o

test: fact_tree_test physical_graph_test all
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_test/unit_test.c -o ${OBJ_PATH}/unit_test.o
	${CC} ${CFLAGS} -o ${BIN_PATH}/test ${TEST_OBJ_FILES_WITH_PATH} ${LFLAGS}
	./${BIN_PATH}/test

fact_tree_test:
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_fact_tree/src/test_fact_tree.c -o ${OBJ_PATH}/test_fact_tree.o

physical_graph:
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_physical_graph/src/physical_graph.c -o ${OBJ_PATH}/physical_graph.o

physical_graph_test:
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_physical_graph/src/test_physical_graph.c -o ${OBJ_PATH}/test_physical_graph.o

generative_proof_machine:
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_generative_proof_machine/src/generative_proof_machine.c -o ${OBJ_PATH}/generative_proof_machine.o
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_generative_proof_machine/src/generative_restriction.c -o ${OBJ_PATH}/generative_restriction.o


clean:
	rm ${BIN_PATH}/* || true
	rm ${OBJ_PATH}/* || true