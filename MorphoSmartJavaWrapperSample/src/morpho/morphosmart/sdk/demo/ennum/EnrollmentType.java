package morpho.morphosmart.sdk.demo.ennum;

/**
 * Specifies the number of fingerprint image acquisitions. Allowed values are 0, 1 and 3.
 * We strongly recommend setting this value to 0 (default value) or 3 for enrollment purpose to increase the system performances: in this case, the template is generated from a consolidation calculation of three consecutive acquisitions of the same fingerprint.
 * It is also possible to set this value to 1 for verification purpose. In this case, it is not possible to save the record in the internal database: in this case, the template is generated from one single fingerprint acquisition.
 * On MorphoSmart™ FINGER VP, the value 1 is deprecated. 
 *
 */
public enum EnrollmentType {
	ONE_ACQUISITIONS(1),
	THREE_ACQUISITIONS(0);

	private int	value;

	private EnrollmentType(int value) {
		this.value = value;
	}

	public int getValue() {
		return value;
	}

}
