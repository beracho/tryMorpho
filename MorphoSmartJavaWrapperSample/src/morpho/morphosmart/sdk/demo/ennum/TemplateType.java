package morpho.morphosmart.sdk.demo.ennum;


public enum TemplateType implements ITemplateType {
	MORPHO_PK_COMP(0, "SAGEM PkComp", ".pkc"), 
	MORPHO_PK_MAT_NORM(1, "SAGEM PkMat Norm", ".pkmn"), 
	MORPHO_PK_COMP_NORM(2, "SAGEM PkComp Norm", ".pkcn"), 
	MORPHO_PK_MAT(3, "SAGEM PkMat", ".pkm"), 
	MORPHO_PK_ANSI_378(4, "ANSI INCITS 378", ".ansi-fmr"), 
	MORPHO_PK_MINEX_A(5, "MINEX A", ".minex-a"), 
	MORPHO_PK_ISO_FMR(6, "ISO 19794-2", ".iso-fmr"), 
	MORPHO_PK_ISO_FMC_NS(7, "ISO 19794-2, FMC Normal Size",".iso-fmc-ns"), 
	MORPHO_PK_ISO_FMC_CS(8, "ISO 19794-2, FMC Compact Size", ".iso-fmc-cs"), 
//	MORPHO_PK_ILO_FMR(9, "ILO International Labour Organisation", ".ilo-fmr"), 
//	MORPHO_PK_MOC(12,"SAGEM PKMOC", ".moc"), 
	MORPHO_PK_DIN_V66400_CS(13, "DIN V66400 Compact Size", ".din-cs"), 
	MORPHO_PK_DIN_V66400_CS_AA(14, "DIN V66400 Compact Size, ordered by Ascending Angle", ".din-cs"), 
	MORPHO_PK_ISO_FMC_CS_AA(15, "ISO 19794-2, FMC Compact Size, ordered by Ascending Angle", ".iso-fmc-cs"), 	
//	MORPHO_PK_CFV(16,"Morpho proprietary CFV Fingerprint Template",".cfv"),
//	MORPHO_PK_BIOSCRYPT(17, "Bioscrypt Fingerprint Template",".bioscrypt"),
	MORPHO_NO_PK_FP(18, "NO PK FP", ""),
	MORPHO_PK_ANSI_378_2009(19,"ANSI INCITS 378-2009",".ansi-fmr-2009"),
	MORPHO_PK_ISO_FMR_2011(20,"ISO 19794-2-2011",".iso-fmr-2011"),
	MORPHO_PK_PKLITE(21,"SAGEM Pklite",".pklite"),
	MORPHO_PK_SAGEM_PKS(22,"SAGEM Pks",".pks");

	private int		code;
	private String	label;
	private String	extension;

	/**
	 * @return TemplateType code
	 */
	public int getCode() {
		return code;
	}

	/**
	 * @return TemplateType label
	 */
	public String getLabel() {
		return label;
	}

	/**
	 * @return TemplateType extension
	 */
	public String getExtension() {
		return extension;
	}

	private TemplateType(int code, String label, String extension) {
		this.code = code;
		this.label = label;
		this.extension = extension;
	}

	/**
	 * 
	 * @param id TemplateType identifier value
	 * @return TemplateType
	 */
	public static TemplateType getValue(int id) {
		TemplateType[] templateTypes = TemplateType.values();
		for (int i = 0; i < templateTypes.length; i++) {
			if (templateTypes[i].code == id)
				return templateTypes[i];
		}
		return TemplateType.MORPHO_NO_PK_FP;
	}

	/**
	 * 
	 * @param label TemplateType name value
	 * @return
	 */
	public static TemplateType getValue(String label) {
		TemplateType[] templateTypes = TemplateType.values();
		for (int i = 0; i < templateTypes.length; i++) {
			if (templateTypes[i].label.equals(label))
				return templateTypes[i];
		}
		return TemplateType.MORPHO_NO_PK_FP;
	}
}