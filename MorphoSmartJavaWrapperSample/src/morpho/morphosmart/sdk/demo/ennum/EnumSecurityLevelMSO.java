package morpho.morphosmart.sdk.demo.ennum;

public enum EnumSecurityLevelMSO {
	DEFAULT ("Default"),
	STANDARD("Low"),
	MEDIIUM("Medium"),
	HIGH("High"),
	CRITICAL("Critical");

	private final String securityLevel;

	private EnumSecurityLevelMSO(final String securityLevel) {
		this.securityLevel = securityLevel;
	}

	/**
	 * returns enum content as an array.
	 * 
	 * @return
	 */
	public static String[] toStringArray() {
		String data[] = {
				"Default",
				"Low",
				"Medium",
				"High",
				"Critical" };
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
