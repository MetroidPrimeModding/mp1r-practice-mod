.macro CODE_BEGIN name
	.section .text.\name, "ax", %progbits
	.global \name
	.type \name, %function
	.align 2
	.cfi_startproc
\name:
.endm

.macro CODE_END
	.cfi_endproc
.endm

/* For these macros, LR is deliberately not backed up as that's handled by the entry's entrypoint. */
.macro armBackupRegisters
    sub sp, sp, #0x200
    stp x0, x1, [sp, #0x00]
    stp x2, x3, [sp, #0x10]
    stp x4, x5, [sp, #0x20]
    stp x6, x7, [sp, #0x30]
    stp x8, x9, [sp, #0x40]
    stp x10, x11, [sp, #0x50]
    stp x12, x13, [sp, #0x60]
    stp x14, x15, [sp, #0x70]
    stp x16, x17, [sp, #0x80]
    stp x18, x19, [sp, #0x90]
    stp x20, x21, [sp, #0xA0]
    stp x22, x23, [sp, #0xB0]
    stp x24, x25, [sp, #0xC0]
    stp x26, x27, [sp, #0xD0]
    stp x28, x29, [sp, #0xE0]
    stp d0, d1, [sp, #0x100]
    stp d2, d3, [sp, #0x110]
    stp d4, d5, [sp, #0x120]
    stp d6, d7, [sp, #0x130]
    stp d8, d9, [sp, #0x140]
    stp d10, d11, [sp, #0x150]
    stp d12, d13, [sp, #0x160]
    stp d14, d15, [sp, #0x170]
    stp d16, d17, [sp, #0x180]
    stp d18, d19, [sp, #0x190]
    stp d20, d21, [sp, #0x1A0]
    stp d22, d23, [sp, #0x1B0]
    stp d24, d25, [sp, #0x1C0]
    stp d26, d27, [sp, #0x1D0]
    stp d28, d29, [sp, #0x1E0]
    stp d30, d31, [sp, #0x1F0]
.endm

.macro armRecoverRegisters
    ldp x0, x1, [sp, #0x00]
    ldp x2, x3, [sp, #0x10]
    ldp x4, x5, [sp, #0x20]
    ldp x6, x7, [sp, #0x30]
    ldp x8, x9, [sp, #0x40]
    ldp x10, x11, [sp, #0x50]
    ldp x12, x13, [sp, #0x60]
    ldp x14, x15, [sp, #0x70]
    ldp x16, x17, [sp, #0x80]
    ldp x18, x19, [sp, #0x90]
    ldp x20, x21, [sp, #0xA0]
    ldp x22, x23, [sp, #0xB0]
    ldp x24, x25, [sp, #0xC0]
    ldp x26, x27, [sp, #0xD0]
    ldp x28, x29, [sp, #0xE0]
    ldp d0, d1, [sp, #0x100]
    ldp d2, d3, [sp, #0x110]
    ldp d4, d5, [sp, #0x120]
    ldp d6, d7, [sp, #0x130]
    ldp d8, d9, [sp, #0x140]
    ldp d10, d11, [sp, #0x150]
    ldp d12, d13, [sp, #0x160]
    ldp d14, d15, [sp, #0x170]
    ldp d16, d17, [sp, #0x180]
    ldp d18, d19, [sp, #0x190]
    ldp d20, d21, [sp, #0x1A0]
    ldp d22, d23, [sp, #0x1B0]
    ldp d24, d25, [sp, #0x1C0]
    ldp d26, d27, [sp, #0x1D0]
    ldp d28, d29, [sp, #0x1E0]
    ldp d30, d31, [sp, #0x1F0]
    add sp, sp, #0x200
.endm

CODE_BEGIN exl_inline_hook_impl
    armBackupRegisters

    /* LR contains a pointer to the called entry here. */
    mov x19, lr

    /* Load inline context for the first argument of the callback. */
    mov x0, sp
    /* Load then call callback. */
    ldr x20, [x19, #0x8]
    blr x20

    /* Keep a hold of entry pointer before restoring all the registers. */
    mov lr, x19

    armRecoverRegisters

    /* Return to entry. */
    ret
CODE_END