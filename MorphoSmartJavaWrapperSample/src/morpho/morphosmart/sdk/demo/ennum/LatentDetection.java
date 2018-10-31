package morpho.morphosmart.sdk.demo.ennum;

/**
 * 
 * Disable or enable the latency detection for Capture operation.
 * 
 */
public enum LatentDetection {
	/*
	 * Disable the latency detection (this is the default).
	 */
	LATENT_DETECT_DISABLE(0),
	/*
	 * Enable the latency detection.
	 */
	LATENT_DETECT_ENABLE(1);

	private int value;

	private LatentDetection(int value) {
		this.value = value;
	}

	/**
	 * @return the value
	 */
	public int getValue() {
		return value;
	}
}
