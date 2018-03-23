/*
  ============================================================================================
    AVR
  ============================================================================================
*/

#include "motion.h"
#ifndef ISPC
#include<arduino.h>

//Known board in boards.h
#define ISRTEMP // 120bytes check board.h
#define MOTOR_0_DIR -1 // 1: normal -1:inverted
#define MOTOR_1_DIR -1 // 1: normal -1:inverted
#define MOTOR_2_DIR -1 // 1: normal -1:inverted
#define MOTOR_3_DIR 1 // 1: normal -1:inverted


// ==========================================================
#if defined(__AVR__)
//#define BOARD_CHCSHIELDV3
//#define BOARD_TARANTHOLE
//#define BOARD_SEMEDIYNANO
//#define BOARD_NANONANO
//#define BOARD_NANONANO_DELTA
//#define BOARD_NANONANO_DELTA_NOSD
//#define BOARD_NANONANO_SDCARD
//#define BOARD_GEN7
//#define BOARD_RAMP13
//#define BOARD_RAMP13_DELTA
//#define BOARD_RAMP13_3DPLEX
#define BOARD_NANO_3DPLEX
//#define BOARD_SEMEDIY128AU
#define ANALOGSHIFT 0 // 10bit adc
// ==========================================================
#elif defined(__ARM__)
#define BOARD_NANONANO_STM32
#define ANALOGSHIFT 2 // 12bit adc
// ==========================================================
#elif defined(ESP8266)
#define BOARD_NANONANO_WEMOS
#define ANALOGSHIFT 0 // 10bit adc ??
//#define BOARD_ESP01CNC_V1
#endif

#include "boards.h"
#define USE_EEPROM

#else
// for PC no pins
#endif


/*
  ============================================================================================
    CONFIGURATION
  ============================================================================================
*/

#define BACKPLANNER // 852Bytes code !
#define BACKPLANNERRATIO 2 // twice acceleration
#define USEDIO // 750bytes this can save almost 20us each bresenham step, is a MUST if not using timer!
//#define USE_BACKLASH  // 400bytes code
#define USETIMER1 // Work in progress // 98 bytes// FLASH SAVING
//#define SAVE_RESETMOTION  // 1000 bytes code, no reset motion, need EEPROM

// ==========================================================
#ifdef SDCARD_CS
#define USE_SDCARD
#endif


#ifndef __AVR__
#undef USEDIO
#undef ISRTEMP
#endif

#ifdef ISPC
#undef USETIMER1
#undef SAVE_RESETMOTION
#endif


//#define motortimeout 10000000 // 10 seconds

//#define DRIVE_COREXY
//#define DRIVE_COREXZ

//#define DRIVE_DELTA
//#define DRIVE_DELTASIAN


#ifdef DRIVE_DELTA
#define NONLINEAR
#endif 
#ifdef DRIVE_DELTASIAN
#define NONLINEAR
#endif 


#define TOWER_X_ANGLE_DEG        210
#define TOWER_Y_ANGLE_DEG        330
#define TOWER_Z_ANGLE_DEG        90
#define DELTA_DIAGONAL_ROD 180
#define DELTA_RADIUS 85

// Motion configuration

#define HOMINGSPEED 60
#define XOFFSET 0
#define YOFFSET 0
#define ZOFFSET 0
#define EOFFSET 0

#define XYJERK 15
#define XACCELL 300
#define XMOVEACCELL 600

#define XMAXFEEDRATE 80
#define YMAXFEEDRATE 80
#define ZMAXFEEDRATE 80
#define E0MAXFEEDRATE 20

#define XSTEPPERMM 400//131//178
#define YSTEPPERMM 400//175//125
#define ZSTEPPERMM 400//1020//1020 //420
#define E0STEPPERMM 720//340//380

#ifndef NUMBUFFER
#define NUMBUFFER 6
#endif

#define XMAX 0
#define YMAX 0
#define ZMAX 152.4

#define MOTOR_X_BACKLASH 0  // MOTOR 0 = X, 1= Y 2=Z 3=E
#define MOTOR_Y_BACKLASH 0
#define MOTOR_Z_BACKLASH 0
#define MOTOR_E_BACKLASH 0

//#define AUTO_MOTOR_Z_OFF



//#define INVERTENDSTOP // uncomment for normally open

#define ENDSTOP_MOVE 2.5   //2mm move back after endstop hit, warning, must 


// KontrolBox a series resistor with switch to a analog PIN
// MCU only
#ifndef ISPC


/*
    MACROS for KBOXKontroller

*/

//#ifdef KBOX_PIN

#define KBOX_KEY_CHECK(k)   case KBOX_KEY##k##_R : lkey = k;kdl=500;break;
#define KBOX_KEY_ACT(k)   case k: KBOX_KEY##k##_ACTION;break;


#define KBOX_KEY1_R 0 ... 10
#define KBOX_KEY1_ACTION zprintf(PSTR("HOMING\n"));homing();
#define KBOX_KEY2_R 500 ... 530
#define KBOX_KEY2_ACTION zprintf(PSTR("HEATING\n"));set_temp(190);
#define KBOX_KEY3_R 670 ... 695
#define KBOX_KEY3_ACTION if (sdcardok) {sdcardok = sdcardok == 1 ? 2 : 1;zprintf(PSTR("SD\n"));} else demoSD();
#define KBOX_KEY4_R 760 ... 780
#define KBOX_KEY4_ACTION sdcardok=0;zprintf(PSTR("STOP\n"));

#define KBOX_DO_CHECK  KBOX_KEY_CHECK(1) KBOX_KEY_CHECK(2) KBOX_KEY_CHECK(3) KBOX_KEY_CHECK(4)
#define KBOX_DO_ACT  KBOX_KEY_ACT(1) KBOX_KEY_ACT(2) KBOX_KEY_ACT(3) KBOX_KEY_ACT(4)

//#endif
#endif


