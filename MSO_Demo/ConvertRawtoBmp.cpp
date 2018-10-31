#include <string.h>
#include "stdafx.h"
#include "ConvertRawToBmp.h"
#include "Morpho_Types.h"

ConvertRawToBmp::ConvertRawToBmp()
{
}

ConvertRawToBmp::~ConvertRawToBmp()
{
}

int ConvertRawToBmp:: SaveImageBMP(CString i_cs_Filename, PUC i_puc_Image , int i_i_Lignes,int i_i_Colonnes, US i_us_Xres, US i_us_Yres)
{
	CFile l_f_File;
	BITMAPFILEHEADER	l_FileHeader;
	BITMAPHEADER		l_InfoBitmap;

	I	l_i_SrcPos, l_i_DstPos;
	I	l_i_Size = i_i_Colonnes * i_i_Lignes;
	I	i;

	PUC l_puc_TempImage = new UC[l_i_Size]; // We must create a local temp image since
	                                        // l_puc_TempImage is not thread safe

	// In m_InfoBitmap, we needed to have biHeight<0 (see CMSOPicture::CMSOPicture)
	// to display correctly the picture on the screen. However, in the .bmp  file,
	// biHeight must be > 0. 
	// So we invert the order of the rows in the image buffer.

	//Init RGBQUAD Palette monochrome de 256 niveaux
	for(i = 0; i < 256; i++)
    {
		l_InfoBitmap.bmiColors[i].rgbBlue = (UC)i; 
        l_InfoBitmap.bmiColors[i].rgbGreen = (UC)i; 
        l_InfoBitmap.bmiColors[i].rgbRed = (UC)i; 
        l_InfoBitmap.bmiColors[i].rgbReserved = 0; 
    }

	//Init BITMAPINFOHEADER    
    l_InfoBitmap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    l_InfoBitmap.bmiHeader.biWidth =  i_i_Colonnes;
    l_InfoBitmap.bmiHeader.biHeight = -(L)i_i_Lignes;
								// the minus sign imply that the bitmap is a top-down DIB 
								// and its origin is the upper left corner

    l_InfoBitmap.bmiHeader.biPlanes = 1;
    l_InfoBitmap.bmiHeader.biBitCount = 8;
    l_InfoBitmap.bmiHeader.biCompression = BI_RGB;
    l_InfoBitmap.bmiHeader.biSizeImage = 0;

    l_InfoBitmap.bmiHeader.biXPelsPerMeter = (int)(10000 * i_us_Xres / 254);	// conversion inch<->meter
    l_InfoBitmap.bmiHeader.biYPelsPerMeter = (int)(10000 * i_us_Yres / 254);
    l_InfoBitmap.bmiHeader.biClrUsed = 0;
    l_InfoBitmap.bmiHeader.biClrImportant = 0;

	l_InfoBitmap.bmiHeader.biHeight = abs(l_InfoBitmap.bmiHeader.biHeight);
	l_InfoBitmap.bmiHeader.biWidth = abs(l_InfoBitmap.bmiHeader.biWidth);

	for (l_i_SrcPos = 0, l_i_DstPos = l_i_Size-i_i_Colonnes; l_i_SrcPos<l_i_Size; l_i_SrcPos += i_i_Colonnes, l_i_DstPos -= i_i_Colonnes)
	{
		memcpy(l_puc_TempImage+l_i_DstPos, i_puc_Image+l_i_SrcPos, i_i_Colonnes);
	}

	l_FileHeader.bfType = 0x4d42;   // 'BM' WINDOWS_BITMAP_SIGNATURE
	l_FileHeader.bfSize = i_i_Colonnes*i_i_Lignes + sizeof(l_InfoBitmap) + sizeof(l_FileHeader);
	l_FileHeader.bfReserved1 = 0;
	l_FileHeader.bfReserved2 = 0;
	l_FileHeader.bfOffBits = sizeof(l_InfoBitmap) + sizeof(l_FileHeader);

  CFileException ex;

	if (l_f_File.Open(i_cs_Filename, CFile::modeCreate | CFile::modeWrite|CFile::modeNoTruncate, &ex))
	{
		l_f_File.Write(&l_FileHeader, sizeof(l_FileHeader));
		l_f_File.Write(&l_InfoBitmap, sizeof(l_InfoBitmap));
		l_f_File.Write(l_puc_TempImage, i_i_Colonnes*i_i_Lignes);
		l_f_File.Close();

		delete[] l_puc_TempImage;
		return 0;
	}
	else
	{
         TCHAR szError[1024];
         ex.GetErrorMessage(szError, 1024);
		 AfxMessageBox ( szError );

 		delete[] l_puc_TempImage;
		return -1;
	}
}