#ifndef MESSAGES_H
#define MESSAGES_H
#include "fact.h"
typedef enum message_type_e {
    StopModuleMessage,
    AddFactsMessage,
    WriteProofMessage,
    WriteDeductionMessage,
    ContradictionFoundMessage,
    FactsFoundMessage,
    NothingFoundMessage
} message_type_e;

typedef struct MessageStruct {
    message_type_e type;
    void *body;
} MessageStruct;

typedef struct AddFactsMessageBody {
    uint32_t fact_count;
    Fact **fact_array;
} AddFactsMessageBody;

typedef struct WriteDeductionMessageBody {
    Fact *fact;
} WriteDeductionMessageBody;

typedef struct ContradictionFoundMessageBody {
    Fact **contradicting_facts;
    uint32_t contradicting_facts_number;
} ContradictionFoundMessageBody;

typedef struct FactsFoundMessageBody {
    Fact **facts_found;
    uint32_t facts_found_number;
} FactsFoundMessageBody;

#endif