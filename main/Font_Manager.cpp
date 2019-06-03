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

#include "Font_Manager.h"

Font_Manager::Font_Manager( uint8_t fontindex, Raster raster )
{
    m_font = fonts [ fontindex ];    // Err out if out of bounds
    m_raster = raster;
}

uint8_t Font_Manager::font_height()
{
    return ( m_font->height );
}

uint8_t Font_Manager::font_c()
{
    return ( m_font->c );
}

uint16_t Font_Manager::measure_string( std::string str )
{
    if ( str.empty() ) return 0;

    uint16_t w = 0;
    unsigned char c;

    for ( std::string::iterator i = str.begin(); i < str.end(); i++ )
    {
        c = *i;
        // we always have space in the font set
        if ( ( c < m_font->char_start ) || ( c > m_font->char_end ) ) c = ' ';
        c = c - m_font->char_start;    // c now become index to tables
        w += m_font->char_descriptors [ c ].width;
        if ( *i ) w += m_font->c;
    }

    return w;
}

Font_Manager::bitmap Font_Manager::rasterize( std::string str )
{
    bitmap scan;
    scan.raster = m_raster;
    scan.bitheight = m_font->height - 1;
    scan.bitwidth = measure_string( str );
    scan.init();

    for ( char& c : str )
    {
        raster( c, scan );
    };

    return scan;
}

Font_Manager::bitmap Font_Manager::rasterize( unsigned char c )
{
    bitmap scan;
    scan.raster = m_raster;
    scan.bitheight = m_font->height;
    scan.bitwidth = m_font->char_descriptors [ c ].width;
    scan.init();
    raster( c, scan );
    return scan;
}

Font_Manager::bitmap Font_Manager::raster( unsigned char c, bitmap& scan )
{

    // ESP_LOGI( TAG, "\tchar:\"%c\" X:%d  Y:%d\n", c, x, y );

    // Calculate the page split for the character
    uint8_t page_start = y / 8;
    uint8_t page_end = ( y + m_font->height ) / 8;
    uint8_t fontsegments [ m_font->char_descriptors [ c ].width ] [ 1 + page_end - page_start ] = {};    // Col|Page L-R, T-B, fill with background

    const uint8_t * bitmap = m_font->bitmap + m_font->char_descriptors [ c ].offset;
    uint8_t scan_length = 1 + ( m_font->char_descriptors [ c ].width / 8 );
    uint8_t scan;

    for ( uint8_t scan_line = 0; scan_line < m_font->height; scan_line++ )
    /*
     * Scan the char line by line, T-B
     */
    {
        uint8_t page = page_start + ( scan_line + y ) / 8;
        uint8_t vertical_shift = ( scan_line + y ) % 8;

        printf( "Shift y:%d Line %d, Pages %d %d  Vertical %d  \n", y, scan_line, page_start, page, vertical_shift );

        for ( uint8_t scan_byte = 0; scan_byte < scan_length; scan_byte++ )
        /*
         * Loop through bytes in a scan line
         */
        {
            scan = * ( bitmap + ( scan_line * scan_length ) + scan_byte );    // Horizontal scan line

            printf( "VShift %d  Char loc 0x%04x  byte 0x%02x\n", vertical_shift,
                    ( scan_line * scan_length ) + scan_byte, scan );

            for ( uint8_t col = 8 * scan_byte; col < m_font->char_descriptors [ c ].width; col++ )
            /*
             * Pick out the bits and OR them into the segment
             */
            {
                uint8_t bit = col % 8;

                if ( ( bit == 0 ) && ( col > 0 ) ) break;    // Byte boundary, breakout to incr scan byte

                bool scanbit = scan & ( 0x80 >> bit );    // Font scans are L-R MSB->LSB, so mask the bit L-R

//              printf( "Col %d  Scan bit 0x%02x  Shifted Scan bit 0x%02x  old Segs 0x%02x ", col, scanbit,
//                      scanbit << vertical_shift, fontsegments [ col ] [ page ] );

                fontsegments [ col ] [ page ] |= ( scanbit << vertical_shift );    // Segment bit-or with scan, shifting it down

            }    // for bits
                 //printf( " New Segs 0x%02x\n", fontsegments [ col ] [ page ] );
        }    // for horizontal bytes
    }    // for horizontal lines

    // Push out the remapped character
    for ( uint8_t page = page_start; page <= page_end; page++ )
    {
        for ( uint8_t col = 0; col < m_font->char_descriptors [ c ].width; col++ )
        {
            m_ssd1306.segment( page, x + col, fontsegments [ col ] [ page ], foreground );
        }    // for
    }    // for

    return scan;
}    // draw_char
