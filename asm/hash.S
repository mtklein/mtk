_lookup:
	stp	x24, x23, [sp, #-0x40]!
	stp	x22, x21, [sp, #0x10]
	stp	x20, x19, [sp, #0x20]
	stp	x29, x30, [sp, #0x30]
	add	x29, sp, #0x30
	cmp	w1, #0x0
	csinc	w22, w1, wzr, ne
	ldr	w8, [x0, #0xc]
	subs	w9, w8, #0x1
	b.lt	0x88
	mov	x19, x3
	mov	x20, x2
	mov	x21, x0
	mov	w23, #0x0
	and	w9, w9, w22
	ldr	x10, [x21]
	sbfiz	x11, x9, #3, #32
	ldr	w11, [x10, x11]
	cbz	w11, 0x88
	sxtw	x24, w9
	cmp	w11, w22
	b.ne	0x70
	add	x8, x10, x24, lsl #3
	ldr	w0, [x8, #0x4]
	mov	x1, x19
	blr	x20
	tbnz	w0, #0x0, 0xa0
	ldr	w8, [x21, #0xc]
	add	w9, w24, #0x1
	sub	w10, w8, #0x1
	and	w9, w10, w9
	add	w23, w23, #0x1
	cmp	w23, w8
	b.lt	0x3c
	mov	w0, #0x0
	ldp	x29, x30, [sp, #0x30]
	ldp	x20, x19, [sp, #0x20]
	ldp	x22, x21, [sp, #0x10]
	ldp	x24, x23, [sp], #0x40
	ret
	mov	w0, #0x1
	b	0x8c
_insert:
	sub	sp, sp, #0x50
	stp	x24, x23, [sp, #0x10]
	stp	x22, x21, [sp, #0x20]
	stp	x20, x19, [sp, #0x30]
	stp	x29, x30, [sp, #0x40]
	add	x29, sp, #0x40
	mov	x19, x2
	mov	x20, x1
	mov	x21, x0
	ldp	w8, w22, [x0, #0x8]
	add	w9, w22, w22, lsl #1
	add	w10, w9, #0x3
	cmp	w9, #0x0
	csel	w9, w10, w9, lt
	cmp	w8, w9, asr #2
	b.lt	0x158
	lsl	w8, w22, #1
	cmp	w22, #0x0
	csinc	w8, w8, wzr, ne
	stp	wzr, w8, [sp, #0x8]
	sxtw	x0, w8
	mov	w1, #0x8
	bl	0x100
	str	x0, [sp]
	cmp	w22, #0x1
	b.lt	0x148
	mov	x23, #0x0
	mov	x24, #0x0
	ldr	x8, [x21]
	ldr	w1, [x8, x23]
	cbz	w1, 0x138
	add	x8, x8, x23
	ldr	w2, [x8, #0x4]
	mov	x0, sp
	bl	0x130
	ldr	w22, [x21, #0xc]
	add	x24, x24, #0x1
	add	x23, x23, #0x8
	cmp	x24, w22, sxtw
	b.lt	0x118
	ldr	x0, [x21]
	bl	0x14c
	ldr	q0, [sp]
	str	q0, [x21]
	mov	x0, x21
	mov	x1, x20
	mov	x2, x19
	ldp	x29, x30, [sp, #0x40]
	ldp	x20, x19, [sp, #0x30]
	ldp	x22, x21, [sp, #0x20]
	ldp	x24, x23, [sp, #0x10]
	add	sp, sp, #0x50
	b	0x178
_just_insert:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	ldp	w8, w9, [x0, #0x8]
	cmp	w8, w9
	b.ge	0x1e8
	cmp	w1, #0x0
	csinc	w10, w1, wzr, ne
	subs	w11, w9, #0x1
	b.lt	0x1e0
	ldr	x12, [x0]
	mov	x13, x10
	and	w14, w13, w11
	sxtw	x13, w14
	add	x14, x12, w14, sxtw #3
	ldr	w15, [x14]
	cbz	w15, 0x1cc
	add	w13, w13, #0x1
	subs	w9, w9, #0x1
	b.ne	0x1a8
	b	0x1e0
	str	w10, [x14]
	add	x9, x12, x13, lsl #3
	str	w2, [x9, #0x4]
	add	w8, w8, #0x1
	str	w8, [x0, #0x8]
	ldp	x29, x30, [sp], #0x10
	ret
	bl	0x1e8
_just_insert.cold.1:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	adrp	x0, 0 ; 0x0
	add	x0, x0, #0x0
	adrp	x1, 0 ; 0x0
	add	x1, x1, #0x0
	adrp	x3, 0 ; 0x0
	add	x3, x3, #0x0
	mov	w2, #0x15
	bl	0x210
