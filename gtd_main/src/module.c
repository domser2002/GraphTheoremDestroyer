/**
 * \file module.c
 * \brief implementation of functions from module.h
 */
#include "module.h"

module_main_loop modules[MODULES_COUNT] = {
    fact_tree_main_loop,
    generative_main_loop
};

char module_names[MODULES_COUNT][MAX_THREAD_NAME_LEN] = {
    "fact_tree",
    "generative"
};

static int write_pipes[MODULES_COUNT][2];
static int read_pipes[MODULES_COUNT][2];

/**
 * \brief function to send a msg to a module at specific index in modules array
 * \param msg pointer to msg struct
 * \param idx index of the module to which a message will be sent
 */
static void send_to_module(MessageStruct *msg, uint8_t idx)
{
    GTD_LOG("Sending message of type %d to module %d", msg->type, idx);
    if(write(write_pipes[idx][1], (const void *)msg, sizeof(MessageStruct)) == -1)
    {
        GTD_LOG("Error in write");
        exit(EXIT_FAILURE);
    }
}

/**
 * \brief function to broadcast a msg to all modules expect specific index in modules array
 * \param msg pointer to msg struct
 * \param exclude index of the module to which a message will not be sent, 
 * if outside of module array bounds, no module will be excluded
 */
static void broadcast(MessageStruct *msg, uint8_t exclude)
{
    GTD_LOG("Broadcasting message of type %d to all modules except %d", msg->type, exclude);
    for(uint8_t i=0;i<MODULES_COUNT;i++)
    {
        if(i != exclude)
        {
            send_to_module(msg, i);
        }
    }
}

void run_modules(ModuleArgs *args)
{
    pthread_t threads[MODULES_COUNT];
    int modules_running = MODULES_COUNT;
    // bool module_running[MODULES_COUNT];
    for(uint8_t i=0;i<MODULES_COUNT;i++)
    {
        if(pipe(write_pipes[i]) != 0)
        {
            GTD_LOG("Error creating a pipe");
            exit(EXIT_FAILURE);
        }
        if(pipe(read_pipes[i]) != 0)
        {
            GTD_LOG("Error creating a pipe");
            exit(EXIT_FAILURE);
        }
        ModuleArgs *args_copy = deep_copy_module_args(args);
        args_copy->read_fd = write_pipes[i][0];
        args_copy->write_fd = read_pipes[i][1];
        pthread_create(&threads[i], NULL, modules[i], (void *)args_copy);
        pthread_setname_np(threads[i], module_names[i]);
        // module_running[i] = true;
    }
    while(true)
    {
        bool continue_loop = true;
        int max_fd = 0;
        fd_set readfds;
        FD_ZERO(&readfds);
        for(uint8_t i=0; i<MODULES_COUNT; i++)
        {
            FD_SET(read_pipes[i][0], &readfds);
            if(read_pipes[i][0] > max_fd)
                max_fd = read_pipes[i][0];
        }
        int ret = select(max_fd+1, &readfds, NULL, NULL, NULL);
        if(ret < 1)
        {
            GTD_LOG("error in select, ret is %d", ret);
            exit(EXIT_FAILURE);
        }
        for(uint8_t i=0; i<MODULES_COUNT; i++)
        {
            if(FD_ISSET(read_pipes[i][0], &readfds))
            {
                MessageStruct msg;
                if(read(read_pipes[i][0], (void *)&msg, sizeof(MessageStruct)) == -1)
                {
                    GTD_LOG("error in read");
                    exit(EXIT_FAILURE);
                }
                GTD_LOG("Received message of type %d, modules_running = %d", msg.type, modules_running);
                switch (msg.type)
                {
                case NothingFoundMessage:
                    modules_running--;
                    // module_running[i] = false;
                    if(modules_running == 0)
                    {
                        MessageStruct stop_module_msg;
                        stop_module_msg.type = StopModuleMessage;
                        stop_module_msg.body = NULL;
                        broadcast(&stop_module_msg, -1);
                        continue_loop = false;
                        FILE *out_file = fopen(args->out_file_path, "a");
                        fprintf(out_file, "Contradiction not found");
                        fclose(out_file);
                    }
                    break;
                case FactsFoundMessage:
                    MessageStruct add_facts_msg;
                    AddFactsMessageBody add_facts_msg_body;
                    add_facts_msg.type = AddFactsMessage;
                    FactsFoundMessageBody *facts_found_msg_body = (FactsFoundMessageBody*)(msg.body);
                    add_facts_msg_body.fact_count = facts_found_msg_body->facts_found_number;
                    add_facts_msg_body.fact_array = deep_copy_fact_array(facts_found_msg_body->facts_found, add_facts_msg_body.fact_count);
                    add_facts_msg.body = (void*)&add_facts_msg_body;
                    broadcast(&add_facts_msg, i);
                    modules_running = MODULES_COUNT - 1;
                    break;
                case ContradictionFoundMessage:
                    MessageStruct stop_module_msg;
                    stop_module_msg.type = StopModuleMessage;
                    stop_module_msg.body = NULL;
                    broadcast(&stop_module_msg, i);
                    MessageStruct write_proof_msg;
                    write_proof_msg.type = WriteProofMessage;
                    write_proof_msg.body = NULL;
                    send_to_module(&write_proof_msg, i);
                    continue_loop = false;
                    break;
                default:
                    GTD_LOG("Invalid message received");
                    exit(EXIT_FAILURE);
                }
                break;
            }
        }
        if(!continue_loop)
            break;
    }
    for(uint8_t i=0;i<MODULES_COUNT;i++)
    {
        pthread_join(threads[i], NULL);
    }
}