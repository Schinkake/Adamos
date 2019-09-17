[BITS 32]
global start
start:
	mov esp, _sys_stack     ; This points the stack to our new stack area
	jmp stublet

; This part MUST be 4 byte aligned, so we solve that issue using 'ALIGN 4'
ALIGN 4
mboot:
	; Multiboot macros to make a few lines later more readable
	MULTIBOOT_PAGE_ALIGN	equ 1<<0
	MULTIBOOT_MEMORY_INFO	equ 1<<1
	MULTIBOOT_AOUT_KLUDGE	equ 1<<16
	MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
	MULTIBOOT_HEADER_FLAGS  equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
	MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
	EXTERN	code, bss, end

	; This is the GRUB multiboot header.  A boot signature
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM

	; AOUT kludge - must be physical addresses.  Make a note of these:
	; the linker script fills in the data for these ones!
	dd mboot
	dd code
	dd bss
	dd end
	dd start

; This is an endless loop here.  Make a note of this:  Later on, we
; will insert an 'extern kernelMain', followed by a 'call kernelMain', right
; before the 'jmp $'.
stublet:
	push esp		; Pushs the stack address
	push ebx		; Pushs the 32-bit address of the multiboot info structure to the stack
	extern kernelMain
	call kernelMain
	jmp $

; This will set up our new segment registers.  We need to do
; something special in order to set CS.  We do what is called
; a far jump.  A jump that includes a segment as well as an
; offset.  This is declared in C as 'extern void gdt_flush();'
global _gdt_flush	; Allows the C code to link to this
extern _gp		; Says that '_gp' is in another file
_gdt_flush:
	lgdt [_gp]	; Load the GDT with our '_gp' which is a special pointer
	mov ax, 0x10	; 0x10 is the offset in the GDT to our data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush2	; 0x08 is the offset to our code segment: Far jump!
flush2:
	ret		; Returns back to the C code!

; This will refresh the TSS for the processor.
global _tss_flush	; Allows the C code to link to this
_tss_flush:
	mov ax, 0x2B	; Load the index of our TSS structure
			; The index is 0x28 as it is the 5th
			; selector and each is 8 bytes long,
			; but we set the bottom two bits (making
			; 0x2B) so that is has an RPL of 3,
			; not zero.
	ltr ax		; Load 0x2B into the task state register
	ret

; Loads the IDT defined in '_idtp' into the processor.
; This is declared in C as 'extern void _idt_load();'
global _idt_load
extern _idtp
_idt_load:
	lidt [_idtp]
	ret

; Define the ISRS handlers and the ISRS stub code.
; Define the global interrupt service routine labels here.
global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31
global _isr80

; Interrupt service routine handlers.
_isr0:
	cli
	push byte 0
	push byte 0
	jmp isr_common_stub
_isr1:
	cli
	push byte 0
	push byte 1
	jmp isr_common_stub
_isr2:
	cli
	push byte 0
	push byte 2
	jmp isr_common_stub
_isr3:
	cli
	push byte 0
	push byte 3
	jmp isr_common_stub
_isr4:
	cli
	push byte 0
	push byte 4
	jmp isr_common_stub
_isr5:
	cli
	push byte 0
	push byte 5
	jmp isr_common_stub
_isr6:
	cli
	xchg bx, bx
	push byte 0
	push byte 6
	jmp isr_common_stub
_isr7:
	cli
	push byte 0
	push byte 7
	jmp isr_common_stub
_isr8:
	cli
	push byte 8
	jmp isr_common_stub
_isr9:
	cli
	push byte 0
	push byte 9
	jmp isr_common_stub
_isr10:
	cli
	push byte 10
	jmp isr_common_stub
_isr11:
	cli
	push byte 11
	jmp isr_common_stub
_isr12:
	cli
	push byte 12
	jmp isr_common_stub
_isr13:
	cli
	push byte 13
	jmp isr_common_stub
