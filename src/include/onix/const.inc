%ifndef ONIX_CONST_INC
%define ONIX_CONST_INC

PROCESS_STACKBASE equ 0
GSREG  equ PROCESS_STACKBASE
FSREG  equ GSREG  + 4
ESREG  equ FSREG  + 4
DSREG  equ ESREG  + 4
EDIREG  equ DSREG  + 4
ESIREG  equ EDIREG  + 4
EBPREG  equ ESIREG  + 4
KERNELESPREG equ EBPREG  + 4
EBXREG  equ KERNELESPREG + 4
EDXREG  equ EBXREG  + 4
ECXREG  equ EDXREG  + 4
EAXREG  equ ECXREG  + 4
RETADR  equ EAXREG  + 4
EIPREG  equ RETADR  + 4
CSREG  equ EIPREG  + 4
EFLAGSREG equ CSREG  + 4
ESPREG  equ EFLAGSREG + 4
SSREG  equ ESPREG  + 4
PROCESS_STACK_TOP equ SSREG  + 4
LDT_SELECTOR equ PROCESS_STACK_TOP
LDT  equ LDT_SELECTOR + 4

TSS3_S_SP0 equ 4

SELECTOR_CODE equ  0x08
SELECTOR_TSS  equ  0x20
SELECTOR_KERNEL_CODE equ  SELECTOR_CODE

INT_M_CTL equ 0x20 ; I/O port for interrupt controller        <Master>
INT_M_CTLMASK equ 0x21 ; setting bits in this port disables ints  <Master>
INT_S_CTL equ 0xA0 ; I/O port for second interrupt controller <Slave>
INT_S_CTLMASK equ 0xA1 ; setting bits in this port disables ints  <Slave>

INT_VECTOR_SYS_CALL equ 0x90

SYSCALL_INDEX_SENDRECV equ 0x00
SYSCALL_INDEX_PAUSE equ 0x01
SYSCALL_INDEX_GET_TICKS equ 0x02


EOI equ 0x20;

%endif
