package morpho.morphosmart.sdk.applet.demo;

import java.awt.Color;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.SwingConstants;
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;
import javax.swing.filechooser.FileFilter;

import morpho.morphosmart.sdk.api.IMorphoEventHandler;
import morpho.morphosmart.sdk.api.MorphoCallbackCommand;
import morpho.morphosmart.sdk.api.MorphoCompressAlgo;
import morpho.morphosmart.sdk.api.MorphoFAR;
import morpho.morphosmart.sdk.api.MorphoFVPTemplateType;
import morpho.morphosmart.sdk.api.MorphoImage;
import morpho.morphosmart.sdk.api.MorphoSmartSDK;
import morpho.morphosmart.sdk.api.MorphoTemplateList;
import morpho.morphosmart.sdk.api.MorphoTemplateType;
import morpho.morphosmart.sdk.api.MorphoUser;
import morpho.morphosmart.sdk.applet.ennum.EnumExportImage;
import morpho.morphosmart.sdk.applet.ennum.EnumMoveFinger;
import morpho.morphosmart.sdk.applet.ennum.MorphoUserFields;
import morpho.morphosmart.sdk.applet.ennum.TemplateFVPType;
import morpho.morphosmart.sdk.applet.ennum.TemplateType;
import morpho.morphosmart.sdk.applet.trt.DialogUtils;
import morpho.morphosmart.sdk.applet.trt.ErrorsMgt;
import morpho.morphosmart.sdk.applet.trt.FilesMgt;
import morpho.morphosmart.sdk.applet.trt.ImageLoader;
import morpho.morphosmart.sdk.applet.trt.MorphoEventHandler;
import morpho.morphosmart.sdk.applet.trt.MorphoInfo;
import morpho.morphosmart.sdk.applet.trt.TemplateFileFilter;
import morpho.morphosmart.sdk.applet.trt.UserData;
import morpho.morphosmart.sdk.applet.trt.UsersMgt;

public class PanelLive extends JPanel {

	static final long serialVersionUID = 1L;

	// UI variables
	JButton btnStartStop = new JButton("Start");
	JLabel lblCurrentImageInfo;
	JLabel lblLiveImage;
	JLabel lblSteps;
	JLabel lblScore;
	JLabel lblInstruction;
	JProgressBar progressBar;
	// border
	Border lowerBorder = BorderFactory.createBevelBorder(BevelBorder.LOWERED);
	Border grayBorder = BorderFactory.createLineBorder(Color.GRAY);
	
	private MsoDemo msoDemo = null;

	private int currentTabIndex = 0;
	
	// images
	private Icon iconArrowUp	= new ImageIcon(ImageLoader.load("arrow_up.png"));
	private Icon iconArrowRight	= new ImageIcon(ImageLoader.load("arrow_right.png"));
	private Icon iconArrowDown	= new ImageIcon(ImageLoader.load("arrow_down.png"));
	private Icon iconArrowLeft	= new ImageIcon(ImageLoader.load("arrow_left.png"));
	
