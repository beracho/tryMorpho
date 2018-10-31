#include "StdAfx.h"
#include "GUIMediaVideo.h"

GUIMediaVideo::GUIMediaVideo(CWnd * in_pobjDisplayingWindow, const LPRECT in_structDisplayingRectangle, const char * in_pcFilePath) :
	m_pstructGraphBuilder(NULL),
	m_pstructVMRControl(NULL),
	m_pstrcutMediaControl(NULL),
	m_pstructMediaEvent(NULL),
	m_pstructMediaSeeking(NULL),
	m_pobjDisplayingWindow(in_pobjDisplayingWindow),
	m_structDisplayingRectangle(*in_structDisplayingRectangle),
	m_bVideoLoop(false),
	m_hMessagePumpThread(NULL),
	m_hMessagePumpThreadStop(NULL)
{
	WCHAR l_pwcFilePath[1024];

	BuildDSGraph();

	MultiByteToWideChar(CP_ACP, 0, in_pcFilePath, -1, l_pwcFilePath, 1024);
	m_pstructGraphBuilder->RenderFile(l_pwcFilePath, NULL);
}

GUIMediaVideo::~GUIMediaVideo()
{
	DestroyDSGraph();
}

void GUIMediaVideo::BuildDSGraph()
{
	HRESULT l_hResult = S_OK;
	IBaseFilter * l_pstructVMRFilter = NULL;
	IVMRFilterConfig * l_pstructVMRFilterConfig = NULL;
	long l_lWidth = 0;
	long l_lHeight = 0;
	RECT l_structSourceRect;

	if(m_pstructGraphBuilder != NULL)
		DestroyDSGraph();

	l_hResult = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pstructGraphBuilder);

	if(SUCCEEDED(l_hResult))
		l_hResult = CoCreateInstance(CLSID_VideoMixingRenderer, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void **)&l_pstructVMRFilter);

	if(SUCCEEDED(l_hResult))
	{
		l_hResult = m_pstructGraphBuilder->AddFilter(l_pstructVMRFilter, L"Video Mixing Renderer");

		if(SUCCEEDED(l_hResult))
		{
			l_hResult = l_pstructVMRFilter->QueryInterface(IID_IVMRFilterConfig, (void **)&l_pstructVMRFilterConfig);

			if(SUCCEEDED(l_hResult))
			{
				l_hResult = l_pstructVMRFilterConfig->SetRenderingMode(VMRMode_Windowless);
				l_pstructVMRFilterConfig->Release();
			}
		}

		if(SUCCEEDED(l_hResult))
			l_hResult = l_pstructVMRFilter->QueryInterface(IID_IVMRWindowlessControl, (void **)&m_pstructVMRControl);

		if(SUCCEEDED(l_hResult) && (m_pobjDisplayingWindow != NULL))
			l_hResult = m_pstructVMRControl->SetVideoClippingWindow(*m_pobjDisplayingWindow);

		l_pstructVMRFilter->Release();
	}

	if(SUCCEEDED(l_hResult))
		l_hResult = m_pstructGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&m_pstrcutMediaControl);

	if(SUCCEEDED(l_hResult))
		l_hResult = m_pstructGraphBuilder->QueryInterface(IID_IMediaEventEx, (void **)&m_pstructMediaEvent);

	if(SUCCEEDED(l_hResult))
		l_hResult = m_pstructGraphBuilder->QueryInterface(IID_IMediaSeeking, (void **)&m_pstructMediaSeeking);

	l_hResult = m_pstructVMRControl->GetNativeVideoSize(&l_lWidth, &l_lHeight, NULL, NULL);

	if(SUCCEEDED(l_hResult))
	{
		// Set the source rectangle
		SetRect(&l_structSourceRect, 0, 0, l_lWidth, l_lHeight);

		// Set the video position
		l_hResult = m_pstructVMRControl->SetVideoPosition(&l_structSourceRect, &m_structDisplayingRectangle);
	}

	DWORD l_ulThreadID = 0;

	m_hMessagePumpThreadStop = CreateEvent(NULL, FALSE, FALSE, "GUIMediaVideoMessagePumpThreadStop");
	m_hMessagePumpThread = CreateThread(NULL, 0, MessagePumpThread, this, 0, &l_ulThreadID);
}

