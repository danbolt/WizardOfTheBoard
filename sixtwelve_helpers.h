#ifndef _SIXTWELVE_HELPERS_H
#define _SIXTWELVE_HELPERS_H

#include "sixtwelve.h"

/*
 * @brief Get character information for a specific ISO/IEC 8859-1 codepoint.
 *
 * This function is not necessary, but a programmer may appreciate having a
 * seperate function available for debug builds.
 *
 */
const sixtwelve_character_info *sixtwelve_get_character_info(unsigned char iso_8859_code);

unsigned int sixtwelve_calculate_string_width(const unsigned char* str);

unsigned int sixtwelve_calculate_string_crass_width(const unsigned char* str);

#endif /* _SIXTWELVE_H */ 
