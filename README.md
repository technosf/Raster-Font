# Raster-Font

C library that produces font characters in bitmaps that scan horizontally or vertically. Fonts are embeded as header files, and individual characters are delivered in the desired raster pattern. 
Developed on Espressif ESP-IDF toolchain, debugged seperately on Eclipse CDT.


## Fonts

The fonts are grouped by character set, with the custom _GLCD5x7_ font always included and first.

The character sets to be compiled into the library are defined in _fonts.h_. Depending on the active charactersets, the index of a particular font may change. If all are chosen, these are the available fonts:

| Num | Description | Size | Char Set |
|-----|-------------|------|----------|
|0|	glcd_5x7|5x7|
|1|	bitocra_4x7_ascii|4x7|ascii|
|2|	roboto_8pt_ascii|8x11|ascii|
|3|	roboto_10pt_ascii|11x14|ascii|
|4|	tahoma_8pt_ascii|10x11|ascii|
|5|	bitocra_6x11_iso8859_1|6x11|iso8859_1|
|6|	bitocra_7x13_iso8859_1|7x13|iso8859_1|
|7|	terminus_6x12_iso8859_1|6x12|iso8859_1|
|8|	terminus_8x14_iso8859_1|8x14|iso8859_1|
|9|	terminus_10x18_iso8859_1|10x18|iso8859_1|
|10|	terminus_11x22_iso8859_1|11x22|iso8859_1|
|11|	terminus_12x24_iso8859_1|12x24|iso8859_1|
|12|	terminus_14x28_iso8859_1|14x28|iso8859_1|
|13|	terminus_16x32_iso8859_1|16x32|iso8859_1|
|14|	terminus_bold_8x14_iso8859_1|8x14|iso8859_1|
|15|	terminus_bold_10x18_iso8859_1|10x18|iso8859_1|
|16|	terminus_bold_11x22_iso8859_1|11x22|iso8859_1|
|17|	terminus_bold_12x24_iso8859_1|12x24|iso8859_1|
|18|	terminus_bold_14x28_iso8859_1|14x28|iso8859_1|
|19|	terminus_bold_16x32_iso8859_1|16x32|iso8859_1|
|20|	terminus_6x12_koi8_r|6x12|koi8_r|
|21|	terminus_8x14_koi8_r|8x14|koi8_r|
|22|	terminus_14x28_koi8_r|14x28|koi8_r|
|23|	terminus_16x32_koi8_r|16x32|koi8_r|
|24|	terminus_bold_8x14_koi8_r|8x14|koi8_r|
|25|	terminus_bold_14x28_koi8_r|14x28|koi8_r|
|26|	terminus_bold_16x32_koi8_r|16x32|koi8_r|

## Features

* Left-Right Top-Bottom rasterization
* Top-Bottom Left-Right rasterization (on the fly)
* Position offset - can shift the bitmap in the byte data along the rasterization axis 

The original fonts are _Left-Right Top-Bottom_ scanned, but on-the-fly _Top-Bottom Left-Right_ rasterization is provided to allow paged type bitmapps to be supported directly in-library.

The position offset moved the bitmap along the major raster axis so that the output can be directly ORed with the destination bitmap without the need to calculate any required shift at the byte-boundary by the implementing app. 


## Architecture and Operation

The library contains two parts - the font manager, which is an instance of a given font and raster orientation, and the codified fonts. The fonts themselves are codified as individual C Header files containing arrays of bitmapped character byte data scanning _left-right_, _top-bottom_.

The font manager contains a fixed index of the available fonts and retrieves the character data for a particular font by calling a routing in that fonts' header. The font manager can then transform the _L-R/T-B_ rasterization of the charater to some other rasterization as needed: In the case of my [SSD1306 driver](https://github.com/technosf/ESP32-SSD1306-Driver), it wants _T-B/L-R_ rasterized fonts to allow the SSD1306 paged memory to be written to more effectively as it's also organized _T-B/L-R_.

This is not a _dynamic_ library, in that it doesn't read the available fonts and index them automagically - to make a font available it has to be scanned, codified and coded into the index. One point to note is that some of the fonts processed for Baoshi's code have character indexes that are 1-out because of C arrays are index from _zero_ and the first non-null character is _one_: The fist *font_char_desc_t* entries should be a dummy to compensate to allow a direct character-number to character-representation mapping.

### Example

In paged display bitmap such as that in the SD1306, to rasterize a 5-bit high character at Y 21 means the character crosses a page boundry, starting in page 2 (21/8) and ending in page 3 (26/8). Using 21 as the offset the resulting bitmap is split into two rows that can be ORed directly into Page 2 and Page 3.


```
   /*
    * Create a font manager for glcd_5x7 to raster to an 
    * SSD1306 driving a 128x64 oled
    */
   Font_Manager fm( 3, Font_Manager::TBLR );    
    
   uint8_t x = 40;
   uint8_t y = 21;
   uint8_t page = y / 8;
   std::string s = "@test";

   
   /*
    * Raster an ampersand at the major raster axis (Y for TBLR) position y
    * and grab the bitmap data
    */
   Font_Manager::bitmap b = fm.rasterize( s,  y ); 
   uint8_t * d = b.data; 

	for ( yy = page; i < ( page + b.height ); i++ )
	/*
	 * For each page
	 */
    {
       uint8_t * oled = getOledAtCoord( x, yy * 8 );  // Oled memory buffer address at x, paged y
       
        for ( int xx = x; j < (x + b.width ); j++ )
        /*
         * for each column, OR the bit map into the oled buffer
         */
        {
            *oled++ |= *d++;
        }
    }
```

Integration and use can be seen in [ESP32-SSD1306-Driver](https://github.com/technosf/ESP32-SSD1306-Driver)


## Future Features

Thinking about what could be added:
* LRU Character cache for TBLR fonts

##  Versions

* _1.0.0_	First release with 27 fonts, verticle and horizontal rasterization, position offset
* _0.1.0_	Initial commit and pre-release


## History and References

This repo is a fork of [Baoshi's ESP-I2C-OLED repo](https://github.com/baoshi/ESP-I2C-OLED), discarding the i2C/SSD1306 code, and developing _font management_ code only. This move came from work on my own [SSD1306 driver fork](https://github.com/technosf/ESP32-SSD1306-Driver), but in organizing the font usage to better integrate with the bitmapped memory architecture of the SSD1306 I decide to just break this out into a seperate repo for re-use.

Looking through the various forks of the SSD1306 and similar drivers, you find plenty of font bitmaps you can you, such as the set from [ubirch](https://github.com/ubirch/esp32-i2c-ssd1306-oled) I've also borrowed here.

## License

Raster-Font - Copyright 2019  technosf  [http://github.com/technosf]

Licensed under the GNU LESSER GENERAL PUBLIC LICENSE, Version 3.0 or greater (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
http://www.gnu.org/licenses/lgpl-3.0.en.html
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
![lglp3 logo](http://www.gnu.org/graphics/lgplv3-88x31.png)

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

