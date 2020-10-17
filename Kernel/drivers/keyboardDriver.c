// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboardDriver.h>

//dataTypes
#include <buffer.h>
#include <keys.h>

//functions
#include <interrupts.h>
#include <keyboardInfo.h>
#include <lib.h>
#include <stringLib.h>
#include <staticQueue.h>
#include <taskManager.h>
#include <semaphores.h>
#include <utils.h>
#include <videoDriver.h>

#define REGISTERS 16

static uint8_t action(uint8_t scanCode);
static void updateSnapshot(uint64_t* rsp);

static char pressCodes[KEYS][2] =
    {{0, 0}, {0, 0}, {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, {'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'}, {'-', '_'}, {'=', '+'}, {'\b', '\b'}, {'\t', '\t'}, {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'[', '{'}, {']', '}'}, {'\n', '\n'}, {0, 0}, {'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'}, {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'}, {'\'', '\"'}, {'`', '~'}, {0, 0}, {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, {'.', '>'}, {'/', '?'}, {0, 0}, {0, 0}, {0, 0}, {' ', ' '}, {0, 0}};

static uint8_t scanCode, currentAction, specialChars = 0, capsLock = 0, l_ctrl = 0;
int EOF = -1;
static char bufferSpace[MAX_SIZE] = {0};
static t_queue buffer = {bufferSpace, 0, -1, 0, MAX_SIZE, sizeof(int)};
static t_queue* currentBuffer = &buffer;
static t_specialKeyCode clearS = CLEAR_SCREEN;
static uint64_t registers[REGISTERS + 1] = {0};
static int keyboardSem;
static void enqueueKey(t_queue * queue, void * c);

int initKeyboardHandler(){
      if ((keyboardSem = sem_open("keyboard_lock", 0)) == -1) {
            return -1;
      }
      return 0;
}

void keyboardHandler(uint64_t * rsp) {
      if (hasKey()) {
            scanCode = getKey();
            currentAction = action(scanCode);
            if (currentAction == PRESSED) {
                  if (currentBuffer)
                        switch (scanCode) {
                              case L_SHFT_SC:
                              case R_SHFT_SC:
                                    specialChars = 1;
                                    break;

                              case CAPS_LCK_SC:
                                    capsLock = capsLock == 1 ? 0 : 1;
                                    break;

                              case L_CONTROL_SC:
                                    l_ctrl = 1;
                                    break;

                              default:
                                    if (pressCodes[scanCode][0] != 0) {
                                          if (l_ctrl) {
                                                if (pressCodes[scanCode][0] == 'l')
                                                      enqueueKey(currentBuffer, &clearS);
                                                else if (pressCodes[scanCode][0] == 's')
                                                      updateSnapshot(rsp);
                                                else if (pressCodes[scanCode][0] == 'c')
                                                      killForeground();
                                                else if (pressCodes[scanCode][0] == 'd')
                                                      enqueueKey(currentBuffer, &EOF);

                                          } else {
                                                if (!IS_LETTER(pressCodes[scanCode][0]))
                                                      enqueueKey(currentBuffer, &pressCodes[scanCode][specialChars]);
                                                else
                                                      enqueueKey(currentBuffer, &pressCodes[scanCode][ABS(capsLock - (specialChars))]);
                                               
                                          }
                                    }
                        }
            } else if (currentAction == RELEASED) {
                  switch (scanCode) {
                        case L_SHFT_SC | 0x80:  //for realease code
                        case R_SHFT_SC | 0x80:
                              specialChars = 0;
                              break;

                        case L_CONTROL_SC | 0x80:
                              l_ctrl = 0;
                              break;
                  }
            }
      }
}

static void enqueueKey(t_queue * queue, void * c){
      sem_post(keyboardSem);
      queueInsert(queue, c);
}

int getKeyboardChar(){
      int c = 0;
      sem_wait(keyboardSem);
      queueRemoveData(currentBuffer, &c);
      return c;
}

uint64_t* getSnapshot() {
      return registers;
}

static void updateSnapshot(uint64_t* rsp) {
      int i;
      for (i = 0; i < REGISTERS; i++) {
            registers[i] = rsp[i];
      }
      registers[i] = rsp[15 + 3];  //load rsp manualy
}

static uint8_t action(uint8_t scanCode) {
      if (scanCode >= 0x01 && scanCode <= 0x3A)
            return PRESSED;
      else if (scanCode >= 0x81 && scanCode <= 0xBA)
            return RELEASED;

      return ERROR;
}
