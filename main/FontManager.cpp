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

#include "FontManager.h"

static const uint8_t MSBITS[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}; ///< Segment bit mask

/**
 * @brief Instantiates a FontManager for the given font and raster orientation
 *
 * @param fontindex the font to produce
 * @param raster The direction to rasterize the font
 */
FontManager::FontManager(uint8_t fontIndex, Raster raster, Orientation orientation) : m_raster{raster}, m_orientation{orientation}
{
    m_font = fonts[fontIndex]; // Err out if out of bounds
} // FontManager

/**
 * @brief The number of fonts available
 * 
 * @return the number of fonts
 */
uint8_t FontManager::FontCount()
{
    return NUM_FONTS;
} // FontCount

/**
 * @brief A list of the available fonts
 * 
 * @return the font list
 */
const char **FontManager::FontList()
{
    static const char **fontlist = new const char *[NUM_FONTS];
    for (int i = 0; i < NUM_FONTS; i++)
    {
        fontlist[i] = fonts[i]->name;
        if (fontlist[i] == NULL)
            fontlist[i] = "** Font Name Missing **";
    }
    return fontlist;
} // FontList

/**
 * @brief   Get the font name
 * 
 * @return  font name
 */
const char *FontManager::FontName()
{
    return (m_font->name);
} // FontName

/**
 * @brief   Get the height of current selected font
 * 
 * @return  Height of the font (in pixels) or 0 if none font selected
 */
uint8_t FontManager::FontHeight()
{
    return (m_font->height);
} // FontHeight

/**
 * @brief   Get the "C" value (space between adjacent characters)
 * 
 * @return  "C" value
 */
uint8_t FontManager::FontC()
{
    return (m_font->c);
} // FontC

/**
 * @brief   Measure width of string with current selected font
 * 
 * @param   str String to measure
 * @return  Width of the string
 */
FontManager::XY FontManager::MeasureString(std::string str)
{
    XY xy;

    if (str.empty())
        return xy;

    unsigned char c;

    for (std::string::iterator i = str.begin(); i < str.end(); i++)
    {
        c = *i;
        if ((c < m_font->char_start) || (c > m_font->char_end))
            c = ' ';                // Replace unknown characters with space
        c = c - m_font->char_start; // c becomes the character index

        if (m_orientation & 1)
        /**
         * Odd - Verticle orientation 
         */
        {
            xy.y_pixels = (xy.y_pixels > m_font->height) ? xy.y_pixels : m_font->height;
        }
        else
        /**
         * Even - Horizontal orientation 
         */
        {
            xy.x_pixels += m_font->char_descriptors[c].width; // increment the width
            if (*i)                                           // Add kerning
                xy.x_pixels += m_font->c;
        }
    }

    if (xy.y_pixels > 0)
    {
        xy.x_pixels = m_font->char_descriptors->width;
    }
    else if (xy.x_pixels > 0)
    {
        xy.y_pixels = m_font->height;
    }

    return xy;
} // MeasureString

/**
 * @brief For wrapping text, the set of characters that break at the pixel positions 
 * 
 * @param str the string to find the character breaks for
 * @param pixels the number of pixels to break the character string at
 * @return std::Vector<uint16_t> the charater positions that abutt the pixel boundry
 */
std::vector<uint16_t> FontManager::CharacterBreaks(std::string str, uint16_t pixels)
{
    std::vector<uint16_t> breaking_chars;

    if (((m_orientation & 1) && pixels < m_font->height) || pixels < m_font->char_descriptors->width)
    // Check pixel width is sane
    {
        return breaking_chars;
    }

    if (m_orientation & 1)
    /**
     * Odd - Verticle orientation - Height is standard, so use division
     */
    {
        uint16_t chars_per_line = pixels / m_font->height;
        for (uint16_t i = chars_per_line; i < strlen(str.c_str()); i += chars_per_line)
        {
            breaking_chars.push_back(i);
        }
        return breaking_chars;
    }

    /**
     * Even - Horizontal orientation, can be variable, so addition
     */

    uint16_t char_pos{0};
    uint16_t pixel_pos{0};
    unsigned char c;

    for (std::string::iterator i = str.begin(); i < str.end(); i++)
    {
        c = *i;
        if ((c < m_font->char_start) || (c > m_font->char_end))
            c = ' ';                // Replace unknown characters with space
        c = c - m_font->char_start; // c becomes the character index

        if ((m_font->char_descriptors[c].width + pixel_pos) > pixels)
        // Char Break
        {
            breaking_chars.push_back(char_pos);
            pixel_pos = 0;
        }
        pixel_pos += m_font->char_descriptors[c].width + m_font->c;
        char_pos++;
    }

    return breaking_chars;
} // CharacterBreaks

/**
 * @brief Create a Bitmap object
 * 
 * @param r 
 * @param o 
 * @param w 
 * @param h 
 * @param bitOffset 
 * @return FontManager::bitmap 
 */
static FontManager::Bitmap createBitmap(FontManager::Raster r, FontManager::Orientation o, FontManager::XY xy, uint16_t bitOffset)
{
    FontManager::Bitmap bm;
    bm.raster = r;
    bm.orientation = o;
    bm.width_pixels = xy.x_pixels;
    bm.height_pixels = xy.y_pixels;

    switch (r)
    {
    case FontManager::LRTB:
        bm.width_offset_pixels = bitOffset % 8;
        bm.width_pixels += bm.width_offset_pixels;
        bm.bytes_per_row = ((bm.width_pixels - 1) / 8) + 1;
        bm.bytes_per_column = bm.height_pixels; // Bytes
        bm.bitpoint = bm.width_offset_pixels;  // Bytes
        break;
    case FontManager::PTBLR:
        bm.height_offset_pixels = bitOffset % 8;
        bm.height_pixels += bm.height_offset_pixels;
        bm.bytes_per_row = bm.width_pixels;                     // Bytes
        bm.bytes_per_column = ((bm.height_pixels - 1) / 8) + 1; // Bytes
        break;
    }
    printf("Calloc %d %d %d %d\n", xy.x_pixels, xy.y_pixels, bm.bytes_per_row, bm.bytes_per_column);
    bm.data = (uint8_t *)calloc(bm.bytes_per_row, bm.bytes_per_column);
    printf("Calloc done\n");
    return bm;
} // createBitmap

