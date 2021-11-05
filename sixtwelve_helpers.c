
#include "sixtwelve_helpers.h"

const sixtwelve_character_info *sixtwelve_get_character_info(unsigned char iso_8859_code) {
    return sixtwelve_characters + iso_8859_code;
}


const unsigned char ISO_8859_SPACE = 0x20;
const unsigned char ISO_8859_NEWLINE = 0x0A;
const unsigned char ISO_8859_HYPHEN = 0x2D;


unsigned int sixtwelve_calculate_string_crass_width(const unsigned char* str) {
	if (!str) {
		return 0;
	}

	int resultingWidth = 0;
	int i = 0;
	while (str[i] != '\0') {
		const sixtwelve_character_info* characterInfo = sixtwelve_get_character_info(str[i]);
		resultingWidth += (characterInfo->width + characterInfo->x_advance);
		i++;
	}

	return resultingWidth;
}

unsigned int sixtwelve_calculate_string_width(const unsigned char* str) {
	if (!str) {
		return 0;
	}

	int resultingWidth = 0;
	int i = 0;
	while (str[i] != '\0') {
		// Stop measuring if we hit a space
		if (str[i] == ISO_8859_SPACE) {
			break;
		}

		// Stop measuring if we hit a newline
		if (str[i] == ISO_8859_NEWLINE) {
			break;
		}

		// Stop measuring if we hit a hypen
		if (str[i] == ISO_8859_HYPHEN) {
			break;
		}

		const sixtwelve_character_info* characterInfo = sixtwelve_get_character_info(str[i]);
		resultingWidth += (characterInfo->width + characterInfo->x_advance);
		i++;
	}

	return resultingWidth;
}