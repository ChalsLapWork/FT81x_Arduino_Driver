/**
 * FT81x on ST7701S Arduino Driver
 * Copyright (C) 2020  Raphael Stäbler
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
 **/

#include "FT81x.h"

#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 480

#define READ  0x000000
#define WRITE 0x800000

#define DLSTART()                    0xFFFFFF00
#define SWAP()                       0xFFFFFF01
#define MEMWRITE()                   0xFFFFFF1A
#define CLEAR(c, s, t)               ((0x26L << 24) | ((c) << 2) | ((s) << 1) | (t))
#define BEGIN(p)                     ((0x1FL << 24) | (p))
#define END()                        (0x21L << 24)
#define END_DL()                     0x00
#define CLEAR_COLOR_RGB(r, g, b)     ((0x02L << 24) | ((r) << 16) | ((g) << 8) | (b))
#define CLEAR_COLOR(rgb)             ((0x02L << 24) | ((rgb)&0xFFFFFF))
#define COLOR_RGB(r, g, b)           ((0x04L << 24) | ((uint32_t)(r) << 16) | ((g) << 8) | (b))
#define COLOR(rgb)                   ((0x04L << 24) | ((rgb)&0xFFFFFF))
#define POINT_SIZE(s)                ((0x0DL << 24) | ((s)&0xFFF))
#define LINE_WIDTH(w)                ((0x0EL << 24) | ((w)&0xFFF))
#define VERTEX2II(x, y, h, c)        ((1L << 31) | (((uint32_t)(x)&0xFFF) << 21) | (((y)&0xFFF) << 12) | ((h) << 7) | (c))
#define VERTEX2F(x, y)               ((1L << 30) | (((uint32_t)(x)&0xFFFF) << 15) | ((y)&0xFFFF))
#define BITMAP_SOURCE(a)             ((1L << 24) | (a))
#define BITMAP_LAYOUT(f, s, h)       ((7L << 24) | ((uint32_t)(f) << 19) | (((s)&0x1FF) << 9) | ((h)&0x1FF))
#define BITMAP_SIZE(f, wx, wy, w, h) ((8L << 24) | ((uint32_t)((f)&1) << 20) | ((uint32_t)((wx)&1) << 19) | ((uint32_t)((wy)&1) << 18) | (((w)&0x1FF) << 9) | ((h)&0x1FF))
#define LOADIDENTITY()               0xFFFFFF26
#define SETMATRIX()                  0xFFFFFF2A
#define SCALE()                      0xFFFFFF28
#define TEXT()                       0xFFFFFF0C
#define SPINNER()                    0xFFFFFF16

#define BITMAPS      1
#define POINTS       2
#define LINES        3
#define LINE_STRIP   4
#define EDGE_STRIP_R 5
#define EDGE_STRIP_L 6
#define EDGE_STRIP_A 7
#define EDGE_STRIP_B 8
#define RECTS        9

#define DISPLAY_CMD(cmd, params...)                                \
    {                                                              \
        const uint8_t d[] = {params};                              \
        sendCommandToDisplay(cmd, sizeof(d) / sizeof(uint8_t), d); \
    }

FT81x::FT81x() {}

void FT81x::begin() {
#ifdef FT81x_USE_DMA
    DmaSpi::Transfer trx(nullptr, 0, nullptr);
    DmaSpi::Transfer trx2(nullptr, 0, nullptr);
#endif

    pinMode(FT81x_CS1, OUTPUT);
    digitalWrite(FT81x_CS1, HIGH);

    pinMode(FT81x_CS2, OUTPUT);
    digitalWrite(FT81x_CS2, HIGH);

    pinMode(FT81x_DC, OUTPUT);
    digitalWrite(FT81x_DC, LOW);

#ifdef FT81x_USE_DMA
    SPI.setCS(FT81x_CS1);

    SPI.beginTransaction(FT81x_SPI_SETTINGS);
    SPI.endTransaction();

    DMASPI0.begin();
    DMASPI0.start();
#endif

    FT81x::initFT81x();
    FT81x::initDisplay();
}

