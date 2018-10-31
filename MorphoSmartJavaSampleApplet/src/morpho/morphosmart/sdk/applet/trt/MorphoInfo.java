package morpho.morphosmart.sdk.applet.trt;

import morpho.morphosmart.sdk.applet.ennum.EnumTabIndex;


public class MorphoInfo {
	
	/**
	 * true, if the connected device is FVP, false otherwise.
	 */
	public static boolean isFVP = false;

	/**
	 * This is used to know in which tab we are actually.
	 */
	public static EnumTabIndex tabIndex = EnumTabIndex.TAB_VERIFY;
	
	/**
	 * This is used to know if the device supports WSQ.
	 */
	public static boolean isWSQSupported = true;
	
	/**
	 * Template ANSI 378 pk number offset
	 */
	public static int ANSI_FMR_378_PK_NUMBER_OFFSET = 0x18;
	
	/**
	 * Template ANSI 378 version 2009 pk number offset
	 */
	public static int ANSI_FMR_378_2009_PK_NUMBER_OFFSET = 0x13;
	
}
