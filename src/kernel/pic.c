// http://www.brokenthorn.com/Resources/OSDevPic.html
#include "kernel/x86.h"
#include "kernel/pic.h"
#include "kernel/types.h"


static inline void enable_uart_interrupts()
{
    uchar tmp = inb(0x21);
    tmp &= 0xEF;
    outb(0x21, tmp);
}


void init_pic(void)
{
    pic_remap(0x20, 0x28);
    enable_uart_interrupts();
}

static inline void io_wait(void)
{
    outb(0x80, 0);
}

/*
 *  Remaps the PIC IRQ numbers because IBM and Intel has sh*tty products
 *  The reason behind this is that IRQ0 == `int $0` - this is invalid and creates
 *  collision with interrupt 0 of Intel's CPU, which is the _divide by zero_ interrupt handler.
 *  To resolve this issue, we remap IRQ0 to be `int $32`
 *  
 *  Arguments:
 * 	pic1_offset - vector offset for master PIC
 * 		vectors on the master become pic1_offset..pic1_offset+7
 * 	pic2_offset - same for slave PIC: pic2_offset..pic2_offset+7
*/
void pic_remap(int pic1_offset, int pic2_offset)
{
    // save masks 
	uchar saved_pic1, saved_pic2;
	saved_pic1 = inb(PIC1_DATA);
	saved_pic2 = inb(PIC2_DATA);
 
    // starts the initialization sequence (in cascade mode)
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); 
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();

    // ICW2: Master PIC vector offset
	outb(PIC1_DATA, pic1_offset);
	io_wait();
    // ditto for slave PIC
	outb(PIC2_DATA, pic2_offset);
	io_wait();

    // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb(PIC1_DATA, 4);
	io_wait();
    // ICW3: tell Slave PIC its cascade identity (0000 0010)
	outb(PIC2_DATA, 2);
	io_wait();
 
    // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
    // Restore saved masks
	outb(PIC1_DATA, saved_pic1);
	outb(PIC2_DATA, saved_pic2);
}


void pic_ack(uint8_t irq)
{
	if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);
 
	outb(PIC1_COMMAND,PIC_EOI);
}