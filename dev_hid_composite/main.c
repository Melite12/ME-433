/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board_api.h"
#include "tusb.h"

#include "usb_descriptors.h"

#include "main.h"
#include "ssd1306.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/cyw43_arch.h"

#define BUTTON 10

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void hid_task(void);

/*------------- MAIN -------------*/
int main(void)
{
  board_init();
  button_init();
  led_init();
    int led = 1;
  
  i2c_init_all();
  //imu_init();
    struct Data data;
  
  //ssd1306_setup();

  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

  while (1)
  { 
    if(gpio_get(BUTTON) == 0){
      led = !led;
      cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led);
    }

    //data = readData(ADDR, ACCEL_XOUT_H);

    // printf("Acc_x: %.3f   Acc_y: %.3f   Acc_z: %.3f\n", data.acc_x, data.acc_y, data.acc_z);
    // printf("Gyro_x: %.3f   Gryo_y: %.3f   Gyro_z: %.3f\n", data.gyro_x, data.gyro_y, data.gyro_z);
    // printf("Temp: %.2f\n", data.temp);

    //drawScreen(data);

    sleep_ms(100);
    tud_task(); // tinyusb device task
    led_blinking_task();

    hid_task();
  }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  blink_interval_ms = tud_mounted() ? BLINK_MOUNTED : BLINK_NOT_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
  // skip if hid is not ready yet
  if ( !tud_hid_ready() ) return;

  switch(report_id)
  {
    case REPORT_ID_KEYBOARD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_keyboard_key = false;

      if ( btn )
      {
        uint8_t keycode[6] = { 0 };
        keycode[0] = HID_KEY_A;

        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        has_keyboard_key = true;
      }else
      {
        // send empty key report if previously has key pressed
        if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_keyboard_key = false;
      }
    }
    break;

    case REPORT_ID_MOUSE:
    {
      int8_t const delta = 5;

      // no button, right + down, no scroll, no pan
      tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, delta, delta, 0, 0);
    } 
    break;

    case REPORT_ID_CONSUMER_CONTROL:
    {
      // use to avoid send multiple consecutive zero report
      static bool has_consumer_key = false;

      if ( btn )
      {
        // volume down
        uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
        has_consumer_key = true;
      }else
      {
        // send empty key report (release key) if previously has key pressed
        uint16_t empty_key = 0;
        if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
        has_consumer_key = false;
      }
    }
    break;

    case REPORT_ID_GAMEPAD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_gamepad_key = false;

      hid_gamepad_report_t report =
      {
        .x   = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0,
        .hat = 0, .buttons = 0
      };

      if ( btn )
      {
        report.hat = GAMEPAD_HAT_UP;
        report.buttons = GAMEPAD_BUTTON_A;
        tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));

        has_gamepad_key = true;
      }else
      {
        report.hat = GAMEPAD_HAT_CENTERED;
        report.buttons = 0;
        if (has_gamepad_key) tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
        has_gamepad_key = false;
      }
    }
    break;

    default: break;
  }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void)
{
  // Poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  uint32_t const btn = board_button_read();

  // Remote wakeup
  if ( tud_suspended() && btn )
  {
    // Wake up host if we are in suspend mode
    // and REMOTE_WAKEUP feature is enabled by host
    tud_remote_wakeup();
  }else
  {
    // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
    send_hid_report(REPORT_ID_MOUSE, btn); // was REPORT_ID_KEYBOARD
  }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) instance;
  (void) len;

  uint8_t next_report_id = report[0] + 1u;

  if (next_report_id < REPORT_ID_COUNT)
  {
    send_hid_report(next_report_id, board_button_read());
  }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD)
    {
      // bufsize should be (at least) 1
      if ( bufsize < 1 ) return;

      uint8_t const kbd_leds = buffer[0];

      if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
      {
        // Capslock On: disable blink, turn led on
        blink_interval_ms = 0;
        board_led_write(true);
      }else
      {
        // Caplocks Off: back to normal blink
        board_led_write(false);
        blink_interval_ms = BLINK_MOUNTED;
      }
    }
  }
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // blink is disabled
  if (!blink_interval_ms) return;

  // Blink every interval ms
  if ( board_millis() - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}



