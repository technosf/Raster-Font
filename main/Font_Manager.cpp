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

uint8_t Font_Manager::fontcount()
{
    return NUM_FONTS;
}

const char** Font_Manager::fontlist()
{
    static const char** fontlist = new const char* [ NUM_FONTS ];
    for ( int i = 0; i < NUM_FONTS; i++ )
    {
        fontlist [ i ] = fonts [ i ]->name;
        if ( fontlist [ i ] == NULL ) fontlist [ i ] = "** Font Name Missing **";
    }
    return fontlist;
}

const char* Font_Manager::font_name()
{
    return ( m_font->name );
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
    bitmap scan( m_raster, measure_string( str ), m_font->height );

    for ( char& c : str )
    {
        raster( c, scan );
    };

    return scan;
}

Font_Manager::bitmap Font_Manager::rasterize( unsigned char c )
{
    if ( ( c < m_font->char_start ) || ( c > m_font->char_end ) ) c = ' ';

    printf( "Char:0x%02x  Offset:0x%04x\n", c, m_font->char_descriptors [ c ].offset );
    bitmap scan( m_raster, m_font->char_descriptors [ c ].width, m_font->height );
    raster( c, scan );
    return scan;
}

//integrate()

void Font_Manager::raster( unsigned char c, bitmap& bm )
{
    font_char_desc_t char_desc = m_font->char_descriptors [ c ];
    const uint8_t * bitmap = m_font->bitmap + char_desc.offset;                   // Pointer to L-R bitmap
    uint8_t horizontal_read_bytes = 1 + ( char_desc.width / 8 );            // Bytes to read for horizontal
    uint8_t* data;                                                      // Data byte placement
    uint8_t lineoffset { 1 };
    if ( bm.raster == TBLR ) lineoffset = 8;
    uint8_t shift = bm.xpoint % 8;                      // Number of bits to shift right on placement

    for ( uint8_t line = 0; line < m_font->height; line++ )
    {
        data = bm.data + ( ( bm.width * ( line / lineoffset ) ) + bm.xpoint / 8 );    // address plus lines plus offset

        printf( "\nLine:%d  Chunks:%d  FMDx:%p  Offset:0x%02x Char:0x%02x  Shift:0x%02x\n", line, horizontal_read_bytes,
                data, ( ( bm.width * ( line / lineoffset ) ) + bm.xpoint / 8 ), char_desc.offset, shift );

        for ( uint8_t chunk = 0; chunk < horizontal_read_bytes; chunk++ )
        /*
         *  Read a horizontal slice of the current character, place it in the scan
         */
        {
            uint8_t word = *bitmap++;    // Read the next byte
            printf( " -> 1Byte:0x%02x\n", word );
            switch ( bm.raster )
            {
                case LRTB:
                    /*
                     * Process the byte into the current location, across byte boundaries if needed
                     */
                    printf( "LRTB Line:%d  Chunk:%d  Word:0x%02x  P:%p  Data:0x%02x  DataX:%02x", line, chunk, word,
                            data, *data, *data | ( word >> shift ) );
                    *data++ |= ( word >> shift );                   // Font char MSBs shifted to end of destination byte
                    if ( shift )
                    {
                        printf( "  xP:%p  DataXX:%02x  ", data, *data );
                        *data |= ( word << ( 8 - shift ) );    // Font char LSB shifted to start of next destination byte
                        printf( "  DataXXX:%02x", *data );
                    }
                    printf( "\n" );
                    break;

                case TBLR:
                    /*
                     * Process the bits, each going into a different vertical segment in the same bit position
                     */
                    uint8_t bitposition = line % 8;     // Vertical in the byte, little endian
                    for ( uint8_t seg = ( 8 * chunk );
                            seg <= std::min( static_cast< uint8_t >( ( chunk * 8 ) + 7 ), char_desc.width ); seg++ )
                    /*
                     * Cycle through the bits in this horizontal byte, each goes to a different segment
                     * Font is Big-Endian, Segment is Little-Endian
                     */
                    {
                        if ( word & MSBITS [ seg % 8 ] )    // Font bit is set in this bit position
                        {
                            printf( "TBLR Line:%d  Chunk:%d  Seg:%02d  Word:0x%02x  BP:0x%02x  Data:0x%02x", line,
                                    chunk, seg, word, bitposition, * ( data + seg ) );
                            * ( data + seg ) |= ( 1 << bitposition );    // Set bit at bitposition
                            printf( "  DataX:%02x  P:%p\n", * ( data + seg ), data + seg );
                        }
                    }
                    break;
            }
        }
    }
    bm.xpoint += char_desc.width + m_font->c;    // Increment pointer to next char
}

