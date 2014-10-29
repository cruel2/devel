
excs00	db	'0. kivetel: 0-val osztas', 0
excs01	db  '1. kivetel: debug', 0
excs02	db  '2. kevetel: NMI', 0
excs03	db  '3. kevetel: torespont', 0
excs04	db  '4. kevetel: INTO', 0
excs05	db  '5. kevetel: BOUND', 0
excs06	db  '6. kevetel: ervenytelen utasitas', 0
excs07	db  '7. kevetel: koprocesszor nem elerheto', 0
excs08	db  '8. kevetel: dupla hiba', 0
excs09	db  '9. kevetel: koprocesszor hatarsertes', 0
excs10	db  '10. kevetel: ervenytelen TSS', 0
excs11	db  '11. kevetel: verem hiba', 0
excs12	db  '12. kevetel: szegmens nincs jelen', 0
excs13	db  '13. kevetel: altalanos vedelmi hiba', 0
excs14	db  '14. kevetel: laphiba', 0
excs15	db  '15. kevetel: fenntartva', 0
excs16	db  '16. kevetel: koprocesszor hiba', 0
excs17	db  '17. kevetel: illeszkedes-ellenorzes', 0
excs18	db  '18. kevetel: processzortipus-ellenorzes', 0


Exception00:
	mov	edx, excs00
	call	printcstr
	jmp	excexit
	
Exception01:
	mov	edx, excs01
	call	printcstr
	jmp	excexit

Exception02:
	mov	edx, excs02
	call	printcstr
	jmp	excexit
Exception03:
	mov	edx, excs03
	call	printcstr
	jmp	excexit

Exception04:
	mov	edx, excs04
	call	printcstr
	jmp	excexit

Exception05:
	mov	edx, excs05
	call	printcstr
	jmp	excexit

Exception06:
	mov	edx, excs06
	call	printcstr
	jmp	excexit

Exception07:
	mov	edx, excs07
	call	printcstr
	jmp	excexit

Exception08:
	mov	edx, excs08
	call	printcstr
	jmp	excexit

Exception09:
	mov	edx, excs09
	call	printcstr
	jmp	excexit

Exception10:
	mov	edx, excs10
	call	printcstr
	jmp	excexit

Exception11:
	mov	edx, excs11
	call	printcstr
	jmp	excexit

Exception12:
	mov	edx, excs12
	call	printcstr
	jmp	excexit

Exception13:
	mov	edx, excs13
	call	printcstr
	jmp	excexit

Exception14:
	mov	edx, excs14
	call	printcstr
	jmp	excexit

Exception15:
	mov	edx, excs15
	call	printcstr
	jmp	excexit

Exception16:
	mov	edx, excs16
	call	printcstr
	jmp	excexit

Exception17:
	mov	edx, excs17
	call	printcstr
	jmp	excexit

Exception18:
	mov	edx, excs18
	call	printcstr
	jmp	excexit
