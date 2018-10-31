package morpho.morphosmart.sdk.demo.ennum;

/**
 * @brief Compression algorithm to be used to compress the fingerprint image.
 *        Available algorithms are: - #MORPHO_NO_COMPRESS - #MORPHO_COMPRESS_V1
 *        - #MORPHO_COMPRESS_WSQ
 * 
 */
public enum CompressionAlgorithm {
	NO_IMAGE(-1, "NO IMAGE", ""),
	MORPHO_NO_COMPRESS(0, "RAW", ".raw"),
	MORPHO_COMPRESS_V1(1, "SAGEM_V1", ".bin"),
	MORPHO_COMPRESS_WSQ(2, "WSQ", ".wsq");

	private int code;
	private String label;
	private String extension;

	/**
	 * @brief Getter of compression algorithm code
	 * @return Code compression algorithm code
	 */
	public int getCode() {
		return code;
	}

	/**
	 * @brief Getter of compression algorithm label
	 * @return Label compression algorithm label
	 */
	public String getLabel() {
		return label;
	}

	/**
	 * @brief Getter of compression algorithm extension
	 * @return Extension compression algorithm extension
	 */
	public String getExtension() {
		return extension;
	}

	private CompressionAlgorithm(int code, String label, String extension) {
		this.code = code;
		this.label = label;
		this.extension = extension;
	}

	/**
	 * @brief Return the compression algorithm for fingerprint image
	 * @param firmwareCompressionAlgorithm
	 *            firmware Compression Algorithm
	 * @return CompressionAlgorithm Compression Algorithm.
	 */
	public static CompressionAlgorithm GetCompressionAlgorithm(int firmwareCompressionAlgorithm) {
		switch (firmwareCompressionAlgorithm) {
			case 44:
				return MORPHO_NO_COMPRESS;
			case 60:
				return MORPHO_COMPRESS_V1;
			case 156:
				return MORPHO_COMPRESS_WSQ;
			default:
				return MORPHO_NO_COMPRESS;
		}
	}

	/**
	 * @brief Return Compression Algorithm corresponding to the given
	 *        identifier.
	 * @param id
	 * @return CompressionAlgorithm
	 */
	protected static CompressionAlgorithm getValue(int id) {
		CompressionAlgorithm[] compressionAlgorithms = CompressionAlgorithm.values();
		for (int i = 0; i < compressionAlgorithms.length; i++) {
			if (compressionAlgorithms[i].code == id)
				return compressionAlgorithms[i];
		}
		return CompressionAlgorithm.MORPHO_NO_COMPRESS;
	}

}
