// newlib's setjmp/longjmp don't save/restore ploating point registers
// these do
int fp_setjmp(int *regs) {
    asm volatile (
        "stmia r0!, {r4-r11}\n"
        "mov r1, sp\n"
        "mov r2, lr\n"
        "stmia r0!, {r1-r2}\n"
        "vstmia r0, {d8-d15}"
    );
    return 0;
}

int fp_longjmp(int *regs) {
    asm volatile (
        "ldmia r0!, {r4-r11}\n"
        "ldmia r0!, {r1-r2}\n"
        "mov sp, r1\n"
        "mov lr, r2\n"
        "vldmia r0, {d8-d15}\n"
    );
    return 1;
}

