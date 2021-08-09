_xret:
	mov	w8, #-0x29a10000
	bfi	w8, w0, #5, #5
	mov	x0, x8
	ret
_xadd:
	cmp	w2, #0x1, lsl #12
	b.hs	0x30
	and	w8, w0, #0x1f
	bfi	w8, w1, #5, #5
	bfi	w8, w2, #10, #12
	mov	w9, #-0x6f000000
	orr	w0, w8, w9
	ret
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	bl	0x38
_xsub:
	cmp	w2, #0x1, lsl #12
	b.hs	0x5c
	and	w8, w0, #0x1f
	bfi	w8, w1, #5, #5
	bfi	w8, w2, #10, #12
	mov	w9, #-0x2f000000
	orr	w0, w8, w9
	ret
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	bl	0x64
_vnot:
	cmp	w0, #0x1
	b.hi	0x8c
	and	w8, w1, #0x1f
	bfi	w8, w0, #30, #1
	bfi	w8, w2, #5, #5
	mov	w9, #0x5800
	movk	w9, #0x2e20, lsl #16
	orr	w0, w8, w9
	ret
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	bl	0x94
_vfadd:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	lsr	w8, w0, #1
	sub	x8, x8, #0x1
	cmp	w8, #0x3
	b.hs	0xd8
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	ldr	w8, [x9, x8, lsl #2]
	and	w9, w1, #0x1f
	bfi	w9, w0, #30, #1
	bfi	w9, w2, #5, #5
	bfi	w9, w3, #16, #5
	orr	w0, w9, w8
	ldp	x29, x30, [sp], #0x10
	ret
	bl	0xd8
_vfmul:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	lsr	w8, w0, #1
	sub	x8, x8, #0x1
	cmp	w8, #0x3
	b.hs	0x11c
	adrp	x9, 0 ; 0x0
	add	x9, x9, #0x0
	ldr	w8, [x9, x8, lsl #2]
	and	w9, w1, #0x1f
	bfi	w9, w0, #30, #1
	bfi	w9, w2, #5, #5
	bfi	w9, w3, #16, #5
	orr	w0, w9, w8
	ldp	x29, x30, [sp], #0x10
	ret
	bl	0x11c
_xadd.cold.1:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	adrp	x0, 0 ; 0x0
	add	x0, x0, #0x0
	adrp	x1, 0 ; 0x0
	add	x1, x1, #0x0
	adrp	x3, 0 ; 0x0
	add	x3, x3, #0x0
	mov	w2, #0x2b
	bl	0x144
_xsub.cold.1:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	adrp	x0, 0 ; 0x0
	add	x0, x0, #0x0
	adrp	x1, 0 ; 0x0
	add	x1, x1, #0x0
	adrp	x3, 0 ; 0x0
	add	x3, x3, #0x0
	mov	w2, #0x2b
	bl	0x16c
_vnot.cold.1:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	adrp	x0, 0 ; 0x0
	add	x0, x0, #0x0
	adrp	x1, 0 ; 0x0
	add	x1, x1, #0x0
	adrp	x3, 0 ; 0x0
	add	x3, x3, #0x0
	mov	w2, #0x3c
	bl	0x194
_vfadd.cold.1:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	adrp	x0, 0 ; 0x0
	add	x0, x0, #0x0
	adrp	x1, 0 ; 0x0
	add	x1, x1, #0x0
	adrp	x3, 0 ; 0x0
	add	x3, x3, #0x0
	mov	w2, #0x45
	bl	0x1bc
_vfmul.cold.1:
	stp	x29, x30, [sp, #-0x10]!
	mov	x29, sp
	adrp	x0, 0 ; 0x0
	add	x0, x0, #0x0
	adrp	x1, 0 ; 0x0
	add	x1, x1, #0x0
	adrp	x3, 0 ; 0x0
	add	x3, x3, #0x0
	mov	w2, #0x4e
	bl	0x1e4
