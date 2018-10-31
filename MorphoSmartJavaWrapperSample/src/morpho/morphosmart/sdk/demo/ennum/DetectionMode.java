package morpho.morphosmart.sdk.demo.ennum;

/**
 * 
 * @brief This class store possible values of a detection mode.
 * 
 */
public enum DetectionMode {

	/** @brief The detection mode is chosen by the scanner. */
	MORPHO_DEFAULT_DETECT_MODE(0),
	/** @brief Fast detection mode. */
	MORPHO_VERIF_DETECT_MODE(1),
	/** @brief Stronger detection mode. */
	MORPHO_ENROLL_DETECT_MODE(2),
	/**
	 * @brief Uses a 'led off' presence detection (only on MorphoSmart™
	 *        MSOxx1).
	 */
	MORPHO_WAKEUP_LED_OFF(4),
	/** @brief The finger must cover an area starting at the top of the image. */
	MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE(16),
	/**
	 * @brief Uses a 'led on' presence detection (only on MorphoSmart™ FINGER
	 *        VP).
	 */
	MORPHO_WAKEUP_LED_ON(64);

	private int value;

	private DetectionMode(int value) {
		this.value = value;
	}

	/**
	 * @return the value
	 */
	public int getValue() {
		return value;
	}
}
