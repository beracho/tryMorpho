package morpho.morphosmart.sdk.applet.ennum;

public enum EnumExportImage {

	NO_IMAGE("NO IMAGE"),
	RAW("RAW"),
	SAGEM_V1("SAGEM_V1"),
	WSQ("WSQ");

	private final String imageFormat;

	private EnumExportImage(final String imageFormat) {
		this.imageFormat = imageFormat;
	}

	/**
	 * returns enum content as an array.
	 * 
	 * @return
	 */
	public static String[] toStringArray() {
		String data[] = {
				"NO IMAGE",
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
