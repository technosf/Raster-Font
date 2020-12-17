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
     * @brief 
     * 
     */
    struct bitmap
    {
       // const Raster raster;
        uint16_t bitwidth; // bits
        uint8_t bitheight; // bits

        uint8_t widthoffset{0};  // bits
        uint8_t heightoffset{0}; // bits
        uint16_t width{0};       // bytes needed
        uint16_t height{0};      // bytes needed
        uint16_t xpoint{0};      // Current bit-point to place scan data
        uint8_t *data{nullptr};

        /**
         * @brief Construct a new output bitmap object
         * 
         * @param r how to rasterize the bitmap
         * @param r how to rasterize the bitmap
         * 
         * @param w width in bits
         * @param h height in bits
         * @param bitoffset offset in bits
         */
        bitmap(Raster r, Orientation o, uint16_t w, uint8_t h, uint16_t bitoffset) : raster{r}, bitwidth{w}, bitheight{h}
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
    const font_info_t *m_font; ///< The font managed by this object
    const Raster m_raster;     ///< Raster direction
    const Orientation orientation; ///< Character orientation

    void raster(unsigned char c, bitmap &scan);
};

#endif /* INCLUDE_FONT_MANAGER_H_ */
