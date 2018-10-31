package morpho.morphosmart.sdk.demo.ennum;

public enum EnumSecurityLevelFVP {

	STANDARD("Standard"),
	MEDIIUM("Medium"),
	HIGH("High");

	private final String securityLevel;

	private EnumSecurityLevelFVP(final String securityLevel) {
		this.securityLevel = securityLevel;
	}

	/**
	 * returns enum content as an array.
	 * 
	 * @return
	 */
	public static String[] toStringArray() {
		String data[] = {
				"Standard",
				"Medium",
				"High" };
		return data;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Enum#toString()
	 */
	@Override
	public String toString() {
		return securityLevel;
	}

}
