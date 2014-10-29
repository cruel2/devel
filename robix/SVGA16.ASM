
; memory for SVGA information
vesainfo:
sign	db	'????'	; signature, 'VESA'
version	dw	0
oem	dd	0
cap	dd	0	; capabilities flags
modes	dd	0	; pointer to modes
memory	dw	0	; in 64k blocks
times	200h	db '?'	; other stuff

str100	db 'SVGA not supported!', 0	
str101	db 'SVGA supported', 0

oldmode	dw	0	; the crt mode's ID


; get VESA information

getvesainfo:
	push	es
	push	ds
	pop	es
	mov	di, vesainfo
	mov	ax, 0x4f00
	int	10h
	cmp	al, 4Fh
	jnz	err100
	cmp	ah, 0
	jnz	err100
	mov	dx, str101
	call	printcstrR
	
	; store video mode
	mov	ax, 4f03h
	int	10h
	cmp	al, 4fh
	jnz	err100
	mov	word [oldmode], bx
	jmp	exit100
	
err100:
	mov	dx, str100		; TODO : kilepes procedura a main-be!
	call	printcstrR
exit100:
	add	word [crtAddR], 0xA0
	pop	es
	ret


; switch to svga

setvesamode:
	mov	ax, 4f02h
	mov	bx, 115h	; 800x600, 16M
	int	10h
	ret


; switch to crt

setcrtmode:
	mov	ax, 4f02h
	mov	bx, word [oldmode]
	int	10h
	ret

