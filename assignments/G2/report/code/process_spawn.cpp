process_id_t process_spawn(const char *executable) {
    
    interrupt_status_t intr_status = _interrupt_disable();
    spinlock_acquire(&process_table_slock);
 
    process_id_t pid = process_create_process(executable);
    
    // inefficient, should be handled only on startup
    if (pid == PROCESS_STARTUP_PID) {
        process_table[pid].parent_id = -1;
        process_table[pid].thread_id = thread_get_current_thread();
    
        spinlock_release(&process_table_slock);
        _interrupt_set_state(intr_status);
        
        process_start(pid); // never returns, start-up process
    }
    
    TID_t thread_id = thread_create(&process_start, pid);
    process_table[pid].thread_id = thread_id;
    
    spinlock_release(&process_table_slock);
    _interrupt_set_state(intr_status);
    
    thread_run(thread_id);
    
    return pid;
}
