/*
 Raster-Font Example code

 v0.1.0

 Copyright 2019 technosf [https://github.com/technosf]

 Licensed under the GNU LESSER GENERAL PUBLIC LICENSE, Version 3.0 or greater (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 https://www.gnu.org/licenses/lgpl-3.0.en.html
 Unless required by applicable law or agreed to in writing,
 software distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and limitations under the License.
 */

#include <graphics.h>
#include <stdio.h>

#include "FontManager.h"

extern "C" int main();

void raster(char c, FontManager &fm, uint8_t o)
{
    printf("\nFont:%s  Char:0x%03x  Offset:%d\n", fm.FontName(), c, o);

    FontManager::Bitmap b = fm.Rasterize(c, o);

    uint8_t *d = b.data;

    for (int i = 0; i < b.bytes_per_column; i++)
    {
        for (int j = 0; j < b.bytes_per_row; j++)
        {
            printf("0x%02x ", *d);
            d++;
        }
    }
    printf("\n");
}

int main()
{
    /*
     * List the fonts and indices
     */
    printf("Font List\n");
    const char **fontlist = FontManager::FontList();
    for (int i = 0; i < FontManager::FontCount(); i++)
    {
        printf("%d\t%s\n", i, fontlist[i]);
    }
    printf("\n\n");

    FontManager fm3LRTB(3, FontManager::LRTB);   // glcd_5x7
    FontManager fm3TBLR(3, FontManager::PTBLR);   // glcd_5x7
    FontManager fm9LRTB(9, FontManager::LRTB);   // terminus_10x18_iso8859_1
    FontManager fm9TBLR(9, FontManager::PTBLR);   // terminus_10x18_iso8859_1
    FontManager fm13LRTB(13, FontManager::LRTB); // terminus_16x32_iso8859_1
    FontManager fm13TBLR(13, FontManager::PTBLR); // terminus_16x32_iso8859_1

    raster('|', fm3LRTB, 0); // glcd_5x7 : 0x20 0x20 0x20 0x00 0x20 0x20 0x20
    raster('|', fm3LRTB, 1); // glcd_5x7 : 0x10 0x10 0x10 0x00 0x10 0x10 0x10
    raster('|', fm3LRTB, 4); // glcd_5x7 : 0x02 0x02 0x02 0x00 0x02 0x02 0x02

    raster('|', fm3TBLR, 0); // glcd_5x7 : 0x00 0x00 0x77 0x00
    raster('|', fm3TBLR, 1); // glcd_5x7 : 0x00 0x00 0xee 0x00
    raster('|', fm3TBLR, 4); // glcd_5x7 : 0x00 0x00 0x70 0x00 0x00 0x00 0x07 0x00

    raster('|', fm9LRTB, 0); // terminus_10x18_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x08 0x00 0x08 0x00 0x08 0x00 0x08 0x00 0x08 0x00 0x08 0x00 0x08 0x00 0x08 0x00 0x08 0x00 0x08 0x00 0x08 0x00 0x08 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    raster('|', fm9LRTB, 1); // terminus_10x18_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x04 0x00 0x04 0x00 0x04 0x00 0x04 0x00 0x04 0x00 0x04 0x00 0x04 0x00 0x04 0x00 0x04 0x00 0x04 0x00 0x04 0x00 0x04 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    raster('|', fm9LRTB, 4); // terminus_10x18_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x80 0x00 0x80 0x00 0x80 0x00 0x80 0x00 0x80 0x00 0x80 0x00 0x80 0x00 0x80 0x00 0x80 0x00 0x80 0x00 0x80 0x00 0x80 0x00 0x00 0x00 0x00 0x00 0x00

    raster('|', fm9TBLR, 0); // terminus_10x18_iso8859_1 : 0x00 0x00 0x00 0x00 0xf8 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x7f 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    raster('|', fm9TBLR, 1); // terminus_10x18_iso8859_1 : 0x00 0x00 0x00 0x00 0xf0 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0xff 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    raster('|', fm9TBLR, 4); // terminus_10x18_iso8859_1 : 0x00 0x00 0x00 0x00 0x80 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0xff 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x07 0x00 0x00 0x00 0x00

    raster('|', fm13LRTB, 0); // terminus_16x32_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x01 0x80 0x81 0x01 0x81 0x80 0x81 0x01 0x81 0x80 0x81 0x01 0x81 0x80 0x81 0x01 0x81 0x80 0x81 0x01 0x81 0x80 0x81 0x01 0x81 0x80 0x81 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x1e 0x1f 0x00 0x83 0x01 0x81 0x80 0x81 0x01 0x81 0x80 0x81 0x00
    raster('|', fm13LRTB, 1); // terminus_16x32_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0xc0 0x40 0x80 0xc0 0xc0 0x40 0x80 0xc0 0xc0 0x40 0x80 0xc0 0xc0 0x40 0x80 0xc0 0xc0 0x40 0x80 0xc0 0xc0 0x40 0x80 0xc0 0xc0 0x40 0x80 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x0f 0x0f 0x80 0x41 0x80 0xc0 0xc0 0x40 0x80 0xc0 0xc0 0x40 0x80
    raster('|', fm13LRTB, 4); // terminus_16x32_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x01 0xe0 0x01 0xf0 0x00 0x38 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x18 0x00 0x0f

    raster('|', fm13TBLR, 0); // terminus_16x32_iso8859_1 : 0xa0 0x00 0x00 0x00 0x00 0x00 0x00 0x50 0x50 0x00 0x00 0x00 0x00 0x00 0x00 0xaa 0xa0 0x00 0x00 0x00 0x00 0x00 0x55 0x55 0x00 0x00 0x00 0x00 0x00 0x00 0xaa 0xaa 0x00 0x00 0x00 0x00 0x00 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0xa8 0x00 0x00 0x04 0x04 0x04 0x04 0x54 0x50 0x00 0x00 0x02 0x02 0x02 0x02
    raster('|', fm13TBLR, 1); // terminus_16x32_iso8859_1 : 0x40 0x00 0x00 0x00 0x00 0x00 0x00 0xa0 0xa0 0x00 0x00 0x00 0x00 0x00 0x00 0x55 0x40 0x00 0x00 0x00 0x00 0x00 0xaa 0xaa 0x00 0x00 0x00 0x00 0x00 0x00 0x55 0x55 0x00 0x00 0x00 0x00 0x00 0x02 0x02 0x00 0x00 0x00 0x00 0x00 0x00 0x51 0x01 0x00 0x08 0x08 0x08 0x08 0xa8 0xa0 0x00 0x00 0x04 0x04 0x04 0x04 0x51 0x50 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    raster('|', fm13TBLR, 4); // terminus_16x32_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0xaa 0x00 0x00 0x00 0x00 0x00 0x00 0x55 0x55 0x00 0x00 0x00 0x00 0x00 0x00 0xaa 0xaa 0x00 0x00 0x00 0x00 0x00 0x15 0x15 0x00 0x00 0x00 0x00 0x00 0x00 0x8a 0x0a 0x00 0x40 0x40 0x40 0x40 0x40 0x00 0x00 0x00 0x20 0x20 0x20 0x20 0x8a 0x80 0x00 0x00 0x00 0x00 0x00 0x05 0x05 0x00 0x00 0x00 0x00 0x00 0x00

    raster('@', fm3LRTB, 0); // glcd_5x7 : 0x70 0x88 0xa8 0xb8 0xb0 0x80 0x78
    raster('@', fm3LRTB, 1); // glcd_5x7 : 0x38 0x44 0x54 0x5c 0x58 0x40 0x3c
    raster('@', fm3LRTB, 4); // glcd_5x7 : 0x07 0x08 0x8a 0x8b 0x8b 0x08 0x07

    raster('@', fm3TBLR, 0); // glcd_5x7 : 0x3e 0x41 0x5d 0x59
    raster('@', fm3TBLR, 1); // glcd_5x7 : 0x7c 0x82 0xba 0xb2
    raster('@', fm3TBLR, 4); // glcd_5x7 : 0xe0 0x10 0xd0 0x90 0xe3 0x04 0x05 0x05

    raster('@', fm9LRTB, 0); // terminus_10x18_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x3f 0x00 0x40 0x80 0x40 0x80 0x47 0x80 0x48 0x80 0x48 0x80 0x48 0x80 0x49 0x80 0x46 0x80 0x40 0x00 0x40 0x00 0x3f 0x80 0x00 0x00 0x00 0x00 0x00 0x00
    raster('@', fm9LRTB, 1); // terminus_10x18_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x1f 0x80 0x20 0x40 0x20 0x40 0x23 0xc0 0x24 0x40 0x24 0x40 0x24 0x40 0x24 0xc0 0x23 0x40 0x20 0x00 0x20 0x00 0x1f 0xc0 0x00 0x00 0x00 0x00 0x00 0x00
    raster('@', fm9LRTB, 4); // terminus_10x18_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x03 0xf0 0x04 0x08 0x04 0x08 0x04 0x78 0x04 0x88 0x04 0x88 0x04 0x88 0x04 0x98 0x04 0x68 0x04 0x00 0x04 0x00 0x03 0xf8 0x00 0x00 0x00 0x00 0x00 0x00

    raster('@', fm9TBLR, 0); // terminus_10x18_iso8859_1 : 0x00 0xf0 0x08 0x08 0x88 0x48 0x48 0x48 0xf0 0x00 0x3f 0x40 0x40 0x47 0x48 0x48 0x44 0x4f 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    raster('@', fm9TBLR, 1); // terminus_10x18_iso8859_1 : 0x00 0xe0 0x10 0x10 0x10 0x90 0x90 0x90 0xe0 0x00 0x7f 0x80 0x80 0x8f 0x90 0x90 0x88 0x9f 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
    raster('@', fm9TBLR, 4); // terminus_10x18_iso8859_1 : 0x00 0x00 0x80 0x80 0x80 0x80 0x80 0x80 0x00 0x00 0xff 0x00 0x00 0x78 0x84 0x84 0x44 0xff 0x00 0x03 0x04 0x04 0x04 0x04 0x04 0x04 0x04

    raster('@', fm13LRTB, 0); // terminus_16x32_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x1f 0xf0 0xff 0x70 0x7c 0x0c 0xfd 0x63 0xff 0x0c 0x6e 0x66 0x6e 0x0c 0x6e 0x66 0x6e 0x0c 0x7f 0x63 0xfd 0xec 0x60 0x70 0x3f 0xfc 0xff 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x0f 0xff 0xf8 0x3c 0x30 0x3c 0x0c 0x3c 0x30 0x3c 0x0c 0x3c 0x30
    raster('@', fm13LRTB, 1); // terminus_16x32_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x0f 0xf8 0x7f 0xb8 0x3e 0x06 0x7e 0xb1 0xff 0x86 0x37 0x33 0x37 0x06 0x37 0x33 0x37 0x06 0x3f 0xb1 0xfe 0xf6 0x30 0x38 0x1f 0xfe 0x7f 0x80 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x07 0xff 0xfc 0x1e 0x18 0x1e 0x06 0x1e 0x18 0x1e 0x06 0x1e 0x18
    raster('@', fm13LRTB, 4); // terminus_16x32_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x01 0xff 0x03 0xff 0x87 0x01 0xc6 0x00 0xc6 0x1f 0xc6 0x3f 0xc6 0x70 0xc6 0x60 0xc6 0x60 0xc6 0x60 0xc6 0x60 0xc6 0x60 0xc6 0x60 0xc6 0x71 0xc6 0x3f 0xc6 0x1e 0xc6 0x00 0x07 0x00 0x03 0xff 0xc1 0xff 0xc0 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0xff 0x01 0xff 0x83 0x81 0xc3 0x00 0xc3 0x00 0xc3 0x00 0xc3 0x00 0xc3 0x00 0xc3 0x00 0xc3 0x00

    raster('@', fm13TBLR, 0); // terminus_16x32_iso8859_1 : 0xa0 0xe0 0xe0 0xb0 0xb0 0x90 0x10 0x10 0x10 0xb0 0xb0 0x30 0x40 0x40 0x80 0x80 0xd5 0x55 0x20 0x2a 0x3f 0x15 0x41 0x40 0xea 0xea 0x80 0x55 0x5f 0x2a 0x22 0x22 0x03 0x03 0x03 0x03 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x00 0x00 0x00 0x50 0x5c 0xac 0xac 0x04 0x04 0x04 0x04 0xac 0xac 0x56 0x52 0x02
    raster('@', fm13TBLR, 1); // terminus_16x32_iso8859_1 : 0x40 0xc0 0xc0 0x60 0x60 0x20 0x20 0x20 0x20 0x60 0x60 0x60 0x80 0x80 0x00 0x01 0xab 0xab 0x41 0x55 0x7f 0x2a 0x82 0x80 0xd5 0xd5 0x00 0xaa 0xbe 0x55 0x45 0x45 0x06 0x06 0x06 0x06 0x02 0x02 0x02 0x03 0x03 0x03 0x02 0x02 0x00 0x00 0x00 0xa0 0xb8 0x58 0x58 0x08 0x08 0x08 0x08 0x58 0x58 0xac 0xa4 0x04 0x04 0x04 0x00 0x00 0x01 0x01 0x00 0x00 0x00 0x00 0x01 0x01 0x00 0x00 0x00
    raster('@', fm13TBLR, 4); // terminus_16x32_iso8859_1 : 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x0a 0x5e 0x5e 0x0b 0xab 0xf9 0x51 0x11 0x01 0xab 0xab 0x03 0x54 0xf4 0xa8 0x28 0x2d 0x35 0x32 0x32 0x33 0x11 0x14 0x14 0x1e 0x1e 0x18 0x15 0x15 0x02 0x02 0x02 0x00 0xc0 0xc0 0xc0 0x40 0x40 0x40 0x40 0xc0 0xc0 0x60 0x20 0x20 0x20 0x20 0x05 0x05 0x0a 0x0a 0x00 0x00 0x00 0x00 0x0a 0x0a 0x05 0x05 0x00

    return 0;
}