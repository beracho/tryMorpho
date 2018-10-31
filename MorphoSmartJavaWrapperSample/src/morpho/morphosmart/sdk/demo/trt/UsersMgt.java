package morpho.morphosmart.sdk.demo.trt;

import java.io.DataInputStream;
import java.io.EOFException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

import morpho.morphosmart.sdk.api.MorphoFVPTemplateType;
import morpho.morphosmart.sdk.api.MorphoSmartSDK;
import morpho.morphosmart.sdk.api.MorphoTemplateList;
import morpho.morphosmart.sdk.api.MorphoTemplateType;
import morpho.morphosmart.sdk.api.MorphoDevice.MorphoDevicePrivacyModeStatus;
import morpho.morphosmart.sdk.demo.MsoDemo;
import morpho.morphosmart.sdk.demo.UserData;
import morpho.morphosmart.sdk.demo.dialog.DialogUtils;

public class UsersMgt {

	public static UserData getUserDataFromFile(String templaeFilePath) {
		return getUserDataFromFile(templaeFilePath, false,null, null, null);
	}
	
	public static UserData getUserDataFromFile(String templaeFilePath, boolean isPrivacy, byte[] key, byte[] random, MsoDemo msoDemo) {
		
		UserData userData = new UserData();
		
		if(isPrivacy && (key==null || random == null ||  msoDemo==null) )
		{
			DialogUtils.showErrorMessage("UsersMgt", "An error occured while calling UsersMgt.getUserDataFromFile() function" );
			return userData;
		}
		
		File f = new File(templaeFilePath);
		try {
			InputStream is = new FileInputStream(f);
			DataInputStream dis = new DataInputStream(is);
			int fileSize = dis.available();
			if (fileSize == 0)
			{
				dis.close();
				is.close();
				return userData;
			}

			byte[] templateData = new byte[dis.available()];

			if (templaeFilePath.endsWith(".pks")) {
				// Format of the .pks structure :
				// - Size ID : 2 bytes
				// - ID : <Size ID> bytes
				// - Size Firstname : 2 bytes
				// - Firstname : <Size Firstname> bytes
				// - Size Lastname : 2 bytes
				// - Lastname : <Size Lastname> bytes
				// - Nb of templates: 1 byte (1 or 2 fingers)
				// - Size 1st Tplate: 2 bytes
				// - 1st Template : <Size 1st Tplate> bytes
				// - Size 2nd Tplate: 2 bytes (if exists)
				// - 2nd Template : <Size 2nd Tplate> bytes (if exists)

				byte[] size = new byte[2];
				byte[] userID = null;
				byte[] firstname = null;
				byte[] lastname = null;
				byte[] template = null;

				// user ID
				dis.read(size);
				int dataSize = ByteBuffer.wrap(new byte[]{size[0],size[1],0,0}).order(ByteOrder.LITTLE_ENDIAN).getInt();
				if (dataSize != 0) {
					userID = new byte[dataSize];
					dis.read(userID);
				}
				// user Firstname
				dis.read(size);
				dataSize = ByteBuffer.wrap(new byte[]{size[0],size[1],0,0}).order(ByteOrder.LITTLE_ENDIAN).getInt();
				if (dataSize != 0) {
					firstname = new byte[dataSize];
					dis.read(firstname);
				}
				// user Lastname
				dis.read(size);
				dataSize = ByteBuffer.wrap(new byte[]{size[0],size[1],0,0}).order(ByteOrder.LITTLE_ENDIAN).getInt();
				if (dataSize != 0) {
					lastname = new byte[dataSize];
					dis.read(lastname);
				}
				// Nb of templates
				int nbTemplate = (int) dis.readByte();
				for (int i = 0; i < nbTemplate; ++i) {
					dis.read(size);
					dataSize = ByteBuffer.wrap(new byte[]{size[0],size[1],0,0}).order(ByteOrder.LITTLE_ENDIAN).getInt();
					if (dataSize != 0) {
						template = new byte[dataSize];
						dis.read(template);
						if(isPrivacy) {
							byte[] templateDataCrypted = msoDemo.encryptPrivacyData(template, key, random);
							userData.addTemplateData(templateDataCrypted);
						}else{
							userData.addTemplateData(template);
						}
						
					}
				}

				userData.setNbFinger(nbTemplate);
				userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_COMP);
				// user id
				if (userID != null) {
					userData.setUserID(new String(userID));
				}
				// first name
				if (firstname != null) {
					userData.setFirstName(new String(firstname));
				}
				// last name
				if (lastname != null) {
					userData.setLastName(new String(lastname));
				}
			} else if (templaeFilePath.endsWith(".tkb")) {
				if(fileSize < TKBHeader.tKBHeaderSize) {
					DialogUtils.showErrorMessage("MSODemo", "bad tkb file : " + templaeFilePath);
					dis.close();
					is.close();
					return userData;
				}

				// the .tkb file structure :
				// _ the .tkb Header File (see the TKBHeader)
				// _ the X984 Biometric Token
				// _ the X509 Mso certificate

				int magicNb = dis.readInt();
				int version = dis.readInt();

				//LITTLE_ENDIAN
				byte[] buffer = new byte[4];
				dis.read(buffer);
				int sizeTkb = ByteBuffer.wrap(buffer).order(ByteOrder.LITTLE_ENDIAN).getInt();
				dis.read(buffer);
				int sizeCertif = ByteBuffer.wrap(buffer).order(ByteOrder.LITTLE_ENDIAN).getInt();

				if(magicNb != TKBHeader.magicNbInt || version != TKBHeader.versionInt) {
					DialogUtils.showErrorMessage("MSODemo", "bad tkb file : " + templaeFilePath);
					dis.close();
					is.close();
					return userData;
				}

				MorphoTemplateList templateList = new MorphoTemplateList();
				byte[] x984Data = new byte[sizeTkb];
				dis.read(x984Data);

				byte[] certificateData = new byte[sizeCertif];
				dis.read(certificateData);
				userData.setPkX984Certificate(certificateData);

				ArrayList<Byte> applicationData = new ArrayList<Byte>();
				int ret = templateList.extractDataX984(x984Data, applicationData);
				if (ret != MorphoSmartSDK.MORPHO_OK || applicationData.size() == 0) {
					DialogUtils.showErrorMessage("MSODemo", "An error occured while calling MorphoTemplateList.extractDataX984() function", ret, 0);
					dis.close();
					is.close();
					return userData;
				}

				int magicNb2 = ByteBuffer.wrap(	new byte[] {
													applicationData.get(0),
													applicationData.get(1),
													applicationData.get(2),
													applicationData.get(3)
												} ).getInt();

				if(magicNb2 != TKBHeader.magicNbInt) {
					userData.setUserID("Unknown data");
					userData.setFirstName("Unknown data");
					userData.setLastName("Unknown data");
				} else {
					byte [] data = new byte[applicationData.size()];
					for(int i=0;i<applicationData.size();++i)
					{
						data[i] = applicationData.get(i);
					}
					// Format of the data :
					// - Magic Number	: 4 bytes
					// - Size ID		: 2 bytes
					// - ID				: <Size ID> bytes
					// - Size Firstname : 2 bytes
					// - Firstname		: <Size Firstname> bytes
					// - Size Lastname	: 2 bytes
					// - Lastname		: <Size Lastname> bytes

					int offset = 4;
					int size = ByteBuffer.wrap(new byte[] {0,0,data[offset+1],data[offset]}).getInt();
					offset += 2;
					userData.setUserID(new String(Arrays.copyOfRange(data, offset, offset + size)));
					offset += size;

					size = ByteBuffer.wrap(new byte[] {0,0,data[offset+1],data[offset]}).getInt();
					offset += 2;
					userData.setFirstName(new String(Arrays.copyOfRange(data, offset, offset + size)));
					offset += size;

					size = ByteBuffer.wrap(new byte[] {0,0,data[offset+1],data[offset]}).getInt();
					offset += 2;
					userData.setLastName(new String(Arrays.copyOfRange(data, offset, offset + size)));
				}
				userData.setPkX984TemplateData(x984Data);
				userData.setNbFinger(1);

			} else {
				userData.setNbFinger(1);
				dis.read(templateData);
				
				if(isPrivacy) {
					
					byte[] templateDataCrypted = msoDemo.encryptPrivacyData(templateData, key, random);
					userData.addTemplateData(templateDataCrypted);
				}else{
					userData.addTemplateData(templateData);
				}
				if (templaeFilePath.endsWith("pkcn")) {
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_COMP_NORM);
				} else if (templaeFilePath.endsWith("pkm") || templaeFilePath.endsWith("pkmat")) {
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_MAT);
				} else if (templaeFilePath.endsWith("pkmn")) {
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_MAT_NORM);
				} else if (templaeFilePath.endsWith("minex-a")) {
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_MINEX_A);
				} else if (templaeFilePath.endsWith("iso-fmc-ns")) {
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_ISO_FMC_NS);
				} else if (templaeFilePath.endsWith("iso-fmc-cs")) {
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_ISO_FMC_CS);
				} else if (templaeFilePath.endsWith("iso-fmr")) {
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_ISO_FMR);
				} else if (templaeFilePath.endsWith("iso-fmr-2011")) {
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_ISO_FMR_2011);
				} else if (templaeFilePath.endsWith("pkV10")) {
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_V10);
				} else if (templaeFilePath.endsWith("din-cs")) {
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_DIN_V66400_CS);
				} else if (templaeFilePath.endsWith("din-cs-aa")) {
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_DIN_V66400_CS_AA);
				} else if (templaeFilePath.endsWith("fvp")) {
					userData.setMorphoFVPTemplateType(MorphoFVPTemplateType.MORPHO_PK_FVP);
				} else if (templaeFilePath.endsWith("fvp-m")) {
					userData.setMorphoFVPTemplateType(MorphoFVPTemplateType.MORPHO_PK_FVP_MATCH);
				} else if (templaeFilePath.endsWith("pkl") || templaeFilePath.endsWith("pklite")) {
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_PKLITE);
				} else if(templaeFilePath.endsWith("ansi-fmr")) {
					int nbFinger = templateData[MorphoInfo.ANSI_FMR_378_PK_NUMBER_OFFSET];
					userData.setNbFinger(nbFinger);
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_ANSI_378);
				} else if(templaeFilePath.endsWith("ansi-fmr-2009")) {
					int nbFinger = templateData[MorphoInfo.ANSI_FMR_378_2009_PK_NUMBER_OFFSET];
					userData.setNbFinger(nbFinger);
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_ANSI_378_2009);
			 	} else { // MORPHO_PK_COMP
					userData.setMorphoTemplateType(MorphoTemplateType.MORPHO_PK_COMP);
				}
			}

			is.close();
			dis.close();
		} catch (FileNotFoundException e) {
			DialogUtils.showErrorMessage("Add User", e.getMessage());
		} catch (EOFException e) {
			DialogUtils.showErrorMessage("Add User", "Erro while reading template file !");
		}
		catch (IOException e) {
			DialogUtils.showErrorMessage("Add User", e.getMessage());
		}

		return userData;
	}

}
