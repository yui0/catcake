/*
    Copyright ©2013 Yuichiro Nakada

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/


extern "C"
{
#include <stdio.h>
#include <jpeglib.h>
}

#include "ck_low_level_api.h"

#include "ck_mem_all.h" // for ckMalloc, ckFree


// dummy funcs to help libjpeg
static void JPEGInitSource(j_decompress_ptr cinfo)
{
}


static boolean JPEGFillInputBuffer(j_decompress_ptr cinfo)
{
	return 0;
}


static void JPEGSkipInputData(j_decompress_ptr cinfo, long num_bytes)
{
	cinfo->src->next_input_byte += num_bytes;
	cinfo->src->bytes_in_buffer -= num_bytes;
}


static void JPEGTermSource(j_decompress_ptr cinfo)
{
}


static bool readJpeg(u16* width, u16* height, ckLowLevelAPI::TextureFormat* format,
	void* buf, size_t buf_size, size_t buf_line_size, const void* data, u32 data_size)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	jpeg_source_mgr srcmgr;

	srcmgr.bytes_in_buffer = data_size;
	srcmgr.next_input_byte = (JOCTET*) data;
	srcmgr.init_source = JPEGInitSource;
	srcmgr.fill_input_buffer = JPEGFillInputBuffer;
	srcmgr.skip_input_data = JPEGSkipInputData;
	srcmgr.resync_to_restart = jpeg_resync_to_restart;
	srcmgr.term_source = JPEGTermSource;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	//jpeg_stdio_src(&cinfo, arq);
	cinfo.src = &srcmgr;
	jpeg_read_header(&cinfo, TRUE);

	if (width) {
		*width = static_cast<u16>(cinfo.image_width);
	}

	if (height) {
		*height = static_cast<u16>(cinfo.image_height);
	}

	if (format) {
		*format = ckLowLevelAPI::FORMAT_RGB;
		//*format = ckLowLevelAPI::FORMAT_RGBA;
		//*format = ckLowLevelAPI::FORMAT_ALPHA;
	}
	//printf("%dx%d %d\n", cinfo.image_width, cinfo.image_height, buf_size);

	// 画像を読む?
	if (buf) {
		if (cinfo.image_width * cinfo.image_height * 4 > buf_size) {
			jpeg_destroy_decompress(&cinfo);
			return false;
		}

		jpeg_start_decompress(&cinfo);

		char *p = (char*)buf;
		int row_stride = cinfo.output_width * cinfo.output_components;
		//printf("%d %d\n", cinfo.output_width, cinfo.output_components);
		char *line = (char*)ckMalloc(static_cast<u32>(cinfo.output_width * cinfo.output_components));

		while (cinfo.output_scanline < cinfo.output_height) {
			jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&line, 1);

			/*for(int  i = 0; i < row_stride; i+=3 ) {
				*p++ = line[i];
				*p++ = line[i+1];
				*p++ = line[i+2];
			}*/
			ckMemMgr::memcpy(p, line, row_stride);
			//p += row_stride;	// 画像の幅(Bytes)
			p += buf_line_size;	// テクスチャの幅(Bytes)
		}
		jpeg_finish_decompress(&cinfo);
		ckFree(line);
	}

	jpeg_destroy_decompress(&cinfo);

	return true;
}


bool ckLowLevelAPI::readJpegInfo(u16* width, u16* height, TextureFormat* format, const void* data, u32 data_size)
{
	return readJpeg(width, height, format, NULL, 0, 0, data, data_size);
}


bool ckLowLevelAPI::readJpegImage(void* buf, u32 buf_size, u32 buf_line_size, const void* data, u32 data_size)
{
	return readJpeg(NULL, NULL, NULL, buf, buf_size, buf_line_size, data, data_size);
}
