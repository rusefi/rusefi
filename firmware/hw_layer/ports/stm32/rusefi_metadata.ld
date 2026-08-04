SECTIONS
{
    .rusefi_stack_root 0 (INFO) :
    {
        KEEP(*(.rusefi_stack_root))
    }
}
INSERT AFTER .rodata;
