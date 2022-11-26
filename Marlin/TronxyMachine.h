#pragma once
#define PROJ_CRUX1         39 //GS g:F446,U,chip4 180-180-180 no probe & auto level

#define UI_MARLIN_DEFAULT  0
#define UI_SIM240X320      3

#define TRONXY_PROJ        PROJ_CRUX1
#define TRONXY_UI          UI_SIM240X320

#if TRONXY_UI > 0
#define HAS_TRONXY_UI      1
#endif
#if TRONXY_PROJ
#define HAS_TRONXY_PROJ    1
#endif

#if TRONXY_PROJ == PROJ_CRUX1

#define TRONXY_MACHINE_NAME       "Crux1"
#define TRONXY_BACKUP_POWER       0 //UPS功能,注意需要某些特定脚作断电检测,配置相应的中断函数
#define TRONXY_SCREEN_SHUTDOWN    1 //熄屏功能
#define TRONXY_HOME_XY_ONLY       0 //手动调试界面复位是否只有XY复位,如果是,则Z复位按钮激活,否则失效
#define WIFI_INTERFACE            0 //WIFI功能
#define TAOLI_SERIAL              0
#define LASER_SERIAL              1 //wifi端子用于激光PWM控制
#define DUAL_ZMAX_HOME            0 //大端复位,向反方向复位,再回到正方向找0点
#define Z_BED_MOVE                0 //Z轴移动:热床
#define EXTRUDERS                 1 //挤出机数量
#define INVERT_E0_DIR             false //general:false
#define USE_EXTRUDER              4 //TITAN:1,BMG:2,TAOLI:3,general:4
// #define FIX_MOUNTED_PROBE

#else

#error "You haven't configure this machine."

#endif
