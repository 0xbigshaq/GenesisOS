#include "drivers/keyboard.h"
#include "drivers/console.h"
#include "kernel/x86.h"
#include "kernel/pic.h"
#include "kernel/string.h"
#include "kernel/proc.h"
#include <stdint.h>


keyboard_ctx_t k_ctx;

keyboard_ctx_t *keyboard_get_ctx() {
    return &k_ctx;
}

void keyboard_init() {
    // Enable the keyboard by sending the appropriate command to the PS/2 controller
    outb(PS2_CMD_PORT, PS2_ENABLE_PORT_1);
    memset2(&k_ctx, 0, sizeof(keyboard_ctx_t));
    keyboard_enable_interrupts();
}

void keyboard_enable_interrupts() {
    // Enable IRQ1 (keyboard interrupt) in the PIC
    uint8_t mask = inb(PIC1_DATA);
    mask &= ~(1 << 1); // Clear the mask for IRQ1
    outb(PIC1_DATA, mask);
}

void keyboard_debug(uint8_t scancode) {
    if(KEY_ESCAPE_PRESSED == scancode) { dmsg("KEY_ESCAPE_PRESSED"); }
    if(KEY_1_PRESSED == scancode) { dmsg("KEY_1_PRESSED"); }
    if(KEY_2_PRESSED == scancode) { dmsg("KEY_2_PRESSED"); }
    if(KEY_3_PRESSED == scancode) { dmsg("KEY_3_PRESSED"); }
    if(KEY_4_PRESSED == scancode) { dmsg("KEY_4_PRESSED"); }
    if(KEY_5_PRESSED == scancode) { dmsg("KEY_5_PRESSED"); }
    if(KEY_6_PRESSED == scancode) { dmsg("KEY_6_PRESSED"); }
    if(KEY_7_PRESSED == scancode) { dmsg("KEY_7_PRESSED"); }
    if(KEY_8_PRESSED == scancode) { dmsg("KEY_8_PRESSED"); }
    if(KEY_9_PRESSED == scancode) { dmsg("KEY_9_PRESSED"); }
    if(KEY_0_PRESSED == scancode) { dmsg("KEY_0_PRESSED"); }
    if(KEY_MINUS_PRESSED == scancode) { dmsg("KEY_MINUS_PRESSED"); }
    if(KEY_EQUAL_PRESSED == scancode) { dmsg("KEY_EQUAL_PRESSED"); }
    if(KEY_BACKSPACE_PRESSED == scancode) { dmsg("KEY_BACKSPACE_PRESSED"); }
    if(KEY_TAB_PRESSED == scancode) { dmsg("KEY_TAB_PRESSED"); }
    if(KEY_Q_PRESSED == scancode) { dmsg("KEY_Q_PRESSED"); }
    if(KEY_W_PRESSED == scancode) { dmsg("KEY_W_PRESSED"); }
    if(KEY_E_PRESSED == scancode) { dmsg("KEY_E_PRESSED"); }
    if(KEY_R_PRESSED == scancode) { dmsg("KEY_R_PRESSED"); }
    if(KEY_T_PRESSED == scancode) { dmsg("KEY_T_PRESSED"); }
    if(KEY_Y_PRESSED == scancode) { dmsg("KEY_Y_PRESSED"); }
    if(KEY_U_PRESSED == scancode) { dmsg("KEY_U_PRESSED"); }
    if(KEY_I_PRESSED == scancode) { dmsg("KEY_I_PRESSED"); }
    if(KEY_O_PRESSED == scancode) { dmsg("KEY_O_PRESSED"); }
    if(KEY_P_PRESSED == scancode) { dmsg("KEY_P_PRESSED"); }
    if(KEY_LEFT_BRACKET_PRESSED == scancode) { dmsg("KEY_LEFT_BRACKET_PRESSED"); }
    if(KEY_RIGHT_BRACKET_PRESSED == scancode) { dmsg("KEY_RIGHT_BRACKET_PRESSED"); }
    if(KEY_ENTER_PRESSED == scancode) { dmsg("KEY_ENTER_PRESSED"); }
    if(KEY_LEFT_CONTROL_PRESSED == scancode) { dmsg("KEY_LEFT_CONTROL_PRESSED"); }
    if(KEY_A_PRESSED == scancode) { dmsg("KEY_A_PRESSED"); }
    if(KEY_S_PRESSED == scancode) { dmsg("KEY_S_PRESSED"); }
    if(KEY_D_PRESSED == scancode) { dmsg("KEY_D_PRESSED"); }
    if(KEY_F_PRESSED == scancode) { dmsg("KEY_F_PRESSED"); }
    if(KEY_G_PRESSED == scancode) { dmsg("KEY_G_PRESSED"); }
    if(KEY_H_PRESSED == scancode) { dmsg("KEY_H_PRESSED"); }
    if(KEY_J_PRESSED == scancode) { dmsg("KEY_J_PRESSED"); }
    if(KEY_K_PRESSED == scancode) { dmsg("KEY_K_PRESSED"); }
    if(KEY_L_PRESSED == scancode) { dmsg("KEY_L_PRESSED"); }
    if(KEY_SEMICOLON_PRESSED == scancode) { dmsg("KEY_SEMICOLON_PRESSED"); }
    if(KEY_SINGLE_QUOTE_PRESSED == scancode) { dmsg("KEY_SINGLE_QUOTE_PRESSED"); }
    if(KEY_BACKTICK_PRESSED == scancode) { dmsg("KEY_BACKTICK_PRESSED"); }
    if(KEY_LEFT_SHIFT_PRESSED == scancode) { dmsg("KEY_LEFT_SHIFT_PRESSED"); }
    if(KEY_BACKSLASH_PRESSED == scancode) { dmsg("KEY_BACKSLASH_PRESSED"); }
    if(KEY_Z_PRESSED == scancode) { dmsg("KEY_Z_PRESSED"); }
    if(KEY_X_PRESSED == scancode) { dmsg("KEY_X_PRESSED"); }
    if(KEY_C_PRESSED == scancode) { dmsg("KEY_C_PRESSED"); }
    if(KEY_V_PRESSED == scancode) { dmsg("KEY_V_PRESSED"); }
    if(KEY_B_PRESSED == scancode) { dmsg("KEY_B_PRESSED"); }
    if(KEY_N_PRESSED == scancode) { dmsg("KEY_N_PRESSED"); }
    if(KEY_M_PRESSED == scancode) { dmsg("KEY_M_PRESSED"); }
    if(KEY_COMMA_PRESSED == scancode) { dmsg("KEY_COMMA_PRESSED"); }
    if(KEY_DOT_PRESSED == scancode) { dmsg("KEY_DOT_PRESSED"); }
    if(KEY_SLASH_PRESSED == scancode) { dmsg("KEY_SLASH_PRESSED"); }
    if(KEY_RIGHT_SHIFT_PRESSED == scancode) { dmsg("KEY_RIGHT_SHIFT_PRESSED"); }
    if(KEY_KEYPAD_STAR_PRESSED == scancode) { dmsg("KEY_KEYPAD_STAR_PRESSED"); }
    if(KEY_LEFT_ALT_PRESSED == scancode) { dmsg("KEY_LEFT_ALT_PRESSED"); }
    if(KEY_SPACE_PRESSED == scancode) { dmsg("KEY_SPACE_PRESSED"); }
    if(KEY_CAPSLOCK_PRESSED == scancode) { dmsg("KEY_CAPSLOCK_PRESSED"); }
    if(KEY_F1_PRESSED == scancode) { dmsg("KEY_F1_PRESSED"); }
    if(KEY_F2_PRESSED == scancode) { dmsg("KEY_F2_PRESSED"); }
    if(KEY_F3_PRESSED == scancode) { dmsg("KEY_F3_PRESSED"); }
    if(KEY_F4_PRESSED == scancode) { dmsg("KEY_F4_PRESSED"); }
    if(KEY_F5_PRESSED == scancode) { dmsg("KEY_F5_PRESSED"); }
    if(KEY_F6_PRESSED == scancode) { dmsg("KEY_F6_PRESSED"); }
    if(KEY_F7_PRESSED == scancode) { dmsg("KEY_F7_PRESSED"); }
    if(KEY_F8_PRESSED == scancode) { dmsg("KEY_F8_PRESSED"); }
    if(KEY_F9_PRESSED == scancode) { dmsg("KEY_F9_PRESSED"); }
    if(KEY_F10_PRESSED == scancode) { dmsg("KEY_F10_PRESSED"); }
    if(KEY_NUMLOCK_PRESSED == scancode) { dmsg("KEY_NUMLOCK_PRESSED"); }
    if(KEY_SCROLLLOCK_PRESSED == scancode) { dmsg("KEY_SCROLLLOCK_PRESSED"); }
    if(KEY_KEYPAD_7_PRESSED == scancode) { dmsg("KEY_KEYPAD_7_PRESSED"); }
    if(KEY_KEYPAD_8_PRESSED == scancode) { dmsg("KEY_KEYPAD_8_PRESSED"); }
    if(KEY_KEYPAD_9_PRESSED == scancode) { dmsg("KEY_KEYPAD_9_PRESSED"); }
    if(KEY_KEYPAD_MINUS_PRESSED == scancode) { dmsg("KEY_KEYPAD_MINUS_PRESSED"); }
    if(KEY_KEYPAD_4_PRESSED == scancode) { dmsg("KEY_KEYPAD_4_PRESSED"); }
    if(KEY_KEYPAD_5_PRESSED == scancode) { dmsg("KEY_KEYPAD_5_PRESSED"); }
    if(KEY_KEYPAD_6_PRESSED == scancode) { dmsg("KEY_KEYPAD_6_PRESSED"); }
    if(KEY_KEYPAD_PLUS_PRESSED == scancode) { dmsg("KEY_KEYPAD_PLUS_PRESSED"); }
    if(KEY_KEYPAD_1_PRESSED == scancode) { dmsg("KEY_KEYPAD_1_PRESSED"); }
    if(KEY_KEYPAD_2_PRESSED == scancode) { dmsg("KEY_KEYPAD_2_PRESSED"); }
    if(KEY_KEYPAD_3_PRESSED == scancode) { dmsg("KEY_KEYPAD_3_PRESSED"); }
    if(KEY_KEYPAD_0_PRESSED == scancode) { dmsg("KEY_KEYPAD_0_PRESSED"); }
    if(KEY_KEYPAD_DOT_PRESSED == scancode) { dmsg("KEY_KEYPAD_DOT_PRESSED"); }
    if(KEY_F11_PRESSED == scancode) { dmsg("KEY_F11_PRESSED"); }
    if(KEY_F12_PRESSED == scancode) { dmsg("KEY_F12_PRESSED"); }
}

