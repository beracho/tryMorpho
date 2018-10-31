#include "afxwin.h"

#if !defined(FINGERPRINTCTRL__INCLUDED_)
#define FINGERPRINTCTRL__INCLUDED_

#include "MORPHO_Interface.h"

class CMSO_DemoDlg;

class CFingerPrintCtrl : public CStatic
{
	public:

	//Constructor
	CFingerPrintCtrl(void);

	//Destructor
	~CFingerPrintCtrl(void);

	void SetImage(T_MORPHO_CALLBACK_IMAGE_STATUS i_x_image);
	void SetParent(CMSO_DemoDlg* pDemoDlg);
	void SetIndex(int index);

	protected:
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
		virtual void PreSubclassWindow();
		CMSO_DemoDlg* m_pDemoDlg;

	private:
		T_MORPHO_CALLBACK_IMAGE_STATUS * m_px_ImageStructure;
		BOOL							 m_bIsFingerPrintLoaded;
		int								 m_i_index;
};

#endif // !defined(FINGERPRINTCTRL__INCLUDED_)