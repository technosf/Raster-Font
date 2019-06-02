# Raster-Font

C library that produces font characters in bitmaps that scan horizontally or vertically. Fonts are embeded as header files, and individual characters are delivered in the desired raster pattern. Developed on Espressif ESP-IDF toolchain.


## Fonts

| Num | Description | Size | Norm |
|-----|-------------|------|------|
|0 | bitocra |4x7 |ascii|
|1 | bitocra |6x11 |iso8859 1|
|2 | bitocra |7x13 |iso8859 1|
|3 | glcd |5x7|
|4 | roboto |8pt|
|5 | roboto |10pt|
|6 | tahoma |10x11 |
|7 | terminus |6x12 |iso8859 1|
|8 | terminus |8x14 |iso8859 1|
|9 | terminus |10x18 |iso8859 1|
|10 | terminus |11x22 |iso8859 1|
|11 | terminus |12x24 |iso8859 1|
|12 | terminus |14x28 |iso8859 1|
|13 | terminus |16x32 |iso8859 1|
|14 | terminus bold |8x14 |iso8859 1|
|15 | terminus bold |10x18 |iso8859 1|
|16 | terminus bold |11x22 |iso8859 1|
|17 | terminus bold |12x24 |iso8859 1|
|18 | terminus bold |14x28 |iso8859 1|
|19 | terminus bold |16x32 |iso8859 1|


## Architecture and Operation

The library contain two parts - the font manager and the codified fonts. The fonts themselves are codified as C Header files containing arrays of bitmapped character byte data scanning _left-right_, _top-bottom_.

The font manager contains a fixed index of the available fonts and retrieves the character data for a particular font by calling a routing in that font header. The font amanager can then transform the _L-R/T-B_ rasterization of the charater to someother rasterization as needed: In the case of my [SSD1306 driver](https://github.com/technosf/ESP32-SSD1306-Driver), it wants _T-B/L-R_ rasterized fonts to allow the SSD1306 paged memory to be written to more effectively as it's also organized _T-B/L-R_.

This is not a _dynamic_ library, in that it doesn't read the available fonts and index them automagically - to make a font available it has to be scanned, codified and coded into the index.
  

## Future Features

Thinking about what could be added:
* LRU Character cache

##  Versions

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
