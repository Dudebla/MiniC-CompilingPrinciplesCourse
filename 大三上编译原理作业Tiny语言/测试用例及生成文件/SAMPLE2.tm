* TINY Compilation to TM Code
* File: C:/Users/Pietra/Desktop/TinyÓïÑÔ±àÒëÆ÷Ô´³ÌÐòsourcer/¸½Â¼B sourcer/SAMPLE2.tm
* Standard prelude:
 20:     LD  6,0(0) 	load maxaddress from location 0
 21:     ST  0,0(0) 	clear location 0
* End of standard prelude.
 22:     IN  0,0,0 	read integer value
 23:     ST  0,-1(5) 	read: store value
* -> if
* -> Op
* -> Const
 24:    LDC  0,0(0) 	load const
* <- Const
 25:     ST  0,0(6) 	op: push left
* -> Id
 26:     LD  0,-1(5) 	load id value
* <- Id
 27:     LD  1,0(6) 	op: load left
 28:    SUB  0,1,0 	op <
 29:    JLT  0,2(7) 	br if true
 30:    LDC  0,0(0) 	false case
 31:    LDA  7,1(7) 	unconditional jmp
 32:    LDC  0,1(0) 	true case
* <- Op
* if: jump to else belongs here
* -> Id
 34:     LD  0,-1(5) 	load id value
* <- Id
 35:    OUT  0,0,0 	write ac
* if: jump to end belongs here
 33:    JEQ  0,3(7) 	if: jmp to else
 36:    LDA  7,0(7) 	jmp to end
* <- if
* End of execution.
 37:   HALT  0,0,0 	
