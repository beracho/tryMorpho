package morpho.morphosmart.sdk.demo.ennum;

public enum EnumFingerIndex {

	FIRST(0),
	SECOND(1);

	private final int fingerIndex;

	/**
	 * Constructor.
	 * 
	 * @param fpTemplateType
	 */
	private EnumFingerIndex(final int fingerIndex) {
		this.fingerIndex = fingerIndex;
	}

	/**
	 * returns enum content as an array.
	 * 
	 * @return
	 */
	public static String[] toStringArray() {
		String data[] = {
				"First Finger",
				"Second Finger" };
		return data;
	}

	public int getValue() {
		return fingerIndex;
	}

}
