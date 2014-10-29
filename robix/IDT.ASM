
%macro	IDTItem	2
	dw	%2	; offszet
	dw	%1	; szelektor
	db	0
	db	8eh
	dw	0
%endmacro

IDTStart:
IDTItem	8, Exception00	; 0. kivetel
IDTItem 8, Exception01
IDTItem 8, Exception02
IDTItem 8, Exception03
IDTItem 8, Exception04
IDTItem 8, Exception05	; 5. kivetel
IDTItem 8, Exception06
IDTItem 8, Exception07
IDTItem 8, Exception08
IDTItem 8, Exception09
IDTItem 8, Exception10	; 10. kivetel
IDTItem 8, Exception11
IDTItem 8, Exception12
IDTItem 8, Exception13
IDTItem 8, Exception14
IDTItem 8, Exception15	; 15. kivetel
IDTItem 8, Exception16	; 16. kivetel


IDTValue:
IDTSize	dw	IDTStart - IDTValue
IDTBase	dd	IDTStart

OldIDTValue:
	dw	0
	dd	0


; IDT betoltese

InitIDT:
	mov	eax, cs
	shl	eax, 4
	add	[IDTBase], eax
	lidt	[IDTValue]
	ret
