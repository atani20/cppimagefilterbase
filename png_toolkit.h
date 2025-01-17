#ifndef PNG_TOOLKIT_H
#define PNG_TOOLKIT_H

#include <string>
#include <map>
#include "stb_image.h"
#include "rect.h"

struct image_data
{
    stbi_uc *pixels;
    int w, h;
    int compPerPixel;
};

class png_toolkit
{
public:
    enum class Error {
        WrongSize,
        WrongFormat,
        Ok
    };
    png_toolkit();
    ~png_toolkit();
    bool load( std::string const &pictureName );
    bool save( std::string const &pictureName );
    image_data getPixelData( void ) const;
	int getWidth();
	int getHeigth();
private:
    image_data imgData;
};

#endif // PNG_TOOLKIT_H
