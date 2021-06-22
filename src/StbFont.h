#ifndef STB_FONT
#define STB_FONT

#include "stb_truetype.h"
#include <iostream>
#include <memory>
#include <cstring>

struct PackedCharactor
{
    unsigned char * bitmap;
    int bw, bh;
    float x0, y0, s0, t0, x1, y1, s1, t1;
    ~PackedCharactor()
    {
	delete [] bitmap;
	// bitmap = nullptr;
    }
};

class StbFont
{
public:
    StbFont(const char * fontfile);	// ttf font file path
    StbFont(const StbFont& o);

    ~StbFont();

    std::shared_ptr<PackedCharactor> getPackedCharactor(wchar_t c, int fontSize, float *x = nullptr, float *y = nullptr);

    float getCharactorWidth(wchar_t c, int fontSize);

    float getTextWidth(std::wstring text, int fontSize);

    float getFontHeight(int fontSize);

private:
    const char * filepath;
    long int filesize;
    unsigned char * ttf_buffer;
    stbtt_fontinfo font;
};

#endif
