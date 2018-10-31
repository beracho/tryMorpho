package morpho.morphosmart.sdk.demo.ennum;

public enum EnumMatchingStrategy {

	DEFAULT("Default"),
	ADVANCED("Advanced");

	private final String matchingStrategy;

	private EnumMatchingStrategy(final String matchingStrategy) {
		this.matchingStrategy = matchingStrategy;
	}

	/**
	 * returns enum content as an array.
	 * 
	 * @return
	 */
	public static String[] toStringArray() {
		String data[] = {
				"Default",
				"Advanced" };
		return data;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Enum#toString()
	 */
	@Override
	public String toString() {
		return matchingStrategy;
	}

}