/**
 * @brief Bitmaps a string using the font, shifting the bitmap as required.
 *
 * Raster the string using the font manager settings into a new bitmap structure.
 * The string can be shifted along or down in the bitmap by up to 7 bits -
 * the x or y shift is calculated from modulus 8 of the shift parameters, allowing
 * absolute position to be used without prior calculation from the caller.
 *
 * @param str String to bitmap
 * @param bitoffset The number of bits to shift the bitmap
 * @return Bitmap of the string
 */
FontManager::Bitmap FontManager::Rasterize(std::string str, uint16_t bitOffset)
{
    Bitmap scan = createBitmap(m_raster, T, MeasureString(str), bitOffset);

    for (char &c : str)
    {
        if ((c < m_font->char_start) || (c > m_font->char_end))
            c = ' ';
        c = c - m_font->char_start;
        RasterChar(c, scan);
    };

    return scan;
} // Rasterize

/**
 * @brief Bitmaps a character using the font, shifting the bitmap as required.
 *
 * Raster the character using the font manager settings into a new bitmap structure.
 * The character can be shifted along or down in the bitmap by up to 7 bits -
 * the x or y shift is calculated from modulus 8 of the shift parameters, allowing
 * absolute position to be used without prior calculation from the caller.
 *
 * @param c The character to bitmap
 * @param bitoffset The number of bits to shift the bitmap
 * @return Bitmap of the char
 */
FontManager::Bitmap FontManager::Rasterize(unsigned char c, uint16_t bitOffset)
{
    if ((c < m_font->char_start) || (c > m_font->char_end))
        c = ' ';
    c = c - m_font->char_start;

    XY xy;

    if (m_orientation & 1)
    {
        xy = {m_font->height, m_font->char_descriptors[c].width};
    }
    else
    {
        xy = {m_font->char_descriptors[c].width, m_font->height};
    }

    printf("rasterize scan\n");

    Bitmap scan = createBitmap(m_raster, T, xy, bitOffset);

    printf("rasterize raster\n");
    RasterChar(c, scan);
    printf("rasterize done\n");
    return scan;
} // Rasterize

/**
 * @brief Rasters the given character and appends to the bitmap
 * 
 * @param c the character to rasterize
 * @param bm the bitmap to append the rasterized character to
 */
void FontManager::RasterChar(unsigned char c, Bitmap &bm)
{
    font_char_desc_t char_desc = m_font->char_descriptors[c];
    const uint8_t *char_bitmap = m_font->bitmap + char_desc.offset;  // Pointer to L-R bitmap
    uint8_t horizontal_read_bytes = 1 + ((char_desc.width - 1) / 8); // Bytes to read for horizontal
    uint8_t *pixel;                                                  // Data byte placement
    uint8_t right_shift{0};                                          // The right shift to apply
    uint8_t column_coefficient{8};                                   // PTBLR default
    uint8_t row_coefficient{1};                                      // PTBLR default

    if (bm.raster == LRTB)
    {
        right_shift = bm.bytes_per_row % 8; // Number of bits to shift right on placement
        row_coefficient = 1;                // For vertical raster, each row is 1/8th shift
        row_coefficient = 8;
    }

    for (uint8_t row = 0; row < m_font->height; row++)
    /**
     * Cycle throught each horizontal scan line of the character 
     */
    {
        uint16_t row_data_offset = bm.bytes_per_column * ((row + bm.height_offset_pixels) / column_coefficient); //
        pixel = bm.data + row_data_offset + (bm.bytes_per_row / row_coefficient);                                // address plus rows plus offset

        for (uint8_t column = 0; column < horizontal_read_bytes; column++)
        /*
         *  Read a horizontal slice of the current character, place it in the scan
         */
        {
            uint8_t word = *char_bitmap++; // Read the next byte

            switch (bm.raster)
            {
            case LRTB:
                /*
                * Process the byte into the current location, across byte boundaries if needed
                */
                *pixel++ |= (word >> right_shift); // Font char MSBs shifted to end of destination byte
                if (right_shift)
                {
                    *pixel |= (word << (8 - right_shift)); // Font char LSB shifted to start of next destination byte
                }
                break;

            case PTBLR:
                /*
                * Process the bits, each going into a different vertical segment in the same bit position
                */
                uint8_t bit_position = (row + bm.height_offset_pixels) % 8; // Vertical in the byte, little endian
                for (uint8_t seg = (8 * column);
                     seg <= std::min(static_cast<uint8_t>((column * 8) + 7), char_desc.width); seg++)
                /*
                * Bit Cycle through this horizontal byte, each goes to a different segment
                * Font is Big-Endian, Segment is Little-Endian
                */
                {
                    if (word & MSBITS[seg % 8]) // Font bit is set in this bit position
                    {
                        *(pixel + seg) |= (1 << bit_position); // Set bit
                    }
                }
                break;
            }
        }
    }
    bm.bytes_per_row += char_desc.width + m_font->c; // Increment pointer to next char
} // RasterChar
