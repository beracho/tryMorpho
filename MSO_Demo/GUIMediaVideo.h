/** @class GUIMediaVideo GUIMediaVideo.h
    This class is used to add a video to a screen. */

#pragma once

#include <Windows.h>
#include <dshow.h>

class GUIMediaVideo
{
	public:
		GUIMediaVideo(CWnd * in_pobjDisplayingWindow, const LPRECT in_structDisplayingRectangle, const char * in_pcFilePath);
		virtual ~GUIMediaVideo();

	public:
		void Play(bool in_bVideoLoop = false);
		void Stop();

		bool IsRunning();

		void RepaintVideo();

	private:
		void BuildDSGraph();
		void DestroyDSGraph();
		static DWORD WINAPI MessagePumpThread(LPVOID in_pobjGUIMediaVideo);
		DWORD MessagePumpThread();

	protected:
	  /** Pointer to the IGraphBuilder.
	  This interface provides methods that enable an application to build a filter graph. */
	  IGraphBuilder * m_pstructGraphBuilder;

	  /** Pointer to the IVMRWindowlessControl.
	  This interface controls how the Video Mixing Renderer Filter 7 (VMR-7) renders a video stream within a container window. */
	  IVMRWindowlessControl * m_pstructVMRControl;

	  /** Pointer to the IMediaControl.
	  This interface provides methods for controlling the flow of data through the filter graph. */
	  IMediaControl * m_pstrcutMediaControl;

	  /** Pointer to the IMediaEventEx.
	  This interface inherits the IMediaEvent interface, which contains methods for retrieving event notifications and for overriding the filter graph's default handling of events. */
	  IMediaEventEx * m_pstructMediaEvent;

	  /** Pointer to the IMediaSeeking.
	  This interface contains methods for seeking to a position within a stream, and for setting the playback rate. The Filter Graph Manager exposes this interface, and so do individual filters. */
	  IMediaSeeking * m_pstructMediaSeeking;

	  /** Pointer to the CWnd Window in which displays the video */
	  CWnd * m_pobjDisplayingWindow;

	  /** Rectangle in which displays the video */
	  RECT m_structDisplayingRectangle;

	  /** Boolean to indicate if video had to loop or not */
	  bool m_bVideoLoop;

	  /** Handle to the displaying window message pump thread */
	  HANDLE m_hMessagePumpThread;

	  /** Handle to the event that will indicate to the window message pump thread to stop */
	  HANDLE m_hMessagePumpThreadStop;
};
