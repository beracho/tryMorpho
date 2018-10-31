package morpho.morphosmart.sdk.demo.ennum;

public enum MorphoUserFields {
	ID_FIELD_INDEX(0),
	FIRSTNAME_FIELD_INDEX(1),
	LASTTNAME_FIELD_INDEX(2);

	private long fieldIndex;

	private MorphoUserFields(long fieldIndex) {
		this.fieldIndex = fieldIndex;
	}

	public long getValue() {
		return fieldIndex;
	}

}
