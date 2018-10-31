package morpho.morphosmart.sdk.applet.ennum;

public enum EnumTabIndex {

	TAB_VERIFY(0),
	TAB_ENROL(1);

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