void FT81x::initFT81x() {
    // reset
    FT81x::sendCommand(FT81x_CMD_RST_PULSE);
    delay(300);

    // select clock
    FT81x::sendCommand(FT81x_CMD_CLKEXT);
    delay(300);

    // activate
    FT81x::sendCommand(FT81x_CMD_ACTIVE);

    // wait for boot-up to complete
    delay(100);
    while (FT81x::read8(FT81x_REG_ID) != 0x7C) {
        __asm__ volatile("nop");
    }
    while (FT81x::read8(FT81x_REG_CPURESET) != 0x00) {
        __asm__ volatile("nop");
    }

    // configure rgb interface
    FT81x::write16(FT81x_REG_HCYCLE, DISPLAY_WIDTH + 8 + 8 + 50 + 2);
    FT81x::write16(FT81x_REG_HOFFSET, 8 + 8 + 50);
    FT81x::write16(FT81x_REG_HSYNC0, 8);
    FT81x::write16(FT81x_REG_HSYNC1, 8 + 8);
    FT81x::write16(FT81x_REG_HSIZE, DISPLAY_WIDTH);

    FT81x::write16(FT81x_REG_VCYCLE, DISPLAY_HEIGHT + 8 + 8 + 20 + 2);
    FT81x::write16(FT81x_REG_VOFFSET, 8 + 8 + 20);
    FT81x::write16(FT81x_REG_VSYNC0, 8);
    FT81x::write16(FT81x_REG_VSYNC1, 8 + 8);
    FT81x::write16(FT81x_REG_VSIZE, DISPLAY_HEIGHT);

    FT81x::write8(FT81x_REG_SWIZZLE, 2);
    FT81x::write8(FT81x_REG_PCLK_POL, 0);
    FT81x::write8(FT81x_REG_CSPREAD, 1);
    FT81x::write8(FT81x_REG_DITHER, 0);
    FT81x::write8(FT81x_REG_ROTATE, 0);

    // write first display list
    FT81x::beginDisplayList();
    FT81x::clear(0);
    FT81x::swapScreen();

    // enable pixel clock
    FT81x::write8(FT81x_REG_PCLK, 10);

    // reset display (somehow this generates a low pulse)
    FT81x::write16(FT81x_REG_GPIOX, 0x8000 | FT81x::read16(FT81x_REG_GPIOX));
    delay(300);
}

void FT81x::initDisplay() {
    // sleep mode off
    DISPLAY_CMD(ST7701_SLPOUT);
    delay(300);

    // Command2, BK0
    DISPLAY_CMD(ST7701_CND2BKxSEL, 0x77, 0x01, 0x00, 0x00, ST7701_CMD2BK0SEL);
    DISPLAY_CMD(ST7701_BK0_LNESET, 0x3B, 0x00);
    DISPLAY_CMD(ST7701_BK0_PORCTRL, 0x14, 0x0A);  // vbp, vfp
    DISPLAY_CMD(ST7701_BK0_INVSEL, 0x21, 0x08);
    DISPLAY_CMD(ST7701_BK0_PVGAMCTRL, 0x00, 0x11, 0x18, 0x0E, 0x11, 0x06, 0x07, 0x08, 0x07, 0x22, 0x04, 0x12, 0x0F, 0xAA, 0x31, 0x18);
    DISPLAY_CMD(ST7701_BK0_NVGAMCTRL, 0x00, 0x11, 0x19, 0x0E, 0x12, 0x07, 0x08, 0x08, 0x08, 0x22, 0x04, 0x11, 0x11, 0xA9, 0x32, 0x18);

    // Command2, BK1
    DISPLAY_CMD(ST7701_CND2BKxSEL, 0x77, 0x01, 0x00, 0x00, ST7701_CMD2BK1SEL);
    DISPLAY_CMD(ST7701_BK1_VRHS, 0x60);
    DISPLAY_CMD(ST7701_BK1_VCOM, 0x30);
    DISPLAY_CMD(ST7701_BK1_VGHSS, 0x87);
    DISPLAY_CMD(ST7701_BK1_TESTCMD, 0x80);
    DISPLAY_CMD(ST7701_BK1_VGLS, 0x49);
    DISPLAY_CMD(ST7701_BK1_PWCTLR1, 0x85);
    DISPLAY_CMD(ST7701_BK1_PWCTLR2, 0x21);
    DISPLAY_CMD(ST7701_BK1_SPD1, 0x78);
    DISPLAY_CMD(ST7701_BK1_SPD2, 0x78);

    DISPLAY_CMD(0xE0, 0x00, 0x1B, 0x02);
    DISPLAY_CMD(0xE1, 0x08, 0xA0, 0x00, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x44, 0x44);
    DISPLAY_CMD(0xE2, 0x11, 0x11, 0x44, 0x44, 0xED, 0xA0, 0x00, 0x00, 0xEC, 0xA0, 0x00, 0x00);
    DISPLAY_CMD(0xE3, 0x00, 0x00, 0x11, 0x11);
    DISPLAY_CMD(0xE4, 0x44, 0x44);
    DISPLAY_CMD(0xE5, 0x0A, 0xE9, 0xD8, 0xA0, 0x0C, 0xEB, 0xD8, 0xA0, 0x0E, 0xED, 0xD8, 0xA0, 0x10, 0xEF, 0xD8, 0xA0);
    DISPLAY_CMD(0xE6, 0x00, 0x00, 0x11, 0x11);
    DISPLAY_CMD(0xE7, 0x44, 0x44);
    DISPLAY_CMD(0xE8, 0x09, 0xE8, 0xD8, 0xA0, 0x0B, 0xEA, 0xD8, 0xA0, 0x0D, 0xEC, 0xD8, 0xA0, 0x0F, 0xEE, 0xD8, 0xA0);
    DISPLAY_CMD(0xEB, 0x02, 0x00, 0xE4, 0xE4, 0x88, 0x00, 0x40);
    DISPLAY_CMD(0xEC, 0x3C, 0x00);
    DISPLAY_CMD(0xED, 0xAB, 0x89, 0x76, 0x54, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x45, 0x67, 0x98, 0xBA);

    DISPLAY_CMD(ST7701_CND2BKxSEL, 0x77, 0x01, 0x00, 0x00, ST7701_CMD2BKxSEL_NONE);

    // display on
    DISPLAY_CMD(ST7701_DISPON);

    // set pixel format
    DISPLAY_CMD(ST7701_COLMOD, 0x70);

    // DISPLAY_CMD(0x23); // all pixels on
}

