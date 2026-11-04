;====================================================
; int.asm
; Timer Interrupt Service Routine
; Starting address: x1200
;
; Increments memory location x4000 by 1.
; Preserves R0 and R1.
;====================================================

.ORIG x1200

; Save registers on supervisor stack
ADD  R6, R6, #-2
STW  R0, R6, #0

ADD  R6, R6, #-2
STW  R1, R6, #0

; Load address x4000
LEA  R1, PTR4000
LDW  R1, R1, #0

; Read, increment, and write the value
LDW  R0, R1, #0
ADD  R0, R0, #1
STW  R0, R1, #0

; Restore registers
LDW  R1, R6, #0
ADD  R6, R6, #2

LDW  R0, R6, #0
ADD  R6, R6, #2

; Return from interrupt
RTI

; Address constant
PTR4000 .FILL x4000

.END