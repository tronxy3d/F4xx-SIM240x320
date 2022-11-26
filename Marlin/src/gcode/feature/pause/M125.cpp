/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(PARK_HEAD_ON_PAUSE)

#include "../../gcode.h"
#include "../../parser.h"
#include "../../../feature/pause.h"
#include "../../../lcd/marlinui.h"
#include "../../../module/motion.h"
#include "../../../module/printcounter.h"
#include "../../../sd/cardreader.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

/**
 * M125: Store current position and move to parking position.
 *       Called on pause (by M25) to prevent material leaking onto the
 *       object. On resume (M24) the head will be moved back and the
 *       print will resume.
 *
 *       When not actively SD printing, M125 simply moves to the park
 *       position and waits, resuming with a button click or M108.
 *       Without PARK_HEAD_ON_PAUSE the M125 command does nothing.
 *
 *    L<linear> = Override retract Length
 *    X<pos>    = Override park position X
 *    Y<pos>    = Override park position Y
 *    A<pos>    = Override park position A (requires AXIS*_NAME 'A')
 *    B<pos>    = Override park position B (requires AXIS*_NAME 'B')
 *    C<pos>    = Override park position C (requires AXIS*_NAME 'C')
 *    U<pos>    = Override park position U (requires AXIS*_NAME 'U')
 *    V<pos>    = Override park position V (requires AXIS*_NAME 'V')
 *    W<pos>    = Override park position W (requires AXIS*_NAME 'W')
 *    Z<linear> = Override Z raise
 *
 *  With an LCD menu:
 *    P<bool>   = Always show a prompt and await a response
 */
void GcodeSuite::M125() {
  
  #if TRONXY_UI
  if(!parser.seen('T')) {
    tronxyStop();
    globalStop = false;
  }
  #endif

  // Initial retract before move to filament change position
  const float retract = TERN0(HAS_EXTRUDERS, -ABS(parser.axisunitsval('L', E_AXIS, PAUSE_PARK_RETRACT_LENGTH)));

  xyz_pos_t park_point = NOZZLE_PARK_POINT;

  // Move to filament change position or given position
  NUM_AXIS_CODE(
    if (parser.seenval('X')) park_point.x = RAW_X_POSITION(parser.linearval('X')),
    if (parser.seenval('Y')) park_point.y = RAW_Y_POSITION(parser.linearval('Y')),
    NOOP,
    if (parser.seenval(AXIS4_NAME)) park_point.i = RAW_I_POSITION(parser.linearval(AXIS4_NAME)),
    if (parser.seenval(AXIS5_NAME)) park_point.j = RAW_J_POSITION(parser.linearval(AXIS5_NAME)),
    if (parser.seenval(AXIS6_NAME)) park_point.k = RAW_K_POSITION(parser.linearval(AXIS6_NAME)),
    if (parser.seenval(AXIS7_NAME)) park_point.u = RAW_U_POSITION(parser.linearval(AXIS7_NAME)),
    if (parser.seenval(AXIS8_NAME)) park_point.v = RAW_V_POSITION(parser.linearval(AXIS8_NAME)),
    if (parser.seenval(AXIS9_NAME)) park_point.w = RAW_W_POSITION(parser.linearval(AXIS9_NAME))
  );

  // Lift Z axis
  #if HAS_Z_AXIS
    if (parser.seenval('Z')) park_point.z = parser.linearval('Z');
  #endif
  #if ENABLED(DUAL_X_CARRIAGE)
    if(active_extruder)park_point.x = X2_MAX_POS - 10;
  #endif

  #if HAS_HOTEND_OFFSET && NONE(DUAL_X_CARRIAGE, DELTA)
    park_point += hotend_offset[active_extruder];
  #endif

  const bool sd_printing = TERN0(SDSUPPORT, IS_SD_PRINTING());

  #if !TRONXY_UI
  ui.pause_show_message(PAUSE_MESSAGE_PARKING, PAUSE_MODE_PAUSE_PRINT);
  #endif

  // If possible, show an LCD prompt with the 'P' flag
  #if TRONXY_UI
  const bool show_lcd = parser.seenval('P');
  yevt.addEvt(TASK_SEQUENCE,screenPopupWaiting);
  #else
  const bool show_lcd = TERN0(HAS_MARLINUI_MENU, parser.boolval('P'));
  #endif

  if (pause_print(retract, park_point, show_lcd, 0)) {
    #if TRONXY_UI
    if(messageSt.currentIndex == POPUP_WAITING) {
      closePopupMessage();
      tronxyUpdate();
    }
    #endif
    if (ENABLED(EXTENSIBLE_UI) || BOTH(EMERGENCY_PARSER, HOST_PROMPT_SUPPORT) || !sd_printing || show_lcd) {
      wait_for_confirmation(false, 0);
      resume_print(0, 0, -retract, 0);
    }
  }
  #if TRONXY_UI
  else {
    if(messageSt.currentIndex == POPUP_WAITING) {
      closePopupMessage();
      tronxyUpdate();
    }
  }
  #endif
}

#endif // PARK_HEAD_ON_PAUSE
