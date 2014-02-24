process_id_t process_get_available_pid() {
    // check for available process id
    for (process_id_t pid = 0; pid < PROCESS_MAX_PROCESSES; ++pid)
        if (process_table[pid].state == PROCESS_DEAD) return pid;
    return -1; // could also wait for one to become available?
}
