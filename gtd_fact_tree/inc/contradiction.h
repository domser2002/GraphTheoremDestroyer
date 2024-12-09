#ifndef CONTRADICTION_H
#define CONTRADICTION_H
#define CAN_ACCESS_FACT
#include "fact.h"
#include "function.h"
#define MIN_CONTRADICTING_FACTS 2
#define MAX_CONTRADICTING_FACTS 2
bool contradict(Fact **factArray, uint32_t n_facts);
#endif