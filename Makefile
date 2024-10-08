CC = gcc
CFLAGS = -std=c99 -fstrict-aliasing -fstack-protector -ftrack-macro-expansion=0 -Wall -Wextra -Wdouble-promotion -Wformat=2 -Wlogical-op -Wstrict-aliasing -Wstrict-prototypes -Werror -Wno-error=cast-function-type -Wno-format-truncation -Wno-error=format-overflow= -D_POSIX_C_SOURCE=20190101L -ggdb3 -DMALLOC_CHECK_=3 -Og
LFLAGS = -lm
C_INCLUDE_PATH = gtd_fact_tree/inc:gtd_log/inc:gtd_common/inc:gtd_proof_machine/inc

all: fact_tree log common proof_machine
	mkdir build/bin || true
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c main.c -o build/artifacts/main.o
	${CC} ${CFLAGS} -o build/bin/main build/artifacts/main.o build/artifacts/graph.o build/artifacts/fact.o build/artifacts/log.o build/artifacts/common.o build/artifacts/machine.o ${LFLAGS}

fact_tree:
	mkdir build || true
	mkdir build/artifacts || true
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_fact_tree/src/graph.c -o build/artifacts/graph.o
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_fact_tree/src/fact.c -o build/artifacts/fact.o

log:
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_log/src/log.c -o build/artifacts/log.o

common:
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_common/src/common.c -o build/artifacts/common.o

proof_machine:
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_proof_machine/src/machine.c -o build/artifacts/machine.o

test: fact_tree_test
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_test/unit_test.c -o build/artifacts/unit_test.o
	${CC} ${CFLAGS} -o build/bin/test build/artifacts/unit_test.o build/artifacts/test_graph.o build/artifacts/test_fact.o
	./build/bin/test

fact_tree_test:
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_fact_tree/src/test_graph.c -o build/artifacts/test_graph.o
	C_INCLUDE_PATH=${C_INCLUDE_PATH} ${CC} ${CFLAGS} -c gtd_fact_tree/src/test_fact.c -o build/artifacts/test_fact.o

clean:
	rm build/bin/* || true
	rm build/artifacts/* || true