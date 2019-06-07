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

#include <stdio.h>

#include "Font_Manager.h"

extern "C" void app_main();

void raster( char c, Font_Manager fm );

void app_main()
{
    /*
     * List the fonts and indices
     */
    printf( "Font List\n" );
    const char** fontlist = Font_Manager::fontlist();
    for ( int i = 0; i < Font_Manager::fontcount(); i++ )
    {
        printf( "%d\t%s\n", i, fontlist [ i ] );
    }
    printf( "\n\n" );

    Font_Manager fm9LRTB( 9, Font_Manager::LRTB );    // terminus_10x18_iso8859_1
    Font_Manager fm9TBLR( 9, Font_Manager::TBLR );    // terminus_10x18_iso8859_1
    Font_Manager fm3LRTB( 3, Font_Manager::LRTB );    // glcd_5x7
    Font_Manager fm3TBLR( 3, Font_Manager::TBLR );    // glcd_5x7

    raster( '@', fm3LRTB );    // glcd_5x7 : 0x70 0x88 0xA8 0xB8 0xB0 0x80 0x78
    raster( '@', fm3TBLR );    // glcd_5x7 : 0x3E 0x41 0x5D 0x59 0x4E
}

void raster( char c, Font_Manager fm )
{
    printf( "\nFont:%s  Char:0x%03x \n", fm.font_name(), c );

    Font_Manager::bitmap x = fm.rasterize( '@' );
    uint8_t * d = x.data;

    for ( int i = 0; i < x.height; i++ )
    {
        for ( int j = 0; j < x.width; j++ )
        {
            printf( "0x%02x ", *d );
            d++;
        }
        printf( "\n" );
    }

}
