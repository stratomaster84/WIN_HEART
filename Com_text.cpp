//---------------------------------------------------------------------------

#pragma hdrstop

#include "Com_text.h"
#include "jpeg.hpp"


#pragma package(smart_init)
#pragma pack(1)
typedef struct
    {
    GLbyte	identsize;              // Size of ID field that follows header (0)
    GLbyte	colorMapType;           // 0 = None, 1 = paletted
    GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
    unsigned short	colorMapStart;          // First colour map entry
    unsigned short	colorMapLength;         // Number of colors
    unsigned char 	colorMapBits;   // bits per palette entry
    unsigned short	xstart;                 // image x origin
    unsigned short	ystart;                 // image y origin
    unsigned short	width;                  // width in pixels
    unsigned short	height;                 // height in pixels
    GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
    GLbyte	descriptor;             // image descriptor
    } TGAHEADER;
#pragma pack(8)

//---------------------------------------------------------------------------


bool _Common_texture::Init_TGA(){

        int pFile;			// File pointer
        TGAHEADER tgaHeader;		// TGA file header
        long lImageSize;		// Size in bytes of image
        short sDepth;			// Pixel depth;
        GLbyte	*pBits = NULL;          // Pointer to bits

        GLint iWidth;
        GLint iHeight;
        GLenum eFormat = GL_BGR_EXT;
        GLint iComponents = GL_RGB8;

        if((pFile = FileOpen(FileName,fmOpenRead)) == -1)
                return false;
        if(FileRead(pFile,&tgaHeader, sizeof(TGAHEADER)) != sizeof(TGAHEADER)){
                FileClose(pFile);
                return false;
        }

        iWidth = tgaHeader.width;
        iHeight = tgaHeader.height;
        sDepth = tgaHeader.bits >>3;

        if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32){
                FileClose(pFile);
                return false;
        }

        lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
        if(lImageSize<2){
                FileClose(pFile);
                return false;
        }
        pBits = new GLbyte [lImageSize];
        if(FileRead(pFile,pBits, lImageSize) != lImageSize){
                FileClose(pFile);
                delete [] pBits;
                return false;
        }

        switch(sDepth)
        {
                case 3:
                        eFormat = GL_BGR_EXT;
                        iComponents = GL_RGB8;
                        break;
                case 4:
                        eFormat = GL_BGRA_EXT;
                        iComponents = GL_RGBA8;
                        break;
                case 1:
                        eFormat = GL_LUMINANCE;
                        iComponents = GL_LUMINANCE8;
                        break;
        };

        FileClose(pFile);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        _Scale_Bits(pBits,iWidth, iHeight,iComponents,eFormat);

        return true;
}


bool _Common_texture::Init_BMP(){
        BYTE *bits;
        Graphics::TBitmap *_image1;
        int i,j;

        _image1 = new Graphics::TBitmap;
        _image1->LoadFromFile(FileName);
        if(_image1 == NULL)
                return false;

        GLint iWidth = _image1->Width;
        GLint iHeight = _image1->Height;
        bits = new BYTE [iWidth*iHeight*3];

        for(i = 0; i < iHeight; i++){
    	        for(j = 0; j < iWidth; j++){
                        bits[((iHeight-i-1)*iWidth + j)*3]= (GLbyte)GetRValue(_image1->Canvas->Pixels[j][i]);
                        bits[((iHeight-i-1)*iWidth + j)*3 +1]= (GLbyte)GetGValue(_image1->Canvas->Pixels[j][i]);
                        bits[((iHeight-i-1)*iWidth + j)*3 +2]= (GLbyte)GetBValue(_image1->Canvas->Pixels[j][i]);
                }
        }
        delete _image1;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        _Scale_Bits(bits,iWidth,iHeight,GL_RGB,GL_RGB);

        return true;
}
bool _Common_texture::Init_JPG(){
        BYTE *bits;
        Graphics::TBitmap *_image1;
        TJPEGImage * _image;
        int i,j;

        _image = new TJPEGImage;

        _image->LoadFromFile(FileName);
        if(_image == NULL)
                return false;
        _image1 = new Graphics::TBitmap;
        _image1->Assign(_image);
        delete _image;
        if(_image1 == NULL)
                return false;

        GLint iWidth = _image1->Width;
        GLint iHeight = _image1->Height;
        bits = new BYTE [iWidth*iHeight*3];

        for(i = 0; i < iHeight; i++){
    	        for(j = 0; j < iWidth; j++){
                        bits[((iHeight-i-1)*iWidth + j)*3]= (GLbyte)GetRValue(_image1->Canvas->Pixels[j][i]);
                        bits[((iHeight-i-1)*iWidth + j)*3 +1]= (GLbyte)GetGValue(_image1->Canvas->Pixels[j][i]);
                        bits[((iHeight-i-1)*iWidth + j)*3 +2]= (GLbyte)GetBValue(_image1->Canvas->Pixels[j][i]);
                }
        }
        delete _image1;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        _Scale_Bits(bits,iWidth,iHeight,GL_RGB,GL_RGB);

        return true;
}

