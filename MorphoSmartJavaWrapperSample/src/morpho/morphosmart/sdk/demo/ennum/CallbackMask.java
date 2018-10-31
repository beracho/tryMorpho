package morpho.morphosmart.sdk.demo.ennum;

/**
 * @brief Those values are used to define the asynchronous status events
 *        returned by the <b>Observer</b>. There are also used to create the
 *        binary mask that describes the asynchronous status events that will
 *        trig the callback function.
 */
public enum CallbackMask {

	/*
	 * This asynchronous status event identifies a command status information.
	 */
	MORPHO_CALLBACK_COMMAND_CMD(1),
	/*
	 * This asynchronous status event identifies a low-resolution image.
	 */
	MORPHO_CALLBACK_IMAGE_CMD(2),
	/*
	 * This asynchronous status event identifies an enrollment status.
	 */
	MORPHO_CALLBACK_ENROLLMENT_CMD(4),
	/*
	 * This asynchronous status event identifies the last image from a live
	 * acquisition which is returned in full resolution.
	 * 
	 */
	MORPHO_CALLBACK_LAST_IMAGE_FULL_RES_CMD(8),
	/*
	 * This asynchronous status event identifies the status of the quality note
	 * of the image detained to be coded.
	 * 
	 */
	MORPHO_CALLBACK_CODEQUALITY(64),
	/*
	 * This asynchronous status event identifies the status of the quality note
	 * calculated by the "presence detection" function.
	 * 
	 */
	MORPHO_CALLBACK_DETECTQUALITY(128);

	private int value;

	private CallbackMask(int value) {
		this.value = value;
	}

	/**
	 * @return the value
	 */
	public int getValue() {
		return value;
	}

	/**
	 * @param value
	 *            the value to set
	 */
	public void setValue(int value) {
		this.value = value;
	}

}
