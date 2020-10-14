#include <timerTick.h>
#include <stringLib.h>

static unsigned long ticks = 0;

void timerHandler() {
      ticks++;
}

int ticksElapsed() {
      return ticks;
}

void sleep(int segs){
      int timeout=secondsElapsed()+segs;
      while(secondsElapsed()<=timeout);
}

int secondsElapsed() {
	return ticks / 18;
}
