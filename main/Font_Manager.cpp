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

uint8_t Font_Manager::measure_string( std::string str )
{
    if ( str.empty() ) return 0;

    uint8_t w = 0;
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

bitmap Font_Manager::map_string( std::string str )
{
    bitmap scan;

    return scan;
}