bool _Common_texture::Tex_Init(char *_filename,bool _DoMipMap){
        if(_inited)
                return false;
        if((format = Get_Format(_filename))== NONE_TEX)
                return false;
        FileName = (AnsiString)_filename;
        _inited = true;
        glGenTextures(1,&_GL_Tex);
        DoMipMap = _DoMipMap;
        BindAndUpdate();
        return _inited;

}
int _Common_texture::Get_Format(char *_filename){

        char ext[4];
        unsigned int len;
        len = StrLen(_filename);
        if(len == 0 || _filename[len-4] != '.')
                return NONE_TEX;
        ext[0] = _filename[len-3];
        ext[1] = _filename[len-2];
        ext[2] = _filename[len-1];
        ext[3] = '\0';
        if(StrIComp(ext,"bmp") == 0)
                return BMP_TEX;
        else if(StrIComp(ext,"tga") == 0)
                return TGA_TEX;
        else if(StrIComp(ext,"jpg") == 0)
                return JPG_TEX;
                
        return NONE_TEX;
}

void _Common_texture::Tex_Destroy(){
        if(!_inited)
                return;
        _inited = false;
        glDeleteTextures(1,&_GL_Tex);

}
void _Common_texture::BindIt(){
        if(!_inited)
                return;
        glBindTexture(GL_TEXTURE_2D,_GL_Tex);

}
_Common_texture::_Common_texture(){
        _inited = false;
}
_Common_texture::_Common_texture(char *_filename,bool _DoMipMap){
        _inited = false;
        Tex_Init(_filename,_DoMipMap);
}
_Common_texture::~_Common_texture(){
        Tex_Destroy();
}
void _Common_texture::BindAndUpdate(){
        if(!_inited)
                return;
        glBindTexture(GL_TEXTURE_2D,_GL_Tex);
        bool _temp=false;
        switch(format){
                case BMP_TEX:
                        _temp = Init_BMP();
                        break;
                case TGA_TEX:
                        _temp = Init_TGA();
                        break;
                case JPG_TEX:
                        _temp = Init_JPG();
                        break;
        };
        if(!_temp)
                Tex_Destroy();
}


bool _Common_texture::_Scale_Bits(BYTE *bits,unsigned short Width,unsigned short Height,GLint iComponents,GLenum eFormat){

        BYTE *scale_bits;
        unsigned short _wid=Width,_heig=Height;
        if(_wid > MAX_COM_TEX_WIDTH)
                _wid = MAX_COM_TEX_WIDTH;
        else __asm{
                mov ax,_wid
                xor cl,cl
                label1:
                        shr ax,1
                        test ax,ax
                        jne _next1
                        mov ax,1
                        shl ax,cl
                        mov _wid,ax
                        jmp short _end1
                _next1:
                        inc cl
                        jmp short label1
                _end1:
        }
        if(_heig > MAX_COM_TEX_HEIGHT)
                _heig = MAX_COM_TEX_HEIGHT;
        else __asm{
                mov ax,_heig
                xor cl,cl
                label2:
                        shr ax,1
                        test ax,ax
                        jne _next2
                        mov ax,1
                        shl ax,cl
                        mov _heig,ax
                        jmp short _end2
                _next2:
                        inc cl
                        jmp short label2
                _end2:
        }
        if(_wid == Width && _heig == Height){
                if(!DoMipMap)
                        glTexImage2D(GL_TEXTURE_2D, 0, iComponents, Width, Height, 0, eFormat, GL_UNSIGNED_BYTE, bits);
                else
                        gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, Width, Height, eFormat, GL_UNSIGNED_BYTE, bits);
                delete [] bits;
                return false;
        }
        if(_wid <2)
                _wid = 2;
        if(_heig<2)
                _heig = 2;
        if(eFormat == GL_RGB || eFormat == GL_BGR_EXT)
                scale_bits = new BYTE [_wid*_heig*3];
        else if(eFormat == GL_RGBA || eFormat == GL_BGRA_EXT)
                scale_bits = new BYTE [_wid*_heig*4];
        else if(eFormat == GL_LUMINANCE)
                scale_bits = new BYTE [_wid*_heig];
        else
                scale_bits = new BYTE [_wid*_heig*4];
        gluScaleImage(eFormat,Width,Height,GL_UNSIGNED_BYTE,bits,_wid,_heig,GL_UNSIGNED_BYTE,scale_bits);
        delete [] bits;
        if(!DoMipMap)
                glTexImage2D(GL_TEXTURE_2D, 0, iComponents, _wid, _heig, 0, eFormat, GL_UNSIGNED_BYTE, scale_bits);
        else
                gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, _wid, _heig, eFormat, GL_UNSIGNED_BYTE, scale_bits);

        delete [] scale_bits;
        return true;
}

