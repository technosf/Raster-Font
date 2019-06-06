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

void app_main()
{
    Font_Manager fm( 0, Font_Manager::LRTB );
    Font_Manager::bitmap x = fm.rasterize( '@' );
    uint8_t * d = x.data;
    for ( int i = 0; i < x.height; i++ )
    {
        for ( int j = 0; j < x.width; j++ )
        {
            printf( "0x00%x", *d++ );
        }
    }

}