void FT81x::clear(const uint32_t color) {
    startCmd(CLEAR_COLOR(color));
    endCmd(CLEAR(1, 1, 1));
}

void FT81x::drawCircle(const int16_t x, const int16_t y, const uint8_t size, const uint32_t color) {
    startCmd(COLOR(color));
    intermediateCmd(POINT_SIZE(size * 16));
    intermediateCmd(BEGIN(POINTS));
    intermediateCmd(VERTEX2F(x * 16, y * 16));
    endCmd(END());
}

void FT81x::drawRect(const int16_t x, const int16_t y, const uint16_t width, const uint16_t height, const uint8_t cornerRadius, const uint32_t color) {
    startCmd(COLOR(color));
    intermediateCmd(LINE_WIDTH(cornerRadius * 16));
    intermediateCmd(BEGIN(RECTS));
    intermediateCmd(VERTEX2F(x * 16, y * 16));
    intermediateCmd(VERTEX2F((x + width) * 16, (y + height) * 16));
    endCmd(END());
}

void FT81x::drawLetter(const int16_t x, const int16_t y, const uint8_t font, const uint32_t color, const uint8_t letter) {
    startCmd(COLOR(color));
    intermediateCmd(BEGIN(BITMAPS));
    intermediateCmd(VERTEX2II(x, y, font, letter));
    endCmd(END());
}

void FT81x::drawBitmap(const uint32_t offset, const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const uint8_t scale) {
    startCmd(COLOR_RGB(255, 255, 255));
    intermediateCmd(BITMAP_SOURCE(FT81x_RAM_G + offset));
    intermediateCmd(BITMAP_LAYOUT(FT81x_BITMAP_LAYOUT_RGB565, width * 2, height));  // only supporting one format for now
    intermediateCmd(BITMAP_SIZE(FT81x_BITMAP_SIZE_NEAREST, 0, 0, width * scale, height * scale));
    intermediateCmd(BEGIN(BITMAPS));
    intermediateCmd(LOADIDENTITY());
    intermediateCmd(SCALE());
    intermediateCmd(scale * 65536);
    intermediateCmd(scale * 65536);
    intermediateCmd(SETMATRIX());
    endCmd(VERTEX2II(x, y, 0, 0));
}

