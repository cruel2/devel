
[org 0x0100]

jmp	start


%include "gdt.asm"


dsbck	dw	0
ssbck	dw	0
spreg	dw	0

; szoveg szine
color	db	0x0A

; az aktualis sor kezdetenek cime valos es vedett modban
crtAddR dw	0
crtAddr	dd	0xb8000

; cs tartalma
csbck	dw	0

str1	db	'Robix 0.01', 0
str2	db	'Atkapcsolas vedett modba', 0
str3	db	'Visszakapcsolas valos modba', 0
str4	db	'Csinalunk valamit', 0
str5	db	'CPU gyarto: '
cputype db	'............', 0

stack:
	times 256 dd 0
stacktop:


%include "crt16.asm"


%include "idt.asm"	


start:
	; saving cs, ds, ss, sp
	pusha
	mov	ax, cs
	mov	word [csbck], ax
	mov	ax, ds
	mov	[dsbck], ax
	mov	ax, ss
	mov	[ssbck], ax
	mov	ax, sp
	mov	[spreg], ax

	; write header
	mov	ax, 0xA0
	mov	word [crtAddR], ax
	call	clrscrR
	xor	edx, edx
	mov	dx, str1
	mov	ah, 0x2A
	mov	byte [color], ah
	call	printcstrR
	add	word [crtAddR], 0xA0
	mov	dx, str2
	mov	ah, 0x0A
	mov	byte [color], ah
	call	printcstrR

	; enabling A20
	mov	ax, 0x2401
	int	0x15

	; setting 32bit code segment's base address
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	mov	[gdt+8+2], ax
	shr	eax, 16
	mov	[gdt+8+4], al

	xor	edx, edx
	mov	dx, cs
	shl	edx, 4
	add	edx, stacktop

	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, gdt
	mov	[gdtreg+2], eax
	lgdt	[gdtreg]

	sidt	[OldIDTValue]
	call	InitIDT

	mov	eax, cr0
	or	eax, 1
	cli
	mov	cr0, eax
	jmp	0x08:pstart

[bits 32]
pstart:
	mov	eax, 0x10
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	ss, ax
	mov	esp, edx

; test area
	pushad

	mov	eax, 0xb8000             ; Ez azert kell, mert valami
	mov	dword [crtAddr], eax     ; mindig atirja.
	mov	al, 0x0A                 ;
	mov	byte [color], al         ;

;	call	clrscr
	call	newline
	call	newline

	xor	bx, bx
	div	bx

	mov	edx, str3
	call	printcstr
	call	newline
	mov	edx, str4
	call	printcstr
;	call	wait4q

	; cpu gyartojanak kiirasa
	call	newline
	call	getcputype
	mov	edx, str5
	call	printcstr
testend:
	popad
; test area end

	mov	byte [0x000b8000], 'A'
;	jmp	$


; Ide fog ugrani a program, ha kivetel tortent.
excexit:

	mov	eax, cr0
	and	al, 0xfe
	mov	cr0, eax


	mov	ax, dsbck
	mov	ds, ax
	mov	ax, ssbck
	mov	ss, ax
	mov	ax, spreg
	mov	sp, ax
	popa

	lidt	[OldIDTValue]

	sti

	mov	ax, 0x4c00
	int	21h


%include "crt32.asm"


; varakozas gombnyomasra - NEM MUKODIK

wait4q:
loop06:
	push	ax
	in	al, 0x60
	cmp	al, 0x10
	jnz	loop06
	pop	ax
	ret


; CPU gyarto string betoltese cputype-ba

getcputype:
	pushad
	xor	eax, eax
	cpuid
	mov	eax, cputype
	mov	dword [cs:eax], ebx
	add	eax, 4
	mov	dword [cs:eax], edx
	add	eax, 4
	mov	dword [cs:eax], ecx
	popad
	ret


%include "exc.asm"

