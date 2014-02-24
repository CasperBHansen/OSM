    process_table[pid].state = PROCESS_RUNNING;
    thread_goto_userland(&user_context);
    
    KERNEL_PANIC("thread_goto_userland failed.");
}
