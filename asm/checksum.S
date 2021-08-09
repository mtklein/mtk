_crc32:
	mov	w8, #-0x1
	cmp	x1, #0x8
	b.lo	0x20
	ldr	x9, [x0], #0x8
	crc32x	w8, w8, x9
	sub	x1, x1, #0x8
	cmp	x1, #0x7
	b.hi	0xc
	cbz	x1, 0x34
	ldrb	w9, [x0], #0x1
	crc32b	w8, w8, w9
	subs	x1, x1, #0x1
	b.ne	0x24
	mvn	w0, w8
	ret
_fnv1a:
	mov	x8, x0
	mov	w0, #0x9dc5
	movk	w0, #0x811c, lsl #16
	cbz	x1, 0x68
	mov	w9, #0x193
	movk	w9, #0x100, lsl #16
	ldrb	w10, [x8], #0x1
	eor	w10, w0, w10
	mul	w0, w10, w9
	subs	x1, x1, #0x1
	b.ne	0x54
	ret
_mix:
	eor	w8, w0, w0, lsr #16
	mov	w9, #0xca6b
	movk	w9, #0x85eb, lsl #16
	mul	w8, w8, w9
	eor	w8, w8, w8, lsr #13
	mov	w9, #0xae35
	movk	w9, #0xc2b2, lsl #16
	mul	w8, w8, w9
	eor	w0, w8, w8, lsr #16
	ret
_murmur3:
	mov	w9, #0x2d51
	movk	w9, #0xcc9e, lsl #16
	mov	w8, #0x3593
	movk	w8, #0x1b87, lsl #16
	mov	w10, #0x8000
	movk	w10, #0x16a8, lsl #16
	cmp	x2, #0x4
	b.lo	0xf4
	mov	w12, #0x6b64
	movk	w12, #0xe654, lsl #16
	mov	x11, x2
	ldr	w13, [x1], #0x4
	mul	w14, w13, w9
	mul	w13, w13, w10
	orr	w13, w13, w14, lsr #17
	mul	w13, w13, w8
	eor	w13, w13, w0
	ror	w13, w13, #0x13
	add	w13, w13, w13, lsl #2
	add	w0, w13, w12
	sub	x11, x11, #0x4
	cmp	x11, #0x3
	b.hi	0xc0
	b	0xf8
	mov	x11, x2
	mov	w12, #0x0
	cmp	x11, #0x1
	b.eq	0x124
	cmp	x11, #0x2
	b.eq	0x11c
	cmp	x11, #0x3
	b.ne	0x140
	ldrsb	w11, [x1, #0x2]
	lsl	w12, w11, #16
	ldrsb	w11, [x1, #0x1]
	orr	w12, w12, w11, lsl #8
	ldrsb	w11, [x1]
	orr	w11, w12, w11
	mul	w9, w11, w9
	mul	w10, w11, w10
	orr	w9, w10, w9, lsr #17
	mul	w8, w9, w8
	eor	w0, w8, w0
	eor	w8, w0, w2
	eor	w8, w8, w8, lsr #16
	mov	w9, #0xca6b
	movk	w9, #0x85eb, lsl #16
	mul	w8, w8, w9
	eor	w8, w8, w8, lsr #13
	mov	w9, #0xae35
	movk	w9, #0xc2b2, lsl #16
	mul	w8, w8, w9
	eor	w0, w8, w8, lsr #16
	ret
