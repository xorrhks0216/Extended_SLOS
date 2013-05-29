CC = arm-elf-gcc
ASSEMBLER = arm-elf-as
LINKER = arm-elf-ld -Map ../image/map.dat 
CONVERT = arm-elf-strip -O binary -o
CFLAGS = -mapcs -fomit-frame-pointer -O1 -c
ASFLAGS =

OBJS = start.o \
init.o \
pcb.o \
swiapi.o \
undef.o \
swi.o \
prefetch.o \
data.o \
reserved.o \
irq.o \
fiq.o \
scheduler.o \
swi_handler.o \
irq_handler.o \
bringup.o \
cpsr.o \
event_init.o \
button_service.o \
tick_service.o \
ddf_io.o \
led_driver.o \
serial_driver.o \
segment_driver.o \
mutex.o \
cinit.o \
cli_com.o \
cli_readline.o \
cli_shell.o \
cli_serial.o \
uid.o \
task1_start.o \
task2_start.o \
task3_start.o

all : ${OBJS}
	${LINKER} -o ../image/slos.elf -T linker.dat
	${CONVERT} ../image/slos.bin -s ../image/slos.elf

start.o : ../src/start.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
init.o : ../src/LDS2000/init.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
pcb.o : ../src/LDS2000/pcb.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
undef.o : ../src/undef.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
swi.o : ../src/swi.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
prefetch.o : ../src/prefetch.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
data.o : ../src/data.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
reserved.o : ../src/reserved.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
irq.o : ../src/irq.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
fiq.o : ../src/fiq.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
scheduler.o : ../src/scheduler.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
irq_handler.o : ../src/LDS2000/events/irq_handler.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
bringup.o : ../src/events/bringup.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
cpsr.o : ../src/events/cpsr.s
	${ASSEMBLER} ${ASFLAGS} -o $@ $<
 
event_init.o : ../src/LDS2000/events/event_init.c
	${CC} ${CFLAGS} -o $@ $<
 
button_service.o : ../src/LDS2000/events/button_service.c
	${CC} ${CFLAGS} -o $@ $<
 
swiapi.o : ../src/core/swiapi.c
	${CC} ${CFLAGS} -o $@ $<

swi_handler.o : ../src/LDS2000/events/swi_handler.c
	${CC} ${CFLAGS} -O1 -c -o $@ $<

tick_service.o : ../src/LDS2000/events/tick_service.c
	${CC} ${CFLAGS} -o $@ $<
 
ddf_io.o : ../src/devices/ddf_io.c
	${CC} ${CFLAGS} -o $@ $<
 
led_driver.o : ../src/LDS2000/devices/led_driver.c
	${CC} ${CFLAGS} -o $@ $<
 
serial_driver.o : ../src/LDS2000/devices/serial_driver.c
	${CC} ${CFLAGS} -o $@ $<
 
segment_driver.o : ../src/LDS2000/devices/segment_driver.c
	${CC} ${CFLAGS} -o $@ $<
 
mutex.o : ../src/core/mutex.c
	${CC} ${CFLAGS} -o $@ $<
 
cinit.o : ../src/core/cinit.c
	${CC} ${CFLAGS} -o $@ $<
 
cli_com.o : ../src/core/cli_com.c
	${CC} ${CFLAGS} -o $@ $<
 
cli_readline.o : ../src/core/cli_readline.c
	${CC} ${CFLAGS} -o $@ $<
 
cli_shell.o : ../src/core/cli_shell.c
	${CC} ${CFLAGS} -o $@ $<
 
cli_serial.o : ../src/core/cli_serial.c
	${CC} ${CFLAGS} -o $@ $<
 
uid.o : ../src/core/uid.c
	${CC} ${CFLAGS} -o $@ $<
 
task1_start.o : ../src/apps/task1_start.c
	${CC} ${CFLAGS} -o $@ $<
 
task2_start.o : ../src/apps/task2_start.c
	${CC} ${CFLAGS} -o $@ $<
 
task3_start.o : ../src/apps/task3_start.c
	${CC} ${CFLAGS} -o $@ $<

clean:
	rm -rf *.o