_isr14:
	cli
	push byte 14
	jmp isr_common_stub
_isr15:
	cli
	push byte 0
	push byte 15
	jmp isr_common_stub
_isr16:
	cli
	push byte 0
	push byte 16
	jmp isr_common_stub
_isr17:
	cli
	push byte 0
	push byte 17
	jmp isr_common_stub
_isr18:
	cli
	push byte 0
	push byte 18
	jmp isr_common_stub
_isr19:
	cli
	push byte 0
	push byte 19
	jmp isr_common_stub
_isr20:
	cli
	push byte 0
	push byte 20
	jmp isr_common_stub
_isr21:
	cli
	push byte 0
	push byte 21
	jmp isr_common_stub
_isr22:
	cli
	push byte 0
	push byte 22
	jmp isr_common_stub
_isr23:
	cli
	push byte 0
	push byte 23
	jmp isr_common_stub
_isr24:
	cli
	push byte 0
	push byte 24
	jmp isr_common_stub
_isr25:
	cli
	push byte 0
	push byte 25
	jmp isr_common_stub
_isr26:
	cli
	push byte 0
	push byte 26
	jmp isr_common_stub
_isr27:
	cli
	push byte 0
	push byte 27
	jmp isr_common_stub
_isr28:
	cli
	push byte 0
	push byte 28
	jmp isr_common_stub
_isr29:
	cli
	push byte 0
	push byte 29
	jmp isr_common_stub
_isr30:
	cli
	push byte 0
	push byte 30
	jmp isr_common_stub
_isr31:
	cli
	push byte 0
	push byte 31
	jmp isr_common_stub
_isr80:
	cli
	push byte 0
	push byte 80
	jmp isr_common_stub

; We call C functions in here.  We need to let the assembler
; know that they exist in another file.
extern _fault_handler

; This is our common ISR stub.  It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, _fault_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

; Define the IRQ handlers and the IRQ stub code.
global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

_irq0:
	cli
	push byte 0
	push byte 32
	jmp irq_common_stub
_irq1:
	cli
	push byte 0
	push byte 33
	jmp irq_common_stub
_irq2:
	cli
	push byte 0
	push byte 34
	jmp irq_common_stub
_irq3:
	cli
	push byte 0
	push byte 35
	jmp irq_common_stub
_irq4:
	cli	
	push byte 0
	push byte 36
	jmp irq_common_stub
_irq5:
	cli
	push byte 0
	push byte 37
	jmp irq_common_stub
_irq6:
	cli
	push byte 0
	push byte 38
	jmp irq_common_stub
_irq7:
	cli
	push byte 0
	push byte 39
	jmp irq_common_stub
_irq8:
	cli
	push byte 0
	push byte 40
	jmp irq_common_stub
_irq9:
	cli
	push byte 0
	push byte 41
	jmp irq_common_stub
_irq10:
	cli
	push byte 0
	push byte 42
	jmp irq_common_stub
_irq11:
	cli
	push byte 0
	push byte 43
	jmp irq_common_stub
_irq12:
	cli
	push byte 0
	push byte 44
	jmp irq_common_stub
_irq13:
	cli
	push byte 0
	push byte 45
	jmp irq_common_stub
_irq14:
	cli
	push byte 0
	push byte 46
	jmp irq_common_stub
_irq15:
	cli
	push byte 0
	push byte 47
	jmp irq_common_stub
 
extern _irq_handler

; This is a stub that we have created for IRQ based ISRs.  This calls
; '_irq_handler' in our C code.  We need to create this in an 'irq.c'
irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp

    push eax
    mov eax, _irq_handler
    call eax
    pop eax

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

; Here is the definition of our BSS section.  Right now, we'll use
; it just to store the stack.  Remember that a stack actually grows
; downwards, so we declare the size of the data before declaring
; the identifier '_sys_stack'
SECTION .bss
	resb 8192	; This reserves 8 kilobytes of memory here
_sys_stack: