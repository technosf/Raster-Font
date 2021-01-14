/*
 Raster-Font Library Font Manager

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

#ifndef INCLUDE_FONT_MANAGER_H_
#define INCLUDE_FONT_MANAGER_H_

#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <cstring>
#include <string>

#include "fonts.h"

/**
 * @brief 
 * 
 */
class Font_Manager
{
public:
    /**
     * @brief Raster direction 
     * 
     * This is the informtion ordering utilized by the target display  
     */
    enum Raster
    {
        LRTB, ///< Left->Right, Top->Bottom
        TBLR, ///< Top->Bottom, Left->Right
    };

    /**
     * @brief Orientation
     * 
     *  The orientation of output characters 
     */
    enum Orientation
    {
        T, ///< Top
        R, ///< Right
        B, ///< Bottom
        L  ///< Left
    };

    /**
     * @brief Rasterized text and configuration info
     * 
     * Container for rasterized text and information on 
     * how it was rasterized and how to map the data to the display.
     * Data is contained within a byte array of size xbytes*ybytes
     */
    struct bitmap
    {
        Raster raster;               ///< Rasterization
        Orientation orientation;     ///< Text orientation
        uint16_t bitwidth;           ///< Width in bits for the rasterized data
        uint8_t bitheight;           ///< Hieght in bits for the rasterized data
        uint8_t widthoffsetbits{0};  ///< bits the raster data is offset to the side
        uint8_t heightoffsetbits{0}; ///< bits the raster data is offset from the top
        uint16_t xbytes{0};          ///< X-width of the rasterized data in bytes
        uint16_t ybytes{0};          ///< Y-height of the rasterized data in bytes
        uint16_t xpoint{0};          ///< Current bit-point to place scan data
        uint8_t *data{nullptr};      ///< The rasterized string

        /**
         * @brief Construct a new output bitmap object
         * 
         * @param r how to rasterize the bitmap
         * @param o how to orientate the characters
         * 
         * @param w width in bits
         * @param h height in bits
         * @param bitoffset offset in bits
         */
        bitmap(Raster r, Orientation o, uint16_t w, uint8_t h, uint16_t bitoffset) : raster{r}, orientation{o}, bitwidth{w}, bitheight{h}
        {
            switch (raster)
            /*
                     * Calc width, height in Bytes
                     */
            {
            case LRTB:
                widthoffset = bitoffset % 8;
                bitwidth += widthoffset;
                width = ((bitwidth - 1) / 8) + 1;
                height = bitheight;   // Bytes
                xpoint = widthoffset; // Bytes
                break;
            case TBLR:
                heightoffset = bitoffset % 8;
                bitheight += heightoffset;
                width = bitwidth;                   // Bytes
                height = ((bitheight - 1) / 8) + 1; // Bytes
                break;
            }
            data = (uint8_t *)calloc(width, height);
        }

        ~bitmap()
        {
            delete[] data;
        }
    };

    static uint8_t fontcount();
    static const char **fontlist();

    Font_Manager(uint8_t fontindex, Raster raster, Orientation orientation = T);
    virtual ~Font_Manager()
    {
    }

    const char *font_name();
    uint8_t font_height();
    uint8_t font_c();
    uint16_t measure_string(std::string str);
    bitmap rasterize(std::string str, uint16_t bitoffset = 0);
    bitmap rasterize(unsigned char c, uint16_t bitoffset = 0);

private:
    const font_info_t *m_font;       ///< The font managed by this object
    const Raster m_raster;           ///< Raster direction
    const Orientation m_orientation; ///< Character orientation

    void raster(unsigned char c, bitmap &scan);
};

#endif /* INCLUDE_FONT_MANAGER_H_ */