void keyboard_handle_irq() {
    KeyCode scancode = inb(PS2_DATA_PORT);
    uint32_t before = k_ctx.recvd;
    k_ctx.incoming_scancode = scancode;

    // keyboard_debug(scancode);

    if(k_ctx.incoming_scancode == KEY_LEFT_SHIFT_PRESSED || k_ctx.incoming_scancode == KEY_RIGHT_SHIFT_PRESSED) {
        k_ctx.shift = 1;
    } else if(k_ctx.incoming_scancode == KEY_LEFT_SHIFT_RELEASED || k_ctx.incoming_scancode == KEY_RIGHT_SHIFT_RELEASED) {
        k_ctx.shift = 0;
    }
    else if(k_ctx.incoming_scancode == KEY_CAPSLOCK_PRESSED) {
        k_ctx.capslock = !k_ctx.capslock;
    }
    else {
        switch(k_ctx.incoming_scancode) {
            case KEY_1_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '!' : '1';
                break;
            case KEY_2_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '@' : '2';
                break;
            case KEY_3_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '#' : '3';
                break;
            case KEY_4_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '$' : '4';
                break;
            case KEY_5_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '%' : '5';
                break;
            case KEY_6_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '^' : '6';
                break;
            case KEY_7_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '&' : '7';
                break;
            case KEY_8_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '*' : '8';
                break;
            case KEY_9_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '(' : '9';
                break;
            case KEY_0_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? ')' : '0';
                break;
            case KEY_MINUS_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '_' : '-';
                break;
            case KEY_EQUAL_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '+' : '=';
                break;
            case KEY_BACKSPACE_PRESSED:
                if(k_ctx.recvd > 0) { k_ctx.recvd--; } k_ctx.pending_buf[k_ctx.recvd] = '\0';
                break;
            case KEY_TAB_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = '\t';
                break;
            case KEY_Q_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'Q' : 'q';
                break;
            case KEY_W_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'W' : 'w';
                break;
            case KEY_E_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'E' : 'e';
                break;
            case KEY_R_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'R' : 'r';
                break;
            case KEY_T_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'T' : 't';
                break;
            case KEY_Y_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'Y' : 'y';
                break;
            case KEY_U_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'U' : 'u';
                break;
            case KEY_I_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'I' : 'i';
                break;
            case KEY_O_PRESSED: 
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'O' : 'o';
                break;
            case KEY_P_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'P' : 'p';
                break;
            case KEY_LEFT_BRACKET_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '{' : '[';
                break;
            case KEY_RIGHT_BRACKET_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '}' : ']';
                break;
            case KEY_ENTER_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = '\n';
                break;
            case KEY_A_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'A' : 'a';
                break;
            case KEY_S_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'S' : 's';
                break;
            case KEY_D_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'D' : 'd';
                break;
            case KEY_F_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'F' : 'f';
                break;
            case KEY_G_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'G' : 'g';
                break;
            case KEY_H_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'H' : 'h';
                break;
            case KEY_J_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'J' : 'j';
                break;
            case KEY_K_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'K' : 'k';
                break;
            case KEY_L_PRESSED: 
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'L' : 'l';
                break;
            case KEY_SEMICOLON_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? ':' : ';';
                break;
            case KEY_SINGLE_QUOTE_PRESSED:  
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '"' : '\'';
                break;
            case KEY_BACKTICK_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '~' : '`';
                break;
            case KEY_BACKSLASH_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '|' : '\\';
                break;
            case KEY_Z_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'Z' : 'z';
                break;
            case KEY_X_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'X' : 'x';
                break;
            case KEY_C_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'C' : 'c';
                break;
            case KEY_V_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'V' : 'v';
                break;
            case KEY_B_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'B' : 'b';
                break;
            case KEY_N_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'N' : 'n';
                break;
            case KEY_M_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? 'M' : 'm';
                break;
            case KEY_COMMA_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '<' : ',';
                break;
            case KEY_DOT_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '>' : '.';
                break;
            case KEY_SLASH_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = k_ctx.shift ? '?' : '/';
                break;
            case KEY_SPACE_PRESSED:
                k_ctx.pending_buf[k_ctx.recvd++] = ' ';
                break;
            default:
                break;

        }
    }
    if(before < k_ctx.recvd) {
        k_ctx.pending_char = 1;
        // pop_cli();
        wakeup(1); // channel 1 is for keyboard
    }
}

static int min(int a, int b) {
    return ((a) < (b) ? (a) : (b));
}

int keyboard_flush_pending_buf(uint32_t count, uint8_t *out) {
    keyboard_ctx_t *k = keyboard_get_ctx();
    uint32_t size;
    if(count == 0 || k->recvd == 0) {
        return 0;
    }
    size = min(count, k->recvd);

    memcpy(out, &k->pending_buf, size);    
    memmove(k->pending_buf, &k->pending_buf[size], (sizeof(k->pending_buf) - size));

    k->recvd -= size;
    k->consumed += size;

    if(k->recvd <= 0)
        keyboard_clear_pending_buf();

    return size;
}

int keyboard_clear_pending_buf() {
    keyboard_ctx_t *k = keyboard_get_ctx();
    dmsg("...");
    k->pending_char = 0;
    k->recvd = 0;
    k->consumed = 0;
    // memset2(k->pending_buf, 0, sizeof(k->pending_buf));
    return 0;
}