// MY FUNCTIONS

void i2c_init_all(){

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    i2c_init(I2C_PORT_1, 400*1000);
    
    gpio_set_function(I2C_SDA_1, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_1, GPIO_FUNC_I2C);

    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
    // Addr = 7 bit address of the chip 
    // buf = array of 8bit data you are sending
    // 2 = length of array (should math buf?)
    // false = write ; true = read
    // first byte of buf = address 
}

void writePin(unsigned char addr, unsigned char reg, unsigned char val){

    uint8_t buf[2] = {reg, val};
    i2c_write_blocking(I2C_PORT_1, addr, buf, 2, false);

}

unsigned char readPin(unsigned char addr, unsigned char reg){
    
    unsigned char readbuf;
    uint8_t buf[1] = {reg};

    i2c_write_blocking(I2C_PORT_1, addr, buf, 1, true);  // true to keep host control of bus
    i2c_read_blocking(I2C_PORT_1, addr, &readbuf, 1, false);  // false - finished with bus

    return readbuf;
}

struct Data readData(unsigned char addr, unsigned char reg){
    
    unsigned char readbuf[14];
    uint8_t buf[1] = {reg};

    int wr = i2c_write_blocking(I2C_PORT_1, addr, buf, 1, true);  // true to keep host control of bus
    int rd = i2c_read_blocking(I2C_PORT_1, addr, readbuf, 14, false);  // false - finished with bus

    if (wr == PICO_ERROR_GENERIC || rd == PICO_ERROR_GENERIC) {
        printf("I2C error! wr=%d rd=%d\n", wr, rd);
        // return empty/last data or handle error
    }

    struct Data data;
    data.acc_x =  (int16_t)((readbuf[0] << 8) | readbuf[1]) * ACC_TO_G ;
    data.acc_y =  (int16_t)((readbuf[2] << 8) | readbuf[3]) * ACC_TO_G;
    data.acc_z =  (int16_t)((readbuf[4] << 8) | readbuf[5]) * ACC_TO_G;
    data.temp =   (int16_t)((readbuf[6] << 8) | readbuf[7]) / 340.0 + 36.53;
    data.gyro_x = (int16_t)((readbuf[8] << 8) | readbuf[9]) * GYRO_TO_DPS;
    data.gyro_y = (int16_t)((readbuf[10] << 8) | readbuf[11]) * GYRO_TO_DPS;
    data.gyro_z = (int16_t)((readbuf[12] << 8) | readbuf[13]) * GYRO_TO_DPS;

    return data;
}

void button_init(){
  gpio_init(BUTTON);
  gpio_set_dir(BUTTON, GPIO_IN); 
  gpio_pull_up(BUTTON);
}

void led_init(){
  int rc = cyw43_arch_init();
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
}

void imu_init(){
  sleep_ms(3000);
  unsigned char testing = readPin(ADDR, WHO_AM_I);
  //printf("%d",testing);

  writePin(ADDR, PWR_MGMT_1, 0x00);
  writePin(ADDR, ACCEL_CONFIG, 0x00);
  writePin(ADDR, GYRO_CONFIG, 0b00011000);
}

void drawScreen(struct Data data){
  int x_val = (int)((data.acc_x) * 64);
  int y_val = (int)((data.acc_y) * 16);

  ssd1306_clear();
  
  if (x_val > 0){
      for (int i = 0; i < x_val; i++){
          ssd1306_drawPixel(64 - i, 16, 1);
      }
  }
  else{
      for (int i = 0; i < abs(x_val); i++){
          ssd1306_drawPixel(64 + i, 16, 1);
      }
  }

  if (y_val > 0){
      for (int i = 0; i < y_val; i++){
          ssd1306_drawPixel(64, i + 16, 1);
      }
  }
  else{
      for (int i = 0; i < abs(y_val); i++){
          ssd1306_drawPixel(64, 16 - i, 1);
      }
  }

  ssd1306_update();
}
