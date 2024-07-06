#include "drivers/keyboard.h"
#include "drivers/console.h"
#include "kernel/x86.h"
#include "kernel/pic.h"
#include <stdint.h>

const char scancode_to_char[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', // 0x00 to 0x0F
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', // 0x10 to 0x1C
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', // 0x1D to 0x29
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, // 0x2A to 0x36
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x37 to 0x45
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x46 to 0x53
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // 0x54 to 0x63
};

uint8_t incoming_char = 0;
volatile uint8_t pending_char = 0;

void init_keyboard() {
    // Enable the keyboard by sending the appropriate command to the PS/2 controller
    outb(PS2_CMD_PORT, PS2_ENABLE_PORT_1);
    enable_keyboard_interrupts();
}

void enable_keyboard_interrupts() {
    // Enable IRQ1 (keyboard interrupt) in the PIC
    uint8_t mask = inb(PIC1_DATA);
    mask &= ~(1 << 1); // Clear the mask for IRQ1
    outb(PIC1_DATA, mask);
}

void keyboard_debug(uint8_t scancode) {
    if(KEY_ESCAPE_PRESSED == scancode) { kprintf("KEY_ESCAPE_PRESSED"); }
    if(KEY_1_PRESSED == scancode) { kprintf("KEY_1_PRESSED"); }
    if(KEY_2_PRESSED == scancode) { kprintf("KEY_2_PRESSED"); }
    if(KEY_3_PRESSED == scancode) { kprintf("KEY_3_PRESSED"); }
    if(KEY_4_PRESSED == scancode) { kprintf("KEY_4_PRESSED"); }
    if(KEY_5_PRESSED == scancode) { kprintf("KEY_5_PRESSED"); }
    if(KEY_6_PRESSED == scancode) { kprintf("KEY_6_PRESSED"); }
    if(KEY_7_PRESSED == scancode) { kprintf("KEY_7_PRESSED"); }
    if(KEY_8_PRESSED == scancode) { kprintf("KEY_8_PRESSED"); }
    if(KEY_9_PRESSED == scancode) { kprintf("KEY_9_PRESSED"); }
    if(KEY_0_PRESSED == scancode) { kprintf("KEY_0_PRESSED"); }
    if(KEY_MINUS_PRESSED == scancode) { kprintf("KEY_MINUS_PRESSED"); }
    if(KEY_EQUAL_PRESSED == scancode) { kprintf("KEY_EQUAL_PRESSED"); }
    if(KEY_BACKSPACE_PRESSED == scancode) { kprintf("KEY_BACKSPACE_PRESSED"); }
    if(KEY_TAB_PRESSED == scancode) { kprintf("KEY_TAB_PRESSED"); }
    if(KEY_Q_PRESSED == scancode) { kprintf("KEY_Q_PRESSED"); }
    if(KEY_W_PRESSED == scancode) { kprintf("KEY_W_PRESSED"); }
    if(KEY_E_PRESSED == scancode) { kprintf("KEY_E_PRESSED"); }
    if(KEY_R_PRESSED == scancode) { kprintf("KEY_R_PRESSED"); }
    if(KEY_T_PRESSED == scancode) { kprintf("KEY_T_PRESSED"); }
    if(KEY_Y_PRESSED == scancode) { kprintf("KEY_Y_PRESSED"); }
    if(KEY_U_PRESSED == scancode) { kprintf("KEY_U_PRESSED"); }
    if(KEY_I_PRESSED == scancode) { kprintf("KEY_I_PRESSED"); }
    if(KEY_O_PRESSED == scancode) { kprintf("KEY_O_PRESSED"); }
    if(KEY_P_PRESSED == scancode) { kprintf("KEY_P_PRESSED"); }
    if(KEY_LEFT_BRACKET_PRESSED == scancode) { kprintf("KEY_LEFT_BRACKET_PRESSED"); }
    if(KEY_RIGHT_BRACKET_PRESSED == scancode) { kprintf("KEY_RIGHT_BRACKET_PRESSED"); }
    if(KEY_ENTER_PRESSED == scancode) { kprintf("KEY_ENTER_PRESSED"); }
    if(KEY_LEFT_CONTROL_PRESSED == scancode) { kprintf("KEY_LEFT_CONTROL_PRESSED"); }
    if(KEY_A_PRESSED == scancode) { kprintf("KEY_A_PRESSED"); }
    if(KEY_S_PRESSED == scancode) { kprintf("KEY_S_PRESSED"); }
    if(KEY_D_PRESSED == scancode) { kprintf("KEY_D_PRESSED"); }
    if(KEY_F_PRESSED == scancode) { kprintf("KEY_F_PRESSED"); }
    if(KEY_G_PRESSED == scancode) { kprintf("KEY_G_PRESSED"); }
    if(KEY_H_PRESSED == scancode) { kprintf("KEY_H_PRESSED"); }
    if(KEY_J_PRESSED == scancode) { kprintf("KEY_J_PRESSED"); }
    if(KEY_K_PRESSED == scancode) { kprintf("KEY_K_PRESSED"); }
    if(KEY_L_PRESSED == scancode) { kprintf("KEY_L_PRESSED"); }
    if(KEY_SEMICOLON_PRESSED == scancode) { kprintf("KEY_SEMICOLON_PRESSED"); }
    if(KEY_SINGLE_QUOTE_PRESSED == scancode) { kprintf("KEY_SINGLE_QUOTE_PRESSED"); }
    if(KEY_BACKTICK_PRESSED == scancode) { kprintf("KEY_BACKTICK_PRESSED"); }
    if(KEY_LEFT_SHIFT_PRESSED == scancode) { kprintf("KEY_LEFT_SHIFT_PRESSED"); }
    if(KEY_BACKSLASH_PRESSED == scancode) { kprintf("KEY_BACKSLASH_PRESSED"); }
    if(KEY_Z_PRESSED == scancode) { kprintf("KEY_Z_PRESSED"); }
    if(KEY_X_PRESSED == scancode) { kprintf("KEY_X_PRESSED"); }
    if(KEY_C_PRESSED == scancode) { kprintf("KEY_C_PRESSED"); }
    if(KEY_V_PRESSED == scancode) { kprintf("KEY_V_PRESSED"); }
    if(KEY_B_PRESSED == scancode) { kprintf("KEY_B_PRESSED"); }
    if(KEY_N_PRESSED == scancode) { kprintf("KEY_N_PRESSED"); }
    if(KEY_M_PRESSED == scancode) { kprintf("KEY_M_PRESSED"); }
    if(KEY_COMMA_PRESSED == scancode) { kprintf("KEY_COMMA_PRESSED"); }
    if(KEY_DOT_PRESSED == scancode) { kprintf("KEY_DOT_PRESSED"); }
    if(KEY_SLASH_PRESSED == scancode) { kprintf("KEY_SLASH_PRESSED"); }
    if(KEY_RIGHT_SHIFT_PRESSED == scancode) { kprintf("KEY_RIGHT_SHIFT_PRESSED"); }
    if(KEY_KEYPAD_STAR_PRESSED == scancode) { kprintf("KEY_KEYPAD_STAR_PRESSED"); }
    if(KEY_LEFT_ALT_PRESSED == scancode) { kprintf("KEY_LEFT_ALT_PRESSED"); }
    if(KEY_SPACE_PRESSED == scancode) { kprintf("KEY_SPACE_PRESSED"); }
    if(KEY_CAPSLOCK_PRESSED == scancode) { kprintf("KEY_CAPSLOCK_PRESSED"); }
    if(KEY_F1_PRESSED == scancode) { kprintf("KEY_F1_PRESSED"); }
    if(KEY_F2_PRESSED == scancode) { kprintf("KEY_F2_PRESSED"); }
    if(KEY_F3_PRESSED == scancode) { kprintf("KEY_F3_PRESSED"); }
    if(KEY_F4_PRESSED == scancode) { kprintf("KEY_F4_PRESSED"); }
    if(KEY_F5_PRESSED == scancode) { kprintf("KEY_F5_PRESSED"); }
    if(KEY_F6_PRESSED == scancode) { kprintf("KEY_F6_PRESSED"); }
    if(KEY_F7_PRESSED == scancode) { kprintf("KEY_F7_PRESSED"); }
    if(KEY_F8_PRESSED == scancode) { kprintf("KEY_F8_PRESSED"); }
    if(KEY_F9_PRESSED == scancode) { kprintf("KEY_F9_PRESSED"); }
    if(KEY_F10_PRESSED == scancode) { kprintf("KEY_F10_PRESSED"); }
    if(KEY_NUMLOCK_PRESSED == scancode) { kprintf("KEY_NUMLOCK_PRESSED"); }
    if(KEY_SCROLLLOCK_PRESSED == scancode) { kprintf("KEY_SCROLLLOCK_PRESSED"); }
    if(KEY_KEYPAD_7_PRESSED == scancode) { kprintf("KEY_KEYPAD_7_PRESSED"); }
    if(KEY_KEYPAD_8_PRESSED == scancode) { kprintf("KEY_KEYPAD_8_PRESSED"); }
    if(KEY_KEYPAD_9_PRESSED == scancode) { kprintf("KEY_KEYPAD_9_PRESSED"); }
    if(KEY_KEYPAD_MINUS_PRESSED == scancode) { kprintf("KEY_KEYPAD_MINUS_PRESSED"); }
    if(KEY_KEYPAD_4_PRESSED == scancode) { kprintf("KEY_KEYPAD_4_PRESSED"); }
    if(KEY_KEYPAD_5_PRESSED == scancode) { kprintf("KEY_KEYPAD_5_PRESSED"); }
    if(KEY_KEYPAD_6_PRESSED == scancode) { kprintf("KEY_KEYPAD_6_PRESSED"); }
    if(KEY_KEYPAD_PLUS_PRESSED == scancode) { kprintf("KEY_KEYPAD_PLUS_PRESSED"); }
    if(KEY_KEYPAD_1_PRESSED == scancode) { kprintf("KEY_KEYPAD_1_PRESSED"); }
    if(KEY_KEYPAD_2_PRESSED == scancode) { kprintf("KEY_KEYPAD_2_PRESSED"); }
    if(KEY_KEYPAD_3_PRESSED == scancode) { kprintf("KEY_KEYPAD_3_PRESSED"); }
    if(KEY_KEYPAD_0_PRESSED == scancode) { kprintf("KEY_KEYPAD_0_PRESSED"); }
    if(KEY_KEYPAD_DOT_PRESSED == scancode) { kprintf("KEY_KEYPAD_DOT_PRESSED"); }
    if(KEY_F11_PRESSED == scancode) { kprintf("KEY_F11_PRESSED"); }
    if(KEY_F12_PRESSED == scancode) { kprintf("KEY_F12_PRESSED"); }
}

void handle_keyboard_irq() {
    KeyCode scancode = inb(PS2_DATA_PORT);
    // keyboard_debug(scancode);

    if(scancode > 0 && scancode < 0x80) {
        incoming_char = scancode_to_char[scancode];
        pending_char = 1;
    } else if(scancode >= 0x80) {
        // handle_key_release(scancode);
    }
}
