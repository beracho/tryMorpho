package morpho.morphosmart.sdk.api;

import java.util.ArrayList;

public interface IMsoSecu {

	public int tunnelingInit1(byte[] tokenMso, byte[] certifMso, byte[] hashMso, byte[] hashHost, ArrayList<Byte> tokenHost);

	public int tunnelingInit2(byte[] tokenMso);

	public int tripleDesSign(byte[] data, ArrayList<Byte> signature);

	public int tripleDesCrypt(byte[] data, ArrayList<Byte> dataCrypt);

	public int desCrypt(byte[] key, byte[] data, ArrayList<Byte> dataCrypt);

	public int tripleDesVerifSign(byte[] data, byte[] signature, boolean[] res);

	public int tripleDesDecrypt(byte[] data, ArrayList<Byte> dataClear);

	public int getHostCertif(ArrayList<Byte> certifHost);

	public int genRandom(byte[] random);

	public int verifOfferedSecuritySignature(byte[] msoCertificate, byte[] signature, byte[] data, int[] result);

	public int verifSignRSA(byte[] msoCertificate, byte[] signature, byte[] data, int[] result);

	public int verifSignDSA(byte[] msoCertificate, byte[] signature, byte[] data, int[] result);

	public int verifCertif(byte[] certif, int[] result);

	public int tripleDesCryptByKey(byte[] key, byte[] data, ArrayList<Byte> dataCrypt);

	public int rsaEncrypt(byte[] Certif, byte[] dataIn, ArrayList<Byte> dataOut);

	public int signRSA(byte[] hostKey, byte[] data, ArrayList<Byte> signature);

	public int encrypt3DesCbcNopad(byte[] key, byte[] in, int len, byte[] iv, byte[] out);

	public int setOpenSSLPath(String openSSLPath);
	
	public  int decryptAes128Cbc(byte[] key, byte[] aes128CbcCryptedData, byte[] iv, boolean usePadding, ArrayList<Byte> aes128CbcClearData);
	
	public  int encryptAes128Cbc(byte[] key, byte[] aes128CbcClearData, byte[] iv, boolean usePadding, ArrayList<Byte> aes128CbcCryptedData);

	public int computeCRC32(byte[] data, long polynomial, long initialCRCValue, boolean inputReflected, boolean outputReflected, long outputXOR, long[] crcValue);
}
