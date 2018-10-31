package morpho.morphosmart.sdk.demo.ennum;

public enum EnumSensorWindowPosition {

	ENUM_0(0),
	ENUM_180(1),
	ENUM_0_NON_ORIENTED(2),
	ENUM_180_NON_ORIENTED(3);


	private final int sensorWindowPosition;

	/**
	 * Constructor.
	 *
	 * @param fpTemplateType
	 */
	private EnumSensorWindowPosition(final int sensorWindowPosition) {
		this.sensorWindowPosition = sensorWindowPosition;
	}

	/**
	 * returns enum content as an array.
	 *
	 * @return
	 */
	public static String[] toStringArray() {
		String data[] = {
				"0\u00b0",
				"180 \u00b0",
				"0 \u00b0-Non Oriented Matching",
				"180 \u00b0-Non Oriented Matching" };
		return data;
	}

	public int getValue() {
		return sensorWindowPosition;
	}

}