void FT81x::drawText(const int16_t x, const int16_t y, const uint8_t font, const uint32_t color, const uint16_t options, const char text[]) {
    startCmd(COLOR(color));
    intermediateCmd(TEXT());
    intermediateCmd(x | ((uint32_t)y << 16));
    intermediateCmd(font | ((uint32_t)options << 16));
    uint32_t data = 0xFFFFFFFF;
    for (uint8_t i = 0; (data >> 24) != 0; i += 4) {
        data = 0;

        if (text[i] != 0) {
            data |= text[i];

            if (text[i + 1] != 0) {
                data |= text[i + 1] << 8;

                if (text[i + 2] != 0) {
                    data |= (uint32_t)text[i + 2] << 16;

                    if (text[i + 3] != 0) {
                        data |= (uint32_t)text[i + 3] << 24;
                    }
                }
            }
        }

        if ((data >> 24) != 0) {
            intermediateCmd(data);
        } else {
            endCmd(data);
        }
    }

    if ((data >> 24) != 0) {
        endCmd(0);
    }
}

void FT81x::drawSpinner(const int16_t x, const int16_t y, const uint16_t style, const uint16_t scale, const uint32_t color) {
    startCmd(COLOR(color));
    intermediateCmd(SPINNER());
    intermediateCmd(x | ((uint32_t)y << 16));
    endCmd(style | ((uint32_t)scale << 16));
}

void FT81x::cmd(const uint32_t cmd) {
    uint16_t cmdWrite = FT81x::read16(FT81x_REG_CMD_WRITE);
    uint32_t addr = FT81x_RAM_CMD + cmdWrite;
    write32(addr, cmd);
    write16(FT81x_REG_CMD_WRITE, (cmdWrite + 4) % 4096);
}

void FT81x::beginDisplayList() {
    // Wait for circular buffer to catch up
    while (FT81x::read16(FT81x_REG_CMD_WRITE) != FT81x::read16(FT81x_REG_CMD_READ)) {
        __asm__ volatile("nop");
    }
    startCmd(DLSTART());
    endCmd(CLEAR(1, 1, 1));
}

void FT81x::swapScreen() {
    startCmd(END_DL());
    endCmd(SWAP());
}

void FT81x::setRotation(uint8_t rotation) { write8(FT81x_REG_ROTATE, rotation & 0x7); }

inline void FT81x::increaseCmdWriteAddress(uint16_t delta) { cmdWriteAddress = (cmdWriteAddress + delta) % 4096; }

inline void FT81x::updateCmdWriteAddress() { write16(FT81x_REG_CMD_WRITE, cmdWriteAddress); }

#ifdef FT81x_USE_DMA

void FT81x::writeGRAM(const uint32_t offset, const uint32_t size, const uint8_t *data) {
    static ActiveLowChipSelectStart csStart(FT81x_CS1, FT81x_SPI_SETTINGS);
    static ActiveLowChipSelectEnd csEnd(FT81x_CS1, FT81x_SPI_SETTINGS);
    static DummyChipSelect noCs;

    uint32_t cmd = (FT81x_RAM_G + offset) | WRITE;

    waitForDMAReady();
    dmaBuffer[0] = cmd >> 16;
    dmaBuffer[1] = cmd >> 8;
    dmaBuffer[2] = cmd;

    trx = DmaSpi::Transfer(dmaBuffer, 3, nullptr, 0, &csStart);
    DMASPI0.registerTransfer(trx);

    waitForDMAReady();

    if (size < 0x8000) {  // 0x7FFF is the limit for the Teensy platform, this check has to be revisited
        trx = DmaSpi::Transfer(data, size, nullptr, 0, &csEnd);
        DMASPI0.registerTransfer(trx);
    } else if (size < 0x10000) {
        uint32_t smallerPart = size - 0x7FFF;

        trx = DmaSpi::Transfer(data, smallerPart, nullptr, 0, &noCs);
        DMASPI0.registerTransfer(trx);

        while (trx2.busy()) {
            __asm__ volatile("nop");
        }

        trx2 = DmaSpi::Transfer(data + smallerPart, 0x7FFF, nullptr, 0, &csEnd);
        DMASPI0.registerTransfer(trx2);
    }
}

