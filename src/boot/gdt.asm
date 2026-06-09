global load_gdt
[extern gdtr_ptr]

load_gdt:
    lgdt [gdtr_ptr]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    push 0x08
    lea rax, [rel .after_flush]
    push rax
    retfq

.after_flush:
    ret
