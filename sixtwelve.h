
#ifndef _SIXTWELVE_H
#define _SIXTWELVE_H

#define SIXTWELVE_TEXTURE_WIDTH 128
#define SIXTWELVE_TEXTURE_HEIGHT 64

#define SIXTWELVE_LINE_HEIGHT 12

/**
 * @brief IA4 texture representation of the bitmap font.
 *
 * You will likely want to load this into TMEM with `gDPLoadTextureBlock_4b`.
 * 
 * See `SIXTWELVE_TEXTURE_WIDTH` and `SIXTWELVE_TEXTURE_HEIGHT` for texture
 * dimensions.
 */
extern unsigned char sixtwelve_tex[] __attribute__((aligned(8)));

/**
 * @brief The underlying size of `sixtwelve_tex`, in bytes.
 */
extern unsigned int sixtwelve_tex_len;

/*
 * @brief Display for a particular ISO/IEC 8859-1 character used by the font.
 */
typedef struct {
    /** @brief The 8-bit ISO/IEC 8859-1 codepoint for the character. */
    unsigned char id;

    /** @brief The horizontal texture location of the character in pixels.  */
    unsigned char x;

    /** @brief The vertical texture location of the character in pixels.*/
    unsigned char y;

    /** @brief The width of the character in pixels. */
    unsigned char width;

    /** @brief The height of the character in pixels. */
    unsigned char height;

    /** @brief The horizontal offset the character should have when rendering. */
    signed char x_offset;

    /** @brief The vertical offset the character should have when rendering. */
    signed char y_offset;

    /** @brief How far ahead the next character should be horizontally. */
    unsigned char x_advance;
} sixtwelve_character_info;

/*
 * @brief The display information for the characters in the font.
 *
 * To fit into one texture, not all of the ISO/IEC 8859-1 codepoints are
 * supported. Unsupported characters will render a `?`.
 */
extern const sixtwelve_character_info sixtwelve_characters[256] __attribute__ ((aligned (16)));




#endif /* _SIXTWELVE_H */ 