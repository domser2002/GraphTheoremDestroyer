#ifndef IMPLICATION_H
#define IMPLICATION_H
#define CAN_ACCESS_FACT
#define MAX_LEFT_SIDE_FACTS 2
#define MIN_LEFT_SIDE_FACTS 1
#include "fact.h"
#include <string.h>
Fact **implies(Fact **factArray, int n_facts, int *count);
#endif