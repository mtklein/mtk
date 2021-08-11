_builder:
	mov	w0, #0x1
	mov	w1, #0x28
	b	0x8
_arg:
	stp	x20, x19, [sp, #-0x20]!
	stp	x29, x30, [sp, #0x10]
	add	x29, sp, #0x10
	mov	x19, x1
	mov	x20, x0
	ldr	x0, [x0, #0x8]
	ldrsw	x1, [x20, #0x14]
	mov	w2, #0x4
	bl	0x2c
	str	x0, [x20, #0x8]
	ldrsw	x8, [x20, #0x14]
	add	w9, w8, #0x1
	str	w9, [x20, #0x14]
	str	w19, [x0, x8, lsl #2]
	ldr	w0, [x20, #0x14]
	ldp	x29, x30, [sp, #0x10]
	ldp	x20, x19, [sp], #0x20
	ret
_compile:
	stp	x26, x25, [sp, #-0x50]!
	stp	x24, x23, [sp, #0x10]
	stp	x22, x21, [sp, #0x20]
	stp	x20, x19, [sp, #0x30]
	stp	x29, x30, [sp, #0x40]
	add	x29, sp, #0x40
	mov	x19, x0
	ldrsw	x21, [x0, #0x10]
	mov	x0, x21
	mov	w1, #0x4
	bl	0x7c
	mov	x20, x0
	cmp	w21, #0x1
	b.lt	0x200
	mov	w22, #0x0
	ldr	x10, [x19]
	add	x8, x21, #0x1
	add	x9, x20, x21, lsl #2
	sub	x9, x9, #0x4
	add	x10, x10, x21, lsl #5
	sub	x10, x10, #0xc
	mov	w11, #0x1
	ldr	w12, [x10, #0x4]
	cbz	w12, 0xe8
	ldr	x13, [x19, #0x8]
	sub	w12, w12, #0x1
	ldr	w12, [x13, w12, sxtw #2]
	cbz	w12, 0xe8
	ldur	w12, [x10, #-0xc]
	cbnz	w12, 0xe4
	ldur	w12, [x10, #-0x8]
	cbnz	w12, 0xe4
	ldur	w12, [x10, #-0x4]
	cbnz	w12, 0xe4
	ldr	w12, [x10]
	cbz	w12, 0xe8
	strb	w11, [x9]
	ldrb	w12, [x9]
	cbz	w12, 0x144
	ldur	w12, [x10, #-0xc]
	cbz	w12, 0x104
	sub	w12, w12, #0x1
	sbfiz	x12, x12, #2, #32
	strb	w11, [x20, x12]
	ldur	w12, [x10, #-0x8]
	cbz	w12, 0x118
	sub	w12, w12, #0x1
	sbfiz	x12, x12, #2, #32
	strb	w11, [x20, x12]
	ldur	w12, [x10, #-0x4]
	cbz	w12, 0x12c
	sub	w12, w12, #0x1
	sbfiz	x12, x12, #2, #32
	strb	w11, [x20, x12]
	ldr	w12, [x10]
	cbz	w12, 0x140
	sub	w12, w12, #0x1
	sbfiz	x12, x12, #2, #32
	strb	w11, [x20, x12]
	add	w22, w22, #0x1
	sub	x8, x8, #0x1
	sub	x9, x9, #0x4
	sub	x10, x10, #0x20
	cmp	x8, #0x1
	b.gt	0xac
	ldr	x10, [x19]
	and	x8, x21, #0xffffffff
	add	x9, x20, #0x1
	add	x10, x10, #0x18
	ldr	w11, [x10]
	cbz	w11, 0x180
	ldr	x12, [x19, #0x8]
	sub	w11, w11, #0x1
	ldr	w11, [x12, w11, sxtw #2]
	cbnz	w11, 0x1c8
	ldur	w11, [x10, #-0x10]
	cbz	w11, 0x198
	sub	w11, w11, #0x1
	add	x11, x20, w11, sxtw #2
	ldrb	w11, [x11, #0x1]
	cbnz	w11, 0x1c8
	ldur	w11, [x10, #-0xc]
	cbz	w11, 0x1b0
	sub	w11, w11, #0x1
	add	x11, x20, w11, sxtw #2
	ldrb	w11, [x11, #0x1]
	cbnz	w11, 0x1c8
	ldur	w11, [x10, #-0x8]
	cbz	w11, 0x1e0
	sub	w11, w11, #0x1
	add	x11, x20, w11, sxtw #2
	ldrb	w11, [x11, #0x1]
	cbz	w11, 0x1e0
	mov	w11, #0x1
	strb	w11, [x9], #0x4
	add	x10, x10, #0x20
	subs	x8, x8, #0x1
	b.ne	0x168
	b	0x204
	ldur	w11, [x10, #-0x4]
	cbz	w11, 0x1cc
	sub	w11, w11, #0x1
	add	x11, x20, w11, sxtw #2
	ldrb	w11, [x11, #0x1]
	cmp	w11, #0x0
	cset	w11, ne
	b	0x1cc
	mov	w22, #0x0
	and	x25, x21, #0xffffffff
	ldr	w24, [x19, #0x14]
	cmp	w24, #0x1
	b.lt	0x238
	mov	w8, #0x0
	ldr	x9, [x19, #0x8]
	mov	x10, x24
	ldr	w11, [x9], #0x4
	cmp	w11, #0x0
	cinc	w8, w8, ne
	subs	x10, x10, #0x1
	b.ne	0x220
	cbnz	w8, 0x23c
	mov	w8, #0x1
	sxtw	x8, w8
	sxtw	x26, w22
	add	x23, x8, w22, sxtw
	mov	w0, #0x8
	bfi	x0, x23, #5, #59
	bl	0x250
	mov	x21, x0
	mov	w13, #0x0
	mov	w10, #0x0
	mov	x8, x0
	str	wzr, [x8], #0x8
	lsl	x9, x25, #2
	mov	w12, #0x1
	mov	x11, x12
	tbnz	w12, #0x0, 0x27c
	str	w13, [x21, #0x4]
	cmp	w25, #0x1
	b.lt	0x368
	mov	x12, #0x0
	ldrb	w14, [x20, x12]
	cbz	w14, 0x35c
	add	x14, x20, x12
	ldrb	w14, [x14, #0x1]
	cmp	w10, w14
	b.ne	0x35c
	sxtw	x13, w13
	add	x14, x8, x13, lsl #5
	ldr	x15, [x19]
	add	x15, x15, x12, lsl #3
	ldp	q1, q0, [x15]
	str	q1, [x14]
	mov	x15, x14
	ldr	w16, [x15, #0x8]!
	stur	q0, [x15, #0x8]
	cbz	w16, 0x2dc
	sub	w16, w16, #0x1
	ldr	w16, [x20, w16, sxtw #2]
	ldr	w17, [x21]
	sub	w16, w16, w17
	str	w16, [x15]
	ldr	w15, [x14, #0xc]!
	cbz	w15, 0x2f8
	sub	w15, w15, #0x1
	ldr	w15, [x20, w15, sxtw #2]
	ldr	w16, [x21]
	sub	w15, w15, w16
	str	w15, [x14]
	add	x14, x8, x13, lsl #5
	mov	x15, x14
	ldr	w16, [x15, #0x10]!
	cbz	w16, 0x31c
	sub	w16, w16, #0x1
	ldr	w16, [x20, w16, sxtw #2]
	ldr	w17, [x21]
	sub	w16, w16, w17
	str	w16, [x15]
	ldr	w15, [x14, #0x14]!
	cbz	w15, 0x338
	sub	w15, w15, #0x1
	ldr	w15, [x20, w15, sxtw #2]
	ldr	w16, [x21]
	sub	w15, w15, w16
	str	w15, [x14]
	add	x13, x8, x13, lsl #5
	ldr	w14, [x13, #0x18]!
	cbz	w14, 0x34c
	sub	w14, w14, #0x1
	str	w14, [x13]
	ldr	w14, [x21]
	add	w13, w14, #0x1
	str	w13, [x21]
	str	w14, [x20, x12]
	add	x12, x12, #0x4
	cmp	x9, x12
	b.ne	0x288
	mov	w12, #0x0
	mov	w10, #0x1
	tbnz	w11, #0x0, 0x270
	cmp	w13, w22
	b.ne	0x444
	add	x9, x8, x26, lsl #5
	cmp	w24, #0x1
	b.lt	0x3c4
	mov	x10, #0x0
	adrp	x11, 0 ; 0x0
	add	x11, x11, #0x0
	ldr	x12, [x19, #0x8]
	ldr	w12, [x12, x10, lsl #2]
	cbz	w12, 0x3b4
	stp	xzr, xzr, [x9, #0x8]
	str	x11, [x9]
	stp	w10, w12, [x9, #0x18]
	add	x9, x9, #0x20
	ldr	w24, [x19, #0x14]
	add	x10, x10, #0x1
	cmp	x10, w24, sxtw
	b.lt	0x394
	ldrsw	x26, [x21]
	add	x10, x8, x26, lsl #5
	cmp	x9, x10
	b.ls	0x3e0
	adrp	x10, 0 ; 0x0
	add	x10, x10, #0x0
	stur	x10, [x9, #-0x20]
	b	0x3f4
	adrp	x10, 0 ; 0x0
	add	x10, x10, #0x0
	stp	x10, xzr, [x9]
	stp	xzr, xzr, [x9, #0x10]
	add	x9, x9, #0x20
	add	x8, x8, x23, lsl #5
	cmp	x9, x8
	b.ne	0x448
	mov	x0, x20
	bl	0x404
	ldr	x0, [x19]
	bl	0x40c
	ldr	x0, [x19, #0x8]
	bl	0x414
	ldr	x0, [x19, #0x18]
	bl	0x41c
	mov	x0, x19
	bl	0x424
	mov	x0, x21
	ldp	x29, x30, [sp, #0x40]
	ldp	x20, x19, [sp, #0x30]
	ldp	x22, x21, [sp, #0x20]
	ldp	x24, x23, [sp, #0x10]
	ldp	x26, x25, [sp], #0x50
	ret
	bl	0x444
	bl	0x448
_op_inc_arg:
	ldp	w8, w9, [x1, #0x18]
	sxtw	x8, w8
	ldr	x10, [x3, x8, lsl #3]
	lsl	w11, w9, #4
	cmp	w0, #0x10
	csel	w9, w9, w11, lt
	add	x9, x10, w9, sxtw
	str	x9, [x3, x8, lsl #3]
	ldr	x4, [x1, #0x20]!
	add	x2, x2, #0x40
	br	x4
_op_inc_arg_and_done:
	ldp	w8, w9, [x1, #0x18]
	sxtw	x8, w8
	ldr	x10, [x3, x8, lsl #3]
	lsl	w11, w9, #4
	cmp	w0, #0x10
	csel	w9, w9, w11, lt
	add	x9, x10, w9, sxtw
	str	x9, [x3, x8, lsl #3]
	ret
_op_done:
	ret
_drop:
	b	_drop
_ld1_8:
	stp	x20, x19, [sp, #-0x20]!
	stp	x29, x30, [sp, #0x10]
	add	x29, sp, #0x10
	mov	x19, x1
	mov	x20, x0
	ldr	x0, [x0]
	ldrsw	x1, [x20, #0x10]
	mov	w2, #0x20
	bl	0x4c4
	str	x0, [x20]
	ldrsw	x8, [x20, #0x10]
	add	w9, w8, #0x1
	str	w9, [x20, #0x10]
	add	x8, x0, x8, lsl #5
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	stp	xzr, xzr, [x8, #0x8]
	str	x9, [x8]
	stp	w19, wzr, [x8, #0x18]
	ldr	w0, [x20, #0x10]
	ldp	x29, x30, [sp, #0x10]
	ldp	x20, x19, [sp], #0x20
	ret
_op_ld1_8:
	ldrsw	x8, [x1, #0x18]
	ldr	x8, [x3, x8, lsl #3]
	cmp	w0, #0xf
	b.gt	0x51c
	ldrb	w8, [x8]
	strb	w8, [x2]
	b	0x524
	ldr	q0, [x8]
	str	q0, [x2]
	ldr	x4, [x1, #0x20]!
	add	x2, x2, #0x40
	br	x4
_ld1_16:
	stp	x20, x19, [sp, #-0x20]!
	stp	x29, x30, [sp, #0x10]
	add	x29, sp, #0x10
	mov	x19, x1
	mov	x20, x0
	ldr	x0, [x0]
	ldrsw	x1, [x20, #0x10]
	mov	w2, #0x20
	bl	0x550
	str	x0, [x20]
	ldrsw	x8, [x20, #0x10]
	add	w9, w8, #0x1
	str	w9, [x20, #0x10]
	add	x8, x0, x8, lsl #5
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	stp	xzr, xzr, [x8, #0x8]
	str	x9, [x8]
	stp	w19, wzr, [x8, #0x18]
	ldr	w0, [x20, #0x10]
	ldp	x29, x30, [sp, #0x10]
	ldp	x20, x19, [sp], #0x20
	ret
_op_ld1_16:
	ldrsw	x8, [x1, #0x18]
	ldr	x8, [x3, x8, lsl #3]
	cmp	w0, #0xf
	b.gt	0x5a8
	ldrh	w8, [x8]
	strh	w8, [x2]
	b	0x5b0
	ldp	q0, q1, [x8]
	stp	q0, q1, [x2]
	ldr	x4, [x1, #0x20]!
	add	x2, x2, #0x40
	br	x4
_ld1_32:
	stp	x20, x19, [sp, #-0x20]!
	stp	x29, x30, [sp, #0x10]
	add	x29, sp, #0x10
	mov	x19, x1
	mov	x20, x0
	ldr	x0, [x0]
	ldrsw	x1, [x20, #0x10]
	mov	w2, #0x20
	bl	0x5dc
	str	x0, [x20]
	ldrsw	x8, [x20, #0x10]
	add	w9, w8, #0x1
	str	w9, [x20, #0x10]
	add	x8, x0, x8, lsl #5
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	stp	xzr, xzr, [x8, #0x8]
	str	x9, [x8]
	stp	w19, wzr, [x8, #0x18]
	ldr	w0, [x20, #0x10]
	ldp	x29, x30, [sp, #0x10]
	ldp	x20, x19, [sp], #0x20
	ret
_op_ld1_32:
	ldrsw	x8, [x1, #0x18]
	ldr	x8, [x3, x8, lsl #3]
	cmp	w0, #0xf
	b.gt	0x634
	ldr	w8, [x8]
	str	w8, [x2]
	b	0x644
	ldp	q0, q1, [x8]
	ldp	q2, q3, [x8, #0x20]
	stp	q2, q3, [x2, #0x20]
	stp	q0, q1, [x2]
	ldr	x4, [x1, #0x20]!
	add	x2, x2, #0x40
	br	x4
_st1_8:
	stp	x22, x21, [sp, #-0x30]!
	stp	x20, x19, [sp, #0x10]
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x19, x2
	mov	x20, x1
	mov	x21, x0
	ldr	x0, [x0]
	ldrsw	x1, [x21, #0x10]
	mov	w2, #0x20
	bl	0x678
	str	x0, [x21]
	ldrsw	x8, [x21, #0x10]
	add	w9, w8, #0x1
	str	w9, [x21, #0x10]
	add	x8, x0, x8, lsl #5
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	str	x9, [x8]
	stp	w19, wzr, [x8, #0x8]
	str	xzr, [x8, #0x10]
	stp	w20, wzr, [x8, #0x18]
	ldp	x29, x30, [sp, #0x20]
	ldp	x20, x19, [sp, #0x10]
	ldp	x22, x21, [sp], #0x30
	ret
_op_st1_8:
	ldrsw	x8, [x1, #0x18]
	ldr	x8, [x3, x8, lsl #3]
	ldrsw	x9, [x1, #0x8]
	add	x9, x2, x9, lsl #6
	cmp	w0, #0xf
	b.gt	0x6dc
	ldrb	w9, [x9]
	strb	w9, [x8]
	b	0x6e4
	ldr	q0, [x9]
	str	q0, [x8]
	ldr	x4, [x1, #0x20]!
	add	x2, x2, #0x40
	br	x4
_st1_16:
	stp	x22, x21, [sp, #-0x30]!
	stp	x20, x19, [sp, #0x10]
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x19, x2
	mov	x20, x1
	mov	x21, x0
	ldr	x0, [x0]
	ldrsw	x1, [x21, #0x10]
	mov	w2, #0x20
	bl	0x718
	str	x0, [x21]
	ldrsw	x8, [x21, #0x10]
	add	w9, w8, #0x1
	str	w9, [x21, #0x10]
	add	x8, x0, x8, lsl #5
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	str	x9, [x8]
	stp	w19, wzr, [x8, #0x8]
	str	xzr, [x8, #0x10]
	stp	w20, wzr, [x8, #0x18]
	ldp	x29, x30, [sp, #0x20]
	ldp	x20, x19, [sp, #0x10]
	ldp	x22, x21, [sp], #0x30
	ret
_op_st1_16:
	ldrsw	x8, [x1, #0x18]
	ldr	x8, [x3, x8, lsl #3]
	ldrsw	x9, [x1, #0x8]
	add	x9, x2, x9, lsl #6
	cmp	w0, #0xf
	b.gt	0x77c
	ldrh	w9, [x9]
	strh	w9, [x8]
	b	0x784
	ldp	q0, q1, [x9]
	stp	q0, q1, [x8]
	ldr	x4, [x1, #0x20]!
	add	x2, x2, #0x40
	br	x4
_st1_32:
	stp	x22, x21, [sp, #-0x30]!
	stp	x20, x19, [sp, #0x10]
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x19, x2
	mov	x20, x1
	mov	x21, x0
	ldr	x0, [x0]
	ldrsw	x1, [x21, #0x10]
	mov	w2, #0x20
	bl	0x7b8
	str	x0, [x21]
	ldrsw	x8, [x21, #0x10]
	add	w9, w8, #0x1
	str	w9, [x21, #0x10]
	add	x8, x0, x8, lsl #5
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	str	x9, [x8]
	stp	w19, wzr, [x8, #0x8]
	str	xzr, [x8, #0x10]
	stp	w20, wzr, [x8, #0x18]
	ldp	x29, x30, [sp, #0x20]
	ldp	x20, x19, [sp, #0x10]
	ldp	x22, x21, [sp], #0x30
	ret
_op_st1_32:
	ldrsw	x8, [x1, #0x18]
	ldr	x8, [x3, x8, lsl #3]
	ldrsw	x9, [x1, #0x8]
	add	x9, x2, x9, lsl #6
	cmp	w0, #0xf
	b.gt	0x81c
	ldr	w9, [x9]
	str	w9, [x8]
	b	0x82c
	ldp	q0, q1, [x9]
	ldp	q2, q3, [x9, #0x20]
	stp	q2, q3, [x8, #0x20]
	stp	q0, q1, [x8]
	ldr	x4, [x1, #0x20]!
	add	x2, x2, #0x40
	br	x4
_ld4_8:
	stp	x22, x21, [sp, #-0x30]!
	stp	x20, x19, [sp, #0x10]
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x20, x1
	mov	x19, x0
	ldr	x0, [x0]
	ldrsw	x1, [x19, #0x10]
	mov	w2, #0x20
	bl	0x85c
	str	x0, [x19]
	ldrsw	x8, [x19, #0x10]
	add	w9, w8, #0x1
	str	w9, [x19, #0x10]
	add	x8, x0, x8, lsl #5
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	stp	x9, xzr, [x8]
	str	xzr, [x8, #0x10]
	stp	w20, wzr, [x8, #0x18]
	ldr	w20, [x19, #0x10]
	ldr	x0, [x19]
	sxtw	x1, w20
	mov	w2, #0x20
	bl	0x898
	str	x0, [x19]
	ldrsw	x8, [x19, #0x10]
	add	w9, w8, #0x1
	str	w9, [x19, #0x10]
	add	x8, x0, x8, lsl #5
	str	xzr, [x8]
	str	w20, [x8, #0x8]
	str	wzr, [x8, #0x1c]
	stur	xzr, [x8, #0xc]
	stur	xzr, [x8, #0x14]
	ldr	x0, [x19]
	ldrsw	x21, [x19, #0x10]
	mov	x1, x21
	mov	w2, #0x20
	bl	0x8d4
	str	x0, [x19]
	ldrsw	x8, [x19, #0x10]
	add	w9, w8, #0x1
	str	w9, [x19, #0x10]
	add	x8, x0, x8, lsl #5
	str	xzr, [x8]
	str	w20, [x8, #0x8]
	str	wzr, [x8, #0x1c]
	stur	xzr, [x8, #0xc]
	stur	xzr, [x8, #0x14]
	ldr	w22, [x19, #0x10]
	ldr	x0, [x19]
	sxtw	x1, w22
	mov	w2, #0x20
	bl	0x910
	str	x0, [x19]
	ldrsw	x8, [x19, #0x10]
	add	w9, w8, #0x1
	str	w9, [x19, #0x10]
	add	x8, x0, x8, lsl #5
	str	xzr, [x8]
	str	w20, [x8, #0x8]
	stur	xzr, [x8, #0x14]
	stur	xzr, [x8, #0xc]
	str	wzr, [x8, #0x1c]
	bfi	x20, x21, #32, #32
	ldr	w8, [x19, #0x10]
	bfi	x22, x8, #32, #32
	mov	x0, x20
	mov	x1, x22
	ldp	x29, x30, [sp, #0x20]
	ldp	x20, x19, [sp, #0x10]
	ldp	x22, x21, [sp], #0x30
	ret
_op_ld4_8:
	ldrsw	x8, [x1, #0x18]
	ldr	x8, [x3, x8, lsl #3]
	cmp	w0, #0xf
	b.gt	0x9b4
	ldrb	w9, [x8]
	fmov	s0, w9
	ldrb	w9, [x8, #0x1]
	mov.h	v0[1], w9
	ldrb	w9, [x8, #0x2]
	mov.h	v0[2], w9
	ldrb	w8, [x8, #0x3]
	mov.h	v0[3], w8
	uzp1.8b	v1, v0, v0
	mov.16b	v2, v1
	st2.8b	{ v1, v2 }, [x2]
	dup.16b	v1, v0[2]
	str	q1, [x2, #0x40]
	dup.16b	v1, v0[4]
	str	q1, [x2, #0x80]
	dup.16b	v3, v0[6]
	b	0x9c4
	ld4.16b	{ v0, v1, v2, v3 }, [x8]
	str	q0, [x2]
	str	q1, [x2, #0x40]
	str	q2, [x2, #0x80]
	str	q3, [x2, #0xc0]
	ldr	x4, [x1, #0x80]!
	add	x2, x2, #0x100
	br	x4
_st4_8:
	stp	x24, x23, [sp, #-0x40]!
	stp	x22, x21, [sp, #0x10]
	stp	x20, x19, [sp, #0x20]
	stp	x29, x30, [sp, #0x30]
	add	x29, sp, #0x30
	mov	x19, x5
	mov	x20, x4
	mov	x21, x3
	mov	x22, x2
	mov	x23, x1
	mov	x24, x0
	ldr	x0, [x0]
	ldrsw	x1, [x24, #0x10]
	mov	w2, #0x20
	bl	0xa0c
	str	x0, [x24]
	ldrsw	x8, [x24, #0x10]
	add	w9, w8, #0x1
	str	w9, [x24, #0x10]
	add	x8, x0, x8, lsl #5
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	str	x9, [x8]
	stp	w22, w21, [x8, #0x8]
	stp	w20, w19, [x8, #0x10]
	stp	w23, wzr, [x8, #0x18]
	ldp	x29, x30, [sp, #0x30]
	ldp	x20, x19, [sp, #0x20]
	ldp	x22, x21, [sp, #0x10]
	ldp	x24, x23, [sp], #0x40
	ret
_op_st4_8:
	ldpsw	x8, x9, [x1, #0x8]
	lsl	x8, x8, #6
	ldr	q0, [x2, x8]
	lsl	x8, x9, #6
	ldr	q1, [x2, x8]
	ldpsw	x8, x9, [x1, #0x10]
	lsl	x8, x8, #6
	ldr	q2, [x2, x8]
	lsl	x8, x9, #6
	ldr	q3, [x2, x8]
	cmp	w0, #0xf
	b.gt	0xab4
	umov.b	w8, v3[0]
	umov.b	w9, v2[0]
	umov.b	w10, v1[0]
	umov.b	w11, v0[0]
	ldrsw	x12, [x1, #0x18]
	ldr	x12, [x3, x12, lsl #3]
	fmov	s0, w11
	mov.h	v0[1], w10
	mov.h	v0[2], w9
	mov.h	v0[3], w8
	xtn.8b	v0, v0
	str	s0, [x12]
	b	0xac0
	ldrsw	x8, [x1, #0x18]
	ldr	x8, [x3, x8, lsl #3]
	st4.16b	{ v0, v1, v2, v3 }, [x8]
	ldr	x4, [x1, #0x20]!
	add	x2, x2, #0x40
	br	x4
_splat_8:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	stp	xzr, xzr, [sp, #0x8]
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	str	x9, [sp]
	stp	wzr, w1, [sp, #0x18]
	mov	x2, sp
	mov	w0, #0x8
	mov	x1, x8
	bl	0xafc
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_cse_:
	stp	x28, x27, [sp, #-0x60]!
	stp	x26, x25, [sp, #0x10]
	stp	x24, x23, [sp, #0x20]
	stp	x22, x21, [sp, #0x30]
	stp	x20, x19, [sp, #0x40]
	stp	x29, x30, [sp, #0x50]
	add	x29, sp, #0x50
	sub	sp, sp, #0x230
	mov	x21, x2
	mov	x19, x1
	mov	x20, x0
	adrp	x8, 0 ; 0x0
	ldr	x8, [x8]
	ldr	x8, [x8]
	stur	x8, [x29, #-0x58]
	mov	w0, #0x0
	mov	x1, x2
	mov	w2, #0x20
	bl	0xb58
	mov	x22, x0
	stp	x19, x21, [sp, #0x20]
	add	x23, x19, #0x18
	str	xzr, [sp, #0x30]
	adrp	x2, 0 ; 0x0
	add	x2, x2, #0x0
	add	x3, sp, #0x20
	mov	x0, x23
	mov	x1, x22
	bl	0xb80
	tbz	w0, #0x0, 0xb90
	ldr	w19, [sp, #0x30]
	b	0xc0c
	ldr	x8, [x21]
	adrp	x26, 0 ; 0x0
	add	x26, x26, #0x0
	cmp	x8, x26
	adrp	x24, 0 ; 0x0
	add	x24, x24, #0x0
	ccmp	x8, x24, #0x4, ne
	adrp	x25, 0 ; 0x0
	add	x25, x25, #0x0
	ccmp	x8, x25, #0x4, ne
	b.eq	0xbc4
	ldr	w8, [x21, #0x18]
	cbz	w8, 0xc48
	ldp	q0, q1, [x21]
	stp	q0, q1, [sp, #0x20]
	ldr	x0, [x19]
	ldrsw	x1, [x19, #0x10]
	mov	w2, #0x20
	bl	0xbd8
	str	x0, [x19]
	ldrsw	x8, [x19, #0x10]
	add	w9, w8, #0x1
	str	w9, [x19, #0x10]
	add	x8, x0, x8, lsl #5
	ldp	q1, q0, [sp, #0x20]
	stp	q1, q0, [x8]
	ldr	w19, [x19, #0x10]
	mov	x0, x23
	mov	x1, x22
	mov	x2, x19
	bl	0xc08
	ldur	x8, [x29, #-0x58]
	adrp	x9, 0 ; 0x0
	ldr	x9, [x9]
	ldr	x9, [x9]
	cmp	x9, x8
	b.ne	0xe70
	mov	x0, x19
	add	sp, sp, #0x230
	ldp	x29, x30, [sp, #0x50]
	ldp	x20, x19, [sp, #0x40]
	ldp	x22, x21, [sp, #0x30]
	ldp	x24, x23, [sp, #0x20]
	ldp	x26, x25, [sp, #0x10]
	ldp	x28, x27, [sp], #0x60
	ret
	add	x8, sp, #0x160
	ldrsw	x9, [x21, #0x8]
	cbz	w9, 0xc84
	add	x8, x8, #0x20
	ldr	x10, [x19]
	add	x9, x10, x9, lsl #5
	ldp	q1, q0, [x9, #-0x20]
	stp	q1, q0, [sp, #0x160]
	ldr	x9, [sp, #0x160]
	cmp	x9, x26
	b.eq	0xc84
	cmp	x9, x24
	b.eq	0xc84
	cmp	x9, x25
	b.ne	0xbc4
	ldrsw	x10, [x21, #0xc]
	cbz	w10, 0xcc0
	add	x9, x8, #0x20
	ldr	x11, [x19]
	add	x10, x11, x10, lsl #5
	ldp	q1, q0, [x10, #-0x20]
	stp	q1, q0, [x8]
	ldr	x8, [x8]
	cmp	x8, x26
	b.eq	0xcc4
	cmp	x8, x24
	b.eq	0xcc4
	cmp	x8, x25
	b.ne	0xbc4
	b	0xcc4
	mov	x9, x8
	ldrsw	x8, [x21, #0x10]
	cbz	w8, 0xd00
	add	x10, x9, #0x20
	ldr	x11, [x19]
	add	x8, x11, x8, lsl #5
	ldp	q1, q0, [x8, #-0x20]
	stp	q1, q0, [x9]
	ldr	x8, [x9]
	cmp	x8, x26
	b.eq	0xd04
	cmp	x8, x24
	b.eq	0xd04
	cmp	x8, x25
	b.ne	0xbc4
	b	0xd04
	mov	x10, x9
	ldrsw	x9, [x21, #0x14]
	cbz	w9, 0xd40
	add	x8, x10, #0x20
	ldr	x11, [x19]
	add	x9, x11, x9, lsl #5
	ldp	q1, q0, [x9, #-0x20]
	stp	q1, q0, [x10]
	ldr	x9, [x10]
	cmp	x9, x26
	b.eq	0xd44
	cmp	x9, x24
	b.eq	0xd44
	cmp	x9, x25
	b.ne	0xbc4
	b	0xd44
	mov	x8, x10
	add	x9, sp, #0x160
	sub	x22, x8, x9
	lsr	x9, x22, #5
	ldr	w10, [x21, #0x8]
	cbz	w10, 0xd60
	neg	w10, w9
	str	w10, [x21, #0x8]
	ldr	w10, [x21, #0xc]
	cbz	w10, 0xd74
	mov	w10, #0x1
	sub	w10, w10, w9
	str	w10, [x21, #0xc]
	ldr	w10, [x21, #0x10]
	cbz	w10, 0xd88
	mov	w10, #0x2
	sub	w10, w10, w9
	str	w10, [x21, #0x10]
	ldr	w10, [x21, #0x14]
	cbz	w10, 0xd9c
	mov	w10, #0x3
	sub	w9, w10, w9
	str	w9, [x21, #0x14]
	ldp	q0, q1, [x21]
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	stp	q0, q1, [x8]
	stp	x9, xzr, [x8, #0x20]
	stp	xzr, xzr, [x8, #0x30]
	ldr	x8, [sp, #0x160]
	add	x21, sp, #0x20
	add	x1, sp, #0x160
	add	x2, sp, #0x20
	mov	w0, #0x1
	mov	x3, #0x0
	blr	x8
	cmp	w20, #0x20
	b.eq	0xe40
	cmp	w20, #0x10
	b.eq	0xe14
	cmp	w20, #0x8
	b.ne	0xe74
	sbfx	x8, x22, #5, #32
	lsl	x8, x8, #6
	add	x9, sp, #0x20
	ldr	q0, [x9, x8]
	smov.b	w8, v0[0]
	stp	xzr, xzr, [sp, #0x8]
	str	x24, [sp]
	stp	wzr, w8, [sp, #0x18]
	mov	x2, sp
	mov	w0, #0x8
	b	0xe60
	sbfx	x8, x22, #5, #32
	lsl	x8, x8, #6
	add	x9, sp, #0x20
	ldr	q0, [x9, x8]
	smov.h	w8, v0[0]
	stp	xzr, xzr, [sp, #0x8]
	str	x25, [sp]
	stp	wzr, w8, [sp, #0x18]
	mov	x2, sp
	mov	w0, #0x10
	b	0xe60
	sbfx	x8, x22, #5, #32
	lsl	x8, x8, #6
	ldr	w8, [x21, x8]
	stp	xzr, xzr, [sp, #0x8]
	str	x26, [sp]
	stp	wzr, w8, [sp, #0x18]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x19
	bl	_cse_
	mov	x19, x0
	b	0xc0c
	bl	0xe70
	bl	0xe74
_op_splat_8:
	add	x8, x1, #0x1c
	ld1r.16b	{ v0 }, [x8]
	movi.2d	v1, #0000000000000000
	stp	q0, q1, [x2]
	stp	q1, q1, [x2, #0x20]
	ldr	x4, [x1, #0x20]!
	add	x2, x2, #0x40
	br	x4
_splat_16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	stp	xzr, xzr, [sp, #0x8]
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	str	x9, [sp]
	stp	wzr, w1, [sp, #0x18]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0xec8
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_splat_16:
	add	x8, x1, #0x1c
	ld1r.8h	{ v0 }, [x8]
	stp	q0, q0, [x2]
	movi.2d	v0, #0000000000000000
	stp	q0, q0, [x2, #0x20]
	ldr	x4, [x1, #0x20]!
	add	x2, x2, #0x40
	br	x4
_splat_32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	stp	xzr, xzr, [sp, #0x8]
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	str	x9, [sp]
	stp	wzr, w1, [sp, #0x18]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0xf2c
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_splat_32:
	add	x8, x1, #0x1c
	ld1r.4s	{ v0 }, [x8]
	stp	q0, q0, [x2, #0x20]
	stp	q0, q0, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_uniform_8:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	stp	xzr, xzr, [sp, #0x8]
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	str	x9, [sp]
	stp	w1, w2, [sp, #0x18]
	mov	x2, sp
	mov	w0, #0x8
	mov	x1, x8
	bl	0xf88
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_uniform_8:
	ldpsw	x8, x9, [x1, #0x18]
	ldr	x8, [x3, x8, lsl #3]
	add	x8, x8, x9
	ld1r.16b	{ v0 }, [x8]
	movi.2d	v1, #0000000000000000
	stp	q0, q1, [x2]
	stp	q1, q1, [x2, #0x20]
	ldr	x4, [x1, #0x20]!
	add	x2, x2, #0x40
	br	x4
_uniform_16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	stp	xzr, xzr, [sp, #0x8]
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	str	x9, [sp]
	stp	w1, w2, [sp, #0x18]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0xff4
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_uniform_16:
	ldpsw	x8, x9, [x1, #0x18]
	ldr	x8, [x3, x8, lsl #3]
	add	x8, x8, x9
	ld1r.8h	{ v0 }, [x8]
	stp	q0, q0, [x2]
	movi.2d	v0, #0000000000000000
	stp	q0, q0, [x2, #0x20]
	ldr	x4, [x1, #0x20]!
	add	x2, x2, #0x40
	br	x4
_uniform_32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	stp	xzr, xzr, [sp, #0x8]
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	stp	w1, w2, [sp, #0x18]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x1060
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_uniform_32:
	ldpsw	x8, x9, [x1, #0x18]
	ldr	x8, [x3, x8, lsl #3]
	add	x8, x8, x9
	ld1r.4s	{ v0 }, [x8]
	stp	q0, q0, [x2, #0x20]
	stp	q0, q0, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_cast_F16_to_S16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0x10cc
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_cast_F16_to_S16:
	ldrsw	x8, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q1, q0, [x8]
	fcvtzs.8h	v1, v1
	fcvtzs.8h	v0, v0
	stp	q1, q0, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_cast_F16_to_U16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0x1138
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_cast_F16_to_U16:
	ldrsw	x8, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q1, q0, [x8]
	fcvtzu.8h	v1, v1
	fcvtzu.8h	v0, v0
	stp	q1, q0, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_cast_S16_to_F16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0x11a4
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_cast_S16_to_F16:
	ldrsw	x8, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q1, q0, [x8]
	scvtf.8h	v1, v1
	scvtf.8h	v0, v0
	stp	q1, q0, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_cast_U16_to_F16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0x1210
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_cast_U16_to_F16:
	ldrsw	x8, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q1, q0, [x8]
	ucvtf.8h	v1, v1
	ucvtf.8h	v0, v0
	stp	q1, q0, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_cast_F32_to_S32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x127c
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_cast_F32_to_S32:
	ldrsw	x8, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q0, q1, [x8, #0x20]
	ldp	q2, q3, [x8]
	fcvtzs.4s	v3, v3
	fcvtzs.4s	v2, v2
	fcvtzs.4s	v1, v1
	fcvtzs.4s	v0, v0
	stp	q0, q1, [x2, #0x20]
	stp	q2, q3, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_cast_F32_to_U32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x12f8
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_cast_F32_to_U32:
	ldrsw	x8, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q0, q1, [x8, #0x20]
	ldp	q2, q3, [x8]
	fcvtzu.4s	v3, v3
	fcvtzu.4s	v2, v2
	fcvtzu.4s	v1, v1
	fcvtzu.4s	v0, v0
	stp	q0, q1, [x2, #0x20]
	stp	q2, q3, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_cast_S32_to_F32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x1374
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_cast_S32_to_F32:
	ldrsw	x8, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q0, q1, [x8, #0x20]
	ldp	q2, q3, [x8]
	scvtf.4s	v3, v3
	scvtf.4s	v2, v2
	scvtf.4s	v1, v1
	scvtf.4s	v0, v0
	stp	q0, q1, [x2, #0x20]
	stp	q2, q3, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_cast_U32_to_F32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x13f0
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_cast_U32_to_F32:
	ldrsw	x8, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q0, q1, [x8, #0x20]
	ldp	q2, q3, [x8]
	ucvtf.4s	v3, v3
	ucvtf.4s	v2, v2
	ucvtf.4s	v1, v1
	ucvtf.4s	v0, v0
	stp	q0, q1, [x2, #0x20]
	stp	q2, q3, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_widen_S8:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0x146c
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_widen_S8:
	ldr	q0, [x2]
	sshll.8h	v1, v0, #0x0
	sshll2.8h	v0, v0, #0x0
	stp	q1, q0, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_widen_U8:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0x14d0
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_widen_U8:
	ldr	q0, [x2]
	ushll.8h	v1, v0, #0x0
	ushll2.8h	v0, v0, #0x0
	stp	q1, q0, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_widen_F16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x1534
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_widen_F16:
	ldp	q1, q0, [x2]
	fcvtl	v2.4s, v1.4h
	fcvtl	v3.4s, v0.4h
	fcvtl2	v1.4s, v1.8h
	fcvtl2	v0.4s, v0.8h
	stp	q1, q3, [x2, #0x10]
	str	q0, [x2, #0x30]
	str	q2, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_widen_S16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x15a8
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_widen_S16:
	ldp	q1, q0, [x2]
	sshll.4s	v2, v1, #0x0
	sshll.4s	v3, v0, #0x0
	sshll2.4s	v1, v1, #0x0
	sshll2.4s	v0, v0, #0x0
	stp	q1, q3, [x2, #0x10]
	str	q0, [x2, #0x30]
	str	q2, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_widen_U16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x161c
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_widen_U16:
	ldp	q1, q0, [x2]
	ushll.4s	v2, v1, #0x0
	ushll.4s	v3, v0, #0x0
	ushll2.4s	v1, v1, #0x0
	ushll2.4s	v0, v0, #0x0
	stp	q1, q3, [x2, #0x10]
	str	q0, [x2, #0x30]
	str	q2, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_narrow_F32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0x1690
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_narrow_F32:
	ldp	q0, q1, [x2]
	ldp	q2, q3, [x2, #0x20]
	fcvtn	v3.4h, v3.4s
	fcvtn	v2.4h, v2.4s
	mov.d	v2[1], v3[0]
	fcvtn	v1.4h, v1.4s
	fcvtn	v0.4h, v0.4s
	mov.d	v0[1], v1[0]
	stp	q0, q2, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_narrow_I32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0x1708
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_narrow_I32:
	ldp	q1, q0, [x2]
	ldp	q3, q2, [x2, #0x20]
	xtn.4h	v3, v3
	xtn2.8h	v3, v2
	xtn.4h	v1, v1
	xtn2.8h	v1, v0
	stp	q1, q3, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_narrow_I16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	wzr, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x8
	mov	x1, x8
	bl	0x1778
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_narrow_I16:
	ldp	q1, q0, [x2]
	xtn.8b	v1, v1
	xtn2.16b	v1, v0
	str	q1, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_add_F16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	ldr	x9, [x0]
	mov	x12, #-0x100000000
	add	x11, x12, x1, lsl #32
	asr	x10, x11, #32
	asr	x11, x11, #27
	ldr	x11, [x9, x11]
	adrp	x13, 0 ; 0x1000
	add	x13, x13, #0x0
	cmp	x11, x13
	adrp	x14, 0 ; 0x1000
	add	x14, x14, #0x0
	ccmp	x11, x14, #0x4, ne
	adrp	x15, 0 ; 0x1000
	add	x15, x15, #0x0
	ccmp	x11, x15, #0x4, ne
	b.ne	0x1804
	add	x16, x9, x10, lsl #5
	ldr	h0, [x16, #0x1c]
	fcmp	h0, #0.0
	b.eq	0x18bc
	add	x16, x12, x2, lsl #32
	asr	x12, x16, #32
	asr	x16, x16, #27
	ldr	x16, [x9, x16]
	cmp	x16, x13
	ccmp	x16, x14, #0x4, ne
	ccmp	x16, x15, #0x4, ne
	b.ne	0x1834
	add	x13, x9, x12, lsl #5
	ldr	h0, [x13, #0x1c]
	fcmp	h0, #0.0
	b.eq	0x1880
	adrp	x13, 0 ; 0x1000
	add	x13, x13, #0x0
	cmp	x11, x13
	b.eq	0x1864
	cmp	x16, x13
	b.eq	0x1888
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	b	0x18a8
	add	x9, x9, x10, lsl #5
	ldr	x9, [x9, #0x8]
	adrp	x10, 0 ; 0x1000
	add	x10, x10, #0x0
	stp	x10, x9, [sp]
	str	w2, [sp, #0x10]
	b	0x18a0
	mov	x2, x1
	b	0x18bc
	add	x9, x9, x12, lsl #5
	ldr	x9, [x9, #0x8]
	adrp	x10, 0 ; 0x1000
	add	x10, x10, #0x0
	stp	x10, x9, [sp]
	str	w1, [sp, #0x10]
	str	wzr, [sp, #0x14]
	str	xzr, [sp, #0x18]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0x18b4
	mov	x2, x0
	mov	w0, w2
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_mul_F16:
	ldpsw	x8, x9, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q1, q0, [x8]
	add	x8, x2, x9, lsl #6
	ldp	q3, q2, [x8]
	fmul.8h	v1, v1, v3
	fmul.8h	v0, v0, v2
	stp	q1, q0, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_op_mla_F16:
	ldpsw	x8, x9, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q0, q1, [x8]
	add	x8, x2, x9, lsl #6
	ldp	q2, q3, [x8]
	ldrsw	x8, [x1, #0x10]
	add	x8, x2, x8, lsl #6
	ldp	q5, q4, [x8]
	fmla.8h	v5, v0, v2
	fmla.8h	v4, v1, v3
	stp	q5, q4, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_op_add_F16:
	ldpsw	x8, x9, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q1, q0, [x8]
	add	x8, x2, x9, lsl #6
	ldp	q3, q2, [x8]
	fadd.8h	v1, v1, v3
	fadd.8h	v0, v0, v2
	stp	q1, q0, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_sub_F16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	ldr	x9, [x0]
	mov	x12, #-0x100000000
	add	x11, x12, x2, lsl #32
	asr	x10, x11, #32
	asr	x11, x11, #27
	ldr	x11, [x9, x11]
	adrp	x13, 0 ; 0x1000
	add	x13, x13, #0x0
	cmp	x11, x13
	adrp	x13, 0 ; 0x1000
	add	x13, x13, #0x0
	ccmp	x11, x13, #0x4, ne
	adrp	x13, 0 ; 0x1000
	add	x13, x13, #0x0
	ccmp	x11, x13, #0x4, ne
	b.ne	0x19b0
	add	x13, x9, x10, lsl #5
	ldr	h0, [x13, #0x1c]
	fcmp	h0, #0.0
	b.eq	0x1a40
	add	x12, x12, x1, lsl #32
	asr	x13, x12, #27
	ldr	x14, [x9, x13]
	adrp	x13, 0 ; 0x1000
	add	x13, x13, #0x0
	cmp	x14, x13
	b.eq	0x19ec
	cmp	x11, x13
	b.eq	0x1a0c
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	b	0x1a2c
	asr	x10, x12, #32
	add	x9, x9, x10, lsl #5
	ldr	x9, [x9, #0x8]
	adrp	x10, 0 ; 0x1000
	add	x10, x10, #0x0
	stp	x10, x9, [sp]
	str	w2, [sp, #0x10]
	b	0x1a24
	add	x9, x9, x10, lsl #5
	ldr	x9, [x9, #0x8]
	adrp	x10, 0 ; 0x1000
	add	x10, x10, #0x0
	stp	x10, x9, [sp]
	str	w1, [sp, #0x10]
	str	wzr, [sp, #0x14]
	str	xzr, [sp, #0x18]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0x1a38
	mov	x1, x0
	mov	w0, w1
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_mls_F16:
	ldpsw	x8, x9, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q0, q1, [x8]
	add	x8, x2, x9, lsl #6
	ldp	q2, q3, [x8]
	ldrsw	x8, [x1, #0x10]
	add	x8, x2, x8, lsl #6
	ldp	q5, q4, [x8]
	fneg.8h	v5, v5
	fmla.8h	v5, v0, v2
	fneg.8h	v0, v4
	fmla.8h	v0, v1, v3
	stp	q5, q0, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_op_nma_F16:
	ldpsw	x8, x9, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q0, q1, [x8]
	add	x8, x2, x9, lsl #6
	ldp	q2, q3, [x8]
	ldrsw	x8, [x1, #0x10]
	add	x8, x2, x8, lsl #6
	ldp	q5, q4, [x8]
	fmls.8h	v5, v0, v2
	fmls.8h	v4, v1, v3
	stp	q5, q4, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_op_sub_F16:
	ldpsw	x8, x9, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q1, q0, [x8]
	add	x8, x2, x9, lsl #6
	ldp	q3, q2, [x8]
	fsub.8h	v1, v1, v3
	fsub.8h	v0, v0, v2
	stp	q1, q0, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_mul_F16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	ldr	x9, [x0]
	mov	x10, #-0x100000000
	add	x14, x10, x1, lsl #32
	asr	x11, x14, #27
	ldr	x15, [x9, x11]
	adrp	x11, 0 ; 0x1000
	add	x11, x11, #0x0
	cmp	x15, x11
	adrp	x12, 0 ; 0x1000
	add	x12, x12, #0x0
	ccmp	x15, x12, #0x4, ne
	adrp	x13, 0 ; 0x1000
	add	x13, x13, #0x0
	ccmp	x15, x13, #0x4, ne
	b.ne	0x1b4c
	asr	x14, x14, #32
	add	x14, x9, x14, lsl #5
	ldr	h0, [x14, #0x1c]
	fmov	h1, #1.00000000
	fcmp	h0, h1
	b.eq	0x1bb0
	add	x10, x10, x2, lsl #32
	asr	x14, x10, #27
	ldr	x14, [x9, x14]
	cmp	x14, x11
	ccmp	x14, x12, #0x4, ne
	ccmp	x14, x13, #0x4, ne
	b.ne	0x1b80
	asr	x10, x10, #32
	add	x9, x9, x10, lsl #5
	ldr	h0, [x9, #0x1c]
	fmov	h1, #1.00000000
	fcmp	h0, h1
	b.eq	0x1bac
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0x1ba0
	mov	x2, x0
	b	0x1bb0
	mov	x2, x1
	mov	w0, w2
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_div_F16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	ldr	x9, [x0]
	mov	x10, #-0x100000000
	add	x10, x10, x2, lsl #32
	asr	x11, x10, #27
	ldr	x11, [x9, x11]
	adrp	x12, 0 ; 0x1000
	add	x12, x12, #0x0
	cmp	x11, x12
	adrp	x12, 0 ; 0x1000
	add	x12, x12, #0x0
	ccmp	x11, x12, #0x4, ne
	adrp	x12, 0 ; 0x1000
	add	x12, x12, #0x0
	ccmp	x11, x12, #0x4, ne
	b.ne	0x1c24
	asr	x10, x10, #32
	add	x9, x9, x10, lsl #5
	ldr	h0, [x9, #0x1c]
	fmov	h1, #1.00000000
	fcmp	h0, h1
	b.eq	0x1c4c
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	mov	x2, sp
	mov	w0, #0x10
	mov	x1, x8
	bl	0x1c44
	mov	x1, x0
	mov	w0, w1
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_div_F16:
	ldpsw	x8, x9, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q1, q0, [x8]
	add	x8, x2, x9, lsl #6
	ldp	q3, q2, [x8]
	fdiv.8h	v1, v1, v3
	fdiv.8h	v0, v0, v2
	stp	q1, q0, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_add_I32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x1cb4
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_add_I32:
	ldpsw	x8, x9, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q0, q1, [x8, #0x20]
	ldp	q2, q3, [x8]
	add	x8, x2, x9, lsl #6
	ldp	q4, q5, [x8, #0x20]
	ldp	q6, q7, [x8]
	add.4s	v3, v7, v3
	add.4s	v2, v6, v2
	add.4s	v1, v5, v1
	add.4s	v0, v4, v0
	stp	q0, q1, [x2, #0x20]
	stp	q2, q3, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_sub_I32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x1d34
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_sub_I32:
	ldpsw	x8, x9, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q0, q1, [x8, #0x20]
	ldp	q2, q3, [x8]
	add	x8, x2, x9, lsl #6
	ldp	q4, q5, [x8, #0x20]
	ldp	q6, q7, [x8]
	sub.4s	v3, v3, v7
	sub.4s	v2, v2, v6
	sub.4s	v1, v1, v5
	sub.4s	v0, v0, v4
	stp	q0, q1, [x2, #0x20]
	stp	q2, q3, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_mul_I32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x1db4
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_mul_I32:
	ldpsw	x8, x9, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q0, q1, [x8, #0x20]
	ldp	q2, q3, [x8]
	add	x8, x2, x9, lsl #6
	ldp	q4, q5, [x8, #0x20]
	ldp	q6, q7, [x8]
	mul.4s	v3, v7, v3
	mul.4s	v2, v6, v2
	mul.4s	v1, v5, v1
	mul.4s	v0, v4, v0
	stp	q0, q1, [x2, #0x20]
	stp	q2, q3, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_shl_I32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	w2, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x1e3c
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_shl_I32:
	ldrsw	x8, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q0, q1, [x8, #0x20]
	ldp	q2, q3, [x8]
	add	x8, x1, #0x1c
	ld1r.4s	{ v4 }, [x8]
	ushl.4s	v3, v3, v4
	ushl.4s	v2, v2, v4
	ushl.4s	v1, v1, v4
	ushl.4s	v0, v0, v4
	stp	q0, q1, [x2, #0x20]
	stp	q2, q3, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_shr_S32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	w2, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x1ec0
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_shr_S32:
	add	x8, x1, #0x1c
	ld1r.4s	{ v0 }, [x8]
	ldrsw	x8, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q1, q2, [x8, #0x20]
	ldp	q3, q4, [x8]
	neg.4s	v0, v0
	sshl.4s	v4, v4, v0
	sshl.4s	v3, v3, v0
	sshl.4s	v2, v2, v0
	sshl.4s	v0, v1, v0
	stp	q0, q2, [x2, #0x20]
	stp	q3, q4, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_shr_U32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	mov	x8, x0
	adrp	x9, 0 ; 0x1000
	add	x9, x9, #0x0
	str	x9, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	w2, [sp, #0x1c]
	mov	x2, sp
	mov	w0, #0x20
	mov	x1, x8
	bl	0x1f48
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_op_shr_U32:
	add	x8, x1, #0x1c
	ld1r.4s	{ v0 }, [x8]
	ldrsw	x8, [x1, #0x8]
	add	x8, x2, x8, lsl #6
	ldp	q1, q2, [x8, #0x20]
	ldp	q3, q4, [x8]
	neg.4s	v0, v0
	ushl.4s	v4, v4, v0
	ushl.4s	v3, v3, v0
	ushl.4s	v2, v2, v0
	ushl.4s	v0, v1, v0
	stp	q0, q2, [x2, #0x20]
	stp	q3, q4, [x2], #0x40
	ldr	x4, [x1, #0x20]!
	br	x4
_run:
	stp	x28, x27, [sp, #-0x50]!
	stp	x24, x23, [sp, #0x10]
	stp	x22, x21, [sp, #0x20]
	stp	x20, x19, [sp, #0x30]
	stp	x29, x30, [sp, #0x40]
	add	x29, sp, #0x40
	sub	sp, sp, #0x410
	mov	x19, x2
	mov	x20, x1
	mov	x22, x0
	adrp	x8, 0 ; 0x1000
	ldr	x8, [x8]
	ldr	x8, [x8]
	stur	x8, [x29, #-0x48]
	ldr	w8, [x0]
	cmp	w8, #0x11
	b.lt	0x1ff0
	lsl	x0, x8, #6
	bl	0x1fe0
	mov	x21, x0
	cbnz	w20, 0x1ff8
	b	0x203c
	mov	x21, sp
	cbz	w20, 0x203c
	ldrsw	x8, [x22, #0x4]
	add	x9, x22, x8, lsl #5
	add	x23, x9, #0x8
	add	x24, x21, x8, lsl #6
	add	x1, x22, #0x8
	mov	w22, #-0x10
	mov	x2, x21
	ldr	x8, [x1]
	mov	x0, x20
	mov	x3, x19
	blr	x8
	cmp	w20, #0x10
	csinv	w8, w22, wzr, ge
	mov	x2, x24
	mov	x1, x23
	adds	w20, w8, w20
	b.ne	0x2014
	mov	x8, sp
	cmp	x21, x8
	b.eq	0x2050
	mov	x0, x21
	bl	0x204c
	ldur	x8, [x29, #-0x48]
	adrp	x9, 0 ; 0x2000
	ldr	x9, [x9]
	ldr	x9, [x9]
	cmp	x9, x8
	b.ne	0x2084
	add	sp, sp, #0x410
	ldp	x29, x30, [sp, #0x40]
	ldp	x20, x19, [sp, #0x30]
	ldp	x22, x21, [sp, #0x20]
	ldp	x24, x23, [sp, #0x10]
	ldp	x28, x27, [sp], #0x50
	ret
	bl	0x2084
_inst_eq:
	ldp	x9, x8, [x1]
	ldr	x9, [x9]
	sxtw	x10, w0
	add	x9, x9, x10, lsl #5
	ldp	x10, x11, [x8]
	ldp	x12, x13, [x9, #-0x20]
	eor	x10, x10, x12
	eor	x11, x11, x13
	ldp	x12, x8, [x8, #0x10]
	ldp	x13, x9, [x9, #-0x10]
	eor	x12, x12, x13
	eor	x8, x8, x9
	orr	x9, x10, x11
	orr	x8, x12, x8
	orr	x8, x9, x8
	cbz	x8, 0x20d0
	mov	w0, #0x0
	ret
	str	w0, [x1, #0x10]
	mov	w0, #0x1
	ret
_compile.cold.1:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	adrp	x0, 0 ; 0x2000
	add	x0, x0, #0x0
	adrp	x1, 0 ; 0x2000
	add	x1, x1, #0x0
	adrp	x3, 0 ; 0x2000
	add	x3, x3, #0x0
	mov	w2, #0x12f
	bl	0x2100
_compile.cold.2:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	adrp	x0, 0 ; 0x2000
	add	x0, x0, #0x0
	adrp	x1, 0 ; 0x2000
	add	x1, x1, #0x0
	adrp	x3, 0 ; 0x2000
	add	x3, x3, #0x0
	mov	w2, #0x11e
	bl	0x2128
_cse_.cold.1:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	adrp	x0, 0 ; 0x2000
	add	x0, x0, #0x0
	adrp	x1, 0 ; 0x2000
	add	x1, x1, #0x0
	adrp	x3, 0 ; 0x2000
	add	x3, x3, #0x0
	mov	w2, #0xb3
	bl	0x2150
