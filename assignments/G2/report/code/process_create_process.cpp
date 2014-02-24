process_id_t process_create_process(const char * executable) {  
    process_id_t pid = process_get_available_pid();
    process_table[pid].parent_id = process_get_current_process();
    process_table[pid].state = PROCESS_NEW;
    stringcopy(process_table[pid].executable, executable, PROCESS_NAME_LENGTH);
    return pid;
}
