package morpho.morphosmart.sdk.applet.ennum;

/**
 * This enumeration list the possible types of an FVP template 
 * 
 */
public enum TemplateFVPType implements ITemplateType {
	MORPHO_NO_PK_FVP	(0, "NO PK FVP", ""), 
	MORPHO_PK_FVP		(1, "SAGEM PkFVP", ".fvp"), 
	MORPHO_PK_FVP_MATCH	(2, "SAGEM PkFVP Match", ".fvp-m");

	private int		code;
	private String	label;
	private String	extension;

	/**
	 * @return TemplateFVPType code
	 */
	public int getCode() {
		return code;
	}

	/**
	 * @return TemplateFVPType label
	 */
	public String getLabel() {
		return label;
	}

	/**
	 * @return TemplateFVPType extension
	 */
	public String getExtension() {
		return extension;
	}

	private TemplateFVPType(int code, String label, String extension) {
		this.code = code;
		this.label = label;
		this.extension = extension;
	}

	/**
	 * 
	 * @param id TemplateFVPType identifier value
	 * @return TemplateFVPType
	 */
	public static TemplateFVPType getValue(int id) {
		TemplateFVPType[] templateFVPTypes = TemplateFVPType.values();
		for (int i = 0; i < templateFVPTypes.length; i++) {
			if (templateFVPTypes[i].code == id)
				return templateFVPTypes[i];
		}
		return TemplateFVPType.MORPHO_NO_PK_FVP;
	}

	/**
	 * 
	 * @param label TemplateFVPType name value
	 * @return
	 */
	public static TemplateFVPType getValue(String label) {
		TemplateFVPType[] templateFVPTypes = TemplateFVPType.values();
		for (int i = 0; i < templateFVPTypes.length; i++)
		{
			if (templateFVPTypes[i].label.equals(label))
				return templateFVPTypes[i];
		}
		return TemplateFVPType.MORPHO_NO_PK_FVP;
	}
}
