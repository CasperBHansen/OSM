void handle_syscall_join(context_t * user_context) {
    const uint32_t pid = (const int) user_context->cpu_regs[MIPS_REGISTER_A1];
    user_context->cpu_regs[MIPS_REGISTER_V0] = process_join(pid);
}
