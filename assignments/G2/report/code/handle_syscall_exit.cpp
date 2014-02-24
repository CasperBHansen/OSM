void handle_syscall_exit(context_t * user_context) {
    const int retval = (const int) user_context->cpu_regs[MIPS_REGISTER_A1];
    user_context->cpu_regs[MIPS_REGISTER_V0] = retval;
    process_id_t pid = process_get_current_process();
    (pid == PROCESS_STARTUP_PID) ? halt_kernel() : process_finish(retval);
}