void FT81x::startCmd(const uint32_t cmd) {
    static ActiveLowChipSelectStart csStart(FT81x_CS1, FT81x_SPI_SETTINGS);

    uint32_t addr = (FT81x_RAM_CMD + cmdWriteAddress) | WRITE;

    waitForDMAReady();
    dmaBuffer[0] = addr >> 16;
    dmaBuffer[1] = addr >> 8;
    dmaBuffer[2] = addr;
    dmaBuffer[3] = cmd;
    dmaBuffer[4] = cmd >> 8;
    dmaBuffer[5] = cmd >> 16;
    dmaBuffer[6] = cmd >> 24;

    trx = DmaSpi::Transfer(dmaBuffer, 7, nullptr, 0, &csStart);
    DMASPI0.registerTransfer(trx);

    increaseCmdWriteAddress(4);
}

void FT81x::intermediateCmd(const uint32_t cmd) {
    static DummyChipSelect noCs;

    waitForDMAReady();
    dmaBuffer[0] = cmd;
    dmaBuffer[1] = cmd >> 8;
    dmaBuffer[2] = cmd >> 16;
    dmaBuffer[3] = cmd >> 24;

    trx = DmaSpi::Transfer(dmaBuffer, 4, nullptr, 0, &noCs);
    DMASPI0.registerTransfer(trx);

    increaseCmdWriteAddress(4);
}

void FT81x::endCmd(const uint32_t cmd) {
    static ActiveLowChipSelectEnd csEnd(FT81x_CS1, FT81x_SPI_SETTINGS);

    waitForDMAReady();
    dmaBuffer[0] = cmd;
    dmaBuffer[1] = cmd >> 8;
    dmaBuffer[2] = cmd >> 16;
    dmaBuffer[3] = cmd >> 24;

    trx = DmaSpi::Transfer(dmaBuffer, 4, nullptr, 0, &csEnd);
    DMASPI0.registerTransfer(trx);

    increaseCmdWriteAddress(4);

    updateCmdWriteAddress();
}

void FT81x::sendCommand(const uint32_t cmd) {
    waitForDMAReady();
    dmaBuffer[0] = cmd >> 16;
    dmaBuffer[1] = cmd >> 8;
    dmaBuffer[2] = cmd;
    transferDMABuffer(3);
}

uint8_t FT81x::read8(const uint32_t address) {
    uint32_t cmd = address | READ;
    waitForDMAReady();
    dmaBuffer[0] = cmd >> 16;
    dmaBuffer[1] = cmd >> 8;
    dmaBuffer[2] = cmd;
    dmaBuffer[3] = 0x00;  // dummy byte
    dmaBuffer[4] = 0x00;  // read byte
    transferDMABuffer(5);
    return dmaBufferOut[4];
}

uint16_t FT81x::read16(const uint32_t address) {
    uint32_t cmd = address | READ;
    waitForDMAReady();
    dmaBuffer[0] = cmd >> 16;
    dmaBuffer[1] = cmd >> 8;
    dmaBuffer[2] = cmd;
    dmaBuffer[3] = 0x00;  // dummy byte
    dmaBuffer[4] = 0x00;  // read byte
    dmaBuffer[5] = 0x00;  // read byte
    transferDMABuffer(6);
    uint16_t result = dmaBufferOut[4] | (dmaBufferOut[5] << 8);
    return result;
}

uint32_t FT81x::read32(const uint32_t address) {
    uint32_t cmd = address | READ;
    waitForDMAReady();
    dmaBuffer[0] = cmd >> 16;
    dmaBuffer[1] = cmd >> 8;
    dmaBuffer[2] = cmd;
    dmaBuffer[3] = 0x00;  // dummy byte
    dmaBuffer[4] = 0x00;  // read byte
    dmaBuffer[5] = 0x00;  // read byte
    dmaBuffer[6] = 0x00;  // read byte
    dmaBuffer[7] = 0x00;  // read byte
    transferDMABuffer(8);
    uint32_t result = dmaBufferOut[4] | (dmaBufferOut[5] << 8) | (dmaBufferOut[6] << 16) | (dmaBufferOut[7] << 24);
    return result;
}

void FT81x::write8(const uint32_t address, const uint8_t data) {
    uint32_t cmd = address | WRITE;
    waitForDMAReady();
    dmaBuffer[0] = cmd >> 16;
    dmaBuffer[1] = cmd >> 8;
    dmaBuffer[2] = cmd;
    dmaBuffer[3] = data;
    transferDMABuffer(4);
}

