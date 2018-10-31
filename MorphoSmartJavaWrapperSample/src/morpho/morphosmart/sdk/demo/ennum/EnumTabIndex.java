package morpho.morphosmart.sdk.demo.ennum;

public enum EnumTabIndex {

	TAB_CAPTURE(0),
	TAB_VERIFY(1),
	TAB_FINGERPTINT_IMAGE(2),
	TAB_ENROL(3),
	TAB_IDENTIFY(4),
	TAB_PROCESS(5);

	private final int tabIndex;

	/**
	 * Constructor.
	 * 
	 * @param fpTemplateType
	 */
	private EnumTabIndex(final int tabIndex) {
		this.tabIndex = tabIndex;
	}

	public int getValue() {
		return tabIndex;
	}

}
