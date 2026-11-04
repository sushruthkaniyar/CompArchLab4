;====================================================
; add.asm
; Main User Program
; Starting address: x3000
;
; Initializes x4000 to 1.
; Calculates the sum of 20 signed bytes at xC000.
; Stores the result at xC014.
; Triggers a protection exception by writing to x0000.
;====================================================

.ORIG x3000

; Initialize memory location x4000 to 1
LEA  R4, PTR4000
LDW  R4, R4, #0       ; R4 = x4000

AND  R5, R5, #0
ADD  R5, R5, #1       ; R5 = 1

STW  R5, R4, #0       ; MEM[x4000] = 1

; Initialize registers for summation

LEA  R1, PTRC000
LDW  R1, R1, #0       ; R1 = xC000
AND  R2, R2, #0
ADD  R2, R2, #15
ADD  R2, R2, #5       ; R2 = 20 (counter)
AND  R3, R3, #0       ; R3 = 0 (sum)


; Sum 20 signed bytes
LOOP
LDB  R4, R1, #0       ; Load signed byte
ADD  R3, R3, R4       ; Add to running sum
ADD  R1, R1, #1       ; Advance byte pointer
ADD  R2, R2, #-1      ; Decrement counter
BRp  LOOP             ; Repeat while counter > 0


; Store sum at xC014
STW  R3, R1, #0       ; MEM[xC014] = sum


; Trigger protection exception
AND  R1, R1, #0       ; R1 = x0000
STW  R3, R1, #0       ; Protection exception


; Address constants
PTR4000 .FILL x4000
PTRC000 .FILL xC000

.END