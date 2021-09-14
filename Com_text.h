//---------------------------------------------------------------------------

#ifndef Com_textH
#define Com_textH
#include <gl\mywingl.h>
#include <system.hpp>

#define MAX_COM_TEX_WIDTH  1024
#define MAX_COM_TEX_HEIGHT 1024

#define BMP_TEX  1
#define TGA_TEX  2
#define JPG_TEX  3
#define NONE_TEX 0

class _Common_texture{
protected:

        bool _inited;
        unsigned int _GL_Tex;
        unsigned int format;

        bool DoMipMap;

        AnsiString FileName;

        bool Init_TGA();
        bool Init_BMP();
        bool Init_JPG();

        bool _Scale_Bits(BYTE *bits,unsigned short Width,unsigned short Height,GLint iComponents,GLenum eFormat);

        int Get_Format(char *_filename);

public:
        bool Tex_Init(char *_filename,bool _DoMipMap);
        void Tex_Destroy();

        void BindIt();
        void BindAndUpdate();

        _Common_texture();
        _Common_texture(char *_filename,bool _DoMipMap);
        ~_Common_texture();

};

//---------------------------------------------------------------------------
#endif
 