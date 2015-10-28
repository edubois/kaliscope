/* Copyright (C) 2015 Eloi DU BOIS - All Rights Reserved
 * The license for this file is available here:
 * https://github.com/edubois/kaliscope/blob/master/LICENSE
 */

#include "TinyDisplayProjector.hpp"
#include <cassert>
#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h> 
#include <unistd.h>
#include <cstring>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

// default framebuffer palette
typedef enum {
  BLACK        =  0, /*   0,   0,   0 */
  BLUE         =  1, /*   0,   0, 172 */
  GREEN        =  2, /*   0, 172,   0 */
  CYAN         =  3, /*   0, 172, 172 */
  RED          =  4, /* 172,   0,   0 */
  PURPLE       =  5, /* 172,   0, 172 */
  ORANGE       =  6, /* 172,  84,   0 */
  LTGREY       =  7, /* 172, 172, 172 */
  GREY         =  8, /*  84,  84,  84 */
  LIGHT_BLUE   =  9, /*  84,  84, 255 */
  LIGHT_GREEN  = 10, /*  84, 255,  84 */
  LIGHT_CYAN   = 11, /*  84, 255, 255 */
  LIGHT_RED    = 12, /* 255,  84,  84 */
  LIGHT_PURPLE = 13, /* 255,  84, 255 */
  YELLOW       = 14, /* 255, 255,  84 */
  WHITE        = 15  /* 255, 255, 255 */
} COLOR_INDEX_T;

static unsigned short def_r[] =
    { 0,   0,   0,   0, 172, 172, 172, 168,
     84,  84,  84,  84, 255, 255, 255, 255};
static unsigned short def_g[] =
    { 0,   0, 168, 168,   0,   0,  84, 168,
     84,  84, 255, 255,  84,  84, 255, 255};
static unsigned short def_b[] =
    { 0, 172,   0, 168,   0, 172,   0, 168,
     84, 255,  84, 255,  84, 255,  84, 255};


namespace kaliscope
{

TinyDisplayProjector::TinyDisplayProjector()
: IProjector( -1 )
{
   init();
}

TinyDisplayProjector::~TinyDisplayProjector()
{
   try
   {
      switchOff();
      closeFramebuffer();
   }
   catch( ... )
   {}
}
    
/**
 * @brief initialize projector
 */
void TinyDisplayProjector::init()
{
    static const char *kFbDevice = "/dev/fb1";

    if ( _fb )
    {
        closeFramebuffer();
    }

    _fb = open( kFbDevice, O_RDWR );
    if ( _fb == -1 )
    {
        std::cerr << "Failed to open fbdevice, did you installed your tiny screen correctly?" << std::endl;
        return;
    }

    if ( ioctl( _fb, FBIOGET_FSCREENINFO, &_fix ) < 0 )
    {
        std::cerr << "Failed ioctl FBIOGET_FSCREENINFO" << std::endl;
        close( _fb );
        return;
    }

    if ( ioctl( _fb, FBIOGET_VSCREENINFO, &_var ) < 0 )
    {
        std::cerr << "Failed ioctl FBIOGET_VSCREENINFO" << std::endl;
        close( _fb );
        return;
    }

    memcpy( &_origVar, &_var, sizeof( fb_var_screeninfo ) );

    // map framebuffer to user memory 
    _screensize = _fix.smem_len;
    _fbp = (char*)mmap(0, 
                _screensize, 
                PROT_READ | PROT_WRITE, 
                MAP_SHARED, 
                _fb, 0);
    if ( (int)_fbp == -1 )
    {
        std::cerr << "Failed to memory map!" << std::endl;
    }

    _width = _var.xres;
    _height = _var.yres;
}

/**
 * @brief switch projector on
 */
void TinyDisplayProjector::switchOn()
{
    // set framebuffer to white
    for( std::size_t x = 0; x < _width; ++x )
        for( std::size_t y = 0; y < _height; ++y )
            drawPixel( x, y, 255, 255, 255 );
}

/**
 * @brief switch projector off
 */
void TinyDisplayProjector::switchOff()
{
    // set framebuffer to black
    for( std::size_t x = 0; x < _width; ++x )
        for( std::size_t y = 0; y < _height; ++y )
            drawPixel( x, y, 0, 0, 0 );
}



void TinyDisplayProjector::drawPixel( const std::size_t x, const std::size_t y, int r, int g, int b)
{
    assert( _fbp != nullptr );

    unsigned int pix_offset;
    unsigned short c;

    // calculate the pixel's byte offset inside the buffer
    pix_offset = x*2 + y * _fix.line_length;

    //some magic to work out the color
    c = ((r / 8) << 11) + ((g / 4) << 5) + (b / 8);

    // write 'two bytes at once'
    *((unsigned short*)(_fbp + pix_offset)) = c;
}


void TinyDisplayProjector::drawSquare( const std::size_t x, const std::size_t y, const std::size_t height, const std::size_t width, int c )
{
    for ( std::size_t h = 0; h < height; ++h )
        for ( std::size_t w = 0; w < width; ++w )
            drawPixel( h + ( x - 2 ), w + ( y - 2 ) , def_r[ c ], def_g[ c ], def_b[ c ] );
}

void TinyDisplayProjector::closeFramebuffer()
{
    switchOff();

    munmap( _fbp, _screensize );
    if (ioctl( _fb, FBIOPUT_VSCREENINFO, &_origVar ) )
    {
        std::cerr << "Error re-setting variable information" << std::endl;
    }
    close( _fb );
}

}
