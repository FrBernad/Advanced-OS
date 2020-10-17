// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <utils.h>
#include <buffer.h>
#include <stddef.h>

//sacada de nvconsole
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base) {
      char *p = buffer;
      char *p1, *p2;
      uint32_t digits = 0;

      //Calculate characters for each digit
      do {
            uint32_t remainder = value % base;
            *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
            digits++;
      } while (value /= base);

      // Terminate string in buffer.
      *p = 0;

      //Reverse string in buffer.
      p1 = buffer;
      p2 = p - 1;
      while (p1 < p2) {
            char tmp = *p1;
            *p1 = *p2;
            *p2 = tmp;
            p1++;
            p2--;
      }

      return digits;
}

//sacada de nvconsole
uint32_t uintToBaseWL(uint64_t value, char *buffer, uint32_t base, uint32_t lenght) {
      char *p = buffer;
      char *p1, *p2;
      uint32_t digits = 0;

      //Calculate characters for each digit
      do {
            uint32_t remainder = value % base;
            *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
            digits++;
      } while (value /= base);

      while (digits < lenght) {
            *p++ = '0';
            digits++;
      }

      // Terminate string in buffer.
      *p = 0;

      //Reverse string in buffer.
      p1 = buffer;
      p2 = p - 1;
      while (p1 < p2) {
            char tmp = *p1;
            *p1 = *p2;
            *p2 = tmp;
            p1++;
            p2--;
      }

      return digits;
}

uint64_t strToHex(char *str, int *error) {
      uint64_t num = 0, val;
      *error = 0;
      int len = strlen(str);
      len--;
      for (int i = 0; str[i] != 0; i++) {
            if (str[i] >= '0' && str[i] <= '9') {
                  val = str[i] - '0';
            } else if (str[i] >= 'a' && str[i] <= 'f') {
                  val = str[i] - 'a' + 10;
            } else if (str[i] >= 'A' && str[i] <= 'F') {
                  val = str[i] - 'A' + 10;
            } else {
                  *error = 1;
                  return 0;
            }

            num += val * pow(16, len);
            len--;
      }
      return num;
}

uint64_t pow(uint64_t x, uint64_t y) {
      if (y == 0)
            return 1;

      for (int i = 1; i < y; i++) {
            x *= x;
      }

      return x;
}

uint8_t BSDToInt(uint8_t num) {
      uint8_t low = num & 0x0F;
      uint8_t high = num & 0xF0;
      high >>= 4;
      return high * 10 + low;
}

char *strtok(char *string, char *result, const char delim) {
      static int currentIndex = 0;
      static char ogString[BUFFER_SIZE] = {0};

      if (result == 0) {
            currentIndex = 0;
            for (int i = 0; i < BUFFER_SIZE && ogString[i] != 0; i++) {
                  ogString[i] = 0;
            }
      }

      for (int i = 0; i < BUFFER_SIZE && result[i] != 0; i++) {
            result[i] = 0;
      }

      if (string != 0) {
            for (int i = 0; string[i] != 0 && i < BUFFER_SIZE; i++) {
                  ogString[i] = string[i];
            }
            for (; ogString[currentIndex] != 0 && ogString[currentIndex] == delim && currentIndex < BUFFER_SIZE; currentIndex++)
                  ;
            for (int i = 0; ogString[currentIndex] != 0 && ogString[currentIndex] != delim && currentIndex < BUFFER_SIZE; currentIndex++, i++) {
                  result[i] = string[currentIndex];
            }
      } else {
            if (currentIndex == BUFFER_SIZE || ogString[currentIndex] == 0) {
                  return 0;
            }
            for (; ogString[currentIndex] != 0 && ogString[currentIndex] == delim && currentIndex < BUFFER_SIZE; currentIndex++)
                  ;
            for (int i = 0; ogString[currentIndex] != 0 && ogString[currentIndex] != delim; currentIndex++, i++) {
                  result[i] = ogString[currentIndex];
            }
      }
      return result;
}

uint64_t strToInt(char *str, int *error) {
      uint64_t num = 0;
      *error = 0;
      for (int i = 0; str[i] != 0; i++) {
            if (IS_DIGIT(str[i])) {
                  num *= 10;
                  num += str[i] - '0';
            } else {
                  *error = 1;
                  return -1;
            }
      }
      return num;
}

uint8_t stringcmp(char *str1, char *str2) {
      if(str1==NULL || str2==NULL)
            return 1;

      int i = 0;
      while (str1[i] != 0 && str2[i] != 0 && str1[i] == str2[i]) {
            i++;
      }
      if (str1[i] == str2[i]) {
            return 0;
      }
      return str1[i] < str2[i] ? -1 : 1;
}

int log2(uint32_t n) {
      if (n == 0) {
            return -1;
      }
      int logValue = -1;
      while (n) {  //
            logValue++;
            n >>= 1;
      }
      return logValue;
}

void cleanBuffer(t_buffer *buffer) {
      for (int i = 0; buffer->buffer[i] != 0; i++) {
            buffer->buffer[i] = 0;
      }
      buffer->index = 0;
}

void cleanString(char *str) {
      for (int i = 0; str[i] != 0; i++) {
            str[i] = 0;
      }
}

int strlen(char *str) {
      int size = 0;
      for (int i = 0; str[i] != 0; i++) {
            size++;
      }
      return size;
}


//itoa
// inline function to swap two numbers
static void swap(char *x, char *y) {
      char t = *x;
      *x = *y;
      *y = t;
}

// function to reverse buffer[i..j]
static char *reverse(char *buffer, int i, int j) {
      while (i < j)
            swap(&buffer[i++], &buffer[j--]);

      return buffer;
}

// Iterative function to implement itoa() function in C
char *itoa(int value, char *buffer, int base) {
      // invalid input
      if (base < 2 || base > 32)
            return buffer;

      // consider absolute value of number
      int n = ABS(value);

      int i = 0;
      while (n) {
            int r = n % base;

            if (r >= 10)
                  buffer[i++] = 65 + (r - 10);
            else
                  buffer[i++] = 48 + r;

            n = n / base;
      }

      // if number is 0
      if (i == 0)
            buffer[i++] = '0';

      // If base is 10 and value is negative, the resulting string
      // is preceded with a minus sign (-)
      // With any other base, value is always considered unsigned
      if (value < 0 && base == 10)
            buffer[i++] = '-';

      buffer[i] = '\0';  // null terminate string

      // reverse the string and return it
      return reverse(buffer, 0, i - 1);
}
