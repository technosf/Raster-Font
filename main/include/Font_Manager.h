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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <cstring>
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
                const Raster raster;
                const uint16_t bitwidth;    // bits
                const uint8_t bitheight;    // bits
                uint16_t width { 0 };       // bytes needed
                uint16_t height { 0 };      // bytes needed
                uint8_t * data { nullptr };
                uint16_t xpoint { 0 };      // Current bit-point to place scan data

                bitmap( Raster r, uint16_t w, uint8_t h ) :
                        raster { r }, bitwidth { w }, bitheight { h }
                {
                    switch ( raster )
                    /*
                     * Calc width, height in Bytes
                     */
                    {
                        case LRTB:
                            width = ( ( bitwidth - 1 ) / 8 ) + 1;
                            height = bitheight;
                            break;
                        case TBLR:
                            width = bitwidth;
                            height = ( ( bitheight - 1 ) / 8 ) + 1;
                            break;
                    }
                    data = (uint8_t*) malloc( width * height );
                    memset( data, 0, width * height );
                }

                ~bitmap()
                {
                    delete [] data;
                }
        };

        Font_Manager( uint8_t fontindex, Raster raster );

        virtual ~Font_Manager()
        {
        }

        /**
         * @brief The number of fonts available
         * @return the number of fonts
         */
        static uint8_t fontcount();

        /**
         * @brief A list of the available fonts
         * @return the font list
         */
        static const char** fontlist();

        /**
         * @brief Get the name of the font
         * @return the name of the font
         */
        const virtual char* font_name();

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
        virtual uint16_t measure_string( std::string str );

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

        void raster( unsigned char c, bitmap& scan );

        // const uint8_t BITS [ 8 ] = { 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };    /// < Segment bit mask
        const uint8_t MSBITS [ 8 ] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };    /// < Segment bit mask

        const font_info_t* m_font;    /// < Current font
        Raster m_raster;
};

#endif /* INCLUDE_FONT_MANAGER_H_ */
