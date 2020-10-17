// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboardDriver.h>
#include <stdint.h>
#include <timerTick.h>
#include <stringLib.h>

#define TIMER_TICK 0
#define KEYBOARD 1

static void int_20();
static void int_21(uint64_t * rsp);

void irqDispatcher(uint64_t irq, uint64_t rsp) {
      switch (irq) {
            case TIMER_TICK:
                  int_20();
                  break;

            case KEYBOARD:
                  int_21((uint64_t *)rsp);
                  break;
      }
}

static void int_20() {
      timerHandler();
}

static void int_21(uint64_t * rsp) {
      keyboardHandler(rsp);
}