void FT81x::write16(const uint32_t address, const uint16_t data) {
    uint32_t cmd = address | WRITE;
    waitForDMAReady();
    dmaBuffer[0] = cmd >> 16;
    dmaBuffer[1] = cmd >> 8;
    dmaBuffer[2] = cmd;
    dmaBuffer[3] = data;
    dmaBuffer[4] = data >> 8;
    transferDMABuffer(5);
}

void FT81x::write32(const uint32_t address, const uint32_t data) {
    uint32_t cmd = address | WRITE;
    waitForDMAReady();
    dmaBuffer[0] = cmd >> 16;
    dmaBuffer[1] = cmd >> 8;
    dmaBuffer[2] = cmd;
    dmaBuffer[3] = data;
    dmaBuffer[4] = data >> 8;
    dmaBuffer[5] = data >> 16;
    dmaBuffer[6] = data >> 24;
    transferDMABuffer(7);
}

inline void FT81x::waitForDMAReady() {
    while (trx.busy()) {
        __asm__ volatile("nop");
    }
}

void FT81x::transferDMABuffer(const uint8_t size) {
    static ActiveLowChipSelect cs(FT81x_CS1, FT81x_SPI_SETTINGS);
    trx = DmaSpi::Transfer((uint8_t *)dmaBuffer, size, dmaBufferOut, 0, &cs);
    DMASPI0.registerTransfer(trx);
}

#else

void FT81x::writeGRAM(const uint32_t offset, const uint32_t size, const uint8_t *data) {
    uint32_t cmd = (FT81x_RAM_G + offset) | WRITE;

    SPI.beginTransaction(FT81x_SPI_SETTINGS);
    digitalWrite(FT81x_CS1, LOW);

    SPI.transfer((cmd >> 16) & 0xFF);
    SPI.transfer((cmd >> 8) & 0xFF);
    SPI.transfer(cmd & 0xFF);

    for (uint32_t i = 0; i < size; i++) {
        SPI.transfer(data[i]);
    }

    digitalWrite(FT81x_CS1, HIGH);
    SPI.endTransaction();
}

void FT81x::startCmd(const uint32_t cmd) {
    uint32_t addr = (FT81x_RAM_CMD + cmdWriteAddress) | WRITE;

    SPI.beginTransaction(FT81x_SPI_SETTINGS);
    digitalWrite(FT81x_CS1, LOW);

    SPI.transfer(addr >> 16);
    SPI.transfer(addr >> 8);
    SPI.transfer(addr);
    SPI.transfer(cmd);
    SPI.transfer(cmd >> 8);
    SPI.transfer(cmd >> 16);
    SPI.transfer(cmd >> 24);

    increaseCmdWriteAddress(4);
}

void FT81x::intermediateCmd(const uint32_t cmd) {
    SPI.transfer(cmd);
    SPI.transfer(cmd >> 8);
    SPI.transfer(cmd >> 16);
    SPI.transfer(cmd >> 24);

    increaseCmdWriteAddress(4);
}

void FT81x::endCmd(const uint32_t cmd) {
    SPI.transfer(cmd);
    SPI.transfer(cmd >> 8);
    SPI.transfer(cmd >> 16);
    SPI.transfer(cmd >> 24);

    digitalWrite(FT81x_CS1, HIGH);
    SPI.endTransaction();

    increaseCmdWriteAddress(4);

    updateCmdWriteAddress();
}

void FT81x::sendCommand(const uint32_t cmd) {
    SPI.beginTransaction(FT81x_SPI_SETTINGS);
    digitalWrite(FT81x_CS1, LOW);
    SPI.transfer(cmd >> 16);
    SPI.transfer(cmd >> 8);
    SPI.transfer(cmd);
    digitalWrite(FT81x_CS1, HIGH);
    SPI.endTransaction();
}

uint8_t FT81x::read8(const uint32_t address) {
    uint32_t cmd = address;
    SPI.beginTransaction(FT81x_SPI_SETTINGS);
    digitalWrite(FT81x_CS1, LOW);
    SPI.transfer(cmd >> 16);
    SPI.transfer(cmd >> 8);
    SPI.transfer(cmd);
    SPI.transfer(0x00);  // dummy byte
    uint8_t result = SPI.transfer(0x00);
    digitalWrite(FT81x_CS1, HIGH);
    SPI.endTransaction();
    return result;
}

