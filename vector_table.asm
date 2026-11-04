;====================================================
; vector_table.asm
; Interrupt and Exception Vector Table
; Starting address: x0200
;====================================================

.ORIG x0200

.FILL x0000      ; Vector x00
.FILL x1200      ; Vector x01: Timer interrupt
.FILL x1600      ; Vector x02: Protection exception
.FILL x1A00      ; Vector x03: Unaligned exception
.FILL x1C00      ; Vector x04: Unknown opcode

.END