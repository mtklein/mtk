_grow:
	sub	x8, x1, #0x1
	tst	x1, x8
	b.ne	0x20
	lsl	x8, x1, #1
	cmp	x1, #0x0
	csinc	x8, x8, xzr, ne
	mul	x1, x8, x2
	b	0x1c
	ret
_shrink:
	sub	x8, x1, #0x1
	tst	x1, x8
	b.ne	0x50
	cbz	x1, 0x3c
	mul	x1, x2, x1
	b	0x38
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	bl	0x44
	mov	x0, #0x0
	ldp	x29, x30, [sp], #0x10
	ret
