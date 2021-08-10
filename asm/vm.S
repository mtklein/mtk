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
	mov	w9, #0x0
	mov	x8, x0
	str	wzr, [x8], #0x8
	lsl	x10, x25, #5
	mov	w12, #0x1
	mov	x11, x12
	tbnz	w12, #0x0, 0x27c
	ldr	w12, [x21]
	str	w12, [x21, #0x4]
	cmp	w25, #0x1
	b.lt	0x338
	mov	x12, #0x0
	mov	x13, x20
	ldrb	w14, [x13]
	cbz	w14, 0x328
	ldrb	w14, [x13, #0x1]
	cmp	w9, w14
	b.ne	0x328
	ldrsw	x14, [x21]
	add	w15, w14, #0x1
	str	w15, [x21]
	str	w14, [x13]
	ldr	x15, [x19]
	add	x16, x15, x12
	ldr	x15, [x16]
	ldp	w1, w3, [x16, #0x8]
	ldp	w2, w0, [x16, #0x10]
	ldp	w17, w16, [x16, #0x18]
	sub	w4, w17, #0x1
	cmp	w17, #0x0
	csel	w17, wzr, w4, eq
	cbz	w1, 0x2e4
	sub	w1, w1, #0x1
	ldr	w1, [x20, w1, sxtw #2]
	sub	w1, w1, w14
	cbz	w3, 0x2f4
	sub	w3, w3, #0x1
	ldr	w3, [x20, w3, sxtw #2]
	sub	w3, w3, w14
	cbz	w2, 0x304
	sub	w2, w2, #0x1
	ldr	w2, [x20, w2, sxtw #2]
	sub	w2, w2, w14
	cbz	w0, 0x314
	sub	w0, w0, #0x1
	ldr	w0, [x20, w0, sxtw #2]
	sub	w0, w0, w14
	add	x14, x8, x14, lsl #5
	str	x15, [x14]
	stp	w1, w3, [x14, #0x8]
	stp	w2, w0, [x14, #0x10]
	stp	w17, w16, [x14, #0x18]
	add	x12, x12, #0x20
	add	x13, x13, #0x4
	cmp	x10, x12
	b.ne	0x28c
	mov	w12, #0x0
	mov	w9, #0x1
	tbnz	w11, #0x0, 0x26c
	ldr	w9, [x21]
	cmp	w9, w22
	b.ne	0x418
	add	x9, x8, x26, lsl #5
	cmp	w24, #0x1
	b.lt	0x398
	mov	x10, #0x0
	adrp	x11, 0 ; 0x0
	add	x11, x11, #0x0
	ldr	x12, [x19, #0x8]
	ldr	w12, [x12, x10, lsl #2]
	cbz	w12, 0x388
	stp	xzr, xzr, [x9, #0x8]
	str	x11, [x9]
	stp	w10, w12, [x9, #0x18]
	add	x9, x9, #0x20
	ldr	w24, [x19, #0x14]
	add	x10, x10, #0x1
	cmp	x10, w24, sxtw
	b.lt	0x368
	ldrsw	x26, [x21]
	add	x10, x8, x26, lsl #5
	cmp	x9, x10
	b.ls	0x3b4
	adrp	x10, 0 ; 0x0
	add	x10, x10, #0x0
	stur	x10, [x9, #-0x20]
	b	0x3c8
	adrp	x10, 0 ; 0x0
	add	x10, x10, #0x0
	stp	x10, xzr, [x9]
	stp	xzr, xzr, [x9, #0x10]
	add	x9, x9, #0x20
	add	x8, x8, x23, lsl #5
	cmp	x9, x8
	b.ne	0x41c
	mov	x0, x20
	bl	0x3d8
	ldr	x0, [x19]
	bl	0x3e0
	ldr	x0, [x19, #0x8]
	bl	0x3e8
	ldr	x0, [x19, #0x18]
	bl	0x3f0
	mov	x0, x19
	bl	0x3f8
	mov	x0, x21
	ldp	x29, x30, [sp, #0x40]
	ldp	x20, x19, [sp, #0x30]
	ldp	x22, x21, [sp, #0x20]
	ldp	x24, x23, [sp, #0x10]
	ldp	x26, x25, [sp], #0x50
	ret
	bl	0x418
	bl	0x41c
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
	bl	0x498
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
	b.gt	0x4f0
	ldrb	w8, [x8]
	strb	w8, [x2]
	b	0x4f8
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
	bl	0x524
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
	b.gt	0x57c
	ldrh	w8, [x8]
	strh	w8, [x2]
	b	0x584
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
	bl	0x5b0
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
	b.gt	0x608
	ldr	w8, [x8]
	str	w8, [x2]
	b	0x618
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
	bl	0x64c
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
	b.gt	0x6b0
	ldrb	w9, [x9]
	strb	w9, [x8]
	b	0x6b8
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
	bl	0x6ec
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
	b.gt	0x750
	ldrh	w9, [x9]
	strh	w9, [x8]
	b	0x758
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
	bl	0x78c
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
	b.gt	0x7f0
	ldr	w9, [x9]
	str	w9, [x8]
	b	0x800
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
	bl	0x830
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
	bl	0x86c
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
	bl	0x8a8
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
	bl	0x8e4
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
	b.gt	0x984
	ldrb	w9, [x8]
	fmov	s0, w9
	ldrb	w9, [x8, #0x1]
	mov.h	v0[1], w9
	ldrb	w9, [x8, #0x2]
	mov.h	v0[2], w9
	ldrb	w8, [x8, #0x3]
	mov.h	v0[3], w8
	dup.16b	v1, v0[0]
	str	q1, [x2]
	dup.16b	v1, v0[2]
	str	q1, [x2, #0x40]
	dup.16b	v1, v0[4]
	str	q1, [x2, #0x80]
	dup.16b	v3, v0[6]
	b	0x994
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
	bl	0x9dc
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
	b.gt	0xa84
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
	b	0xa90
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
	stp	xzr, xzr, [sp, #0x8]
	adrp	x8, 0 ; 0x0
	add	x8, x8, #0x0
	str	x8, [sp]
	stp	wzr, w1, [sp, #0x18]
	mov	x1, sp
	bl	0xac0
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
_cse:
	sub	sp, sp, #0x50
	stp	x22, x21, [sp, #0x20]
	stp	x20, x19, [sp, #0x30]
	stp	x29, x30, [sp, #0x40]
	add	x29, sp, #0x40
	mov	x22, x1
	mov	x19, x0
	mov	w0, #0x0
	mov	w2, #0x20
	bl	0xaf8
	mov	x20, x0
	stp	x19, x22, [sp]
	add	x21, x19, #0x18
	str	xzr, [sp, #0x10]
	adrp	x2, 0 ; 0x0
	add	x2, x2, #0x0
	mov	x3, sp
	mov	x0, x21
	mov	x1, x20
	bl	0xb20
	tbz	w0, #0x0, 0xb30
	ldr	w19, [sp, #0x10]
	b	0xb78
	ldp	q0, q1, [x22]
	stp	q0, q1, [sp]
	ldr	x0, [x19]
	ldrsw	x1, [x19, #0x10]
	mov	w2, #0x20
	bl	0xb44
	str	x0, [x19]
	ldrsw	x8, [x19, #0x10]
	add	w9, w8, #0x1
	str	w9, [x19, #0x10]
	add	x8, x0, x8, lsl #5
	ldp	q1, q0, [sp]
	stp	q1, q0, [x8]
	ldr	w19, [x19, #0x10]
	mov	x0, x21
	mov	x1, x20
	mov	x2, x19
	bl	0xb74
	mov	x0, x19
	ldp	x29, x30, [sp, #0x40]
	ldp	x20, x19, [sp, #0x30]
	ldp	x22, x21, [sp, #0x20]
	add	sp, sp, #0x50
	ret
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
	stp	xzr, xzr, [sp, #0x8]
	adrp	x8, 0 ; 0x0
	add	x8, x8, #0x0
	str	x8, [sp]
	stp	wzr, w1, [sp, #0x18]
	mov	x1, sp
	bl	0xbd4
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
	stp	xzr, xzr, [sp, #0x8]
	adrp	x8, 0 ; 0x0
	add	x8, x8, #0x0
	str	x8, [sp]
	stp	wzr, w1, [sp, #0x18]
	mov	x1, sp
	bl	0xc2c
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
_uniform_32:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	stp	xzr, xzr, [sp, #0x8]
	adrp	x8, 0 ; 0x0
	add	x8, x8, #0x0
	str	x8, [sp]
	stp	w1, w2, [sp, #0x18]
	mov	x1, sp
	bl	0xc7c
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
_add_F16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	adrp	x8, 0 ; 0x0
	add	x8, x8, #0x0
	str	x8, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	mov	x1, sp
	bl	0xcd4
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
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
	adrp	x8, 0 ; 0x0
	add	x8, x8, #0x0
	str	x8, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	mov	x1, sp
	bl	0xd34
	mov	w0, w0
	ldp	x29, x30, [sp, #0x20]
	add	sp, sp, #0x30
	ret
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
	adrp	x8, 0 ; 0x0
	add	x8, x8, #0x0
	str	x8, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	mov	x1, sp
	bl	0xd94
	mov	w0, w0
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
_div_F16:
	sub	sp, sp, #0x30
	stp	x29, x30, [sp, #0x20]
	add	x29, sp, #0x20
	adrp	x8, 0 ; 0x0
	add	x8, x8, #0x0
	str	x8, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	mov	x1, sp
	bl	0xdf4
	mov	w0, w0
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
	adrp	x8, 0 ; 0x0
	add	x8, x8, #0x0
	str	x8, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	mov	x1, sp
	bl	0xe54
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
	adrp	x8, 0 ; 0x0
	add	x8, x8, #0x0
	str	x8, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	mov	x1, sp
	bl	0xec8
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
	adrp	x8, 0 ; 0x0
	add	x8, x8, #0x0
	str	x8, [sp]
	stp	w1, w2, [sp, #0x8]
	stp	xzr, xzr, [sp, #0x10]
	mov	x1, sp
	bl	0xf3c
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
	adrp	x8, 0 ; 0x0
	add	x8, x8, #0x0
	str	x8, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	w2, [sp, #0x1c]
	mov	x1, sp
	bl	0xfb8
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
	adrp	x8, 0 ; 0x1000
	add	x8, x8, #0x0
	str	x8, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	w2, [sp, #0x1c]
	mov	x1, sp
	bl	0x1030
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
	adrp	x8, 0 ; 0x1000
	add	x8, x8, #0x0
	str	x8, [sp]
	str	w1, [sp, #0x8]
	stur	xzr, [sp, #0x14]
	stur	xzr, [sp, #0xc]
	str	w2, [sp, #0x1c]
	mov	x1, sp
	bl	0x10ac
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
	b.lt	0x1154
	lsl	x0, x8, #6
	bl	0x1144
	mov	x21, x0
	cbnz	w20, 0x115c
	b	0x11a0
	mov	x21, sp
	cbz	w20, 0x11a0
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
	b.ne	0x1178
	mov	x8, sp
	cmp	x21, x8
	b.eq	0x11b4
	mov	x0, x21
	bl	0x11b0
	ldur	x8, [x29, #-0x48]
	adrp	x9, 0 ; 0x1000
	ldr	x9, [x9]
	ldr	x9, [x9]
	cmp	x9, x8
	b.ne	0x11e8
	add	sp, sp, #0x410
	ldp	x29, x30, [sp, #0x40]
	ldp	x20, x19, [sp, #0x30]
	ldp	x22, x21, [sp, #0x20]
	ldp	x24, x23, [sp, #0x10]
	ldp	x28, x27, [sp], #0x50
	ret
	bl	0x11e8
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
	cbz	x8, 0x1234
	mov	w0, #0x0
	ret
	str	w0, [x1, #0x10]
	mov	w0, #0x1
	ret
_compile.cold.1:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	adrp	x0, 0 ; 0x1000
	add	x0, x0, #0x0
	adrp	x1, 0 ; 0x1000
	add	x1, x1, #0x0
	adrp	x3, 0 ; 0x1000
	add	x3, x3, #0x0
	mov	w2, #0xf5
	bl	0x1264
_compile.cold.2:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	adrp	x0, 0 ; 0x1000
	add	x0, x0, #0x0
	adrp	x1, 0 ; 0x1000
	add	x1, x1, #0x0
	adrp	x3, 0 ; 0x1000
	add	x3, x3, #0x0
	mov	w2, #0xe4
	bl	0x128c
