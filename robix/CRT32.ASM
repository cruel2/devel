
; uj sor

newline:
	push	eax
	mov	eax, [crtAddr]
	add	eax, 0x140
	mov	dword [crtAddr], eax
	pop	eax
	ret


; eax kiirasa

printeax:
	mov	ecx, 0xb8BB0
	mov	ebx, 0x10
loop03:
	xor	edx, edx
	div	ebx
	cmp	edx, 10
	jb	below10
	add	edx, 7
below10:
	add	edx, 0x30
	mov	byte [ecx], dl
	sub	ecx, 2
	cmp	ecx, 0x0b8BA0
	jnz	loop03

	ret


; c string kiirasa - TODO : tenyleg NEM JO?
; edx a string kezdo cimet tartalmazza

printcstr:
	pushad
	mov	ebx, dword [crtAddr]
	mov	ah, [color]
loop02:
	mov	al, byte [cs:edx]
	cmp	al, 0       ; TODO erre szukseg van? a mov-tol nem lesz 1 z flag?
	jz	exit01
	mov	word [ebx], ax
	add	ebx, 2
	inc	edx
	jmp	loop02
exit01:
	popad
	ret


; Kepernyo torlese, szokozokkel tolti ki. A karakter szinet 'color'-bol veszi.

clrscr:
	pushad
	mov	al, ' '
	mov	ah, [color]
	mov	ebx, 0xb8000
loop01:
	mov	word [ebx], ax
	add	ebx, 2
	cmp	ebx, 0b8A00h     ; pontositani ezt a szamot!
	jnz	loop01
	popad
	ret
