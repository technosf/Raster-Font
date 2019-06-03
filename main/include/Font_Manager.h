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
#include <string>

#include "fonts.h"

class Font_Manager
{
    public:

        enum Raster
        {
            LRTB, TBLR,
        };

        struct bitmap
        {
                Raster raster { TBLR };
                uint16_t bitwidth { 0 };
                uint8_t bitheight { 0 };
                uint8_t ** scan { nullptr };
                uint16_t scanwidth { 0 };
                uint16_t scanheight { 0 };
                uint16_t xpoint { 0 };      // Current point to place scan data

                void init()
                {
                    switch ( raster )
                    {
                        case LRTB:
                            scanwidth = ( ( bitwidth - 1 ) / 8 ) + 1;
                            scanheight = bitheight;
                            break;
                        default:
                            scanwidth = bitwidth;
                            scanheight = ( ( bitheight - 1 ) / 8 ) + 1;
                            break;
                    }
                    scan = new uint8_t [ scanwidth ] [ scanheight ];
                }
        };

        Font_Manager( uint8_t fontindex, Raster raster );

        virtual ~Font_Manager()
        {
        }

        /**
         * @brief   Get the height of current selected font
         * @return  Height of the font (in pixels) or 0 if none font selected
         */
        virtual uint8_t font_height();

        /**
         * @brief   Get the "C" value (space between adjacent characters)
         * @return  "C" value
         */
        virtual uint8_t font_c();

        /**
         * @brief   Measure width of string with current selected font
         * @param   str         String to measure
         * @return  Width of the string
         */
        virtual uint8_t measure_string( std::string str );

        /**
         * @brief Bitmaps a string using the font
         * @param str String to bitmap
         * @return Bitmap of the string
         */
        virtual bitmap rasterize( std::string str );

        /**
         *
         * @param c
         * @return
         */
        virtual bitmap rasterize( unsigned char c );

    private:

        bitmap raster( unsigned char c, bitmap& scan );

        const font_info_t* m_font;    /// < Current font
        Raster m_raster;
};

#endif /* INCLUDE_FONT_MANAGER_H_ */
