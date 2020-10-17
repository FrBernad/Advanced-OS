// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <RTCTime.h>
#include <lib.h>
#include <utils.h>

uint8_t getDecimalTimeInfo(t_timeInfo info){
      if (info != HOURS && info != MINUTES && info != SECONDS)
            return 0;

      return BSDToInt(getBSDTimeInfo(info));
}
