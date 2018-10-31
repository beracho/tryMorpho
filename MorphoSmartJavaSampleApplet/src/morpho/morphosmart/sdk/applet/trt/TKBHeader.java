package morpho.morphosmart.sdk.applet.trt;

public class TKBHeader {
	public static final byte[] magicNbByte = {0x47,0x47,0x46,0x43};
	public static final int magicNbInt = 1195853379;
	public static final byte[] versionByte = {0,0,0,0};
	public static final int versionInt = 0;
	public static final int tKBHeaderSize = 16;
	public int tkbSize;
	public int certifSize;
}
