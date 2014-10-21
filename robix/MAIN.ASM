[org 0x0100]

jmp	start

gdt:
	dd	0x00000000,	0x00000000
	dd	0x0000ffff,	0x00cf9a00
	dd	0x0000ffff,	0x00cf9200

gdtreg:
	dw	0x17
	dd	0

dsbck	dw	0
ssbck	dw	0
spreg	dw	0

stack:
	times 256 dd 0
stacktop:

start:
	mov	ax, ds
	mov	[dsbck], ax
	mov	ax, ss
	mov	[ssbck], ax
	mov	ax, sp
	mov	[spreg], ax

	mov	ax, 0x2401
	int	0x15

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

	mov	byte [0x000b8000], 'A'
;	jmp	$

	mov	eax, cr0
	and	al, 0xfe
	mov	cr0, eax


	mov	ax, dsbck
	mov	ds, ax
	mov	ax, ssbck
	mov	ss, ax
	mov	ax, spreg
	mov	sp, ax

	sti

	mov	ah, 4ch
	int	21h


