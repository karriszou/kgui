#ifndef STB_FONT
#define STB_FONT

#include <iostream>
#include "stb_truetype.h"

struct PackedCharactor
{
    unsigned char * bimap;
    int bw, bh;
    float x0, y0, s0, t0, x1, y1, s1, t1;
};

class StbFont
{
public:
    StbFont(const char * fontfile);	// ttf font file path
    ~StbFont();

    PackedCharactor& getPackedCharactor(wchar_t c, int fontSize, float *x = NULL, float *y = NULL);

    float getCharactorWidth(wchar_t c, int fontSize);

    float getTextWidth(std::wstring text, int fontSize);

    float getFontHeight(int fontSize);

private:
    const char * filepath;
    unsigned char * ttf_buffer;
    stbtt_fontinfo font;
};

#endif
