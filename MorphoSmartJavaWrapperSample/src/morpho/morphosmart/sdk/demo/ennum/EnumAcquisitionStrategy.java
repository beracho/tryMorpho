package morpho.morphosmart.sdk.demo.ennum;

public enum EnumAcquisitionStrategy {

	EXPERT_DEFAULT("Expert(Default)"),
	FAST_STANDARD("Fast(Standard)"),
	SLOW_ACCURATE("Slow(Accurate)"),
	FULl_MULTIMOMDAL("Full MultiModal"),
	ANTI_SPOOFING("Anti Spoofing");

	private final String acquisitionStrategy;

	private EnumAcquisitionStrategy(final String acquisitionStrategy) {
		this.acquisitionStrategy = acquisitionStrategy;
	}

	/**
	 * returns enum content as an array.
	 * 
	 * @return
	 */
	public static String[] toStringArray() {
		String data[] = {
				"Expert(Default)",
				"Fast(Standard)",
				"Slow(Accurate)",
				"Full MultiModal",
				"Anti Spoofing" };
		return data;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Enum#toString()
	 */
	@Override
	public String toString() {
		return acquisitionStrategy;
	}

}
