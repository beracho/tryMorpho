#if !defined(__ConvertRawToBmp__h)
#define __ConvertRawToBmp__h

class ConvertRawToBmp  
{
public :	
	ConvertRawToBmp();
    int	SaveImageBMP(CString i_cs_Filename,unsigned char* i_puc_Image, int i_i_Lignes,int i_i_Colonnes, unsigned short i_us_Xres, unsigned short i_us_Yres);
	
	struct BITMAPHEADER
	{
         BITMAPINFOHEADER    bmiHeader;
         RGBQUAD             bmiColors[256];
	} m_InfoBitmap;

	virtual	~ConvertRawToBmp ();	
	
	//unsigned char*	m_puc_Image;

//private:
//	CWnd*			m_pParentWnd;
//	LPBITMAPINFO	m_pBitmapInfo;

};

#endif