void GUIMediaVideo::DestroyDSGraph()
{
	Stop();

	DWORD l_ulWaitResult = 0;

	if(m_hMessagePumpThreadStop != NULL)
		SetEvent(m_hMessagePumpThreadStop);

	if(m_hMessagePumpThread != NULL)
	{
		l_ulWaitResult = WaitForSingleObject(m_hMessagePumpThread, 1000);

		if(l_ulWaitResult == WAIT_TIMEOUT)
			TerminateThread(m_hMessagePumpThread, EXIT_FAILURE);

		CloseHandle(m_hMessagePumpThreadStop);
		m_hMessagePumpThreadStop = NULL;
	}

	if(m_hMessagePumpThreadStop != NULL)
	{
		CloseHandle(m_hMessagePumpThreadStop);
		m_hMessagePumpThreadStop = NULL;
	}

	if(m_pstructMediaSeeking != NULL)
	{
		m_pstructMediaSeeking->Release();
		m_pstructMediaSeeking = NULL;
	}

	if(m_pstructMediaEvent != NULL)
	{
		m_pstructMediaEvent->SetNotifyWindow(NULL, 0, NULL);
		m_pstructMediaEvent->Release();
		m_pstructMediaEvent = NULL;
	}

	if(m_pstrcutMediaControl != NULL)
	{
		m_pstrcutMediaControl->Release();
		m_pstrcutMediaControl = NULL;
	}

	if(m_pstructVMRControl != NULL)
	{
		m_pstructVMRControl->Release();
		m_pstructVMRControl = NULL;
	}

	if(m_pstructGraphBuilder != NULL)
	{
		m_pstructGraphBuilder->Release();
		m_pstructGraphBuilder = NULL;
	}
}

void GUIMediaVideo::Play(bool in_bVideoLoop)
{
	LONGLONG l_llPosition = 0;

	Stop();

	m_bVideoLoop = in_bVideoLoop;

	if(m_pstructMediaSeeking != NULL)
		m_pstructMediaSeeking->SetPositions(&l_llPosition, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);

	if((m_pstructVMRControl != NULL) && (m_pobjDisplayingWindow != NULL))
		m_pstructVMRControl->SetVideoClippingWindow(*m_pobjDisplayingWindow);

	if(m_pstrcutMediaControl != NULL)
		m_pstrcutMediaControl->Run();

	RepaintVideo();
}

void GUIMediaVideo::Stop()
{
	if(m_pstrcutMediaControl != NULL)
		m_pstrcutMediaControl->Stop();
}

bool GUIMediaVideo::IsRunning()
{
	bool l_bIsRunning = false;
	OAFilterState l_lFilterSate = State_Stopped;

	if(m_pstrcutMediaControl != NULL)
	{
		m_pstrcutMediaControl->GetState(100, &l_lFilterSate);
		l_bIsRunning = (l_lFilterSate == State_Running);
	}

	return l_bIsRunning;
}

void GUIMediaVideo::RepaintVideo()
{
	CDC * l_pobjDC = NULL;

	if(IsRunning() && (m_pstructVMRControl != NULL) && (m_pobjDisplayingWindow != NULL))
	{
		l_pobjDC = m_pobjDisplayingWindow->GetDC();

		if(l_pobjDC != NULL)
		{
			m_pstructVMRControl->RepaintVideo(*m_pobjDisplayingWindow, l_pobjDC->GetSafeHdc());
			m_pobjDisplayingWindow->ReleaseDC(l_pobjDC);
		}
	}
}

DWORD WINAPI GUIMediaVideo::MessagePumpThread(LPVOID in_pobjGUIMediaVideo)
{
	DWORD l_ulReturn = EXIT_FAILURE;

	if(in_pobjGUIMediaVideo != NULL)
	{
		GUIMediaVideo * l_pobjGUIMediaVideo = (GUIMediaVideo *)in_pobjGUIMediaVideo;
		l_ulReturn = l_pobjGUIMediaVideo->MessagePumpThread();
	}

	return l_ulReturn;
}

DWORD GUIMediaVideo::MessagePumpThread()
{
	DWORD l_ulReturn = EXIT_FAILURE;
	DWORD l_ulWaitResult = 0;
	HRESULT l_hResult = E_FAIL;
	HANDLE l_arrayhEvent[2];
	LONGLONG l_llPosition = 0;
	long l_lEventCode = 0;
	LONG_PTR l_pParam1 = NULL;
	LONG_PTR l_pParam2 = NULL;

	if((m_pstructMediaEvent != NULL) && (m_hMessagePumpThreadStop != NULL))
	{
		l_arrayhEvent[0] = m_hMessagePumpThreadStop;
		l_hResult = m_pstructMediaEvent->GetEventHandle((OAEVENT *)(&(l_arrayhEvent[1])));

		if(SUCCEEDED(l_hResult))
		{
			while((l_ulWaitResult = WaitForMultipleObjects(2, l_arrayhEvent, FALSE, INFINITE)) != WAIT_OBJECT_0)
			{
				while(SUCCEEDED(m_pstructMediaEvent->GetEvent(&l_lEventCode, &l_pParam1, &l_pParam2, 0)))
				{
					if(l_lEventCode == EC_COMPLETE && m_pstructMediaSeeking != NULL)
						m_pstructMediaSeeking->SetPositions(&l_llPosition, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);

					m_pstructMediaEvent->FreeEventParams(l_lEventCode, l_pParam1, l_pParam2);
				}
			}

			if(l_ulWaitResult == WAIT_OBJECT_0)
				l_ulReturn = EXIT_SUCCESS;
		}
	}

	return l_ulReturn;
}
