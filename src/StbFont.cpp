#include "StbFont.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"


StbFont::StbFont(const char * fontfile)
    :filepath(fontfile),
     ttf_buffer(NULL)
{
    FILE *file = fopen(fontfile, "rb");
    if(!file)
    {
	std::cout << "Error::StbFont can not load font file: " << fontfile << std::endl;
	return;
    }

    // get file size(in bytes)
    fseek(file, 0, SEEK_END);
    long int filesize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    this->ttf_buffer = new unsigned char[filesize];
    fread(this->ttf_buffer, 1, filesize, file);
    fclose(file);
    stbtt_InitFont(&this->font, this->ttf_buffer, 0);
}

PackedCharactor& StbFont::getPackedCharactor(wchar_t c, int fontSize, float *x, float *y)
{
    static stbtt_pack_context pc;
    static stbtt_packedchar pdata[1];

    int bw =  fontSize * 2, bh = fontSize * 2;
    unsigned char *atlas = new unsigned char[bw * bh];

    stbtt_PackBegin(&pc, atlas, bw, bh, 0, 0, NULL);
    stbtt_PackSetOversampling(&pc, 2, 2);
    stbtt_PackFontRange(&pc, ttf_buffer, 0, fontSize, c, 1, pdata);
    stbtt_PackEnd(&pc);

    float dx = 0, dy = fontSize;
    if(x == NULL) x = &dx;
    if(y == NULL) y = &dy;

    // float scale = stbtt_ScaleForPixelHeight(&font, fontSize);
    // int ascent, decent, lineGap;
    // stbtt_GetFontVMetrics(&font, &ascent, &decent, &lineGap);
    // std::cout << fontSize * scale << " - "
    // 	      << "ascent: " << ascent * scale << " - "
    // 	      << "decent: " << decent * scale << " - "
    // 	      << "lineGap: " << lineGap * scale << " - "
    // 	      << "vsum: " << (ascent + decent + lineGap) * scale << " - "
    // 	      << std::endl;
    
    static stbtt_aligned_quad q;
    stbtt_GetPackedQuad(pdata, bw, bh, 0, x, y, &q, 1);
    
    PackedCharactor *pchar = new PackedCharactor();
    pchar->bimap = atlas;
    pchar->bw = bw; pchar->bh = bh;
    pchar->x0 = q.x0; pchar->s0 = q.s0;
    pchar->y0 = q.y0; pchar->t0 = q.t0;
    pchar->x1 = q.x1; pchar->s1 = q.s1;
    pchar->y1 = q.y1; pchar->t1 = q.t1;

    return *pchar;
}

float StbFont::getCharactorWidth(wchar_t c, int fontSize)
{
    float scale = stbtt_ScaleForPixelHeight(&this->font, (float)fontSize);
    int advance, lsb;
    stbtt_GetCodepointHMetrics(&this->font, c, &advance, &lsb);
    // std::cout << "advance: " << advance * scale << ", "
    // 	      << "lsb: " << lsb * scale << std::endl;
    return advance * scale;
}

float StbFont::getTextWidth(std::wstring text, int fontSize)
{
    float width = 0, kern = 0;
    float scale = stbtt_ScaleForPixelHeight(&this->font, (float)fontSize);
    for(size_t i = 0; i < text.length(); i++)
    {
	wchar_t c = text[i];
	int advance, lsb;
	stbtt_GetCodepointHMetrics(&this->font, c, &advance, &lsb);
	width += scale * advance;
	if(i < text.length() - 1)
	    kern += scale * stbtt_GetCodepointKernAdvance(&this->font, c, text[i + 1]);
    }
    // std::cout << "width: " << width << ", "
    // 	      << "kern: " << kern << std::endl;
    return width + kern;
}

float StbFont::getFontHeight(int fontSize)
{
    float scale = stbtt_ScaleForPixelHeight(&this->font, (float)fontSize);
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&this->font, &ascent, &descent, &lineGap);
    int baseline = ascent * scale;
    return baseline;
}

StbFont::~StbFont()
{
    // delete this->ttf_buffer;
}
