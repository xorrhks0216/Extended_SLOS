	.file	"swi_handler.c"
	.text
	.align	2
	.global	eventsSWIHandler
	.type	eventsSWIHandler, %function
eventsSWIHandler:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #16
	str	r0, [fp, #-20]
	str	r1, [fp, #-24]
	ldr	r2, [fp, #-20]
	mov	r3, #5056
	add	r3, r3, #19
	cmp	r2, r3
	beq	.L3
	b	.L2
.L3:
	ldr	r3, [fp, #-24]
	ldr	r3, [r3, #0]
	str	r3, [fp, #-32]
	mov	r3, #1792
	add	r3, r3, #1
	ldr	r2, [fp, #-32]
	cmp	r2, r3
	beq	.L11
	mov	r3, #1792
	add	r3, r3, #1
	ldr	r2, [fp, #-32]
	cmp	r2, r3
	bhi	.L28
	ldr	r3, [fp, #-32]
	cmp	r3, #1792
	beq	.L5
	b	.L14
.L28:
	ldr	r2, [fp, #-32]
	cmp	r2, #2048
	beq	.L13
	b	.L14
.L5:
	ldr	r3, [fp, #-24]
	ldr	r3, [r3, #4]
	str	r3, [fp, #-28]
	ldr	r3, [fp, #-28]
	cmp	r3, #123
	beq	.L7
	mov	r3, #320
	add	r3, r3, #1
	ldr	r2, [fp, #-28]
	cmp	r2, r3
	beq	.L8
	b	.L2
.L7:
	ldr	r3, [fp, #-24]
	ldr	r0, [r3, #8]
	bl	bringUpSetR9fiq
	b	.L2
.L8:
	ldr	r3, [fp, #-24]
	ldr	r0, [r3, #8]
	bl	bringUpSetR10fiq
	b	.L2
.L11:
	ldr	r3, [fp, #-24]
	ldr	r0, [r3, #4]
	bl	bringUpSetR8fiq
.L12:
	b	.L12
.L13:
	bl	ioInitializeDrivers
	b	.L2
.L14:
	ldr	r3, .L31
	ldr	r3, [r3, #0]
	cmp	r3, #1
	beq	.L15
	MSR CPSR_c,#0x5f
.L15:
	ldr	r3, [fp, #-24]
	ldr	r3, [r3, #0]
	sub	r3, r3, #2048
	sub	r3, r3, #1
	cmp	r3, #7
	ldrls	pc, [pc, r3, asl #2]
	b	.L16
	.p2align 2
.L25:
	.word	.L17
	.word	.L18
	.word	.L19
	.word	.L20
	.word	.L21
	.word	.L22
	.word	.L23
	.word	.L24
.L17:
	ldr	r4, [fp, #-24]
	ldr	r3, [fp, #-24]
	ldr	r2, [fp, #-24]
	ldr	ip, [fp, #-24]
	ldr	r0, [r3, #4]
	ldr	r1, [r2, #8]
	ldr	r2, [ip, #12]
	bl	ioOpenDriver
	str	r0, [r4, #0]
	b	.L16
.L18:
	ldr	r4, [fp, #-24]
	ldr	r3, [fp, #-24]
	ldr	r2, [fp, #-24]
	ldr	r0, [r3, #4]
	ldr	r1, [r2, #8]
	bl	ioCloseDriver
	str	r0, [r4, #0]
	b	.L16
.L19:
	ldr	r2, [fp, #-24]
	ldr	r1, [fp, #-24]
	ldr	r3, [fp, #-24]
	ldrb	r3, [r3, #12]	@ zero_extendqisi2
	ldr	r0, [r2, #4]
	ldr	r1, [r1, #8]
	mov	r2, r3
	bl	ioWriteByte
	b	.L16
.L20:
	ldr	r4, [fp, #-24]
	ldr	r3, [fp, #-24]
	ldr	r2, [fp, #-24]
	ldr	r0, [r3, #4]
	ldr	r1, [r2, #8]
	bl	ioReadByte
	mov	r3, r0
	and	r3, r3, #255
	str	r3, [r4, #0]
	b	.L16
.L21:
	ldr	r2, [fp, #-24]
	ldr	r1, [fp, #-24]
	ldr	r3, [fp, #-24]
	ldrb	r3, [r3, #12]	@ zero_extendqisi2
	ldr	r0, [r2, #4]
	ldr	r1, [r1, #8]
	mov	r2, r3
	bl	ioWriteBit
	b	.L16
.L22:
	ldr	r4, [fp, #-24]
	ldr	r3, [fp, #-24]
	ldr	r2, [fp, #-24]
	ldr	r0, [r3, #4]
	ldr	r1, [r2, #8]
	bl	ioReadBit
	mov	r3, r0
	and	r3, r3, #255
	str	r3, [r4, #0]
	b	.L16
.L23:
	ldr	r3, [fp, #-24]
	ldr	r2, [fp, #-24]
	ldr	ip, [fp, #-24]
	ldr	r0, [r3, #4]
	ldr	r1, [r2, #8]
	ldr	r2, [ip, #12]
	bl	ioWriteBlock
	b	.L16
.L24:
	ldr	r4, [fp, #-24]
	ldr	r3, [fp, #-24]
	ldr	r2, [fp, #-24]
	ldr	r0, [r3, #4]
	ldr	r1, [r2, #8]
	bl	ioReadBlock
	str	r0, [r4, #0]
.L16:
	MSR CPSR_c,#0xD3
.L2:
	bl	reportOK
	mov	r3, r0
	mov	r0, r3
	ldmea	fp, {r4, fp, sp, pc}
.L32:
	.align	2
.L31:
	.word	STATE
	.size	eventsSWIHandler, .-eventsSWIHandler
	.ident	"GCC: (GNU) 3.3.1"
