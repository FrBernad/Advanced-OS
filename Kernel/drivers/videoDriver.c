#include <font.h>
#include <lib.h>
#include <stringLib.h>
#include <videoDriver.h>

#define PIXEL_SIZE 3

struct vbe_mode_info_structure {
      uint16_t attributes;   // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
      uint8_t window_a;      // deprecated
      uint8_t window_b;      // deprecated
      uint16_t granularity;  // deprecated; used while calculating bank numbers
      uint16_t window_size;
      uint16_t segment_a;
      uint16_t segment_b;
      uint32_t win_func_ptr;  // deprecated; used to switch banks from protected mode without returning to real mode
      uint16_t pitch;         // number of bytes per horizontal line
      uint16_t width;         // width in pixels
      uint16_t height;        // height in pixels
      uint8_t w_char;         // unused...
      uint8_t y_char;         // ...
      uint8_t planes;
      uint8_t bpp;    // bits per pixel in this mode
      uint8_t banks;  // deprecated; total number of banks in this mode
      uint8_t memory_model;
      uint8_t bank_size;  // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
      uint8_t image_pages;
      uint8_t reserved0;

      uint8_t red_mask;
      uint8_t red_position;
      uint8_t green_mask;
      uint8_t green_position;
      uint8_t blue_mask;
      uint8_t blue_position;
      uint8_t reserved_mask;
      uint8_t reserved_position;
      uint8_t direct_color_attributes;

      uint32_t framebuffer;  // physical address of the linear frame buffer; write here to draw to the screen
      uint32_t off_screen_mem_off;
      uint16_t off_screen_mem_size;  // size of memory in the framebuffer but not being displayed on the screen
      uint8_t reserved1[206];
} __attribute__((packed));

static int getPixelDataByPosition(uint32_t x, uint32_t y);

static struct vbe_mode_info_structure *screen_info = (void *)0x5C00;
static uint32_t SCREEN_WIDTH = 1024;  //VESA default values
static uint32_t SCREEN_HEIGHT = 768;

static t_screen screen;

void initVideoDriver(t_colour BGColour, t_colour FontColour) {
      screen.defaultBGColour = BGColour;
      screen.defaultFontColour = FontColour;
      screen.currentX = 0;
      screen.currentY = 0;
      screen.margin = 2 * CHAR_WIDTH;
      screen.height = SCREEN_HEIGHT - 2 * CHAR_WIDTH - screen.margin;
      screen.width = SCREEN_WIDTH - 2 * CHAR_WIDTH - screen.margin;
}

void writePixel(uint32_t x, uint32_t y, t_colour colour)  //BGR
{
      char *currentFrame = (char *)((uint64_t)screen_info->framebuffer);  //casteo a uint64 para evitar warning
      int offset = getPixelDataByPosition(x, y);
      currentFrame[offset] = (char)((colour >> 16) & 0xFF);     //BLUE
      currentFrame[offset + 1] = (char)((colour >> 8) & 0xFF);  //GREEN
      currentFrame[offset + 2] = (char)(colour & 0xFF);         //RED
}

void printCharOnScreen(char c, t_colour bgColour, t_colour fontColour, int advance) {
      if (screen.currentX != 0 && screen.width - screen.currentX < CHAR_WIDTH) {
            screen.currentY += CHAR_HEIGHT;
            screen.currentX = 0;
            if (screen.height - screen.currentY < CHAR_HEIGHT) {
                  screen.currentY -= CHAR_HEIGHT;
                  scrollDownScreen();
            }
      }      

      char *charMap = getCharMap(c);

      uint32_t x = screen.currentX + screen.margin, y = screen.currentY + screen.margin;

      for (int i = 0; i < CHAR_HEIGHT; i++) {
            for (int j = 0; j < CHAR_WIDTH; j++) {
                  uint8_t isFont = (charMap[i] >> (CHAR_WIDTH - j - 1)) & 0x01;  //-1 para no romper el decalaje, primera vez tengo q decalar 7
                  if (isFont) {
                        writePixel(x, y, fontColour);
                  } else {
                        writePixel(x, y, bgColour);
                  }
                  x++;
            }
            x = screen.currentX + screen.margin;
            y++;
      }

      if (advance) {
            screen.currentX += CHAR_WIDTH;
      }
}

void scrollDownScreen() {
      memcpy((void *)((uint64_t)screen_info->framebuffer + screen.margin * SCREEN_WIDTH * PIXEL_SIZE),
             (void *)((uint64_t)screen_info->framebuffer + (screen.margin + CHAR_HEIGHT) * SCREEN_WIDTH * PIXEL_SIZE),
             SCREEN_WIDTH * (SCREEN_HEIGHT - CHAR_HEIGHT - 2*screen.margin) * PIXEL_SIZE);
      
      clearLineOnScreen();
}

void clearLineOnScreen() {
      for (int y = 0; y < CHAR_HEIGHT; y++) {
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                  writePixel(x, screen.currentY + screen.margin + y, screen.defaultBGColour);
            }
      }
}

void removeCharFromScreen() {
      if (screen.currentX == 0) {
            if (screen.currentY == 0) {
                  return;
            }
            screen.currentY -= CHAR_HEIGHT;
            screen.currentX = screen.width;
      }

      screen.currentX -= CHAR_WIDTH;

      printCharOnScreen(' ', BLACK, WHITE, 0);  //remove char
}

void changeLineOnScreen() {
      screen.currentY += CHAR_HEIGHT;
      screen.currentX = 0;
      if (screen.height - screen.currentY < CHAR_HEIGHT) {
            screen.currentY -= CHAR_HEIGHT;
            scrollDownScreen();
      }
}

void clearScreen() {
      for (int y = 0; y < screen.height; y++) {
            for (int x = 0; x < screen.width; x++) {
                  writePixel(x + screen.margin, y + screen.margin, screen.defaultBGColour);
            }
      }
      screen.currentX = 0;
      screen.currentY = 0;
}

static int getPixelDataByPosition(uint32_t x, uint32_t y) {
      return PIXEL_SIZE * (x + y * SCREEN_WIDTH);
}
