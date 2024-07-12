#include "kernel/x86.h"
#include "kernel/pic.h"
#include "kernel/string.h"
#include "kernel/kmalloc.h"
#include "drivers/mouse.h"
#include "drivers/console.h"
#include "drivers/gfx/framebuffer.h"

// globs
mouse_ctx_t mouse_ctx = {
    // mouse position
    .mouse_x = SCREEN_WIDTH / 2,
    .mouse_y = SCREEN_HEIGHT / 2,

    // latest packet
    .packet.left_button = NULL,
    .packet.right_button = NULL,
    .packet.middle_button = NULL,
    .packet.x_movement = NULL,
    .packet.y_movement = NULL,
    .packet.x_overflow = NULL,
    .packet.y_overflow = NULL,
    .packet.x_sign = NULL,
    .packet.y_sign = NULL
};

mouse_ctx_t* get_mouse_ctx() {
    return &mouse_ctx;
}

// funcs
MousePacket* parse_mouse_packet(uint8_t packet[3]) {
    MousePacket incoming;
    incoming.left_button    = packet[0] & 0x01;
    incoming.right_button   = (packet[0] >> 1) & 0x01;
    incoming.middle_button  = (packet[0] >> 2) & 0x01;
    incoming.x_overflow     = (packet[0] >> 6) & 0x01;
    incoming.y_overflow     = (packet[0] >> 7) & 0x01;
    incoming.x_sign         = (packet[0] >> 4) & 0x01;
    incoming.y_sign         = (packet[0] >> 5) & 0x01;

    incoming.x_movement = (int8_t)packet[1];
    incoming.y_movement = (int8_t)packet[2];

    memcpy(&mouse_ctx.packet, &incoming, sizeof(MousePacket));

    return &mouse_ctx.packet;
}

void update_mouse_position(MousePacket *mp) {
    mouse_ctx.mouse_x += mp->x_movement;
    mouse_ctx.mouse_y -= mp->y_movement; // Typically, Y movement is inverted

    // Clamp the position to screen bounds
    if (mouse_ctx.mouse_x < 0) mouse_ctx.mouse_x = 0;
    if (mouse_ctx.mouse_x >= SCREEN_WIDTH) mouse_ctx.mouse_x = SCREEN_WIDTH - 1;
    if (mouse_ctx.mouse_y < 0) mouse_ctx.mouse_y = 0;
    if (mouse_ctx.mouse_y >= SCREEN_HEIGHT) mouse_ctx.mouse_y = SCREEN_HEIGHT - 1;
}

void print_mouse_packet(MousePacket *mp) {
    kprintf("\nLeft Button: %s\t\t", mp->left_button ? "Pressed" : "Released");
    kprintf("Right Button: %s\t", mp->right_button ? "Pressed" : "Released");
    // kprintf("Middle Button: %s\n", mp->middle_button ? "Pressed" : "Released");
    // kprintf("X Movement: %d\t", mp->x_movement);
    // kprintf("Y Movement: %d\t", mp->y_movement);

    kprintf("\tx=%d\ty=%d", mouse_ctx.mouse_x, mouse_ctx.mouse_y);
}

void mouse_send_cmd(uint8_t data) {
    outb(PS2_CMD_PORT, PS2_MOUSE_PORT);
    outb(PS2_DATA_PORT, data);
}

uint8_t mouse_read_byte(void) {
    return inb(PS2_DATA_PORT);
}

void handle_mouse_irq() {
    static uint8_t mouse_cycle = 1;
    static char mouse_byte[3];
    uint8_t packet[3];
    MousePacket *mp;

    switch (mouse_cycle) {
        case 1:
            mouse_byte[0] = mouse_read_byte();
            mouse_cycle++;
            break;
        case 2:
            mouse_byte[1] = mouse_read_byte();
            mouse_cycle++;
            break;
        case 3:
            mouse_byte[2] = mouse_read_byte();
            packet[0] = mouse_byte[0];
            packet[1] = mouse_byte[1];
            packet[2] = mouse_byte[2];
            mp = parse_mouse_packet(packet);
            // print_mouse_packet(mp);

            // if(!mp->left_button) { draw_mouse(BG_COLOR); }
            update_mouse_position(mp);
            // draw_mouse(C_COLOR);
            // if(mp->right_button) {  fill_screen(BG_COLOR); }

            mouse_cycle = 1;
            break;
    }
}

void enable_mouse_interrupts() {
    uint8_t result;
    // Read "byte 0" from internal RAM (Controller Configuration Byte)
    outb(PS2_CMD_PORT, 0x20);

    // Enable the 2nd bit = Second PS/2 port interrupt (1 = enabled, 0 = disabled)
    result = (inb(PS2_DATA_PORT) | 2);

    // Update Controller Configuration Byte
    outb(PS2_CMD_PORT, PS2_DATA_PORT);
    outb(PS2_DATA_PORT, result);
}

void init_mouse(void) {
    uint8_t result;

    // Enable the auxiliary device (mouse) by sending the appropriate cmd to the PS/2 controller
    outb(PS2_CMD_PORT, ENABLE_SECOND_PS2);

    // Read Mouse ID
    mouse_send_cmd(MOUSE_CMD_MOUSE_ID);
    mouse_read_byte(); // ack
    result = mouse_read_byte();
    kprintf("[*] Mouse ID: %d\n", result);
    if(result != 0) {
        PANIC("Fatal: Mouse ID is expected to be 0(=Standard PS/2 mouse).")
    }

    // Set defaults
    mouse_send_cmd(MOUSE_CMD_SET_DEFAULTS);
    result = mouse_read_byte();
    if(result != MOUSE_ACK) {
        PANIC("Fatal: Failed to set default mouse settings.");
    }

    // Enable streaming
    mouse_send_cmd(MOUSE_CMD_ENABLE_DATA_REPORTING);
    result = mouse_read_byte();

    if(result != MOUSE_ACK) {
        PANIC("Fatal: failed to enable mouse PS2 data reporting.");
    }

    // Enable mouse interrupts
    enable_mouse_interrupts();
}

void draw_mouse(uint32_t color) {
    fill_square(mouse_ctx.mouse_x, mouse_ctx.mouse_y, 0x5, color);
}