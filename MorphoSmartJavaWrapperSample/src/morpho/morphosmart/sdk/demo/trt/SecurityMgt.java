package morpho.morphosmart.sdk.demo.trt;

import java.util.ArrayList;

import morpho.morphosmart.sdk.api.MorphoDevice;
import morpho.morphosmart.sdk.api.IMsoSecu;
import morpho.morphosmart.sdk.api.MorphoTemplateList;

public class SecurityMgt {

	public static int tunnelingOpen(MorphoDevice device, IMsoSecu msoSecu,
			byte[] hostCertificate) {
		return device.tunnelingOpen(msoSecu, hostCertificate);
	}

	public static int offeredSecuOpen(MorphoDevice device, IMsoSecu msoSecu) {
		return device.offeredSecuOpen(msoSecu);
	}

	public static int tunnelingClose(MorphoDevice device) {
		return device.tunnelingClose();
	}

	public static int offeredSecuClose(MorphoDevice device) {
		return device.offeredSecuClose();
	}

	public static int secuStoPkcs12(MorphoDevice device, byte[] pkcs12, int ac) {
		return device.secuStoPkcs12(pkcs12, ac);
	}

	public static int secuStoCertif(MorphoDevice device, byte[] certificate) {
		return device.secuStoCertif(certificate);
	}

	public static int secuReadCertificate(MorphoDevice device, short index,
			ArrayList<Byte> certificate) {
		return device.secuReadCertificate(index, certificate);
	}

	public static int verifSignX984(MorphoTemplateList templateList,
			byte[] pkX984Data, byte[] certificate, IMsoSecu msoSecu,
			int[] result) {
		return templateList.verifSignX984(pkX984Data, certificate.length,
				certificate, msoSecu, result);
	}

}
