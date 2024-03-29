/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Description: Tone function for Arduino Due and compatible (SAM3X8E)
 * Derived from http://forum.arduino.cc/index.php?topic=136500.msg2903012#msg2903012
 */

#ifdef __SAMD51__

#include "../../inc/MarlinConfig.h"
#include "HAL_timers_SAMD51.h"

static pin_t tone_pin;
volatile static int32_t toggles;

void tone(const pin_t _pin, const unsigned int frequency, const unsigned long duration) {
  tone_pin = _pin;
  toggles = 2 * frequency * duration / 1000;
  HAL_timer_start(TONE_TIMER_NUM, 2 * frequency);
}

void noTone(const pin_t _pin) {
  HAL_timer_disable_interrupt(TONE_TIMER_NUM);
  extDigitalWrite(_pin, LOW);
}

HAL_TONE_TIMER_ISR() {
  static bool pin_state = false;
  HAL_timer_isr_prologue(TONE_TIMER_NUM);

  if (toggles) {
    toggles--;
    extDigitalWrite(tone_pin, (pin_state = !pin_state));
  }
  else noTone(tone_pin);                         // turn off interrupt

  HAL_timer_isr_epilogue(TONE_TIMER_NUM);
}

#endif // __SAMD51__
