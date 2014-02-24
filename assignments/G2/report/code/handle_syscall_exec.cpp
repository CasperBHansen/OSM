void handle_syscall_exec(context_t * user_context) {
    const char * executable = (const char *)user_context->cpu_regs[MIPS_REGISTER_A1];
    user_context->cpu_regs[MIPS_REGISTER_V0] = process_spawn(executable);
}