uint16_t FT81x::read16(const uint32_t address) {
    uint32_t cmd = address | READ;
    SPI.beginTransaction(FT81x_SPI_SETTINGS);
    digitalWrite(FT81x_CS1, LOW);
    SPI.transfer(cmd >> 16);
    SPI.transfer(cmd >> 8);
    SPI.transfer(cmd);
    SPI.transfer(0x00);  // dummy byte
    uint16_t result = SPI.transfer(0x00);
    result |= (SPI.transfer(0x00) << 8);
    digitalWrite(FT81x_CS1, HIGH);
    SPI.endTransaction();
    return result;
}

uint32_t FT81x::read32(const uint32_t address) {
    uint32_t cmd = address | READ;
    SPI.beginTransaction(FT81x_SPI_SETTINGS);
    digitalWrite(FT81x_CS1, LOW);
    SPI.transfer(cmd >> 16);
    SPI.transfer(cmd >> 8);
    SPI.transfer(cmd);
    SPI.transfer(0x00);  // dummy byte
    uint32_t result = SPI.transfer(0x00);
    result |= (SPI.transfer(0x00) << 8);
    result |= ((uint32_t)SPI.transfer(0x00) << 16);
    result |= ((uint32_t)SPI.transfer(0x00) << 24);
    digitalWrite(FT81x_CS1, HIGH);
    SPI.endTransaction();
    return result;
}

void FT81x::write8(const uint32_t address, const uint8_t data) {
    uint32_t cmd = address | WRITE;
    SPI.beginTransaction(FT81x_SPI_SETTINGS);
    digitalWrite(FT81x_CS1, LOW);
    SPI.transfer(cmd >> 16);
    SPI.transfer(cmd >> 8);
    SPI.transfer(cmd);
    SPI.transfer(data);
    digitalWrite(FT81x_CS1, HIGH);
    SPI.endTransaction();
}

void FT81x::write16(const uint32_t address, const uint16_t data) {
    uint32_t cmd = address | WRITE;
    SPI.beginTransaction(FT81x_SPI_SETTINGS);
    digitalWrite(FT81x_CS1, LOW);
    SPI.transfer(cmd >> 16);
    SPI.transfer(cmd >> 8);
    SPI.transfer(cmd);
    SPI.transfer(data);
    SPI.transfer(data >> 8);
    digitalWrite(FT81x_CS1, HIGH);
    SPI.endTransaction();
}

void FT81x::write32(const uint32_t address, const uint32_t data) {
    uint32_t cmd = address | WRITE;
    SPI.beginTransaction(FT81x_SPI_SETTINGS);
    digitalWrite(FT81x_CS1, LOW);
    SPI.transfer(cmd >> 16);
    SPI.transfer(cmd >> 8);
    SPI.transfer(cmd);
    SPI.transfer(data);
    SPI.transfer(data >> 8);
    SPI.transfer(data >> 16);
    SPI.transfer(data >> 24);
    digitalWrite(FT81x_CS1, HIGH);
    SPI.endTransaction();
}

#endif

void FT81x::sendCommandToDisplay(const uint8_t cmd, const uint8_t numParams, const uint8_t *params) {
    SPI.beginTransaction(FT81x_SPI_SETTINGS);
    digitalWrite(FT81x_DC, LOW);
    digitalWrite(FT81x_CS2, LOW);
    SPI.transfer(cmd);
    if (numParams > 0) {
        digitalWrite(FT81x_DC, HIGH);
        for (uint8_t i = 0; i < numParams; i++) {
            SPI.transfer(params[i]);
        }
        digitalWrite(FT81x_DC, LOW);
    }
    digitalWrite(FT81x_CS2, HIGH);
    SPI.endTransaction();
}

uint8_t FT81x::queryDisplay(const uint8_t cmd) {
    SPI.beginTransaction(FT81x_SPI_SETTINGS);
    digitalWrite(FT81x_DC, LOW);
    digitalWrite(FT81x_CS2, LOW);
    SPI.transfer(cmd);
    uint8_t result = SPI.transfer(0x00);
    digitalWrite(FT81x_CS2, HIGH);
    SPI.endTransaction();
    return result;
}
