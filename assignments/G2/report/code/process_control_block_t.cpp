typedef struct {
    char executable[PROCESS_NAME_LENGTH];
    process_state_t state;
    process_id_t parent_id;
    
    int thread_id;
    int retval;
    
    struct {
        uint64_t cpu;
        uint64_t user;
    } usage;
} process_control_block_t;
