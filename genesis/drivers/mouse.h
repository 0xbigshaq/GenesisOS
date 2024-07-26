/**
 * @file mouse.h
 * @brief Mouse PS2 driver
 */
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

/**
 * @brief   Mouse packet structure, as it received from the PS2 microcontroller.
 */
typedef struct {
    bool left_button;       //!< Left button state
    bool right_button;      //!< Right button state
    bool middle_button;     //!< Middle button state
    int8_t x_movement;      //!< X-axis movement
    int8_t y_movement;      //!< Y-axis movement
    bool x_overflow;        //!< X-axis overflow
    bool y_overflow;        //!< Y-axis overflow
    bool x_sign;            //!< X-axis sign
    bool y_sign;            //!< Y-axis sign
} MousePacket;


/**
 * @brief   Mouse context object.
 */
typedef struct {
    MousePacket packet;     //!< Last mouse packet received
    int mouse_x;            //!< Mouse X position
    int mouse_y;            //!< Mouse Y position
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