#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include "kernel/types.h"

#define PS2_DATA_PORT      0x60
#define PS2_CMD_PORT       0x64
#define PS2_ENABLE_PORT_1  0xAE


typedef enum {
    KEY_ESCAPE_PRESSED = 0x01,
    KEY_1_PRESSED = 0x02,
    KEY_2_PRESSED = 0x03,
    KEY_3_PRESSED = 0x04,
    KEY_4_PRESSED = 0x05,
    KEY_5_PRESSED = 0x06,
    KEY_6_PRESSED = 0x07,
    KEY_7_PRESSED = 0x08,
    KEY_8_PRESSED = 0x09,
    KEY_9_PRESSED = 0x0A,
    KEY_0_PRESSED = 0x0B,
    KEY_MINUS_PRESSED = 0x0C,
    KEY_EQUAL_PRESSED = 0x0D,
    KEY_BACKSPACE_PRESSED = 0x0E,
    KEY_TAB_PRESSED = 0x0F,
    KEY_Q_PRESSED = 0x10,
    KEY_W_PRESSED = 0x11,
    KEY_E_PRESSED = 0x12,
    KEY_R_PRESSED = 0x13,
    KEY_T_PRESSED = 0x14,
    KEY_Y_PRESSED = 0x15,
    KEY_U_PRESSED = 0x16,
    KEY_I_PRESSED = 0x17,
    KEY_O_PRESSED = 0x18,
    KEY_P_PRESSED = 0x19,
    KEY_LEFT_BRACKET_PRESSED = 0x1A,
    KEY_RIGHT_BRACKET_PRESSED = 0x1B,
    KEY_ENTER_PRESSED = 0x1C,
    KEY_LEFT_CONTROL_PRESSED = 0x1D,
    KEY_A_PRESSED = 0x1E,
    KEY_S_PRESSED = 0x1F,
    KEY_D_PRESSED = 0x20,
    KEY_F_PRESSED = 0x21,
    KEY_G_PRESSED = 0x22,
    KEY_H_PRESSED = 0x23,
    KEY_J_PRESSED = 0x24,
    KEY_K_PRESSED = 0x25,
    KEY_L_PRESSED = 0x26,
    KEY_SEMICOLON_PRESSED = 0x27,
    KEY_SINGLE_QUOTE_PRESSED = 0x28,
    KEY_BACKTICK_PRESSED = 0x29,
    KEY_LEFT_SHIFT_PRESSED = 0x2A,
    KEY_BACKSLASH_PRESSED = 0x2B,
    KEY_Z_PRESSED = 0x2C,
    KEY_X_PRESSED = 0x2D,
    KEY_C_PRESSED = 0x2E,
    KEY_V_PRESSED = 0x2F,
    KEY_B_PRESSED = 0x30,
    KEY_N_PRESSED = 0x31,
    KEY_M_PRESSED = 0x32,
    KEY_COMMA_PRESSED = 0x33,
    KEY_DOT_PRESSED = 0x34,
    KEY_SLASH_PRESSED = 0x35,
    KEY_RIGHT_SHIFT_PRESSED = 0x36,
    KEY_KEYPAD_STAR_PRESSED = 0x37,
    KEY_LEFT_ALT_PRESSED = 0x38,
    KEY_SPACE_PRESSED = 0x39,
    KEY_CAPSLOCK_PRESSED = 0x3A,
    KEY_F1_PRESSED = 0x3B,
    KEY_F2_PRESSED = 0x3C,
    KEY_F3_PRESSED = 0x3D,
    KEY_F4_PRESSED = 0x3E,
    KEY_F5_PRESSED = 0x3F,
    KEY_F6_PRESSED = 0x40,
    KEY_F7_PRESSED = 0x41,
    KEY_F8_PRESSED = 0x42,
    KEY_F9_PRESSED = 0x43,
    KEY_F10_PRESSED = 0x44,
    KEY_NUMLOCK_PRESSED = 0x45,
    KEY_SCROLLLOCK_PRESSED = 0x46,
    KEY_KEYPAD_7_PRESSED = 0x47,
    KEY_KEYPAD_8_PRESSED = 0x48,
    KEY_KEYPAD_9_PRESSED = 0x49,
    KEY_KEYPAD_MINUS_PRESSED = 0x4A,
    KEY_KEYPAD_4_PRESSED = 0x4B,
    KEY_KEYPAD_5_PRESSED = 0x4C,
    KEY_KEYPAD_6_PRESSED = 0x4D,
    KEY_KEYPAD_PLUS_PRESSED = 0x4E,
    KEY_KEYPAD_1_PRESSED = 0x4F,
    KEY_KEYPAD_2_PRESSED = 0x50,
    KEY_KEYPAD_3_PRESSED = 0x51,
    KEY_KEYPAD_0_PRESSED = 0x52,
    KEY_KEYPAD_DOT_PRESSED = 0x53,
    KEY_F11_PRESSED = 0x57,
    KEY_F12_PRESSED = 0x58,
    KEY_ESCAPE_RELEASED = 0x81,
    KEY_1_RELEASED = 0x82,
    KEY_2_RELEASED = 0x83,
    KEY_3_RELEASED = 0x84,
    KEY_4_RELEASED = 0x85,
    KEY_5_RELEASED = 0x86,
    KEY_6_RELEASED = 0x87,
    KEY_7_RELEASED = 0x88,
    KEY_8_RELEASED = 0x89,
    KEY_9_RELEASED = 0x8A,
    KEY_0_RELEASED = 0x8B,
    KEY_MINUS_RELEASED = 0x8C,
    KEY_EQUAL_RELEASED = 0x8D,
    KEY_BACKSPACE_RELEASED = 0x8E,
    KEY_TAB_RELEASED = 0x8F,
    KEY_Q_RELEASED = 0x90,
    KEY_W_RELEASED = 0x91,
    KEY_E_RELEASED = 0x92,
    KEY_R_RELEASED = 0x93,
    KEY_T_RELEASED = 0x94,
    KEY_Y_RELEASED = 0x95,
    KEY_U_RELEASED = 0x96,
    KEY_I_RELEASED = 0x97,
    KEY_O_RELEASED = 0x98,
    KEY_P_RELEASED = 0x99,
    KEY_LEFT_BRACKET_RELEASED = 0x9A,
    KEY_RIGHT_BRACKET_RELEASED = 0x9B,
    KEY_ENTER_RELEASED = 0x9C,
    KEY_LEFT_CONTROL_RELEASED = 0x9D,
    KEY_A_RELEASED = 0x9E,
    KEY_S_RELEASED = 0x9F,
    KEY_D_RELEASED = 0xA0,
    KEY_F_RELEASED = 0xA1,
    KEY_G_RELEASED = 0xA2,
    KEY_H_RELEASED = 0xA3,
    KEY_J_RELEASED = 0xA4,
    KEY_K_RELEASED = 0xA5,
    KEY_L_RELEASED = 0xA6,
    KEY_SEMICOLON_RELEASED = 0xA7,
    KEY_SINGLE_QUOTE_RELEASED = 0xA8,
    KEY_BACKTICK_RELEASED = 0xA9,
    KEY_LEFT_SHIFT_RELEASED = 0xAA,
    KEY_BACKSLASH_RELEASED = 0xAB,
    KEY_Z_RELEASED = 0xAC,
    KEY_X_RELEASED = 0xAD,
    KEY_C_RELEASED = 0xAE,
    KEY_V_RELEASED = 0xAF,
    KEY_B_RELEASED = 0xB0,
    KEY_N_RELEASED = 0xB1,
    KEY_M_RELEASED = 0xB2,
    KEY_COMMA_RELEASED = 0xB3,
    KEY_DOT_RELEASED = 0xB4,
    KEY_SLASH_RELEASED = 0xB5,
    KEY_RIGHT_SHIFT_RELEASED = 0xB6,
    KEY_KEYPAD_STAR_RELEASED = 0xB7,
    KEY_LEFT_ALT_RELEASED = 0xB8,
    KEY_SPACE_RELEASED = 0xB9,
    KEY_CAPSLOCK_RELEASED = 0xBA,
    KEY_F1_RELEASED = 0xBB,
    KEY_F2_RELEASED = 0xBC,
    KEY_F3_RELEASED = 0xBD,
    KEY_F4_RELEASED = 0xBE,
    KEY_F5_RELEASED = 0xBF,
    KEY_F6_RELEASED = 0xC0,
    KEY_F7_RELEASED = 0xC1,
    KEY_F8_RELEASED = 0xC2,
    KEY_F9_RELEASED = 0xC3,
    KEY_F10_RELEASED = 0xC4,
    KEY_NUMLOCK_RELEASED = 0xC5,
    KEY_SCROLLLOCK_RELEASED = 0xC6,
    KEY_KEYPAD_7_RELEASED = 0xC7,
    KEY_KEYPAD_8_RELEASED = 0xC8,
    KEY_KEYPAD_9_RELEASED = 0xC9,
    KEY_KEYPAD_MINUS_RELEASED = 0xCA,
    KEY_KEYPAD_4_RELEASED = 0xCB,
    KEY_KEYPAD_5_RELEASED = 0xCC,
    KEY_KEYPAD_6_RELEASED = 0xCD,
    KEY_KEYPAD_PLUS_RELEASED = 0xCE,
    KEY_KEYPAD_1_RELEASED = 0xCF,
    KEY_KEYPAD_2_RELEASED = 0xD0,
    KEY_KEYPAD_3_RELEASED = 0xD1,
    KEY_KEYPAD_0_RELEASED = 0xD2,
    KEY_KEYPAD_DOT_RELEASED = 0xD3,
    KEY_F11_RELEASED = 0xD7,
    KEY_F12_RELEASED = 0xD8
} KeyCode;

extern uint8_t incoming_char;
extern volatile uint8_t pending_char;

void init_keyboard();
void enable_keyboard_interrupts();
void handle_keyboard_irq();


#endif // KEYBOARD_DRIVER_H