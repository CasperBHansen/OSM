void process_init() {
    spinlock_reset(&process_table_slock);
    
    spinlock_acquire(&process_table_slock);
    
    // initialize processes
    for (int i = 0; i < PROCESS_MAX_PROCESSES; ++i)
        process_table[i].state = PROCESS_DEAD;
    
    spinlock_release(&process_table_slock);
}
