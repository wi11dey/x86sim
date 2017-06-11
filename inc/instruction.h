#ifndef INSTRUCTION_H
#define INSTRUCTION_H

// instruction format
#define ROB_SIZE 256
#define LQ_SIZE 72
#define SQ_SIZE 56
#define NUM_INSTR_DESTINATIONS 2
#define NUM_INSTR_SOURCES 4

class input_instr {
  public:

    // instruction pointer or PC (Program Counter)
    uint64_t ip;

    // branch info
    uint8_t is_branch;
    uint8_t branch_taken;

    uint8_t destination_registers[NUM_INSTR_DESTINATIONS]; // output registers
    uint8_t source_registers[NUM_INSTR_SOURCES]; // input registers

    uint64_t destination_memory[NUM_INSTR_DESTINATIONS]; // output memory
    uint64_t source_memory[NUM_INSTR_SOURCES]; // input memory

    input_instr() {
        ip = 0;
        is_branch = 0;
        branch_taken = 0;

        for (uint32_t i=0; i<NUM_INSTR_SOURCES; i++) {
            source_registers[i] = 0;
            source_memory[i] = 0;
        }

        for (uint32_t i=0; i<NUM_INSTR_DESTINATIONS; i++) {
            destination_registers[i] = 0;
            destination_memory[i] = 0;
        }
    };
};

class ooo_model_instr {
  public:
    uint64_t instr_id,
             ip,
             fetch_producer,
             producer_id,
             translated_cycle,
             fetched_cycle,
             execute_begin_cycle,
             retired_cycle,
             event_cycle;

    uint8_t is_branch,
            is_memory,
            branch_taken,
            branch_mispredicted,
            translated,
            data_translated,
            source_added[NUM_INSTR_SOURCES],
            destination_added[NUM_INSTR_DESTINATIONS],
            is_producer,
            is_consumer,
            reg_RAW_producer,
            reg_ready,
            mem_ready,
            reg_RAW_checked[NUM_INSTR_SOURCES];
    uint32_t fetched, scheduled;
    int num_reg_ops, num_mem_ops, num_reg_dependent;

    // executed bit is set after all dependencies are eliminated and this instr is chosen on a cycle, according to EXEC_WIDTH
    int executed;

    uint8_t destination_registers[NUM_INSTR_DESTINATIONS]; // output registers

    uint8_t source_registers[NUM_INSTR_SOURCES]; // input registers 

    // these are instruction ids of other instructions in the window
    //int64_t registers_instrs_i_depend_on[NUM_INSTR_SOURCES];
    // these are indices of instructions in the window that depend on me
    uint8_t registers_instrs_depend_on_me[ROB_SIZE], registers_index_depend_on_me[ROB_SIZE][NUM_INSTR_SOURCES];


    // memory addresses that may cause dependencies between instructions
    uint64_t instruction_pa, data_pa, virtual_address, physical_address;
    uint64_t destination_memory[NUM_INSTR_DESTINATIONS]; // output memory
    uint64_t source_memory[NUM_INSTR_SOURCES]; // input memory
    //int source_memory_outstanding[NUM_INSTR_SOURCES];  // a value of 2 here means the load hasn't been issued yet, 1 means it has been issued, but not returned yet, and 0 means it has returned

    // keep around a record of what the original virtual addresses were
    uint64_t destination_virtual_address[NUM_INSTR_DESTINATIONS];
    uint64_t source_virtual_address[NUM_INSTR_SOURCES];

    // these are instruction ids of other instructions in the window
    //uint32_t memory_instrs_i_depend_on[NUM_INSTR_SOURCES];

    // these are indices of instructions in the ROB that depend on me
    uint8_t memory_instrs_depend_on_me[ROB_SIZE];

    uint32_t lq_index[NUM_INSTR_SOURCES],
             sq_index[NUM_INSTR_DESTINATIONS],
             forwarding_index[NUM_INSTR_DESTINATIONS];

    //uint8_t source_registers_dependent[NUM_INSTR_SOURCES],
    //source_memory_dependent[NUM_INSTR_SOURCES],
    //destination_registers_dependent[NUM_INSTR_DESTINATIONS],
    //destination_memory_dependent[NUM_INSTR_DESTINATIONS];

    ooo_model_instr() {
        instr_id = 0;
        ip = 0;
        fetch_producer = 0;
        producer_id = 0;
        translated_cycle = 0;
        fetched_cycle = 0;
        execute_begin_cycle = 0;
        retired_cycle = 0;
        event_cycle = 0;

        is_branch = 0;
        is_memory = 0;
        branch_taken = 0;
        branch_mispredicted = 0;
        translated = 0;
        data_translated = 0;
        is_producer = 0;
        is_consumer = 0;
        reg_RAW_producer = 0;
        fetched = 0;
        scheduled = 0;
        executed = 0;
        reg_ready = 0;
        mem_ready = 0;

        instruction_pa = 0;
        data_pa = 0;
        virtual_address = 0;
        physical_address = 0;

        num_reg_ops = 0;
        num_mem_ops = 0;
        num_reg_dependent = 0;

        for (uint32_t i=0; i<NUM_INSTR_SOURCES; i++) {
            source_registers[i] = 0;
            //registers_instrs_i_depend_on[i] = 0;
            source_memory[i] = 0;
            //source_memory_outstanding[i] = 0;
            source_virtual_address[i] = 0;
            //memory_instrs_i_depend_on[i] = 0;
            //source_registers_dependent[i] = 0;
            //source_memory_dependent[i] = 0;
            source_added[i] = 0;
            lq_index[i] = UINT32_MAX;
            reg_RAW_checked[i] = 0;
        }

        for (uint32_t i=0; i<NUM_INSTR_DESTINATIONS; i++) {
            destination_memory[i] = 0;
            destination_registers[i] = 0;
            destination_virtual_address[i] = 0;
            //destination_registers_dependent[i] = 0;
            //destination_memory_dependent[i] = 0;
            destination_added[i] = 0;
            sq_index[i] = UINT32_MAX;
            forwarding_index[i] = 0;
        }

        for (uint32_t i=0; i<ROB_SIZE; i++) {
            registers_instrs_depend_on_me[i] = 0;
            memory_instrs_depend_on_me[i] = 0;

            for (uint32_t j=0; j<NUM_INSTR_SOURCES; j++)
                registers_index_depend_on_me[i][j] = 0;
        }
    };
};

#endif