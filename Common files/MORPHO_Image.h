/************************************************************/
/**
 * @file MORPHO_Image.h
 * @brief Definitions of the C_MORPHO_Image class
 *
 * Copyright &copy; 2011, Morpho
 *
 * - PROJECT     : MorphoSmart&tm;
 * - MODULE      : Interfaces
 *
 */

#ifndef MORPHO_IMAGE_H
#define MORPHO_IMAGE_H

/**
 * @brief Helper class to display images from the device.
 * This class displays the images received from MorphoSmart&tm;.
 */
class C_MORPHO_Image
{
public:
	// Constants
	#define	MORPHO_LINEAR_INTERPOLATION		0x0001

	/**
	 * Constructor.
	 * @param[in] i_ul_ImageNbIndex Number of different images to manage.
	 */
	C_MORPHO_Image(UL	i_ul_ImageNbIndex);

	/**
	 * Copy constructor.
	 * @param[in] i_px_Image source object
	 */
	C_MORPHO_Image(const C_MORPHO_Image& i_px_Image);

	/**
	 * Default destructor
	 */
	virtual ~C_MORPHO_Image();

	/**
	 * Assignation operator.
	 * @param[in] i_px_Image source object
	 */
	C_MORPHO_Image& operator= (const C_MORPHO_Image& i_px_Image);

	/**
	 * Save and display a new image. This method should be called each time a new image
	 * is received thanks to MORPHO_CALLBACK_IMAGE_CMD or MORPHO_CALLBACK_LAST_IMAGE_FULL_RES_CMD
	 * asynchronous events.
	 *
	 * @param[in] i_ul_ImageIndex
	 * - Image index to retrieve and display (first index is index 0).
	 * @param[in] i_x_Image
	 * - Image header and image.
	 * @param[in] i_ul_imageFilter
	 * - Filter applied on the image. Possibles values are:
	 *    - 	0: no specific treatment (better image with a small display)
	 *    - 	MORPHO_LINEAR_INTERPOLATION: linear interpolation (better image with a big display).
	 * @param[in] i_p_Cdc
	 * - Device context object (class is defined in Microsoft MFC) to display the image. A device context is allocated with MFC method CWnd::GetDC() and released with MFC CWnd::ReleaseDC(). For more details, see the given examples.
	 * @param[in] i_p_Rect
	 * - Size of the rectangle to display the image.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_BADPARAMETER	Bad value of i_ul_ImageIndex (maximum value defined in the constructor).
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I SetImage(	UL								i_ul_ImageIndex,
				T_MORPHO_CALLBACK_IMAGE_STATUS	i_x_Image,
				UL								i_ul_imageFilter,
				CDC								*i_p_Cdc,
				CRect &							i_p_Rect);

	/**
	 * Display an image already saved. This method should be called each time a refresh is required.
	 * We recommend to use this method with the OnPaint() Visual C++ function.
	 * Fore more details, see the given examples.
	 *
	 * @param[in] i_ul_ImageIndex
	 * - Image index to refresh (first index is index 0).
	 * @param[in] i_p_Rect
	 * - Size of the rectangle to display the image.
	 * @param[in] i_p_Cdc
	 * - Device context object (class is defined in Microsoft MFC) to display the image.
	 * A device context is allocated with MFC method CWnd::GetDC() and released with
	 * MFC CWnd::ReleaseDC(). For more details, see the given examples.
	 *
	 * @retval #MORPHO_OK	The execution of the function was successful.
	 * @retval #MORPHOERR_BADPARAMETER	Bad value of i_ul_ImageIndex (maximum value defined in the constructor).
	 * @retval #MORPHOERR_CORRUPTED_CLASS	Class has been corrupted.
	 */
	I RefreshViewer(	UL		i_ul_ImageIndex,
					CRect &	i_p_Rect,
					CDC*	i_p_Cdc);

private:

#ifdef _WIN32_WCE
	I MonochromeArrayToDIB(	UL	   i_ul_ImageIndex,
							UL	   i_ul_ImageRow,
							UL	   i_ul_ImageCol,
							PUC    i_puc_Image,
							CDC	  *i_p_Cdc,
							CRect &i_p_Rect);
#else
	I MonochromeArrayToDIB(	UL	i_ul_ImageIndex,
							UL	i_ul_ImageRow,
							UL	i_ul_ImageCol,
							PUC i_puc_Image,
							CDC	*i_p_Cdc);
#endif

	VOID ZoomImageBilineaire(	PUC entree, PUC sortie, UC background,
								L larg_entree, L haut_entree, L reso_entree,
								L larg_sortie, L haut_sortie, L reso_sortie);

private:
	typedef struct
	{
		HBITMAP	*m_pBitmap;
		UL		m_ul_ImageNbIndex;
		PUI		m_pui_image8bits;
		CStatic	*m_p_Static_Viewer;
	} T_MORPHO_IMAGE,*PT_MORPHO_IMAGE;

	PT_MORPHO_IMAGE	m_px_MorphoImage;
};

#endif