	/**
	 * Create the panel.
	 * @param msoDemo 
	 * @param  
	 */
	public PanelLive(MsoDemo msoDemo) {
		this.msoDemo = msoDemo;
		// label
		lblCurrentImageInfo = new JLabel(".");
		lblCurrentImageInfo.setBorder(lowerBorder);
		lblLiveImage = new JLabel("", SwingConstants.CENTER);
		lblLiveImage.setBorder(lowerBorder);
		lblInstruction = new JLabel("", SwingConstants.CENTER);
		lblInstruction.setBorder(grayBorder);
		lblSteps = new JLabel("", SwingConstants.CENTER);
		lblSteps.setBorder(grayBorder);
		lblSteps.setOpaque(true);
		lblSteps.setBackground(Color.WHITE);
		lblScore = new JLabel("", SwingConstants.CENTER);
		lblScore.setBorder(grayBorder );
		// progress
		progressBar = new JProgressBar();
		progressBar.setOrientation(SwingConstants.VERTICAL);
		progressBar.setMaximum(150);
		
		GroupLayout groupLayout = new GroupLayout(this);
		groupLayout.setHorizontalGroup(
			groupLayout.createParallelGroup(Alignment.TRAILING)
				.addGroup(groupLayout.createSequentialGroup()
					.addContainerGap()
					.addGroup(groupLayout.createParallelGroup(Alignment.TRAILING, false)
						.addComponent(lblCurrentImageInfo, Alignment.LEADING, GroupLayout.PREFERRED_SIZE, 316, GroupLayout.PREFERRED_SIZE)
						.addGroup(groupLayout.createSequentialGroup()
							.addComponent(lblLiveImage, GroupLayout.PREFERRED_SIZE, 140, GroupLayout.PREFERRED_SIZE)
							.addGap(18)
							.addGroup(groupLayout.createParallelGroup(Alignment.LEADING, false)
								.addComponent(progressBar, GroupLayout.PREFERRED_SIZE, 35, GroupLayout.PREFERRED_SIZE)
								.addComponent(lblScore, GroupLayout.PREFERRED_SIZE, 35, GroupLayout.PREFERRED_SIZE))
							.addGap(18)
							.addGroup(groupLayout.createParallelGroup(Alignment.LEADING, false)
								.addComponent(lblInstruction, GroupLayout.PREFERRED_SIZE, 105, GroupLayout.PREFERRED_SIZE)
								.addComponent(btnStartStop, GroupLayout.PREFERRED_SIZE, 105, GroupLayout.PREFERRED_SIZE)
								.addComponent(lblSteps, GroupLayout.PREFERRED_SIZE, 105, GroupLayout.PREFERRED_SIZE))))
					.addGap(100))
		);
		groupLayout.setVerticalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addContainerGap()
					.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
						.addComponent(lblLiveImage, GroupLayout.PREFERRED_SIZE, 186, GroupLayout.PREFERRED_SIZE)
						.addGroup(groupLayout.createSequentialGroup()
							.addGroup(groupLayout.createParallelGroup(Alignment.TRAILING, false)
								.addComponent(btnStartStop, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
								.addComponent(lblScore, GroupLayout.DEFAULT_SIZE, 35, Short.MAX_VALUE))
							.addPreferredGap(ComponentPlacement.RELATED)
							.addGroup(groupLayout.createParallelGroup(Alignment.LEADING, false)
								.addGroup(groupLayout.createSequentialGroup()
									.addComponent(lblSteps, GroupLayout.PREFERRED_SIZE, 105, GroupLayout.PREFERRED_SIZE)
									.addPreferredGap(ComponentPlacement.UNRELATED)
									.addComponent(lblInstruction, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
								.addComponent(progressBar, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))))
					.addGap(18)
					.addComponent(lblCurrentImageInfo)
					.addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
		);
		groupLayout.linkSize(SwingConstants.HORIZONTAL, new Component[] {btnStartStop, lblInstruction, lblSteps});
		groupLayout.linkSize(SwingConstants.HORIZONTAL, new Component[] {lblScore, progressBar});
		setLayout(groupLayout);
		
		btnStartStop.addActionListener(btnStartActionListener);
	}
	
	// Button start listener
		private ActionListener btnStartActionListener = new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				Thread startStopThread = new Thread(new Runnable() {					

					@Override
					public void run() {
						int ret = 0;
						// Button 'Start' is clicked
						if (btnStartStop.getText().equals("Start")) {
							
							if(msoDemo.isTabEnroll() && msoDemo.getTabEnroll().textIdNumber.getText().isEmpty())
							{
								DialogUtils.showWarningMessage("MSO_Demo Java", "You must at least type an ID.");
								return;
							}
														
							// save the current tab index
							currentTabIndex = msoDemo.getSelectedTabIndex();
							
							
							if(msoDemo.isTabEnroll())
							{
								msoDemo.addTabProcess();
							}
														
							// change button text
							btnStartStop.setText("Stop");

							switch (currentTabIndex) {
								case 0:
									verify();
									break;
								case 1:
									enroll();
									break;
								default: break;
							}
						}
						// Button 'Stop' is clicked
						else if (btnStartStop.getText().equals("Stop")) {
							ret = msoDemo.mDevice.cancelLiveAcquisition();
							if (ret != MorphoSmartSDK.MORPHO_OK)
								DialogUtils.showErrorMessage("Error", "Error in CancelLiveAcquisition");
						}
						// finish capture process
						msoDemo.finishCapture(currentTabIndex);
						clearLive();
						btnStartStop.setText("Start");
					}
				});
				startStopThread.start();
			}
		};
		
		private int verify() {
			if (msoDemo.getTabVerify().rbFileSimulation.isSelected()) {
				return verityWithFile();
			} else {
				return verifyInBase();
			}			
		}	
		
		private int verifyInBase() {
			String selectedUserID = msoDemo.panelRight.getSelectedUserId();
			if (selectedUserID.equals("")) {
				DialogUtils.showWarningMessage("MSO_Demo", "Select a user in the list view.");
				return 0;
			}
			int ret = 0;
			// get user data
			MorphoUser mUser = new MorphoUser();
			ret = msoDemo.mDatabase.getUser(selectedUserID, mUser);
			if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("MSO_Demo", "An error occured while calling MorphoDatabase.getUser() function");
				return ret;
			}
			// timeout
			int timeout = 0;
			// callback mask
			int callbackMask = msoDemo.callbackMask;
			callbackMask &= ~MorphoCallbackCommand.MORPHO_CALLBACK_ENROLLMENT_CMD.swigValue();
			// FAR matchingThreshold
			MorphoFAR far = MorphoFAR.MORPHO_FAR_5;
			// coderChoice
			int coderChoice = 0;
			// detectModeChoice
			int detectModeChoice = 1;			
			// security Level
			msoDemo.mDevice.setSecurityLevel(msoDemo.getSecurityLevel(0));
			// callback function
			IMorphoEventHandler mEventHandlerCallback = new MorphoEventHandler(msoDemo.tabProcess,false);
			// matching
			long matchingStrategy = 0;
			// matchingScore
			long[] matchingScore = {0};
			
			if(msoDemo.isTUNNELING() || msoDemo.isOFFERED_SECURITY())
			{
				matchingScore = null;
			}
			
			// exportNumPk
			byte[] exportNumPk = {0};
			

			// Verify process
			ret = mUser.verify(timeout, far, callbackMask, mEventHandlerCallback, matchingScore,
					exportNumPk, coderChoice, detectModeChoice, matchingStrategy);

			// response
			String result = "User authenticated";
			String msg1 = "";
			String msg2 = "";
			String msg3 = "";
			String msg4 = "";
			if (ret == MorphoSmartSDK.MORPHO_OK) {
				ArrayList<String> firstName = new ArrayList<String>();
				ArrayList<String> lastName = new ArrayList<String>();
				mUser.getField(1, firstName);
				mUser.getField(2, lastName);
				msg1 = "Firstname : " + firstName.get(0);
				msg2 = "Lastname : " + lastName.get(0);
				msg3 = "ID : " + selectedUserID;
				msg4 = "Matching Pk Number : " + exportNumPk[0];				
				DialogUtils.showInfoMessage(result, msg1 + "\n" + msg2 + "\n" + msg3+ "\n" +msg4);
			} else {
				result = "Verification Failed";
				switch (ret) {
				case MorphoSmartSDK.MORPHOERR_CMDE_ABORTED:
					msg1 = "Command Aborted.";
					break;
				case MorphoSmartSDK.MORPHOERR_NO_HIT:
					msg1 = "Person not Authenticated.";
					break;
				case MorphoSmartSDK.MORPHOERR_TIMEOUT:
					msg1 = "Timeout has expired.";
					msg2 = "Command Aborted.";
					break;
				case MorphoSmartSDK.MORPHOERR_FFD:
					msg1 = "False finger detected !!!";
					break;
				case MorphoSmartSDK.MORPHOERR_MOIST_FINGER:
					msg1 = "Finger too moist !!!";
					break;
				case MorphoSmartSDK.MORPHOERR_MOVED_FINGER :
					msg1 = "Finger moved !!!";
					break;
				case MorphoSmartSDK.MORPHOERR_SATURATED_FINGER :
					msg1 = "Finger saturated !!!";
					break;
				case MorphoSmartSDK.MORPHOERR_INVALID_FINGER :
					msg1 = "Invalid finger !!!";
					break;
				case MorphoSmartSDK.MORPHOERR_NOT_IMPLEMENTED:
					msg1 = "Not Implemented !!!";
					break;
				case MorphoSmartSDK.MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN:
					msg1 = "Finger is misplaced or has been";
					msg2 = "withdrawn from sensor during acquisition.";
					break;
				case MorphoSmartSDK.MORPHOERR_BADPARAMETER:
					msg1 = "One or more input parameters are out of range";
					break;
				default:
					msg1 = "An error occured while calling";
					msg2 = "MorphoDatabase.Verify() function";
					msg3 = ErrorsMgt.convertSDKError(ret);
					break;
				}
				DialogUtils.showErrorMessage(result, msg1 + "\n" + msg2 + "\n" + msg3+ "\n" +msg4);
			}

			return ret;
		}

		private int verityWithFile() {
			int ret = 0;
			boolean isTkbFile = false;
			ArrayList<String> templateListArray = FilesMgt.getTemplateFiles("Verify", true, msoDemo);
			if(templateListArray.size() == 0)
				return ret;
			else if (templateListArray.size() > 20) {
				DialogUtils.showWarningMessage("Mso_Demo", "The number of templates must be less or equal to 20.");
				return ret;
			}
			// Get templates from selected files
			MorphoTemplateList morphoTemplateList = new MorphoTemplateList();
			List<UserData> usersData = new ArrayList<UserData>();
			if (templateListArray.size() > 0) {
				for (int i = 0; i < templateListArray.size(); ++i) {
					short[] indexTemplate = {0};
					String filePath = templateListArray.get(i);
					if (i == 0) {
						if (filePath.endsWith(".tkb"))
							isTkbFile = true;
					} else {
						if ((isTkbFile && !filePath.endsWith(".tkb")) || (!isTkbFile && filePath.endsWith(".tkb"))) {
							DialogUtils.showWarningMessage("Mso_Demo", "You can not mix tkb template with standard templates.");
							return ret;
						}
					}
					if(isTkbFile && templateListArray.size() > 1) {
						DialogUtils.showWarningMessage("Mso_Demo", "You cannot use more than one tkb file.");
						return ret;
					}

					UserData userData = UsersMgt.getUserDataFromFile(filePath);
					if(userData.getNbFinger() == 0) return -1;
					usersData.add(userData);

					if(isTkbFile) {
						ret = morphoTemplateList.putX984(userData.getPkX984TemplateData());
						if (ret != MorphoSmartSDK.MORPHO_OK) {
							DialogUtils.showErrorMessage("Verify", "An error occured while calling MorphoTemplateList.putX984() function", ret, msoDemo.mDevice.getInternalError());
							return ret;
						}
					} else {
						String fct = "";
						if (userData.getMorphoTemplateType() != MorphoTemplateType.MORPHO_NO_PK_FP) {
							ret = morphoTemplateList.putTemplate(userData.getMorphoTemplateType(), userData.getTemplateData(0), (short) 0xFF, (short) 0, indexTemplate);
							fct = "putTemplate";
						} else if (userData.getMorphoFVPTemplateType() != MorphoFVPTemplateType.MORPHO_NO_PK_FVP) {
							ret = morphoTemplateList.putFVPTemplate(userData.getMorphoFVPTemplateType(), userData.getTemplateData(0), (short) 0, (short) 0, indexTemplate);
							fct = "putFVPTemplate";
						}

						if (ret != MorphoSmartSDK.MORPHO_OK) {
							DialogUtils.showErrorMessage("Verify", "An error occured while calling MorphoTemplateList." + fct + "() function", ret, msoDemo.mDevice.getInternalError());
							return ret;
						}
					}
				}
			}
			// timeout
			int timeout = 0;
			// callback mask
			int callbackMask = msoDemo.callbackMask;
			callbackMask &= ~MorphoCallbackCommand.MORPHO_CALLBACK_ENROLLMENT_CMD.swigValue();
			// FAR matchingThreshold
			MorphoFAR far = MorphoFAR.MORPHO_FAR_5;
			// coderChoice
			int coderChoice = 0;
			// detectModeChoice
			int detectModeChoice = 1;		
			// security Level
			msoDemo.mDevice.setSecurityLevel(msoDemo.getSecurityLevel(0));
			// callback function
			IMorphoEventHandler mEventHandlerCallback = new MorphoEventHandler(msoDemo.tabProcess,false);
			// matching
			long matchingStrategy = 0;
			// matchingScore
			long[] matchingScore = {0};
			// exportNumPk
			byte[] exportNumPk = {0};
			
			if(msoDemo.isTUNNELING() || msoDemo.isOFFERED_SECURITY())
			{
				matchingScore = null;
			}
			// verify
			ret = msoDemo.mDevice.verify(timeout, far, morphoTemplateList, callbackMask, mEventHandlerCallback,
					matchingScore, exportNumPk, coderChoice, detectModeChoice, matchingStrategy);

			// response process
			String msg1 = "";
			String msg2 = "";
			String msg3 = "";
			String msg4 = "";
			if (ret != MorphoSmartSDK.MORPHO_OK) {
				switch(ret) {
					case MorphoSmartSDK.MORPHOERR_CMDE_ABORTED:
						msg1 = "Command Aborted.";
						break;
					case MorphoSmartSDK.MORPHOERR_NO_HIT:
						msg1 = "Person not Authenticated.";
						break;
					case MorphoSmartSDK.MORPHOERR_TIMEOUT:
						msg1 = "Timeout has expired.";
						msg2 = "Command Aborted.";
						break;
					case MorphoSmartSDK.MORPHOERR_BADPARAMETER:
						msg1 = "One or more input parameters are out of range";
						break;
					case MorphoSmartSDK.MORPHOERR_FFD:
						msg1 = "False finger detected !!!";
						break;
					case MorphoSmartSDK.MORPHOERR_MOIST_FINGER:
						msg1 = "Finger too moist !!!";
						break;
					case MorphoSmartSDK.MORPHOERR_MOVED_FINGER :
						msg1 = "Finger moved !!!";
						break;
					case MorphoSmartSDK.MORPHOERR_SATURATED_FINGER :
						msg1 = "Finger saturated !!!";
						break;
					case MorphoSmartSDK.MORPHOERR_INVALID_FINGER :
						msg1 = "Invalid finger !!!";
						break;
					case MorphoSmartSDK.MORPHOERR_NOT_IMPLEMENTED:
						msg1 = "Not Implemented !!!";
						break;
					case MorphoSmartSDK.MORPHOERR_FVP_MINUTIAE_SECURITY_MISMATCH:
						msg1 = "An error occured while calling";
						msg1 = "MorphoDatabase.Verify() function";
						msg3 = "MORPHOERR_FVP_MINUTIAE_SECURITY_MISMATCH.";
						break;
					case MorphoSmartSDK.MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN:
						msg1 = "Finger is misplaced or has been";
						msg2 = "withdrawn from sensor during acquisition.";
						break;
					case MorphoSmartSDK.MORPHOERR_LICENSE_MISSING :
						msg1 = "A required license is missing.";
						break;
					case MorphoSmartSDK.MORPHOERR_CERTIF_UNKNOW:
						//the MSO can not verify the X984 biometric token, the MSO does not have the certificate.
						//the new certificate is stored in the MSO.
						//the new X984 biometric token can be used.
						// TODO extractAndStoreCertif(fileName);
						DialogUtils.showWarningMessage("Verify", "X984 biometric token has just been stored in the MSO.\nPlease redo the Verify process.");
						break;
					default:
						msg1 = "An error occured while calling";
						msg2 = "MorphoDatabase.Verify() function";
						msg3 = ErrorsMgt.convertSDKError(ret);
						break;
				}
				DialogUtils.showErrorMessage("Verification Failed", msg1 + "\n" + msg2 + "\n" + msg3+ "\n" +msg4);
				
			} else {
				// Get authenticated user index
				int indexUser = -1;
				if(exportNumPk[0] >= 0 && exportNumPk[0] < 20) {
					indexUser = exportNumPk[0];
				}
				msg4 = "Matching Pk Number : " + exportNumPk[0];
				// Get user details
				if (indexUser != -1) {
					if(isTkbFile) indexUser = 0;
					UserData userdata = usersData.get(indexUser);
					msg1 = "Firstname : " + userdata.getFirstName();
					msg2 = "Lastname : " + userdata.getLastName();
					msg3 = "ID : " + userdata.getUserID();
					String fileName = FilesMgt.getFileNameFromPath(templateListArray.get(indexUser));
					msg4 = "Matching File : [" + fileName + "]";
				} else {
					msg2 = "You should enable the";
					msg3 = "\"Export Matching Pk Number\"";
					msg4 = "to retrieve the information about the matching file";
				}
				DialogUtils.showErrorMessage("User authenticated", msg1 + "\n" + msg2 + "\n" + msg3+ "\n" +msg4);
			}

			return ret;
		}

		private void clearLive() {
			lblLiveImage.setIcon(null);
			lblCurrentImageInfo.setText(".");
			setScore((short) 0);
			setInstruction("");
		}
		
		private int enroll() {			
			int ret = 0;
			// timeout
			int timeout = 0;
			// callback
			int callbackCmd = msoDemo.callbackMask;
			// acquisitionThreshold
			short acquisitionThreshold = 0;
			// advancedSecurityLevelsRequired
			short advancedSecurityLevelsRequired = 0;
			// coderChoice
			int coderChoice = 0;
			// detectModeChoice
			int detectModeChoice = 0;			
			// StrategyAcquisitionMode			
			IMorphoEventHandler mEventHandlerCallback = new MorphoEventHandler(msoDemo.tabProcess,true);
			// security Level
			msoDemo.mDevice.setSecurityLevel(msoDemo.getSecurityLevel(currentTabIndex));
			
			MorphoCompressAlgo compressAlgo = MorphoCompressAlgo.MORPHO_NO_COMPRESS;
			short compressRate = 0;
			short exportMinutiae = 0;
			short fingerNumber = (short) (msoDemo.getTabEnroll().rbNbOfFinger2.isSelected()? 2 : 1);
			MorphoTemplateType fpTemplateType = MorphoTemplateType.MORPHO_NO_PK_FP;
			MorphoFVPTemplateType fvpTemplateType = MorphoFVPTemplateType.MORPHO_NO_PK_FVP;
			// save PK in Database
			short saveRecord = (short) (msoDemo.getTabEnroll().cbSavePkInDB.isSelected()? 1 : 0);

			// FP Template
			String selectedTemplateType = (String) msoDemo.getTabEnroll().comboFpTemplateType.getSelectedItem();
			TemplateType templateFp = TemplateType.getValue(selectedTemplateType);

			if(templateFp != TemplateType.MORPHO_PK_SAGEM_PKS)
			{
				fpTemplateType = MorphoTemplateType.swigToEnum(templateFp.getCode());
				if (fpTemplateType == MorphoTemplateType.MORPHO_NO_PK_FP) {
					if (!MorphoInfo.isFVP) {
						fpTemplateType = MorphoTemplateType.MORPHO_PK_COMP; //	MSO_Demo always export templates MORPHO_PK_COMP
					}
				}
			}
			else
			{
				fpTemplateType = MorphoTemplateType.MORPHO_PK_COMP;
			}

			// FVP Template
			String selectedFVPTemplateType = (String) msoDemo.getTabEnroll().comboFvpTemplateType.getSelectedItem();
			TemplateFVPType templateFvp = TemplateFVPType.getValue(selectedFVPTemplateType);

			fvpTemplateType = (MorphoInfo.isFVP) ? MorphoFVPTemplateType.MORPHO_PK_FVP : MorphoFVPTemplateType.MORPHO_NO_PK_FVP;
			// exportMinutiae
			if (fpTemplateType != MorphoTemplateType.MORPHO_NO_PK_FP || fvpTemplateType != MorphoFVPTemplateType.MORPHO_NO_PK_FVP)
			{
				exportMinutiae = 1;
			}
			// output template
			MorphoTemplateList templateList = new MorphoTemplateList();
			templateList.setActiveFullImageRetrieving(false);
			// Image acquisition enabled
			if (msoDemo.getTabEnroll().comboExportImage.getSelectedIndex() != 0) { // NO_IMAGE
				templateList.setActiveFullImageRetrieving(true);
				String exportImage = msoDemo.getTabEnroll().comboExportImage.getSelectedItem().toString();
				if(exportImage.equals(EnumExportImage.WSQ.toString())) {
					compressAlgo = MorphoCompressAlgo.MORPHO_COMPRESS_WSQ;
					compressRate = 15;
				} else if(exportImage.equals(EnumExportImage.RAW.toString())) {
					compressAlgo = MorphoCompressAlgo.MORPHO_NO_COMPRESS;
					compressRate = 0;
				} else if(exportImage.equals(EnumExportImage.SAGEM_V1.toString())) {
					compressAlgo = MorphoCompressAlgo.MORPHO_COMPRESS_V1;
					compressRate = 0;
				}
			}
			// checking parameters
			if (!templateList.getActiveFullImageRetrieving() &&
					saveRecord == 0 &&
					fpTemplateType == MorphoTemplateType.MORPHO_NO_PK_FP &&
					fvpTemplateType == MorphoFVPTemplateType.MORPHO_NO_PK_FVP)
			{
				ret = MorphoSmartSDK.MORPHOERR_BADPARAMETER;
				return ret;
			}

			// create a new user
			MorphoUser user		= new MorphoUser();
			String userID		= msoDemo.getTabEnroll().textIdNumber.getText();
			String firstName	= msoDemo.getTabEnroll().textFirstName.getText();
			String lastName		= msoDemo.getTabEnroll().textLastName.getText();

			ret = msoDemo.mDatabase.getUser(userID, user);
			if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("MSO_Demo", "An error occured while calling MorphoDatabase.getUser() function", ret, msoDemo.mDevice.getInternalError());
				return ret;
			}
			// adding user fields
			ret = user.putField(MorphoUserFields.FIRSTNAME_FIELD_INDEX.getValue(), firstName);
			if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("MSO_Demo", "An error occured while calling MorphoUser.putField() function", ret, msoDemo.mDevice.getInternalError());
				return ret;
			}
			ret = user.putField(MorphoUserFields.LASTTNAME_FIELD_INDEX.getValue(), lastName);
			if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("MSO_Demo", "An error occured while calling MorphoUser.putField() function", ret, msoDemo.mDevice.getInternalError());
				return ret;
			}
			// Prepare finger update if necessary
			if (saveRecord == 1 && msoDemo.getTabEnroll().cbUpdateTemplate.isSelected()) {
				if (msoDemo.getTabEnroll().rbNbOfFinger2.isSelected()) // Update both fingers
					ret = user.setTemplateUpdateMask(3);
				else if (msoDemo.getTabEnroll().comboFingerIndex.getSelectedIndex() == 0) // Update first finger only
					ret = user.setTemplateUpdateMask(1);
				else // Update second finger
					ret = user.setTemplateUpdateMask(2);
			}

			if(ret == MorphoSmartSDK.MORPHO_OK)
			{
			ret = user.enroll(timeout, acquisitionThreshold, advancedSecurityLevelsRequired, compressAlgo ,
					compressRate , exportMinutiae, fingerNumber, fpTemplateType, fvpTemplateType, saveRecord ,
					callbackCmd, mEventHandlerCallback, coderChoice, detectModeChoice, templateList);
			}
			
			String result = "";
			String msg1 = "";
			String msg2 = "";
			String msg3 = "";
			String msg4 = "";
			if (ret != MorphoSmartSDK.MORPHO_OK) {
				result = "Enrollment failed";
				switch(ret) {
					case MorphoSmartSDK.MORPHOERR_CMDE_ABORTED:
						msg1 = "Command Aborted.";
						break;
					case MorphoSmartSDK.MORPHOERR_NO_HIT :
						msg1 = "Bad Capture Sequence.";
						break;
					case MorphoSmartSDK.MORPHOERR_ALREADY_ENROLLED :
						msg1 = "User already enrolled.";
						break;
					case MorphoSmartSDK.MORPHOERR_INVALID_USER_ID :
						msg1 = "ID " + userID + " already used.";
						break;
					case MorphoSmartSDK.MORPHOERR_TIMEOUT:
						msg1 = "Timeout has expired.";
						msg2 = "Command aborted.";
						break;
					case MorphoSmartSDK.MORPHOERR_DB_FULL :
						msg1 = "Cannot enroll more users.";
						msg2 = "Database is full.";
						break;
					case MorphoSmartSDK.MORPHOERR_FFD :
						msg1 = "False finger detected !!!";
						break;
					case MorphoSmartSDK.MORPHOERR_MOIST_FINGER :
						msg1 = "Finger too moist !!!";
						break;
					case MorphoSmartSDK.MORPHOERR_MOVED_FINGER :
						msg1 = "Finger moved !!!";
						break;
					case MorphoSmartSDK.MORPHOERR_SATURATED_FINGER :
						msg1 = "Finger saturated !!!";
						break;
					case MorphoSmartSDK.MORPHOERR_INVALID_FINGER :
						msg1 = "Invalid finger !!!";
						break;
					case MorphoSmartSDK.MORPHOERR_BADPARAMETER :
						msg1 = "One or more input parameters are out of range";
						break;
					case MorphoSmartSDK.MORPHOERR_LICENSE_MISSING :
						msg1 = "A required license is missing.";
						break;
					case MorphoSmartSDK.MORPHOERR_ADVANCED_SECURITY_LEVEL_MISMATCH :
						msg1 = "Failed to make a multimodal template compatible with advanced security levels.";
						break;
					case MorphoSmartSDK.MORPHOERR_BAD_FINAL_FINGER_PRINT_QUALITY :
						msg1 = "Failed to reach the fingerprint quality threshold.";
						break;
					default:
						msg1 = "An error occured while calling";
						msg2 = "MorphoUser.enroll() function";
						msg3 = ErrorsMgt.convertSDKError(ret);
						break;
				}
				DialogUtils.showErrorMessage(result, msg1 + "\n" + msg2 + "\n" + msg3+ "\n" +msg4);
			} else { // enroll successful

				if(templateFp == TemplateType.MORPHO_PK_SAGEM_PKS)
				{
					if(MorphoInfo.isFVP && fvpTemplateType != MorphoFVPTemplateType.MORPHO_NO_PK_FVP)
					{
						saveFVPFiles("Enroll", msoDemo.getTabEnroll().textIdNumber.getText(), fvpTemplateType, templateList);
					}

					byte [] personnalInformation = getPersonnalInformation(msoDemo.getTabEnroll().textIdNumber.getText(),msoDemo.getTabEnroll().textFirstName.getText(),msoDemo.getTabEnroll().textLastName.getText());

					String pksFileName = msoDemo.getTabEnroll().textIdNumber.getText() + ".pks";
					ret = savePKSFile(pksFileName, personnalInformation, templateList);

					if(ret != 0)
					{
						DialogUtils.showInfoMessage("Enroll Successful", "Saving FP template file \n aborted by user.");
					}
					else
					{
						DialogUtils.showInfoMessage("Enroll Successful", "FP Template successfully" + "\n" + "exported in file" + "\n" + FilesMgt.fileChooserDirectory + "/" + pksFileName);
					}
				}
				else
				{
					saveTemplatesFiles("Enroll", msoDemo.getTabEnroll().textIdNumber.getText(), fingerNumber, templateFvp, templateFp, templateList);
				}

				// UpdateNumberOfRecords
				if (saveRecord == 1) {
					msoDemo.panelRight.loadUsers();
				}
				// Image acquisition enabled
				if(templateList.getActiveFullImageRetrieving())
				{
					for (int i = 0; i<fingerNumber; i++) {
						MorphoImage morphoImage = new MorphoImage();
						ret = templateList.getFullImageRetrieving((short)i, morphoImage);
						if (ret != MorphoSmartSDK.MORPHO_OK) {
							DialogUtils.showErrorMessage("Enroll Successful", "Error Image Unknown.");
							break;
						}
						String fileName = userID + "_finger_" + (i + 1);
						ret = saveImagesFiles("Enroll", fileName, morphoImage, compressAlgo, ret);
						if(ret != MorphoSmartSDK.MORPHO_OK)
						{
							break;
						}
					}
				}
			}
			return ret;
		}
		
		private int saveImagesFiles(String fonction, String imageFilePath, MorphoImage morphoImage, MorphoCompressAlgo cAlgorithm, int fonctionRet)
		{
			int ret = 0;

			JFileChooser fileChooser = new JFileChooser();
			fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));
			fileChooser.setSelectedFile(new File(imageFilePath));
			String extention = "";			

			if(cAlgorithm == MorphoCompressAlgo.MORPHO_NO_COMPRESS)
			{
				FileFilter ffRaw = new TemplateFileFilter("Raw Files (.raw"+extention+")", ".raw" + extention);
				fileChooser.addChoosableFileFilter(ffRaw);
				FileFilter ffBmp = new TemplateFileFilter("Bmp Files (.bmp"+extention+")", ".bmp" + extention);
				fileChooser.addChoosableFileFilter(ffBmp);
			}
			else if(cAlgorithm == MorphoCompressAlgo.MORPHO_COMPRESS_V1) // image V1
			{
				FileFilter ffSagemV1 = new TemplateFileFilter("Bin Files (.bin"+extention+")", ".bin" + extention);
				fileChooser.addChoosableFileFilter(ffSagemV1);
			}
			else // image WSQ
			{
				FileFilter ffWsq = new TemplateFileFilter("Wsq Files (.Wsq"+extention+")", ".Wsq" + extention);
				fileChooser.addChoosableFileFilter(ffWsq);
			}

			String resulat = "";
			String message1 = "";
			String message2 = "";
			String message3 = "";
			String message4 = "";

			int userSelection = fileChooser.showSaveDialog(msoDemo);
			if (userSelection == JFileChooser.APPROVE_OPTION) {


				FilesMgt.fileChooserDirectory = fileChooser.getCurrentDirectory().getAbsolutePath();

				File fileToSave = fileChooser.getSelectedFile();
			    String savedFilePath = fileToSave.getAbsolutePath().toLowerCase();

			    String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
			    if(!savedFilePath.endsWith(ext)) {
			    	savedFilePath += ext;
			    }

			    // save into file
			    FileOutputStream fos;
				try {
					fos = new FileOutputStream(savedFilePath);

					if(cAlgorithm == MorphoCompressAlgo.MORPHO_NO_COMPRESS)
					{
						if(ext.equalsIgnoreCase(".bmp")) {
							fos.write(morphoImage.getBMPImage());
						} else {
							fos.write(morphoImage.getImage());
						}
					}
					else if(cAlgorithm == MorphoCompressAlgo.MORPHO_COMPRESS_V1) // image V1
					{
						fos.write(morphoImage.getCompressedImage());
					}
					else // image WSQ
					{
						fos.write(morphoImage.getCompressedImage());
					}

				    fos.close();

				    resulat = fonction +  " Successful";
				    message1 = "Image successfully";
				    message2 = "exported in file";
				    message3 = savedFilePath;

				    if(fonctionRet == MorphoSmartSDK.MORPHOERR_FFD)
					{
						message4 = "False finger saved.";
					}
					else if(fonctionRet == MorphoSmartSDK.MORPHOERR_MOIST_FINGER)
					{
						message4 = "Moist finger saved.";
					}
					else if(fonctionRet == MorphoSmartSDK.MORPHOERR_MOVED_FINGER)
					{
						message4 = "Moved finger saved.";
					}
					else if(fonctionRet == MorphoSmartSDK.MORPHOERR_SATURATED_FINGER)
					{
						message4 = "Saturated finger saved.";
					}
					else if(fonctionRet == MorphoSmartSDK.MORPHOERR_INVALID_FINGER)
					{
						message4 = "Invalid finger saved.";
					}

				    DialogUtils.showInfoMessage(resulat,message1 + "\n"+ message2 + "\n" + message3 + "\n" + message4);

				} catch (FileNotFoundException fnfe) {
					DialogUtils.showErrorMessage("Save Image File", fnfe.getMessage());
					ret = -1;
				} catch (IOException ioe) {
					DialogUtils.showErrorMessage("Save Image File", ioe.getMessage());
					ret = -1;
				}

				return 0;
			}
			else
			{
				resulat = fonction +  " Successful";
			    message1 = "Saving image file";
			    message2 = "aborted by user.";

			    if(fonctionRet == MorphoSmartSDK.MORPHOERR_FFD)
				{
			    	message3 = "Saving false finger failed.";
				}
				else if(fonctionRet == MorphoSmartSDK.MORPHOERR_MOIST_FINGER)
				{
					message3 = "Saving moist finger failed.";
				}

			    DialogUtils.showInfoMessage(resulat,message1 +"\n"+ message2 + "\n" + message3);
			}

			return ret;
		}
		
		private void saveTemplatesFiles(String fonction, String userID, int nbFinger, TemplateFVPType templateFvp, TemplateType templateFp, MorphoTemplateList templateList) {
			int ret = 0;
			boolean isQualityScoreNotified = false;
			String message1 = "";
		    String message2 = "";
		    String message3 = "";
		    String message4 = "";

		    String extenstion = "";		    

			for(int i=0;i<nbFinger;++i) {
				if(
					(MorphoInfo.isFVP && (templateFp == TemplateType.MORPHO_NO_PK_FP) && (templateFvp == TemplateFVPType.MORPHO_NO_PK_FVP)) ||
					(MorphoInfo.isFVP && templateFvp != TemplateFVPType.MORPHO_NO_PK_FVP)
				)
				{
					MorphoFVPTemplateType[] typTemplate = {MorphoFVPTemplateType.MORPHO_NO_PK_FVP};
					ArrayList<Byte> dataTemplate = new ArrayList<Byte>();
					short[] pkFVPQuality = {0};
					short[] advancedSecurityLevelsCompatibility = {0};
					short[] dataIndex = {0};
					ret = templateList.getFVPTemplate((short) i, typTemplate, dataTemplate, pkFVPQuality, advancedSecurityLevelsCompatibility, dataIndex);

					if(ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage(fonction,"An error occured while calling MorphoTemplateList.getFVPTemplate() function",ret,msoDemo.mDevice.getInternalError());
						return;
					}

					message1 = fonction + " Successful";
				    message2 = "Finger #"+(i+1)+" - Quality Score: " + pkFVPQuality[0];
				    message3 = "";				    
				    message4 = "";
				    DialogUtils.showInfoMessage(message1, message2 + "\n" + message3 + "\n" + message4);

				    isQualityScoreNotified = true;
				    if(templateFvp != TemplateFVPType.MORPHO_NO_PK_FVP) {
				    	String fvpFileName = "";
						FileFilter fvp;

						if(templateFvp == TemplateFVPType.MORPHO_PK_FVP) {
							fvpFileName = userID + "_finger_" + (i+1) + ".fvp";
							fvp = new TemplateFileFilter("SAGEM PkFVP (.fvp)", ".fvp");
						} else {
							fvpFileName = userID + "_finger_" + (i+1) + ".fvp-m";
							fvp = new TemplateFileFilter("SAGEM PkFVP Match (.fvp-m)", ".fvp-m");
						}

						JFileChooser fileChooser = new JFileChooser();
						fileChooser.setSelectedFile(new File(fvpFileName));
						fileChooser.addChoosableFileFilter(fvp);
						fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));
						int userSelection = fileChooser.showSaveDialog(msoDemo);
						if (userSelection == JFileChooser.APPROVE_OPTION) {
							FilesMgt.fileChooserDirectory = fileChooser.getCurrentDirectory().getAbsolutePath();
							File fileToSave = fileChooser.getSelectedFile();
						    String savedFilePath = fileToSave.getAbsolutePath();

						    String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
						    if(!savedFilePath.endsWith(ext)) {
						    	savedFilePath += ext;
						    }

						    // save into file
						    try {
						    	FileOutputStream fos;
						    	fos = new FileOutputStream(savedFilePath);
						    	byte[] data = new byte[dataTemplate.size()];
								for(int j=0;j<dataTemplate.size();j++) {
									data[j] = dataTemplate.get(j);
								}
								fos.write(data);
							    fos.close();

							    message2 = "FVP Template successfully";
							    message3 = "exported in file";
							    message4 = savedFilePath;							    
							    DialogUtils.showInfoMessage(message1, message2 + "\n" + message3 + "\n" + message4);
							} catch (FileNotFoundException fnfe) {
								DialogUtils.showErrorMessage("Save FVP File " + savedFilePath, fnfe.getMessage());
							} catch (IOException ioe) {
								DialogUtils.showErrorMessage("Save FVP File" + savedFilePath, ioe.getMessage());
							}
						} else {
							DialogUtils.showInfoMessage(fonction + " Successful", "Saving FVP template file" + "\n" + "aborted by user.");
						}
				    }
				}

				if(!MorphoInfo.isFVP || (MorphoInfo.isFVP && templateFp != TemplateType.MORPHO_NO_PK_FP)) {
					MorphoTemplateType[] typTemplate = {MorphoTemplateType.MORPHO_NO_PK_FP};
					ArrayList<Byte> dataTemplate = new ArrayList<Byte>();
					short[] dataIndex = {0};
					short[] pkFpQuality = {0};
					ret = templateList.getTemplate((short) i, typTemplate, dataTemplate, pkFpQuality , dataIndex);

					if(ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage(fonction,"An error occured while calling MorphoTemplateList.getTemplate() function",ret,msoDemo.mDevice.getInternalError());
						return;
					}

					if(!isQualityScoreNotified) {
						message1 = fonction + " Successful";
					    message2 = "Finger #"+(i+1)+" - Quality Score: " + pkFpQuality[0];
					    message3 = "";
					    message4 = "";

					    DialogUtils.showInfoMessage(message1, message2 + "\n" + message3 + "\n" + message4);
					}

					if(templateFp != TemplateType.MORPHO_NO_PK_FP) {
						String fpFileName = "";
						String fpExtension = "";
						FileFilter fp;

						fpExtension =  templateFp.getExtension() + extenstion;
						fpFileName = userID + "_finger_" + (i+1);

						if(templateFp == TemplateType.MORPHO_PK_DIN_V66400_CS_AA || templateFp == TemplateType.MORPHO_PK_ISO_FMC_CS_AA)
						{
							fpFileName += "_aa";
						}
						fpFileName += fpExtension;

						fp = new TemplateFileFilter(templateFp.getLabel() +" ("+fpExtension+")", "+fpExtension+");

						JFileChooser fileChooser = new JFileChooser();
						fileChooser.setSelectedFile(new File(fpFileName));
						fileChooser.addChoosableFileFilter(fp);
						fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));
						int userSelection = fileChooser.showSaveDialog(msoDemo);
						if (userSelection == JFileChooser.APPROVE_OPTION) {
							FilesMgt.fileChooserDirectory = fileChooser.getCurrentDirectory().getAbsolutePath();
							File fileToSave = fileChooser.getSelectedFile();
						    String savedFilePath = fileToSave.getAbsolutePath();

						    String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
						    if(!savedFilePath.endsWith(ext)) {
						    	savedFilePath += ext;
						    }

						    // save into file
						    try {
						    	FileOutputStream fos;
						    	fos = new FileOutputStream(savedFilePath);
						    	byte[] data = new byte[dataTemplate.size()];
								for(int j=0;j<dataTemplate.size();j++) {
									data[j] = dataTemplate.get(j);
								}
								fos.write(data);
							    fos.close();

							    message2 = "FP Template successfully";
							    message3 = "exported in file";
							    message4 = savedFilePath;
							    DialogUtils.showInfoMessage(message1, message2 + "\n" + message3 + "\n" + message4);
							} catch (FileNotFoundException fnfe) {
								DialogUtils.showErrorMessage("Save FP File " + savedFilePath, fnfe.getMessage());
							} catch (IOException ioe) {
								DialogUtils.showErrorMessage("Save FP File" + savedFilePath, ioe.getMessage());
							}
						} else {
							DialogUtils.showInfoMessage(fonction + " Successful", "Saving FP template file" + "\n" + "aborted by user.");
						}
					}
				}
			}
		}
		
		private int savePKSFile(String fpFileName, byte [] personnalInformation, MorphoTemplateList mTemplateList) {
			int ret = 0;
		    String extenstion = "";		    

			//---------------------
			// Pks File format
			//---------------------
			// the .pks file structure :
			// - Personnal Information
			// - Nb of templates: 1 byte (1 or 2 fingers)
			// - Size 1st Tplate: 2 bytes
			// - 1st Template	: <Size 1st Tplate> bytes
			// - Size 2nd Tplate: 2 bytes					(if exists)
			// - 2nd Template	: <Size 2nd Tplate> bytes	(if exists)

			JFileChooser fileChooser = new JFileChooser();
			fileChooser.setSelectedFile(new File(fpFileName));
			FileFilter pks = new TemplateFileFilter("SAGEM Pks (.pks"+extenstion+")", ".pks" + extenstion);
			fileChooser.addChoosableFileFilter(pks);
			fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));

			int userSelection = fileChooser.showSaveDialog(msoDemo);
			if (userSelection == JFileChooser.APPROVE_OPTION) {

				FilesMgt.fileChooserDirectory = fileChooser.getCurrentDirectory().getAbsolutePath();

				short[] nbTemplate = {0};
				mTemplateList.getNbTemplate(nbTemplate);

				File fileToSave = fileChooser.getSelectedFile();
			    String savedFilePath = fileToSave.getAbsolutePath();

			    String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
			    if(!savedFilePath.endsWith(ext)) {
			    	savedFilePath += ext;
			    }

			    // save into file
			    FileOutputStream fos;
				try {
					fos = new FileOutputStream(savedFilePath);
					fos.write(personnalInformation);
					fos.write((byte)(nbTemplate[0]));

					for(int i=0;i<nbTemplate[0];++i) {
						MorphoTemplateType[] typTemplate = {MorphoTemplateType.MORPHO_NO_PK_FP};
						ArrayList<Byte> dataTemplate = new ArrayList<Byte>();
						short[] pkFpQuality = {0};
						short[] dataIndex = {0};
						ret = mTemplateList.getTemplate((short) i, typTemplate, dataTemplate, pkFpQuality, dataIndex);
						if(ret != MorphoSmartSDK.MORPHO_OK) {
							DialogUtils.showErrorMessage("Capture","An error occured while calling MorphoTemplateList.getTemplate() function",ret,msoDemo.mDevice.getInternalError());
							fos.close();
							return ret;
						}

						fos.write(intToByteArray2(dataTemplate.size()));
						byte[] data = new byte[dataTemplate.size()];
						for(int j=0;j<dataTemplate.size();j++) {
							data[j] = dataTemplate.get(j);
						}

						fos.write(data);
					}

				    fos.close();
				} catch (FileNotFoundException fnfe) {
					DialogUtils.showErrorMessage("Save PKS File", fnfe.getMessage());
					ret = -1;
				} catch (IOException ioe) {
					DialogUtils.showErrorMessage("Save PKS File", ioe.getMessage());
					ret = -1;
				}

				return 0;
			} else {
				ret = -1;
			}

			return ret;
		}
		
		public byte[] intToByteArray2(int value) {
		    return new byte[] {
		    		(byte)value,
		    		(byte)(value >>> 8)
		            };
		}
		
		public byte[] getPersonnalInformation(String userID, String userFirstname, String userLastname) {
			// Personnal Information :
			// - Size ID		: 2 bytes
			// - ID				: <Size ID> bytes
			// - Size Firstname : 2 bytes
			// - Firstname		: <Size Firstname> bytes
			// - Size Lastname	: 2 bytes
			// - Lastname		: <Size Lastname> bytes

			int personnalInformationLength = 2 + userID.length() + 2 + userFirstname.length() + 2 + userLastname.length();
			byte personnalInformation[] = new byte[personnalInformationLength];
			int position = 0;

			int length = userID.length();
			System.arraycopy(new byte[] { (byte) (length), (byte) (length >>> 8)} , 0, personnalInformation, position, 2);
			position += 2;

			System.arraycopy(userID.getBytes(), 0, personnalInformation, position, userID.length());
			position += userID.length();

			length = userFirstname.length();
			System.arraycopy(new byte[] { (byte) (length), (byte) (length >>> 8)} , 0, personnalInformation, position, 2);
			position += 2;

			System.arraycopy(userFirstname.getBytes(), 0, personnalInformation, position, userFirstname.length());
			position += userFirstname.length();

			length = userLastname.length();
			System.arraycopy(new byte[] { (byte) (length), (byte) (length >>> 8)} , 0, personnalInformation, position, 2);
			position += 2;

			System.arraycopy(userLastname.getBytes(), 0, personnalInformation, position, userLastname.length());
			position += userLastname.length();

			return personnalInformation;
		}

		private void saveFVPFiles(String fonction, String userID, MorphoFVPTemplateType mFvpTemplateType, MorphoTemplateList templateList) {
			int ret = 0;

			short[] nbFVPTemplate = {0};
			templateList.getNbFVPTemplate(nbFVPTemplate);

			for(int i=0;i<nbFVPTemplate[0];++i)
			{
				MorphoFVPTemplateType[] typTemplate = {MorphoFVPTemplateType.MORPHO_NO_PK_FVP};
				ArrayList<Byte> dataTemplate = new ArrayList<Byte>();
				short[] pkFVPQuality = {0};
				short[] advancedSecurityLevelsCompatibility = {0};
				short[] dataIndex = {0};
				ret = templateList.getFVPTemplate((short) i, typTemplate, dataTemplate, pkFVPQuality, advancedSecurityLevelsCompatibility, dataIndex);

				if(ret != MorphoSmartSDK.MORPHO_OK)
				{
					DialogUtils.showErrorMessage(fonction,"An error occured while calling MorphoTemplateList.getFVPTemplate() function",ret,msoDemo.mDevice.getInternalError());
					return;
				}

				String message1 = fonction + " Successful";
			    String message2 = "Finger #"+(i+1)+" - Quality Score: " + pkFVPQuality[0];
			    String message3 = "";
			    String message4 = "";

			    
			    DialogUtils.showInfoMessage(message1, message2 + "\n" + message3+ "\n" + message4);

				String fvpFileName = "";
				FileFilter fvp;

				if(mFvpTemplateType == MorphoFVPTemplateType.MORPHO_PK_FVP)
				{
					fvpFileName = userID + "_finger_" + (i+1) + ".fvp";
					fvp = new TemplateFileFilter("SAGEM PkFVP (.fvp)", ".fvp");
				}
				else
				{
					fvpFileName = userID + "_finger_" + (i+1) + ".fvp-m";
					fvp = new TemplateFileFilter("SAGEM PkFVP Match (.fvp-m)", ".fvp-m");
				}

				JFileChooser fileChooser = new JFileChooser();
				fileChooser.setSelectedFile(new File(fvpFileName));
				fileChooser.addChoosableFileFilter(fvp);
				fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));
				int userSelection = fileChooser.showSaveDialog(msoDemo);
				if (userSelection == JFileChooser.APPROVE_OPTION) {
				    DialogUtils.showInfoMessage(message1, message2 + "\n" + message3);
				    
					FilesMgt.fileChooserDirectory = fileChooser.getCurrentDirectory().getAbsolutePath();
					File fileToSave = fileChooser.getSelectedFile();
				    String savedFilePath = fileToSave.getAbsolutePath();

				    String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
				    if(!savedFilePath.endsWith(ext)) {
				    	savedFilePath += ext;
				    }

				    // save into file
				    try {

				    	FileOutputStream fos;
				    	fos = new FileOutputStream(savedFilePath);

				    	byte[] data = new byte[dataTemplate.size()];

						for(int j=0;j<dataTemplate.size();j++)
						{
							data[j] = dataTemplate.get(j);
						}

						fos.write(data);
					    fos.close();

					    message2 = "FVP Template successfully";
					    message3 = "exported in file";
					    message4 = savedFilePath;

					    DialogUtils.showInfoMessage(message1, message2 + "\n" + message3+"\n" + message4);

					} catch (FileNotFoundException fnfe) {
						DialogUtils.showErrorMessage("Save FVP File " + savedFilePath, fnfe.getMessage());
					} catch (IOException ioe) {
						DialogUtils.showErrorMessage("Save FVP File" + savedFilePath, ioe.getMessage());
					}
				}
				else
				{
					DialogUtils.showErrorMessage( fonction + " Successful", "Saving FVP template file \n aborted by user.");
				}
			}
		}
		
		public void setScore(short value) {
			lblScore.setText(String.valueOf(value));
			setProgressBar((int) value);
		}
		
		public void setProgressBar(int value) {
			progressBar.setValue(value);
			if (value < 20)
				progressBar.setForeground(Color.BLUE);
			else
				progressBar.setForeground(Color.GREEN);
		}

		
		public void setLiveImage(MorphoImage morphoImage) {
			BufferedImage bufferedImage = MorphoEventHandler.toBufferedImage(morphoImage.getImage(),morphoImage.getImageHeader().getNbCol(),morphoImage.getImageHeader().getNbRow());
			
			ImageIcon image = new ImageIcon(MorphoEventHandler.resizeImage(bufferedImage,lblLiveImage.getWidth(),lblLiveImage.getHeight()));
			lblLiveImage.setIcon(image);
		}

		public void setCurrentImageInfo(int nbCol, int nbRow, int resX, int resY, int bitPerPixel) {
			lblCurrentImageInfo.setText("Size: " + nbCol + "*" + nbRow + " pix, Res: " + resX + "*" + resY + " dpi, " + bitPerPixel + " bits/pixels");
		}


		public void setStepsImage(EnumMoveFinger move) {
			if (move == null) {
				lblSteps.setIcon(null);
			} else {
				switch (move) {
					case MOVE_UP:	lblSteps.setIcon(iconArrowUp);		break;
					case MOVE_LEFT:	lblSteps.setIcon(iconArrowLeft);	break;
					case MOVE_DOWN:	lblSteps.setIcon(iconArrowDown);	break;
					case MOVE_RIGHT:lblSteps.setIcon(iconArrowRight);	break;
				}
			}
		}
		
		public void setInstruction(String status) {
			lblInstruction.setText(status);
		}
}
