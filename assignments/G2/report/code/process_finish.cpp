void process_finish(int retval) {
    interrupt_status_t intr_status;
    thread_table_t *thr = thread_get_current_thread_entry();
    process_id_t pid = process_get_current_process();
    
    int i;
    // remove parent references in other processes
    for (i = 0; i < PROCESS_MAX_PROCESSES; ++i) {
        intr_status = _interrupt_disable();
        spinlock_acquire(&process_table_slock);
        if (process_table[i].parent_id == pid)
            process_table[i].parent_id = -1;
        spinlock_release(&process_table_slock);
        _interrupt_set_state(intr_status);
    }
    
    intr_status = _interrupt_disable();
    spinlock_acquire(&process_table_slock);
    
    process_table[pid].retval = retval;
    process_table[pid].state = PROCESS_ZOMBIE;
    
    spinlock_release(&process_table_slock);
    _interrupt_set_state(intr_status);
    
    // clean up virtual memory
    vm_destroy_pagetable(thr->pagetable);
    thr->pagetable = NULL;
    
    thread_finish();
}
