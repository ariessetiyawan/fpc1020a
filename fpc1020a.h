//
//  FPC1020.h
//  Created by Deray on 2015-10-07.
//

#ifndef FPC1020_h
#define FPC1020_h


#include "Arduino.h"

#define ACK_SUCCESS    0x00
#define ACK_FAIL       0x01
#define ACK_FULL       0x04
#define ACK_NOUSER     0x05
#define ACK_USER_EXIST 0x07
#define ACK_TIMEOUT    0x08

#define ACK_GO_OUT 0x0F

#define ACK_ALL_USER    0x00
#define ACK_GUEST_USER  0x01
#define ACK_NORMAL_USER 0x02
#define ACK_MASTER_USER 0x03

#define USER_MAX_CNT 50

#define HEAD 0
#define CMD  1
#define CHK  6
#define TAIL 7

#define P1 2
#define P2 3
#define P3 4
#define Q1 2
#define Q2 3
#define Q3 4

#define CMD_HEAD     0xF5
#define CMD_TAIL     0xF5
#define CMD_ADD_1    0x01
#define CMD_ADD_2    0x02
#define CMD_ADD_3    0x03
#define CMD_MATCH    0x0C
#define CMD_DEL      0x04
#define CMD_DEL_ALL  0x05
#define CMD_USER_CNT 0x09

#define CMD_BAUD 0x21

#define CMD_SLEEP_MODE 0x2C
#define CMD_ADD_MODE   0x2D

#define CMD_FINGER_DETECTED 0x14

typedef enum {
    FINGER_NOT_REPEAT,
    FINGER_ALLOW_REPEAT
} fpc1020a_add_finger_mode_t;

typedef enum {
    BAUD_9600 = 1,
    BAUD_19200,
    BAUD_38400,
    BAUD_57600,
    BAUD_115200,
} fpc1020a_uart_baud_t;

class FPC1020A {
   private:
    HardwareSerial *_serial;
    HardwareSerial *_debug;
    bool _enable_debug = false;
    uint8_t tx;
    uint8_t rx;

   public:
    FPC1020A();
    bool begin(HardwareSerial *serial = &Serial2, uint8_t rx = 16,
               uint8_t tx = 17, unsigned long baud = 19200);
    bool enableDebug(HardwareSerial *debug_serial);
    uint8_t sendCMD(uint16_t timeout);
    bool sleep(void);

    bool setFingerMode(fpc1020a_add_finger_mode_t mode = FINGER_NOT_REPEAT);
    uint8_t getFingerMode();

    uint16_t getUserCount(void);
    bool addFinger(uint8_t id, uint8_t permission, uint8_t adding_count);
    bool delFinger(uint8_t id);
    bool delAllFinger();

    uint8_t available(time_t timeout = 500);
    uint8_t getFingerID(void);
    uint8_t getFingerPermission(void);

    bool setBaud(unsigned long baud = 19200);

   public:
    uint8_t TxBuf[9];
    uint8_t RxBuf[9];
};

#endif