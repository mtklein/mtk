_done:
	ret
_seed_xy:
	ldr	x8, [x0]
	ldr	w9, [x8]
	and	w10, w1, #0xfffffff8
	add	w9, w9, w10
	scvtf	s4, w9
	dup.4s	v4, v4[0]
	adrp	x9, 0 ; 0x0
	ldr	q5, [x9]
	fadd.4s	v5, v4, v5
	adrp	x9, 0 ; 0x0
	ldr	q6, [x9]
	fadd.4s	v4, v4, v6
	movi.4s	v6, #0x3f, lsl #24
	fadd.4s	v4, v4, v6
	fadd.4s	v5, v5, v6
	stp	q4, q5, [x2, #0x40]
	ldr	s4, [x8, #0x4]
	scvtf	s4, s4
	fmov	s5, #0.50000000
	fadd	s4, s4, s5
	dup.4s	v4, v4[0]
	stp	q4, q4, [x2, #0x60]
	ldr	x3, [x0, #0x8]
	add	x0, x0, #0x10
	br	x3
_matrix_2x3:
	ldr	x8, [x0]
	ldp	q4, q5, [x2, #0x40]
	mov	x9, x8
	ld1r.4s	{ v6 }, [x9], #4
	ldp	q16, q7, [x2, #0x60]
	ldr	s17, [x9]
	add	x9, x8, #0x8
	ld1r.4s	{ v18 }, [x9]
	mov.16b	v19, v18
	fmla.4s	v19, v16, v17[0]
	fmla.4s	v18, v7, v17[0]
	fmla.4s	v18, v6, v5
	fmla.4s	v19, v6, v4
	ldp	s6, s17, [x8, #0xc]
	add	x8, x8, #0x14
	ld1r.4s	{ v20 }, [x8]
	mov.16b	v21, v20
	fmla.4s	v21, v16, v17[0]
	fmla.4s	v20, v7, v17[0]
	fmla.4s	v20, v5, v6[0]
	fmla.4s	v21, v4, v6[0]
	stp	q19, q18, [x2, #0x40]
	stp	q21, q20, [x2, #0x60]
	ldr	x3, [x0, #0x8]
	add	x0, x0, #0x10
	br	x3
_matrix_3x3:
	ldr	x8, [x0]
	ldp	q5, q4, [x2, #0x40]
	mov	x9, x8
	ld1r.4s	{ v6 }, [x9], #4
	ldp	q7, q16, [x2, #0x60]
	ldr	s17, [x9]
	add	x9, x8, #0x8
	ld1r.4s	{ v18 }, [x9]
	mov.16b	v19, v18
	fmla.4s	v19, v16, v17[0]
	fmla.4s	v18, v7, v17[0]
	fmla.4s	v18, v6, v5
	fmla.4s	v19, v6, v4
	add	x9, x8, #0x14
	ld1r.4s	{ v6 }, [x9]
	ldp	s17, s20, [x8, #0xc]
	mov.16b	v21, v6
	fmla.4s	v21, v16, v20[0]
	fmla.4s	v6, v7, v20[0]
	fmla.4s	v6, v5, v17[0]
	fmla.4s	v21, v4, v17[0]
	ldp	s17, s20, [x8, #0x18]
	add	x8, x8, #0x20
	ld1r.4s	{ v22 }, [x8]
	mov.16b	v23, v22
	fmla.4s	v23, v7, v20[0]
	fmla.4s	v22, v16, v20[0]
	fmla.4s	v22, v4, v17[0]
	fmla.4s	v23, v5, v17[0]
	fmov.4s	v4, #1.00000000
	fdiv.4s	v5, v4, v23
	fdiv.4s	v4, v4, v22
	fmul.4s	v7, v19, v4
	fmul.4s	v16, v18, v5
	stp	q16, q7, [x2, #0x40]
	fmul.4s	v4, v21, v4
	fmul.4s	v5, v6, v5
	stp	q5, q4, [x2, #0x60]
	ldr	x3, [x0, #0x8]
	add	x0, x0, #0x10
	br	x3
_shade_rgba_f32:
	ldp	x8, x3, [x0], #0x10
	ldr	q0, [x8]
	fcvtn	v3.4h, v0.4s
	dup.8h	v0, v3[0]
	dup.8h	v1, v3[1]
	dup.8h	v2, v3[2]
	dup.8h	v3, v3[3]
	br	x3
_blend_src:
	ldr	x3, [x0], #0x8
	br	x3
_blend_dst:
	ldp	q0, q1, [x2]
	ldp	q2, q3, [x2, #0x20]
	ldr	x3, [x0], #0x8
	br	x3
_blend_srcover:
	movi.8h	v4, #0x3c, lsl #8
	fsub.8h	v4, v4, v3
	ldp	q5, q6, [x2]
	fmla.8h	v0, v4, v5
	fmla.8h	v1, v4, v6
	ldp	q5, q6, [x2, #0x20]
	fmla.8h	v2, v4, v5
	fmla.8h	v3, v4, v6
	ldr	x3, [x0], #0x8
	br	x3
_clamp_01:
	movi.8h	v4, #0x3c, lsl #8
	fmin.8h	v0, v0, v4
	movi.2d	v5, #0000000000000000
	fmaxnm.8h	v0, v0, v5
	fmin.8h	v1, v1, v4
	fmaxnm.8h	v1, v1, v5
	fmin.8h	v2, v2, v4
	fmaxnm.8h	v2, v2, v5
	fmin.8h	v3, v3, v4
	fmaxnm.8h	v3, v3, v5
	ldr	x3, [x0], #0x8
	br	x3
_load:
	sub	sp, sp, #0x80
	stp	x24, x23, [sp, #0x40]
	stp	x22, x21, [sp, #0x50]
	stp	x20, x19, [sp, #0x60]
	stp	x29, x30, [sp, #0x70]
	add	x29, sp, #0x70
	mov	x19, x2
	mov	x20, x1
	mov	x21, x0
	ldp	x23, x8, [x0]
	ldr	x9, [x0, #0x10]
	and	x10, x1, #0xfffffffffffffff8
	madd	x1, x8, x10, x9
	ands	x9, x20, #0x7
	stp	q2, q3, [sp, #0x20]
	stp	q0, q1, [sp]
	b.eq	0x268
	add	x22, x19, #0x80
	mul	x2, x8, x9
	mov	x0, x22
	bl	0x254
	mov	x0, x22
	ldp	q0, q1, [sp]
	ldp	q2, q3, [sp, #0x20]
	b	0x26c
	mov	x0, x1
	blr	x23
	stp	q0, q1, [x19]
	stp	q2, q3, [x19, #0x20]
	ldr	x3, [x21, #0x18]
	add	x0, x21, #0x20
	mov	x1, x20
	ldp	q0, q1, [sp]
	ldp	q2, q3, [sp, #0x20]
	mov	x2, x19
	ldp	x29, x30, [sp, #0x70]
	ldp	x20, x19, [sp, #0x60]
	ldp	x22, x21, [sp, #0x50]
	ldp	x24, x23, [sp, #0x40]
	add	sp, sp, #0x80
	br	x3
_store:
	sub	sp, sp, #0x90
	stp	x26, x25, [sp, #0x40]
	stp	x24, x23, [sp, #0x50]
	stp	x22, x21, [sp, #0x60]
	stp	x20, x19, [sp, #0x70]
	stp	x29, x30, [sp, #0x80]
	add	x29, sp, #0x80
	mov	x19, x2
	mov	x20, x1
	mov	x21, x0
	ldp	x8, x24, [x0]
	ldr	x9, [x0, #0x10]
	and	x10, x1, #0xfffffffffffffff8
	madd	x22, x24, x10, x9
	ands	x25, x1, #0x7
	b.eq	0x324
	add	x23, x19, #0x80
	mov	x0, x23
	blr	x8
	stp	q1, q0, [sp, #0x20]
	stp	q3, q2, [sp]
	mul	x2, x24, x25
	mov	x0, x22
	mov	x1, x23
	bl	0x308
	ldr	x3, [x21, #0x18]
	add	x0, x21, #0x20
	mov	x1, x20
	ldp	q1, q0, [sp, #0x20]
	ldp	q3, q2, [sp]
	b	0x338
	mov	x0, x22
	blr	x8
	ldr	x3, [x21, #0x18]
	add	x0, x21, #0x20
	mov	x1, x20
	mov	x2, x19
	ldp	x29, x30, [sp, #0x80]
	ldp	x20, x19, [sp, #0x70]
	ldp	x22, x21, [sp, #0x60]
	ldp	x24, x23, [sp, #0x50]
	ldp	x26, x25, [sp, #0x40]
	add	sp, sp, #0x90
	br	x3
_load_rgba_f16:
	ld4.8h	{ v0, v1, v2, v3 }, [x0]
	ret
_store_rgba_f16:
	st4.8h	{ v0, v1, v2, v3 }, [x0]
	ret
_load_rgb_unorm8:
	ld3.8b	{ v2, v3, v4 }, [x0]
	ushll.8h	v0, v2, #0x0
	mov	w8, #0x1c04
	dup.8h	v5, w8
	ucvtf.8h	v0, v0
	fmul.8h	v0, v0, v5
	ushll.8h	v1, v3, #0x0
	ucvtf.8h	v1, v1
	fmul.8h	v1, v1, v5
	ushll.8h	v2, v4, #0x0
	ucvtf.8h	v2, v2
	fmul.8h	v2, v2, v5
	movi.8h	v3, #0x3c, lsl #8
	ret
_store_rgb_unorm8:
	mov	w8, #0x5bf8
	dup.8h	v4, w8
	movi.8h	v5, #0x38, lsl #8
	movi.8h	v6, #0x38, lsl #8
	fmla.8h	v6, v0, v4
	fcvtzu.8h	v6, v6
	xtn.8b	v16, v6
	movi.8h	v6, #0x38, lsl #8
	fmla.8h	v6, v1, v4
	fcvtzu.8h	v6, v6
	xtn.8b	v17, v6
	fmla.8h	v5, v2, v4
	fcvtzu.8h	v4, v5
	xtn.8b	v18, v4
	st3.8b	{ v16, v17, v18 }, [x0]
	ret
_load_rgba_unorm8:
	mov	w8, #0x1c04
	dup.8h	v3, w8
	ld4.8b	{ v4, v5, v6, v7 }, [x0]
	ushll.8h	v0, v4, #0x0
	ucvtf.8h	v0, v0
	fmul.8h	v0, v0, v3
	ushll.8h	v1, v5, #0x0
	ucvtf.8h	v1, v1
	fmul.8h	v1, v1, v3
	ushll.8h	v2, v6, #0x0
	ucvtf.8h	v2, v2
	fmul.8h	v2, v2, v3
	ushll.8h	v4, v7, #0x0
	ucvtf.8h	v4, v4
	fmul.8h	v3, v4, v3
	ret
_store_rgba_unorm8:
	mov	w8, #0x5bf8
	dup.8h	v4, w8
	movi.8h	v5, #0x38, lsl #8
	movi.8h	v6, #0x38, lsl #8
	fmla.8h	v6, v0, v4
	fcvtzu.8h	v6, v6
	xtn.8b	v16, v6
	movi.8h	v6, #0x38, lsl #8
	fmla.8h	v6, v1, v4
	fcvtzu.8h	v6, v6
	xtn.8b	v17, v6
	movi.8h	v6, #0x38, lsl #8
	fmla.8h	v6, v2, v4
	fcvtzu.8h	v6, v6
	xtn.8b	v18, v6
	fmla.8h	v5, v3, v4
	fcvtzu.8h	v4, v5
	xtn.8b	v19, v4
	st4.8b	{ v16, v17, v18, v19 }, [x0]
	ret
_load_rgba_unorm16:
	mov	w8, #0x80
	movk	w8, #0x3780, lsl #16
	dup.4s	v3, w8
	ld4.8h	{ v4, v5, v6, v7 }, [x0]
	ushll2.4s	v0, v4, #0x0
	ucvtf.4s	v0, v0
	ushll.4s	v1, v4, #0x0
	ucvtf.4s	v1, v1
	fmul.4s	v1, v1, v3
	fmul.4s	v0, v0, v3
	fcvtn	v2.4h, v0.4s
	fcvtn	v0.4h, v1.4s
	mov.d	v0[1], v2[0]
	ushll2.4s	v1, v5, #0x0
	ucvtf.4s	v1, v1
	ushll.4s	v2, v5, #0x0
	ucvtf.4s	v2, v2
	fmul.4s	v2, v2, v3
	fmul.4s	v1, v1, v3
	fcvtn	v16.4h, v1.4s
	fcvtn	v1.4h, v2.4s
	mov.d	v1[1], v16[0]
	ushll2.4s	v2, v6, #0x0
	ucvtf.4s	v2, v2
	ushll.4s	v16, v6, #0x0
	ucvtf.4s	v16, v16
	fmul.4s	v16, v16, v3
	fmul.4s	v2, v2, v3
	fcvtn	v17.4h, v2.4s
	fcvtn	v2.4h, v16.4s
	mov.d	v2[1], v17[0]
	ushll2.4s	v16, v7, #0x0
	ucvtf.4s	v16, v16
	ushll.4s	v4, v7, #0x0
	ucvtf.4s	v4, v4
	fmul.4s	v4, v4, v3
	fmul.4s	v3, v16, v3
	fcvtn	v5.4h, v3.4s
	fcvtn	v3.4h, v4.4s
	mov.d	v3[1], v5[0]
	ret
_store_rgba_unorm16:
	fcvtl	v4.4s, v0.4h
	fcvtl2	v5.4s, v0.8h
	mov	w8, #0xff00
	movk	w8, #0x477f, lsl #16
	dup.4s	v6, w8
	movi.4s	v7, #0x3f, lsl #24
	movi.4s	v16, #0x3f, lsl #24
	fmla.4s	v16, v6, v5
	movi.4s	v5, #0x3f, lsl #24
	fmla.4s	v5, v6, v4
	fcvtzu.4s	v4, v5
	xtn.4h	v17, v4
	fcvtzu.4s	v4, v16
	xtn2.8h	v17, v4
	fcvtl	v4.4s, v1.4h
	fcvtl2	v5.4s, v1.8h
	movi.4s	v16, #0x3f, lsl #24
	fmla.4s	v16, v6, v5
	movi.4s	v5, #0x3f, lsl #24
	fmla.4s	v5, v6, v4
	fcvtzu.4s	v4, v5
	xtn.4h	v18, v4
	fcvtzu.4s	v4, v16
	xtn2.8h	v18, v4
	fcvtl	v4.4s, v2.4h
	fcvtl2	v5.4s, v2.8h
	movi.4s	v16, #0x3f, lsl #24
	fmla.4s	v16, v6, v5
	movi.4s	v5, #0x3f, lsl #24
	fmla.4s	v5, v6, v4
	fcvtzu.4s	v4, v5
	xtn.4h	v19, v4
	fcvtzu.4s	v4, v16
	xtn2.8h	v19, v4
	fcvtl	v4.4s, v3.4h
	fcvtl2	v5.4s, v3.8h
	movi.4s	v16, #0x3f, lsl #24
	fmla.4s	v16, v6, v5
	fmla.4s	v7, v6, v4
	fcvtzu.4s	v4, v7
	xtn.4h	v20, v4
	fcvtzu.4s	v4, v16
	xtn2.8h	v20, v4
	st4.8h	{ v17, v18, v19, v20 }, [x0]
	ret
_drive:
	sub	sp, sp, #0x150
	stp	x28, x27, [sp, #0x110]
	stp	x22, x21, [sp, #0x120]
	stp	x20, x19, [sp, #0x130]
	stp	x29, x30, [sp, #0x140]
	add	x29, sp, #0x140
	mov	x20, x1
	mov	x19, x0
	adrp	x8, 0 ; 0x0
	ldr	x8, [x8]
	ldr	x8, [x8]
	stur	x8, [x29, #-0x38]
	cmp	w1, #0x8
	b.ge	0x608
	mov	w8, #0x0
	b	0x638
	mov	x22, #0x0
	add	x21, x19, #0x8
	ldr	x8, [x19]
	mov	x2, sp
	mov	x0, x21
	mov	x1, x22
	blr	x8
	add	x8, x22, #0x8
	add	w9, w22, #0x10
	mov	x22, x8
	cmp	w9, w20
	b.le	0x610
	cmp	w8, w20
	b.ge	0x654
	ldr	x8, [x19], #0x8
	sxtw	x1, w20
	mov	x2, sp
	mov	x0, x19
	blr	x8
	ldur	x8, [x29, #-0x38]
	adrp	x9, 0 ; 0x0
	ldr	x9, [x9]
	ldr	x9, [x9]
	cmp	x9, x8
	b.ne	0x684
	ldp	x29, x30, [sp, #0x140]
	ldp	x20, x19, [sp, #0x130]
	ldp	x22, x21, [sp, #0x120]
	ldp	x28, x27, [sp, #0x110]
	add	sp, sp, #0x150
	ret
	bl	0x684
