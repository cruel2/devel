
; c string kiirasa valos modban
; ds:dx - a string cime
printcstrR:
	push	es
	pusha
	mov	di, [crtAddR]
	mov	ax, 0xb800
	mov	es, ax
	mov	ah, byte [color]
	mov	si, dx
loop04:
	mov	al, byte [ds:si]
	cmp	al, 0
	jz	exit02
	mov	word [es:di], ax
	add	di, 2
	inc	si
	jmp	loop04
exit02:
	popa	
	pop	es
	ret


; kepernyo torlese - valos mod

clrscrR:
	push	es
	mov	ax, 0xb800
	mov	es, ax
	mov	al, ' '
	mov	ah, [color]
	xor	di, di
loop05:
	mov	word [es:di], ax
	add	di, 2
	cmp	di, 0xA00
	jnz	loop05
	pop	es
	ret
