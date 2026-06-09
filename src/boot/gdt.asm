global load_gdt
[extern gdt_table]

load_gdt:
  lgdt [gdt_table]
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  ret
