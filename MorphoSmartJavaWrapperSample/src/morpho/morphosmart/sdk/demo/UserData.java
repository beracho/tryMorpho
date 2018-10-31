package morpho.morphosmart.sdk.demo;

import java.util.ArrayList;

import morpho.morphosmart.sdk.api.MorphoFVPTemplateType;
import morpho.morphosmart.sdk.api.MorphoTemplateType;

/**
 * UserData bean.
 * 
 */
public class UserData {

	private String userID = "";
	private String firstName = "";
	private String lastName = "";
	private int nbFinger = 0;

	private MorphoTemplateType morphoTemplateType = MorphoTemplateType.MORPHO_NO_PK_FP;
	private MorphoFVPTemplateType morphoFVPTemplateType = MorphoFVPTemplateType.MORPHO_NO_PK_FVP;

	private ArrayList<byte[]> templateData = new ArrayList<byte[]>();
	private byte[] pkX984TemplateData = null;
	private byte[] pkX984Certificate = null;

	/**
	 * @return the userID
	 */
	public String getUserID() {
		return userID;
	}

	/**
	 * @param userID
	 *            the userID to set
	 */
	public void setUserID(String userID) {
		this.userID = userID;
	}

	/**
	 * @return the firstName
	 */
	public String getFirstName() {
		return firstName;
	}

	/**
	 * @param firstName
	 *            the firstName to set
	 */
	public void setFirstName(String firstName) {
		this.firstName = firstName;
	}

	/**
	 * @return the lastName
	 */
	public String getLastName() {
		return lastName;
	}

	/**
	 * @param lastName
	 *            the lastName to set
	 */
	public void setLastName(String lastName) {
		this.lastName = lastName;
	}

	/**
	 * @return the nbFinger
	 */
	public int getNbFinger() {
		return nbFinger;
	}

	/**
	 * @param nbFinger
	 *            the nbFinger to set
	 */
	public void setNbFinger(int nbFinger) {
		this.nbFinger = nbFinger;
	}

	/**
	 * @return the morphoTemplateType
	 */
	public MorphoTemplateType getMorphoTemplateType() {
		return morphoTemplateType;
	}

	/**
	 * @param morphoTemplateType
	 *            the morphoTemplateType to set
	 */
	public void setMorphoTemplateType(MorphoTemplateType morphoTemplateType) {
		this.morphoTemplateType = morphoTemplateType;
	}

	/**
	 * @return the morphoFVPTemplateType
	 */
	public MorphoFVPTemplateType getMorphoFVPTemplateType() {
		return morphoFVPTemplateType;
	}

	/**
	 * @param morphoFVPTemplateType
	 *            the morphoFVPTemplateType to set
	 */
	public void setMorphoFVPTemplateType(MorphoFVPTemplateType morphoFVPTemplateType) {
		this.morphoFVPTemplateType = morphoFVPTemplateType;
	}

	/**
	 * @return the pkX984TemplateData
	 */
	public byte[] getPkX984TemplateData() {
		return pkX984TemplateData;
	}

	/**
	 * @param pkX984TemplateData
	 *            the pkX984TemplateData to set
	 */
	public void setPkX984TemplateData(byte[] pkX984TemplateData) {
		this.pkX984TemplateData = pkX984TemplateData;
	}

	public byte[] getTemplateData(int index) {
		return templateData.get(index);
	}

	public void addTemplateData(byte[] templateData) {
		this.templateData.add(templateData);
	}

	public byte[] getPkX984Certificate() {
		return pkX984Certificate;
	}

	public void setPkX984Certificate(byte[] pkX984Certificate) {
		this.pkX984Certificate = pkX984Certificate;
	}
}
