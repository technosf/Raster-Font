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

#ifndef INCLUDE_FONTMANAGER_H_
#define INCLUDE_FONTMANAGER_H_

#include <stdint.h>
//#include <stdlib.h>
#include <vector>
#include <cstring>
#include <string>

#include "fonts.h"

/**
 * @brief 
 * 
 */
class FontManager
{
public:
    /**
     * @brief Raster direction 
     * 
     * This is the informtion ordering utilized by the target display  
     */
    enum Raster
    {
        LRTB,  ///< Cartesian Left->Right, Top->Bottom
        PTBLR, ///< Paged Top->Bottom, Left->Right
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
     * @brief XY dimensions
     * 
     */
    struct XY
    {
        uint16_t x_pixels{0};
        uint16_t y_pixels{0};
    };

    /**
     * @brief Rasterized text and configuration info
     * 
     * Container for rasterized text, information on 
     * how it was rasterized and how to map the data to the display.
     * Data is contained within a byte array of size bytes_per_row*bytes_per_column
     */
    struct Bitmap
    {
        Raster raster;                   ///< Rasterization
        Orientation orientation;         ///< Text orientation
        uint16_t width_pixels;           ///< Width in bits for the rasterized data
        uint16_t height_pixels;          ///< Hieght in bits for the rasterized data
        uint8_t width_offset_pixels{0};  ///< bits the raster data is offset to the side
        uint8_t height_offset_pixels{0}; ///< bits the raster data is offset from the top
        uint16_t bytes_per_row{0};       ///< Byte row width to contain data and position offset
        uint16_t bytes_per_column{0};    ///< Byte column height to contain data and position offset
        uint16_t bitpoint{0};           ///< Current bit-point to place scan data
        uint8_t *data{nullptr};          ///< The rasterized string data

        ~Bitmap()
        {
            printf("Delete\n");
            delete[] data;
        }
    };

    static uint8_t FontCount();
    static const char **FontList();

    FontManager(uint8_t fontIndex, Raster raster, Orientation orientation = T);
    virtual ~FontManager()
    {
    }

    const char *FontName();
    uint8_t FontHeight();
    uint8_t FontC();
    XY MeasureString(std::string str);
    std::vector<uint16_t> CharacterBreaks(std::string str, uint16_t pixels);
    Bitmap Rasterize(std::string str, uint16_t bitOffset = 0);
    Bitmap Rasterize(unsigned char c, uint16_t bitOffset = 0);

private:
    const font_info_t *m_font;       ///< The font managed by this object
    const Raster m_raster;           ///< Raster direction
    const Orientation m_orientation; ///< Character orientation

    void RasterChar(unsigned char c, Bitmap &scan);
};

#endif /* INCLUDE_FONTMANAGER_H_ */
