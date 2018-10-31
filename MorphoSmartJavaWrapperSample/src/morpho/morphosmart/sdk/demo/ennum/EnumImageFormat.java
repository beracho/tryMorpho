package morpho.morphosmart.sdk.demo.ennum;

public enum EnumImageFormat {

	RAW("RAW"),
	SAGEM_V1("SAGEM_V1"),
	WSQ("WSQ");

	private final String imageFormat;

	private EnumImageFormat(final String imageFormat) {
		this.imageFormat = imageFormat;
	}

	/**
	 * returns enum content as an array.
	 * 
	 * @return
	 */
	public static String[] toStringArray() {
		String data[] = {
				"RAW",
				"SAGEM_V1",
				"WSQ" };
		return data;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Enum#toString()
	 */
	@Override
	public String toString() {
		return imageFormat;
	}

}
