#include "StdAfx.h"
#include "Phylix_TextureUtil.h"

#include <png.h>
#include <pnginfo.h>


namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
bool createTextureFromPngFile(const char* fileName, TextureInfo& textureInfo)
{
	/* open file */
	FILE* fp = fopen(fileName, "rb");
	if(fp==0) return false;

	/* check png file head */
	char png_header[8]={0};
	fread(png_header, 1, 8, fp);
    if(png_sig_cmp((png_bytep)png_header, 0, 8))
    {
        fclose(fp); return false;
    }
	
	/* initialize stuff */
	png_structp  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr==0)
	{
		fclose(fp); return false;
	}

    png_infop info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr==0)
	{
		fclose(fp); return false;
	}

	/* error handler */
	if(setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        fclose(fp); return false;
    }

	/* I/O initialisation methods */
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);  //Required!!!

	/* **************************************************
     * The high-level read interface in libpng (http://www.libpng.org/pub/png/libpng-1.2.5-manual.html)
     * **************************************************
     */
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;

	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);

	//check type
	if(bit_depth!=8 || (color_type!=PNG_COLOR_TYPE_RGB && color_type!=PNG_COLOR_TYPE_RGB_ALPHA))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp); return false;
	}

	bool alpha = (color_type==PNG_COLOR_TYPE_RGB_ALPHA);
	int pixel_size = alpha?4:3;

	unsigned char* rgb_buf = new unsigned char[width * height * pixel_size];  //each pixel(RGBA) has 4 bytes
	png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);


	/* unlike store the pixel data from top-left corner, store them from bottom-left corner for OGLES Texture drawing... */
	int pos = (width * height * pixel_size) - (pixel_size * width);
	for(png_uint_32 row = 0; row < height; row++)
	{
		for(png_uint_32 col = 0; col < (pixel_size * width); col += pixel_size)
		{
			rgb_buf[pos++] = row_pointers[row][col];		// red
			rgb_buf[pos++] = row_pointers[row][col + 1];	// green
			rgb_buf[pos++] = row_pointers[row][col + 2];	// blue

			if(alpha) rgb_buf[pos++] = row_pointers[row][col + 3];	// alpha
		}
		pos=(pos - (width * pixel_size)*2); //move the pointer back two rows
	}

	/** create opengl texture */
	glGenTextures(1, &(textureInfo.textureID));
	glBindTexture(GL_TEXTURE_2D, textureInfo.textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, rgb_buf);
	glTexParameteri(GL_TEXTURE_2D ,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	textureInfo.height = height;
	textureInfo.width = width;
	textureInfo.alpha = alpha;

	//clean up after the read, and free any memory allocated
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	delete[] rgb_buf;
	fclose(fp);

	return true;
}


}
