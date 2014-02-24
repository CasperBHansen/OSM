int process_join(process_id_t pid) {
    interrupt_status_t intr_status;
    int ret;

    if (process_table[pid].state == PROCESS_DEAD)
        return -1;
    
    intr_status = _interrupt_disable();
    spinlock_acquire(&process_table_slock);
    
    // wait for the process to become a zombie
    process_table[process_get_current_process()].state = PROCESS_WAITING;
    while (process_table[pid].state != PROCESS_ZOMBIE) {
        sleepq_add( (void *)process_table[pid].executable );
        spinlock_release(&process_table_slock);
        thread_switch();
        spinlock_acquire(&process_table_slock);
    }
    
    ret = process_table[pid].retval;
    process_table[pid].state = PROCESS_DEAD;
    process_table[process_get_current_process()].state = PROCESS_RUNNING;
    
    spinlock_release(&process_table_slock);
    _interrupt_set_state(intr_status);
    
    return ret;
}
