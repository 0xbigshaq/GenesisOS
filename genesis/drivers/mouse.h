#ifndef MOUSE_DRIVER_H
#define MOUSE_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

#define MOUSE_CMD_MOUSE_ID                      0xF2
#define MOUSE_CMD_SAMPLE_RATE                   0xF3
#define MOUSE_CMD_ENABLE_DATA_REPORTING         0xF4
#define MOUSE_CMD_DISABLE_DATA_REPORTING        0xF5
#define MOUSE_CMD_SET_DEFAULTS                  0xF6

#define MOUSE_ACK         0xFA
#define PS2_DATA_PORT     0x60
#define PS2_CMD_PORT      0x64
#define PS2_MOUSE_PORT    0xD4

#define ENABLE_SECOND_PS2 0xA8

typedef struct {
    bool left_button;
    bool right_button;
    bool middle_button;
    int8_t x_movement;
    int8_t y_movement;
    bool x_overflow;
    bool y_overflow;
    bool x_sign;
    bool y_sign;
} MousePacket;


typedef struct {
    MousePacket packet;
    int mouse_x;
    int mouse_y;
} mouse_ctx_t;


void update_mouse_position(MousePacket *mp);
void print_mouse_packet(MousePacket *mp);
void mouse_send_cmd(uint8_t data);
uint8_t mouse_read_byte(void);
void handle_mouse_irq();
void init_mouse(void);
void draw_mouse(uint32_t color);
mouse_ctx_t* get_mouse_ctx(void);

#endif // MOUSE_DRIVER_H