package morpho.morphosmart.sdk.demo;

import java.awt.Color;
import java.awt.Component;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

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
import javax.swing.JTabbedPane;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.SwingConstants;
import javax.swing.border.BevelBorder;
import javax.swing.border.TitledBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.filechooser.FileFilter;

import morpho.morphosmart.sdk.api.IMorphoEventHandler;
import morpho.morphosmart.sdk.api.MorphoCallbackCommand;
import morpho.morphosmart.sdk.api.MorphoCompressAlgo;
import morpho.morphosmart.sdk.api.MorphoDatabase;
import morpho.morphosmart.sdk.api.MorphoDevice;
import morpho.morphosmart.sdk.api.MorphoFAR;
import morpho.morphosmart.sdk.api.MorphoFVPTemplateType;
import morpho.morphosmart.sdk.api.MorphoImage;
import morpho.morphosmart.sdk.api.MorphoSmartSDK;
import morpho.morphosmart.sdk.api.MorphoTemplateEnvelope;
import morpho.morphosmart.sdk.api.MorphoTemplateList;
import morpho.morphosmart.sdk.api.MorphoTemplateType;
import morpho.morphosmart.sdk.api.MorphoUser;
import morpho.morphosmart.sdk.api.MorphoDevice.MorphoDevicePrivacyModeStatus;
import morpho.morphosmart.sdk.demo.dialog.DialogGetImage;
import morpho.morphosmart.sdk.demo.dialog.DialogResultWindow;
import morpho.morphosmart.sdk.demo.dialog.DialogUtils;
import morpho.morphosmart.sdk.demo.ennum.Coder;
import morpho.morphosmart.sdk.demo.ennum.DetectionMode;
import morpho.morphosmart.sdk.demo.ennum.EnrollmentType;
import morpho.morphosmart.sdk.demo.ennum.EnumExportImage;
import morpho.morphosmart.sdk.demo.ennum.EnumImageFormat;
import morpho.morphosmart.sdk.demo.ennum.EnumMoveFinger;
import morpho.morphosmart.sdk.demo.ennum.EnumTabIndex;
import morpho.morphosmart.sdk.demo.ennum.LatentDetection;
import morpho.morphosmart.sdk.demo.ennum.MorphoUserFields;
import morpho.morphosmart.sdk.demo.ennum.TemplateFVPType;
import morpho.morphosmart.sdk.demo.ennum.TemplateType;
import morpho.morphosmart.sdk.demo.trt.ErrorsMgt;
import morpho.morphosmart.sdk.demo.trt.FilesMgt;
import morpho.morphosmart.sdk.demo.trt.ImageLoader;
import morpho.morphosmart.sdk.demo.trt.MorphoEventHandler;
import morpho.morphosmart.sdk.demo.trt.MorphoFileChooser;
import morpho.morphosmart.sdk.demo.trt.MorphoInfo;
import morpho.morphosmart.sdk.demo.trt.SecurityMgt;
import morpho.morphosmart.sdk.demo.trt.TKBHeader;
import morpho.morphosmart.sdk.demo.trt.UsersMgt;
import morpho.msosecu.sdk.api.MsoSecu;

public class PanelBasicBiometricOperation extends JPanel {

	private static final long serialVersionUID = 1L;
	private static final int OK = MorphoSmartSDK.MORPHO_OK;

	// Tabs
	protected JTabbedPane tabbedPane = new JTabbedPane(JTabbedPane.TOP);
	protected TabCapture tabCapture;
	protected TabVerify tabVerify;
	protected TabFingerprintImage tabFpImage;
	protected TabEnroll tabEnroll;
	protected TabIdentify tabIdentify;
	protected TabProcess tabProcess;
	// Button
	JButton btnStartStop = new JButton("Start");
	JButton btnCloseComAndExit = new JButton("Close Com and Exit");
	JButton btnRebootSoft = new JButton("Reboot Soft");
	// labels
	private JLabel lblCurrentImageInfo;
	private JLabel lblLiveImage;
	private JLabel lblSteps;
	private JLabel lblScore;
	private JLabel lblInstruction;
	// progress
	private JProgressBar progressBar;
	// images
	private Icon iconArrowUp = new ImageIcon(ImageLoader.load("arrow_up.png"));
	private Icon iconArrowRight = new ImageIcon(ImageLoader.load("arrow_right.png"));
	private Icon iconArrowDown = new ImageIcon(ImageLoader.load("arrow_down.png"));
	private Icon iconArrowLeft = new ImageIcon(ImageLoader.load("arrow_left.png"));
	private Icon gifCapture = new ImageIcon(ImageLoader.load("capture.gif"));
	private Icon gifMaillage = new ImageIcon(ImageLoader.load("maillage.gif"));
	private ImageIcon imgFvpOk = new ImageIcon(ImageLoader.load("ok.png"));
	private ImageIcon imgFvpKo = new ImageIcon(ImageLoader.load("ko.png"));
	// MsoDemo context
	protected MsoDemo msoDemo;
	// variables
	private EnumTabIndex currentTabIndex;
	private boolean btnCloseCom;
	private boolean btnReboot;
	// MorphoSmart SDK
	private MorphoDevice mDevice;
	private MorphoDatabase mDatabase;

	/**
	 * Create the panel.
	 * 
	 * @param msoDemo
	 */
	public PanelBasicBiometricOperation(MsoDemo msoDemo) {
		this.msoDemo = msoDemo;
		this.mDevice = msoDemo.getMorphoDeviceInstance();
		this.mDatabase = msoDemo.getMorphoDatabaseInstance();

		JPanel panel = new JPanel();
		panel.setBorder(new TitledBorder(null, "Basic Biometric Operations", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		GroupLayout groupLayout = new GroupLayout(this);
		groupLayout.setHorizontalGroup(groupLayout.createParallelGroup(Alignment.LEADING).addGroup(
				groupLayout
						.createSequentialGroup()
						.addContainerGap()
						.addGroup(
								groupLayout.createParallelGroup(Alignment.LEADING).addGroup(groupLayout.createSequentialGroup().addComponent(panel, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addPreferredGap(ComponentPlacement.RELATED))
										.addGroup(groupLayout.createSequentialGroup().addComponent(btnCloseComAndExit, GroupLayout.PREFERRED_SIZE, 176, GroupLayout.PREFERRED_SIZE).addGap(75).addComponent(btnRebootSoft, GroupLayout.PREFERRED_SIZE, 120, GroupLayout.PREFERRED_SIZE).addPreferredGap(ComponentPlacement.RELATED, 75, Short.MAX_VALUE))).addGap(81)));
		groupLayout.setVerticalGroup(groupLayout.createParallelGroup(Alignment.TRAILING).addGroup(
				groupLayout.createSequentialGroup().addContainerGap().addComponent(panel, GroupLayout.PREFERRED_SIZE, 612, GroupLayout.PREFERRED_SIZE).addGap(14).addGroup(groupLayout.createParallelGroup(Alignment.BASELINE).addComponent(btnCloseComAndExit, GroupLayout.PREFERRED_SIZE, 32, GroupLayout.PREFERRED_SIZE).addComponent(btnRebootSoft, GroupLayout.PREFERRED_SIZE, 27, GroupLayout.PREFERRED_SIZE)).addContainerGap()));
		groupLayout.linkSize(SwingConstants.VERTICAL, new Component[] { btnCloseComAndExit, btnRebootSoft });

		lblCurrentImageInfo = new JLabel("...");
		lblCurrentImageInfo.setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));
		lblLiveImage = new JLabel("", SwingConstants.CENTER);
		lblLiveImage.setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));
		lblInstruction = new JLabel("", SwingConstants.CENTER);
		lblInstruction.setBorder(BorderFactory.createLineBorder(Color.GRAY));
		lblSteps = new JLabel("", SwingConstants.CENTER);
		lblSteps.setBorder(BorderFactory.createLineBorder(Color.GRAY));
		lblSteps.setOpaque(true);
		lblSteps.setBackground(Color.WHITE);
		lblScore = new JLabel("", SwingConstants.CENTER);
		lblScore.setBorder(BorderFactory.createLineBorder(Color.GRAY));

		progressBar = new JProgressBar();
		progressBar.setOrientation(SwingConstants.VERTICAL);
		progressBar.setMaximum(150);

		GroupLayout gl_panel = new GroupLayout(panel);
		gl_panel.setHorizontalGroup(gl_panel
				.createParallelGroup(Alignment.TRAILING)
				.addGroup(
						gl_panel.createSequentialGroup()
								.addContainerGap()
								.addGroup(
										gl_panel.createParallelGroup(Alignment.LEADING)
												.addGroup(
														gl_panel.createSequentialGroup().addComponent(lblLiveImage, GroupLayout.PREFERRED_SIZE, 143, GroupLayout.PREFERRED_SIZE).addGap(10).addGroup(gl_panel.createParallelGroup(Alignment.LEADING).addComponent(lblScore, GroupLayout.PREFERRED_SIZE, 31, GroupLayout.PREFERRED_SIZE).addComponent(progressBar, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)).addPreferredGap(ComponentPlacement.RELATED)
																.addGroup(gl_panel.createParallelGroup(Alignment.TRAILING).addComponent(lblSteps, GroupLayout.DEFAULT_SIZE, 108, Short.MAX_VALUE).addComponent(btnStartStop, GroupLayout.DEFAULT_SIZE, 108, Short.MAX_VALUE).addComponent(lblInstruction, GroupLayout.DEFAULT_SIZE, 108, Short.MAX_VALUE))).addComponent(lblCurrentImageInfo, GroupLayout.DEFAULT_SIZE, 298, Short.MAX_VALUE)).addContainerGap())
				.addComponent(tabbedPane, GroupLayout.DEFAULT_SIZE, 318, Short.MAX_VALUE));
		gl_panel.setVerticalGroup(gl_panel.createParallelGroup(Alignment.LEADING).addGroup(
				gl_panel.createSequentialGroup()
						.addComponent(tabbedPane, GroupLayout.PREFERRED_SIZE, 367, GroupLayout.PREFERRED_SIZE)
						.addGap(18)
						.addGroup(
								gl_panel.createParallelGroup(Alignment.LEADING)
										.addGroup(
												gl_panel.createSequentialGroup().addGroup(gl_panel.createParallelGroup(Alignment.BASELINE).addComponent(lblScore).addComponent(btnStartStop, GroupLayout.PREFERRED_SIZE, 31, GroupLayout.PREFERRED_SIZE)).addPreferredGap(ComponentPlacement.RELATED)
														.addGroup(gl_panel.createParallelGroup(Alignment.LEADING).addGroup(gl_panel.createSequentialGroup().addComponent(lblSteps, GroupLayout.PREFERRED_SIZE, 87, GroupLayout.PREFERRED_SIZE).addGap(18).addComponent(lblInstruction, GroupLayout.PREFERRED_SIZE, 25, GroupLayout.PREFERRED_SIZE)).addComponent(progressBar, GroupLayout.DEFAULT_SIZE, 130, Short.MAX_VALUE)))
										.addComponent(lblLiveImage, GroupLayout.PREFERRED_SIZE, 164, GroupLayout.PREFERRED_SIZE)).addPreferredGap(ComponentPlacement.UNRELATED).addComponent(lblCurrentImageInfo).addGap(32)));
		gl_panel.linkSize(SwingConstants.VERTICAL, new Component[] { btnStartStop, lblScore });
		gl_panel.linkSize(SwingConstants.HORIZONTAL, new Component[] { lblScore, progressBar });

		// Capture tab
		tabCapture = new TabCapture(this);
		tabbedPane.addTab("Capture", tabCapture);
		// Verify tab
		tabVerify = new TabVerify();
		tabbedPane.addTab("Verify", tabVerify);
		// FingerprintImage tab
		tabFpImage = new TabFingerprintImage(this);
		if (System.getProperty("os.name").startsWith("Windows")) {
			tabbedPane.addTab("Fingerprint Image", tabFpImage);
		}
		else
		{
			tabbedPane.addTab("<html><body style='text-align: center;'>Fingerprint<br>Image</body></html>", tabFpImage);
		}

		// AddTabsEnrollIdentify(msoDemo.isBaseAvailable);

		// Process tab
		tabProcess = new TabProcess(this);
		tabbedPane.addTab("Process", tabProcess);
		tabbedPane.remove(tabProcess);

		tabbedPane.setSelectedComponent(tabCapture);

		panel.setLayout(gl_panel);
		setSize(400, 680);
		setLayout(groupLayout);

		btnStartStop.addActionListener(btnStartActionListener);
		btnRebootSoft.addActionListener(rebootActionListener);

		btnCloseComAndExit.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				mDevice.closeDevice();
				System.exit(0);
			}
		});

		tabbedPane.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				switch (tabbedPane.getSelectedIndex()) {
				case 0:
					MorphoInfo.tabIndex = EnumTabIndex.TAB_CAPTURE;
					break;
				case 1:
					MorphoInfo.tabIndex = EnumTabIndex.TAB_VERIFY;
					break;
				case 2:
					MorphoInfo.tabIndex = EnumTabIndex.TAB_FINGERPTINT_IMAGE;
					break;
				case 3:
					MorphoInfo.tabIndex = EnumTabIndex.TAB_ENROL;
					break;
				case 4:
					MorphoInfo.tabIndex = EnumTabIndex.TAB_IDENTIFY;
					break;
				case 5:
					MorphoInfo.tabIndex = EnumTabIndex.TAB_PROCESS;
					break;
				}
			}
		});

	}
	
	public void selectTabVerify()
	{
		tabbedPane.setSelectedComponent(tabVerify);
		tabVerify.rbCheckInLocal.setSelected(true);
	}

	public void AddTabsEnrollIdentify(boolean baseStat) {
		if (baseStat == true) {
			// Enroll tab
			tabEnroll = new TabEnroll(this);
			tabbedPane.addTab("Enroll", tabEnroll);
			// Identify tab
			tabIdentify = new TabIdentify();
			tabbedPane.addTab("Identify", tabIdentify);
		} else {
			try {
				tabbedPane.remove(tabEnroll);
				tabbedPane.remove(tabIdentify);
			} catch (Exception ex) {

			}
		}
	}

	// Button RebootSoft listener
	private ActionListener rebootActionListener = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent ae) {
			try {
				msoDemo.waitCursor();
				mDevice.rebootSoft();
				Thread.sleep(3000); // wait 3 seconds for complete
									// initialization
				if (!System.getProperty("os.name").startsWith("Windows")) {
					Thread.sleep(2000);
					if(msoDemo.i_msoPipe != null) {
						mDevice.closeDevice();
						MsoPipeImpl tmp_msoPipe = (MsoPipeImpl)msoDemo.i_msoPipe;
						mDevice.openDevice(tmp_msoPipe, tmp_msoPipe.getServer_ip(), tmp_msoPipe.getServer_port(),(short)4);	
					} else {
						mDevice.openUsbDevice(msoDemo.serialNumber, 0);
					}
				}
			} catch (InterruptedException e) {
				System.err.println(e.getMessage());
			} finally {
				msoDemo.defaultCursor();
			}
		}
	};

	// Button start listener
	private ActionListener btnStartActionListener = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			Thread startStopThread = new Thread(new Runnable() {
				@Override
				public void run() {
					startStop();					
				}
			});
			startStopThread.start();
		}
	};

	//
	// Setters
	// ---------------------------------------------------------------------
	//

	private void clearLive() {
		lblLiveImage.setIcon(null);
		lblSteps.setIcon(null);
		lblCurrentImageInfo.setText("...");
	}

	public void startStop()
	{
		int ret = 0;
		// Button 'Start' is clicked
		if (btnStartStop.getText().equals("Start")) {
			// tab capture & tab enroll
			if ((MorphoInfo.tabIndex == EnumTabIndex.TAB_CAPTURE && tabCapture.textID.getText().isEmpty()) || (MorphoInfo.tabIndex == EnumTabIndex.TAB_ENROL && tabEnroll.textIdNumber.getText().isEmpty())) {
				DialogUtils.showWarningMessage("MSO_Demo Java", "You must at least type an ID.");
				return;
			}

			// save the current tab index
			currentTabIndex = MorphoInfo.tabIndex;
			if (currentTabIndex == EnumTabIndex.TAB_CAPTURE || currentTabIndex == EnumTabIndex.TAB_ENROL) {
				// add Process tab
				if (msoDemo.getPanelDatabaseMgtInstance().tabOptions.cbImageViewer.isSelected()) {
					tabbedPane.addTab("Process", tabProcess);
					tabbedPane.setSelectedComponent(tabProcess);
				}
			}
			// change button text
			btnStartStop.setText("Stop");
			// Disable panel Buttons
			msoDemo.disableAllButons();

			// if isFVP
			if (MorphoInfo.isFVP)
				playVideo(false);

			switch (currentTabIndex) {
			case TAB_CAPTURE:
				capture();
				break;
			case TAB_VERIFY:
				verify();
				break;
			case TAB_FINGERPTINT_IMAGE:
				getImage();
				break;
			case TAB_ENROL:
				enroll();
				break;
			case TAB_IDENTIFY:
				identify();
				break;
			default:
				break;
			}

			// FFD Log
			FilesMgt.appendFFDLog(mDevice);
		}
		// Button 'Stop' is clicked
		else if (btnStartStop.getText().equals("Stop")) {
			if (MorphoInfo.isFVP)
				fingerKo();
			// cancel acquisition
			ret = mDevice.cancelLiveAcquisition();
			if (ret != MorphoSmartSDK.MORPHO_OK)
				DialogUtils.showErrorMessage("Error", "Error in CancelLiveAcquisition");
		}
		// finish capture process
		finishCapture();

		if ( (currentTabIndex == EnumTabIndex.TAB_ENROL) && (tabEnroll.cbSavePkInDB.isSelected()) ) {
			msoDemo.updateDataBaseInformation();
			msoDemo.getPanelDatabaseMgtInstance().loadUsers();
		}
	}
	/**
	 * 
	 * @param nbCol
	 * @param nbRow
	 * @param resX
	 * @param resY
	 * @param bitPerPixel
	 */
	public void setCurrentImageInfo(int nbCol, int nbRow, int resX, int resY, int bitPerPixel) {
		lblCurrentImageInfo.setText("Size: " + nbCol + "*" + nbRow + " pix, Res: " + resX + "*" + resY + " dpi, " + bitPerPixel + " bits/pixels");
	}

	public void setFVPFingerState(int fctRet)
	{
		if (MorphoInfo.isFVP && fctRet != MorphoSmartSDK.MORPHO_OK)
		{
			fingerKo();
		}
	}
	/**
	 * 
	 * @param morphoImage
	 */
	public void setLiveImage(MorphoImage morphoImage) {
		BufferedImage bufferedImage = MorphoEventHandler.toBufferedImage(morphoImage.getImage(), morphoImage.getImageHeader().getNbCol(), morphoImage.getImageHeader().getNbRow());

		ImageIcon image = new ImageIcon(MorphoEventHandler.resizeImage(bufferedImage, lblLiveImage.getWidth(), lblLiveImage.getHeight()));
		lblLiveImage.setIcon(image);
	}

	/**
	 * finger VP video.
	 * 
	 * @param isfingerDetected
	 *            : true, play Maillage.gif otherwise play capture.gif
	 */
	public void playVideo(boolean isfingerDetected) {
		if (isfingerDetected)
			lblLiveImage.setIcon(gifMaillage);
		else
			lblLiveImage.setIcon(gifCapture);
	}

	/**
	 * finger VP 'OK'
	 */
	public void fingerOk() {
		Icon iconOK = new ImageIcon(imgFvpOk.getImage().getScaledInstance(lblLiveImage.getWidth(), lblLiveImage.getHeight(), Image.SCALE_AREA_AVERAGING));
		lblLiveImage.setIcon(iconOK);
	}

	/**
	 * finger VP 'KO'
	 */
	public void fingerKo() {
		Icon iconKO = new ImageIcon(imgFvpKo.getImage().getScaledInstance(lblLiveImage.getWidth(), lblLiveImage.getHeight(), Image.SCALE_AREA_AVERAGING));
		lblLiveImage.setIcon(iconKO);
	}

	/**
	 * 
	 * @param move
	 */
	public void setStepsImage(EnumMoveFinger move) {
		if (move == null) {
			lblSteps.setIcon(null);
		} else {
			switch (move) {
			case MOVE_UP:
				lblSteps.setIcon(iconArrowUp);
				break;
			case MOVE_LEFT:
				lblSteps.setIcon(iconArrowLeft);
				break;
			case MOVE_DOWN:
				lblSteps.setIcon(iconArrowDown);
				break;
			case MOVE_RIGHT:
				lblSteps.setIcon(iconArrowRight);
				break;
			}
		}
	}

	/**
	 * 
	 * @param value
	 */
	public void setScore(short value) {
		lblScore.setText(String.valueOf(value));
		setProgressBar((int) value);
	}

	/**
	 * 
	 * @param value
	 */
	public void setProgressBar(int value) {
		progressBar.setValue(value);
		if (value < 20)
			progressBar.setForeground(Color.BLUE);
		else
			progressBar.setForeground(Color.GREEN);
	}

	/**
	 * 
	 * @param status
	 */
	public void setInstruction(String status) {
		lblInstruction.setText(status);
	}

	//
	// UI
	// functions------------------------------------------------------------------
	//

	public void reinitUI() {
		lblScore.setText("");
		setInstruction("");
		lblCurrentImageInfo.setText("");
		progressBar.setValue(0);
	}

	public EnumTabIndex getCallingTab() {
		return currentTabIndex;
	}

	public void disableAllButons() {
		btnCloseCom = btnCloseComAndExit.isEnabled();
		btnReboot = btnRebootSoft.isEnabled();

		btnCloseComAndExit.setEnabled(false);
		btnRebootSoft.setEnabled(false);
	}

	public void enableAllButtons() {
		btnCloseComAndExit.setEnabled(btnCloseCom);
		btnRebootSoft.setEnabled(btnReboot);
	}

	private void finishCapture() {
		// remove Process tab
		tabbedPane.remove(tabProcess);
		tabbedPane.setSelectedIndex(currentTabIndex.getValue());
		// Clear images from tab process
		tabProcess.clearLive();
		this.clearLive();
		// change button text
		btnStartStop.setText("Start");
		// re-enable all buttons
		msoDemo.enableAllButtons();
	}

	//
	// Activities
	// process-------------------------------------------------------------
	//

	/**
	 * Capture process.
	 * 
	 * @return
	 */
	private int capture() {
		TabBioSettings tabBioSettings = msoDemo.rightPanel.tabBioSettings;
		int ret = 0;
		int timeout = Integer.parseInt(tabBioSettings.txtTimeout.getText().toString());
		short advancedSecurityLevelsRequired;
		short fingerNumber = (short) (tabCapture.rbOneFinger.isSelected() ? 1 : 2);
		MorphoTemplateType templateType = MorphoTemplateType.MORPHO_PK_COMP;
		MorphoFVPTemplateType templateFVPType = MorphoFVPTemplateType.MORPHO_NO_PK_FVP;
		int maxSizeTemplate = 255;
		short enrolType = (short) EnrollmentType.THREE_ACQUISITIONS.getValue();
		long callbackMask;
		MorphoTemplateList templateList = new MorphoTemplateList();
		MorphoTemplateEnvelope typEnvelop;
		byte[] applicationData = null;
		short latentDetection = (short) (tabCapture.cbLatentDetect.isSelected() ? LatentDetection.LATENT_DETECT_ENABLE.getValue() : LatentDetection.LATENT_DETECT_DISABLE.getValue());
		int coderChoice;
		long detectModeChoice;

		int securityLevel = msoDemo.getSecurityLevel(EnumTabIndex.TAB_CAPTURE);
		mDevice.setSecurityLevel(securityLevel);

		if (tabCapture.cbEmbedInToken.isSelected()) { // X984 selected
			typEnvelop = MorphoTemplateEnvelope.MORPHO_X984_SIGNED_TEMPLATE;

			ArrayList<Byte> certif = new ArrayList<Byte>();

			ret = SecurityMgt.secuReadCertificate(mDevice, (short) 0, certif);

			if (ret != MorphoSmartSDK.MORPHO_OK) {
				new DialogResultWindow(msoDemo, "Read certificate Failed", "Please check that the device is secured", "MorphoDevice.secuReadCertificate()", ErrorsMgt.convertSDKError(ret), "").setVisible(true);
				return ret;
			}

			byte[] hostCertificate = new byte[certif.size()];

			for (int i = 0; i < certif.size(); i++) {
				hostCertificate[i] = certif.get(i);
			}

			msoDemo.setHostCertificate(hostCertificate);

			byte[] personnalInformation = getPersonalInformation(tabCapture.textID.getText(), tabCapture.textFirstName.getText(), tabCapture.txtLastname.getText());

			int applicationDataLength = 4 + personnalInformation.length;

			applicationData = new byte[applicationDataLength];

			// Magic Number : 4 bytes
			System.arraycopy(TKBHeader.magicNbByte, 0, applicationData, 0, 4);
			// Personnal Information
			System.arraycopy(personnalInformation, 0, applicationData, 4, personnalInformation.length);

		} else { // X984 not selected
			typEnvelop = MorphoTemplateEnvelope.MORPHO_RAW_TEMPLATE;
		}

		String selectedTemplateType = (String) tabCapture.comboFpTemplateType.getSelectedItem();
		TemplateType templateFp = TemplateType.getValue(selectedTemplateType);

		String selectedFVPTemplateType = (String) tabCapture.comboFvpTemplateType.getSelectedItem();
		TemplateFVPType templateFvp = TemplateFVPType.getValue(selectedFVPTemplateType);

		if (templateFp != TemplateType.MORPHO_PK_SAGEM_PKS) {
			templateType = MorphoTemplateType.swigToEnum(templateFp.getCode());

			if (templateType != MorphoTemplateType.MORPHO_NO_PK_FP) {
				if (templateType == MorphoTemplateType.MORPHO_PK_MAT || templateType == MorphoTemplateType.MORPHO_PK_MAT_NORM || templateType == MorphoTemplateType.MORPHO_PK_PKLITE) {
					maxSizeTemplate = 1;
				} else {
					maxSizeTemplate = 255;
				}
			} else {
				if (!MorphoInfo.isFVP) {
					templateType = MorphoTemplateType.MORPHO_PK_COMP; // MSO_Demo
																		// always
																		// ask
																		// export
																		// of
																		// templates
																		// MORPHO_PK_COMP
				}
			}
		} else { // template pks
			templateType = MorphoTemplateType.MORPHO_PK_COMP;
			maxSizeTemplate = 255;
		}

		if (MorphoInfo.isFVP) {
			if (tabCapture.rbCaptureTypeEnroll.isSelected()) {
				templateFVPType = MorphoFVPTemplateType.MORPHO_PK_FVP;
			} else {
				templateFVPType = MorphoFVPTemplateType.MORPHO_PK_FVP_MATCH;
			}
		}

		callbackMask = msoDemo.callbackMask;

		detectModeChoice = MorphoSmartSDK.MORPHO_ENROLL_DETECT_MODE;

		if (tabBioSettings.cbForceFingerPlacement.isSelected()) {
			detectModeChoice |= MorphoSmartSDK.MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE;
		}

		detectModeChoice |= msoDemo.wakeUpMode;

		if (tabBioSettings.cbAdvancedSecurityLevel.isSelected()) {
			advancedSecurityLevelsRequired = 1;
		} else {
			if (tabCapture.rbCaptureTypeEnroll.isSelected()) {
				advancedSecurityLevelsRequired = 0;
			} else {
				advancedSecurityLevelsRequired = 0xFF;
			}
		}

		if (tabCapture.rbCaptureTypeVerif.isSelected()) {
			enrolType = (short) EnrollmentType.ONE_ACQUISITIONS.getValue();
		}

		// acquisitionThreshold
		short acquisitionThreshold = 0;
		if (tabBioSettings.cbFingerQualityThreshold.isSelected()) {
			String quality = tabBioSettings.txtFingerQualityThreshold.getText().toString().trim();
			if (!quality.equals("-"))
				acquisitionThreshold = Short.parseShort(quality);
		}

		// coderChoice
		String selectedCoder = (String) tabBioSettings.comboCoderChoice.getSelectedItem();
		coderChoice = Coder.getValue(selectedCoder);

		// callback
		IMorphoEventHandler mEventHandlerCallback = new MorphoEventHandler(tabProcess);

		// capture
		ret = mDevice.capture(timeout, acquisitionThreshold, advancedSecurityLevelsRequired, fingerNumber, templateType, templateFVPType, maxSizeTemplate, enrolType, callbackMask, mEventHandlerCallback, templateList, typEnvelop, applicationData, latentDetection, coderChoice, detectModeChoice, (short) 0, MorphoCompressAlgo.MORPHO_NO_COMPRESS, (short) 0);

		setFVPFingerState(ret);
		
		if (ret != MorphoSmartSDK.MORPHO_OK) {
			String message1 = "Capture Failed";
			String message2 = "";
			String message3 = "";
			String message4 = "";
			switch (ret) {
			case MorphoSmartSDK.MORPHOERR_NO_HIT:
				message2 = "Bad Capture Sequence.";
				break;
			case MorphoSmartSDK.MORPHOERR_CMDE_ABORTED:
				message2 = "Command aborted by user.";
				break;
			case MorphoSmartSDK.MORPHOERR_TIMEOUT:
				message2 = "Timeout has expired.";
				message3 = "Command aborted.";
				break;
			case MorphoSmartSDK.MORPHOERR_FFD:
			case MorphoSmartSDK.MORPHOERR_MOIST_FINGER:
			case MorphoSmartSDK.MORPHOERR_MOVED_FINGER:
			case MorphoSmartSDK.MORPHOERR_SATURATED_FINGER:
			case MorphoSmartSDK.MORPHOERR_INVALID_FINGER:
				message2 = "Warning : Low quality finger !!!";
				break;
			case MorphoSmartSDK.MORPHOERR_BADPARAMETER:
				message2 = "One or more input parameters are out of range";
				break;
			case MorphoSmartSDK.MORPHOERR_LICENSE_MISSING:
				message2 = "A required license is missing.";
				break;
			case MorphoSmartSDK.MORPHOERR_ADVANCED_SECURITY_LEVEL_NOT_AVAILABLE:
				message2 = "Cannot make a multimodal template compatible with advanced security levels.";
				break;
			case MorphoSmartSDK.MORPHOERR_ADVANCED_SECURITY_LEVEL_MISMATCH:
				message2 = "Failed to make a multimodal template compatible with advanced security levels.";
				break;
			case MorphoSmartSDK.MORPHOERR_BAD_FINAL_FINGER_PRINT_QUALITY:
				message2 = "Failed to reach the fingerprint quality threshold.";
				break;
			default:
				message2 = "An error occured while calling";
				message3 = "MorphoDevice.capture() function";
				message4 = ErrorsMgt.convertSDKError(ret);
				break;
			}

			new DialogResultWindow(msoDemo, message1, message2, message3, message4, "").setVisible(true);
		} else {
			short[] nbTemplate = { 0 };
			templateList.getNbTemplate(nbTemplate);

			if (tabCapture.cbEmbedInToken.isSelected()) {
				TKBHeader tkbh = new TKBHeader();

				// ---------------------
				// Token Bio File
				// ---------------------
				// the .tkb file structure :
				// _ the .tkb Header File (see the T_TKB_HDR structure)
				// _ the X984 Biometric Token
				// _ the X509 Mso certificate

				if (templateType != MorphoTemplateType.MORPHO_NO_PK_FP) {
					ArrayList<Byte> dataX984 = new ArrayList<Byte>();
					templateList.getX984(dataX984);

					// set the .tkb Header File
					tkbh.tkbSize = dataX984.size();
					tkbh.certifSize = msoDemo.getHostCertificate().length;

					String fpFileName = tabCapture.textID.getText() + "_fp.tkb";

					ret = saveTKBFile(fpFileName, tkbh, dataX984, msoDemo.getHostCertificate(), msoDemo.isDataEncryption());
					if (ret != 0) {
						new DialogResultWindow(msoDemo, "Capture Successful", "Saving FP template file", "aborted by user.", "", "").setVisible(true);
					} else {
						new DialogResultWindow(msoDemo, "Capture Successful", "FP template with a X9.84 envelop", "successfully exported in file", FilesMgt.fileChooserDirectory + "/" + fpFileName, "").setVisible(true);
					}
				}

				if (MorphoInfo.isFVP) {
					ArrayList<Byte> fvp_X984 = new ArrayList<Byte>();
					templateList.getFVPX984(fvp_X984);

					// set the .tkb Header File
					tkbh.tkbSize = fvp_X984.size();
					tkbh.certifSize = msoDemo.getHostCertificate().length;

					String fvpFileName = tabCapture.textID.getText() + "_fvp.tkb";

					ret = saveTKBFile(fvpFileName, tkbh, fvp_X984, msoDemo.getHostCertificate(), msoDemo.isDataEncryption());

					if (ret != 0) {
						new DialogResultWindow(msoDemo, "Capture Successful", "Saving FVP template file", "aborted by user.", "", "").setVisible(true);
					} else {
						new DialogResultWindow(msoDemo, "Capture Successful", "FVP template with a X9.84 envelop", "successfully exported in file", FilesMgt.fileChooserDirectory + "/" + fvpFileName, "").setVisible(true);
					}
				}
			} else if (templateFp == TemplateType.MORPHO_PK_SAGEM_PKS) {
				if (MorphoInfo.isFVP && templateFVPType != MorphoFVPTemplateType.MORPHO_NO_PK_FVP) {
					saveFVPFiles("Capture", tabCapture.textID.getText(), templateFVPType, templateList);
				}

				byte[] personnalInformation = getPersonalInformation(tabCapture.textID.getText(), tabCapture.textFirstName.getText(), tabCapture.txtLastname.getText());

				String pksFileName = tabCapture.textID.getText() + ".pks";
				ret = savePKSFile(pksFileName, personnalInformation, templateList, msoDemo.isDataEncryption());

				if (ret != 0) {
					new DialogResultWindow(msoDemo, "Capture Successful", "Saving FP template file", "aborted by user.", "", "").setVisible(true);
				} else {
					new DialogResultWindow(msoDemo, "Capture Successful", "FP Template successfully", "exported in file", FilesMgt.fileChooserDirectory + "/" + pksFileName, "").setVisible(true);
				}
			} else if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) { // privacy mode
				int response = DialogUtils.showQuestionMessage("MSO_Demo Java", "Privacy mode : template data is crypted.\nWould you like to decrypt it before saving ?");
				if (0 == response) {
					MorphoTemplateList decryptedTemplateList = new MorphoTemplateList();
					// decrypt template data
					for (int index = 0; index < fingerNumber; ++index) {
						// get crypted template buffer
						MorphoTemplateType[] typTemplate = { MorphoTemplateType.MORPHO_NO_PK_FP };
						ArrayList<Byte> dataTemplate = new ArrayList<Byte>();
						short[] dataIndex = { 0 };
						short[] pkFpQuality = { 0 };
						ret = templateList.getTemplate((short) index, typTemplate, dataTemplate, pkFpQuality, dataIndex);
						if (ret != MorphoSmartSDK.MORPHO_OK) {
							DialogUtils.showErrorMessage("Capture", "An error occured while calling MorphoTemplateList.getTemplate() function", ret, this.msoDemo.getMorphoDeviceInstance().getInternalError());
							return ret;
						}
						byte[] cryptedTemplate = msoDemo.convertArrayListToByteArray(dataTemplate);
						// decrypt and check template buffer
						byte[] privacyKey = msoDemo.getPrivacyModeKey();
						if (null != privacyKey) {
							byte[] decryptedTemplate = msoDemo.decryptAndCheckPrivacyData(cryptedTemplate, privacyKey);
							if (null != decryptedTemplate) {
								ret = decryptedTemplateList.putTemplate(typTemplate[index], decryptedTemplate, dataIndex[index], pkFpQuality[index], dataIndex);
								if (ret != MorphoSmartSDK.MORPHO_OK) {
									DialogUtils.showErrorMessage("Capture", "An error occured while calling MorphoTemplateList.putTemplate() function", ret, this.msoDemo.getMorphoDeviceInstance().getInternalError());
									return ret;
								}
							} else {
								return -1;
							}
						} else {
							DialogUtils.showErrorMessage("Capture", "Invalid Kprivacy key");
							return -1;
						}						
					}
					saveTemplatesFiles("Capture", tabCapture.textID.getText(), fingerNumber, templateFvp, templateFp, decryptedTemplateList, false);
				} else if (1 == response) {
					saveTemplatesFiles("Capture", tabCapture.textID.getText(), fingerNumber, templateFvp, templateFp, templateList, false);
				}
			} else {
				saveTemplatesFiles("Capture", tabCapture.textID.getText(), fingerNumber, templateFvp, templateFp, templateList, msoDemo.isDataEncryption());
			}
		}
		return ret;
	}

	private void saveTemplatesFiles(String fonction, String userID, int nbFinger, TemplateFVPType templateFvp, TemplateType templateFp, MorphoTemplateList templateList, boolean isDataEncryption) {
		int ret = 0;
		boolean isQualityScoreNotified = false;
		String message1 = "";
		String message2 = "";
		String message3 = "";
		String message4 = "";

		String extenstion = "";
		if (isDataEncryption && !MorphoInfo.isFVP) {
			extenstion = ".crypt";
		}

		for (int i = 0; i < nbFinger; ++i) {
			if ((MorphoInfo.isFVP && (templateFp == TemplateType.MORPHO_NO_PK_FP) && (templateFvp == TemplateFVPType.MORPHO_NO_PK_FVP)) || (MorphoInfo.isFVP && templateFvp != TemplateFVPType.MORPHO_NO_PK_FVP)) {
				MorphoFVPTemplateType[] typTemplate = { MorphoFVPTemplateType.MORPHO_NO_PK_FVP };
				ArrayList<Byte> dataTemplate = new ArrayList<Byte>();
				short[] pkFVPQuality = { 0 };
				short[] advancedSecurityLevelsCompatibility = { 0 };
				short[] dataIndex = { 0 };
				ret = templateList.getFVPTemplate((short) i, typTemplate, dataTemplate, pkFVPQuality, advancedSecurityLevelsCompatibility, dataIndex);

				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils.showErrorMessage(fonction, "An error occured while calling MorphoTemplateList.getFVPTemplate() function", ret, this.msoDemo.getMorphoDeviceInstance().getInternalError());
					return;
				}

				message1 = fonction + " Successful";
				message2 = "Finger #" + (i + 1) + " - Quality Score: " + pkFVPQuality[0];
				message3 = "";
				if (tabCapture.rbCaptureTypeEnroll.isSelected()) {
					message3 = "Advanced Security Levels Compatibility: " + (advancedSecurityLevelsCompatibility[0] == 1 ? "Yes" : "No");
				}
				message4 = "";
				new DialogResultWindow(msoDemo, message1, message2, message3, message4, "").setVisible(true);

				isQualityScoreNotified = true;
				if (templateFvp != TemplateFVPType.MORPHO_NO_PK_FVP) {
					String fvpFileName = "";
					FileFilter fvp;

					if (templateFvp == TemplateFVPType.MORPHO_PK_FVP) {
						fvpFileName = userID + "_finger_" + (i + 1) + ".fvp";
						fvp = new TemplateFileFilter("SAGEM PkFVP (.fvp)", ".fvp");
					} else {
						fvpFileName = userID + "_finger_" + (i + 1) + ".fvp-m";
						fvp = new TemplateFileFilter("SAGEM PkFVP Match (.fvp-m)", ".fvp-m");
					}

					JFileChooser fileChooser = new MorphoFileChooser();
					fileChooser.setSelectedFile(new File(fvpFileName));
					fileChooser.setAcceptAllFileFilterUsed(false);
					fileChooser.addChoosableFileFilter(fvp);
					fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));
					int userSelection = fileChooser.showSaveDialog(msoDemo);
					if (userSelection == JFileChooser.APPROVE_OPTION) {
						FilesMgt.fileChooserDirectory = fileChooser.getCurrentDirectory().getAbsolutePath();
						File fileToSave = fileChooser.getSelectedFile();
						String savedFilePath = fileToSave.getAbsolutePath();

						String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
						String savedFilePathTmp = savedFilePath.toLowerCase();
						if (!savedFilePathTmp.endsWith(ext)) {
							savedFilePath += ext;
						}

						// save into file
						try {
							FileOutputStream fos;
							fos = new FileOutputStream(savedFilePath);
							byte[] data = msoDemo.convertArrayListToByteArray(dataTemplate);
							boolean ok = true;
							if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) {
								int response = DialogUtils.showQuestionMessage("MSO_Demo Java", "Privacy mode : template data is crypted.\nWould you like to decrypt it before saving ?");
								if (0 == response) {
									byte[] privacyKey = msoDemo.getPrivacyModeKey();
									if (null != privacyKey) {
										data = msoDemo.decryptAndCheckPrivacyData(data, privacyKey);
										if (null == data) {
											ok = false;
											message1 = "FVP Template failed";
											message2 = "";
											message3 = "";
											message4 = "Error while decrypting data";
										}
									} else {
										ok = false;
										message1 = "FVP Template failed";
										message2 = "";
										message3 = "";
										message4 = "Invalid privacy key";
									}			
								} else if (1 != response) {
									ok = false;
								}
							} 
							
							if (ok) {
								fos.write(data);
								fos.close();
								
								message2 = "FVP Template successfully";
								message3 = "exported in file";
								message4 = savedFilePath;
							}
										
							new DialogResultWindow(msoDemo, message1, message2, message3, message4, "").setVisible(true);
							
						} catch (FileNotFoundException fnfe) {
							DialogUtils.showErrorMessage("Save FVP File " + savedFilePath, fnfe.getMessage());
						} catch (IOException ioe) {
							DialogUtils.showErrorMessage("Save FVP File" + savedFilePath, ioe.getMessage());
						}
					} else {
						new DialogResultWindow(msoDemo, fonction + " Successful", "Saving FVP template file", "aborted by user.", "", "").setVisible(true);
					}
				}
			}

			if (!MorphoInfo.isFVP || (MorphoInfo.isFVP && templateFp != TemplateType.MORPHO_NO_PK_FP)) {
				MorphoTemplateType[] typTemplate = { MorphoTemplateType.MORPHO_NO_PK_FP };
				ArrayList<Byte> dataTemplate = new ArrayList<Byte>();
				short[] dataIndex = { 0 };
				short[] pkFpQuality = { 0 };
				ret = templateList.getTemplate((short) i, typTemplate, dataTemplate, pkFpQuality, dataIndex);

				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils.showErrorMessage(fonction, "An error occured while calling MorphoTemplateList.getTemplate() function", ret, this.msoDemo.getMorphoDeviceInstance().getInternalError());
					return;
				}

				if (!isQualityScoreNotified) {
					message1 = fonction + " Successful";
					message2 = "Finger #" + (i + 1) + " - Quality Score: " + pkFpQuality[0];
					message3 = "";
					message4 = "";

					new DialogResultWindow(msoDemo, message1, message2, message3, message4, "").setVisible(true);
				}

				if (templateFp != TemplateType.MORPHO_NO_PK_FP) {
					String fpFileName = "";
					String fpExtension = "";
					FileFilter fp;

					fpExtension = templateFp.getExtension() + extenstion;
					fpFileName = userID + "_finger_" + (i + 1);

					if (templateFp == TemplateType.MORPHO_PK_DIN_V66400_CS_AA || templateFp == TemplateType.MORPHO_PK_ISO_FMC_CS_AA) {
						fpFileName += "_aa";
					}
					
					if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) { // privacy mode
					fpFileName += "_pcrypt";
					} 

					// fpFileName += fpExtension;

					fp = new TemplateFileFilter(templateFp.getLabel() + " (" + fpExtension + ")", fpExtension);

					JFileChooser fileChooser = new MorphoFileChooser();
					fileChooser.setAcceptAllFileFilterUsed(false);
					
					fileChooser.addChoosableFileFilter(fp);

					if (templateFp == TemplateType.MORPHO_PK_MAT) {
						fileChooser.addChoosableFileFilter(new TemplateFileFilter("SAGEM PkMat (.pkmat)", ".pkmat"));
					}

					if (templateFp == TemplateType.MORPHO_PK_PKLITE) {
						fileChooser.addChoosableFileFilter(new TemplateFileFilter("SAGEM Pklite (.pkl)", ".pkl"));
					}

					fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));
					fileChooser.setSelectedFile(new File(fpFileName));
					int userSelection = fileChooser.showSaveDialog(msoDemo);
					if (userSelection == JFileChooser.APPROVE_OPTION) {
						FilesMgt.fileChooserDirectory = fileChooser.getCurrentDirectory().getAbsolutePath();
						File fileToSave = fileChooser.getSelectedFile();
						String savedFilePath = fileToSave.getAbsolutePath();

						String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
						String savedFilePathTmp = savedFilePath.toLowerCase();
						if (!savedFilePathTmp.endsWith(ext)) {
							savedFilePath += ext;
						}

						// save into file
						try {
							FileOutputStream fos;
							fos = new FileOutputStream(savedFilePath);
							byte[] data = msoDemo.convertArrayListToByteArray(dataTemplate);
							boolean ok = true;
							if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) {
								int response = DialogUtils.showQuestionMessage("MSO_Demo Java", "Privacy mode : template data is crypted.\nWould you like to decrypt it before saving ?");
								if (0 == response) {
									byte[] privacyKey = msoDemo.getPrivacyModeKey();
									if (null != privacyKey) {
										data = msoDemo.decryptAndCheckPrivacyData(data, privacyKey);
										if (null == data) {
											ok = false;
											message1 = "FP Template failed";
											message2 = "";
											message3 = "";
											message4 = "Error while decrypting data";
										}
									} else {
										ok = false;
										message1 = "FP Template failed";
										message2 = "";
										message3 = "";
										message4 = "Invalid privacy key";
									}	
								} else if (1 != response) {
									ok = false;
								}
							} 
							
							if (ok) {
								fos.write(data);
								fos.close();
								
								message2 = "FP Template successfully";
								message3 = "exported in file";
								message4 = savedFilePath;
							}

							new DialogResultWindow(msoDemo, message1, message2, message3, message4, "").setVisible(true);
							
						} catch (FileNotFoundException fnfe) {
							DialogUtils.showErrorMessage("Save FP File " + savedFilePath, fnfe.getMessage());
						} catch (IOException ioe) {
							DialogUtils.showErrorMessage("Save FP File" + savedFilePath, ioe.getMessage());
						}
					} else {
						new DialogResultWindow(msoDemo, fonction + " Successful", "Saving FP template file", "aborted by user.", "", "").setVisible(true);
					}
				}
			}
		}
	}

	private void saveFVPFiles(String fonction, String userID, MorphoFVPTemplateType mFvpTemplateType, MorphoTemplateList templateList) {
		int ret = 0;

		short[] nbFVPTemplate = { 0 };
		templateList.getNbFVPTemplate(nbFVPTemplate);

		for (int i = 0; i < nbFVPTemplate[0]; ++i) {
			MorphoFVPTemplateType[] typTemplate = { MorphoFVPTemplateType.MORPHO_NO_PK_FVP };
			ArrayList<Byte> dataTemplate = new ArrayList<Byte>();
			short[] pkFVPQuality = { 0 };
			short[] advancedSecurityLevelsCompatibility = { 0 };
			short[] dataIndex = { 0 };
			ret = templateList.getFVPTemplate((short) i, typTemplate, dataTemplate, pkFVPQuality, advancedSecurityLevelsCompatibility, dataIndex);

			if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage(fonction, "An error occured while calling MorphoTemplateList.getFVPTemplate() function", ret, this.msoDemo.getMorphoDeviceInstance().getInternalError());
				return;
			}

			String message1 = fonction + " Successful";
			String message2 = "Finger #" + (i + 1) + " - Quality Score: " + pkFVPQuality[0];
			String message3 = "";
			if (tabCapture.rbCaptureTypeEnroll.isSelected()) {
				message3 = "Advanced Security Levels Compatibility: " + (advancedSecurityLevelsCompatibility[0] == 1 ? "Yes" : "No");
			}
			String message4 = "";

			new DialogResultWindow(msoDemo, message1, message2, message3, message4, "").setVisible(true);

			String fvpFileName = "";
			FileFilter fvp;

			if (mFvpTemplateType == MorphoFVPTemplateType.MORPHO_PK_FVP) {
				fvpFileName = userID + "_finger_" + (i + 1) + ".fvp";
				fvp = new TemplateFileFilter("SAGEM PkFVP (.fvp)", ".fvp");
			} else {
				fvpFileName = userID + "_finger_" + (i + 1) + ".fvp-m";
				fvp = new TemplateFileFilter("SAGEM PkFVP Match (.fvp-m)", ".fvp-m");
			}
			
			if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) { // privacy mode
					fvpFileName += "_pcrypt";
					}

			JFileChooser fileChooser = new MorphoFileChooser();
			fileChooser.setSelectedFile(new File(fvpFileName));
			fileChooser.setAcceptAllFileFilterUsed(false);
			fileChooser.addChoosableFileFilter(fvp);
			fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));
			int userSelection = fileChooser.showSaveDialog(msoDemo);
			if (userSelection == JFileChooser.APPROVE_OPTION) {
				new DialogResultWindow(msoDemo, message1, message2, message3, "", "").setVisible(true);
				FilesMgt.fileChooserDirectory = fileChooser.getCurrentDirectory().getAbsolutePath();
				File fileToSave = fileChooser.getSelectedFile();
				String savedFilePath = fileToSave.getAbsolutePath();

				String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
				String savedFilePathTmp = savedFilePath.toLowerCase();
				if (!savedFilePathTmp.endsWith(ext)) {
					savedFilePath += ext;
				}

				// save into file
				try {

					FileOutputStream fos;
					fos = new FileOutputStream(savedFilePath);

					byte[] data = msoDemo.convertArrayListToByteArray(dataTemplate);
					boolean ok = true;
					if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) {
						byte[] privacyKey = msoDemo.getPrivacyModeKey();
						if (null != privacyKey) {
							data = msoDemo.decryptAndCheckPrivacyData(data, privacyKey);
							if (null == data) {
								ok = false;
								message1 = "FVP Template failed";
								message2 = "";
								message3 = "";
								message4 = "Error while decrypting data";
							}
						} else {
							ok = false;
							message1 = "FVP Template failed";
							message2 = "";
							message3 = "";
							message4 = "Invalid privacy key";
						}						
					} 
					
					if (ok) {
						fos.write(data);
						fos.close();
						
						message2 = "FVP Template successfully";
						message3 = "exported in file";
						message4 = savedFilePath;
					}

					new DialogResultWindow(msoDemo, message1, message2, message3, message4, "").setVisible(true);

				} catch (FileNotFoundException fnfe) {
					DialogUtils.showErrorMessage("Save FVP File " + savedFilePath, fnfe.getMessage());
				} catch (IOException ioe) {
					DialogUtils.showErrorMessage("Save FVP File" + savedFilePath, ioe.getMessage());
				} 
			} else {
				new DialogResultWindow(msoDemo, fonction + " Successful", "Saving FVP template file", "aborted by user.", "", "").setVisible(true);
			}
		}
	}

	/**
	 * Convert a user structure as a byte array.
	 * 
	 * @param userID
	 * @param userFirstname
	 * @param userLastname
	 * @return
	 */
	public byte[] getPersonalInformation(String userID, String userFirstname, String userLastname) {
		// Personal Information :
		// - Size ID : 2 bytes
		// - ID : <Size ID> bytes
		// - Size Firstname : 2 bytes
		// - Firstname : <Size Firstname> bytes
		// - Size Lastname : 2 bytes
		// - Lastname : <Size Lastname> bytes

		int personnalInformationLength = 2 + userID.length() + 2 + userFirstname.length() + 2 + userLastname.length();
		byte personnalInformation[] = new byte[personnalInformationLength];
		int position = 0;

		int length = userID.length();
		System.arraycopy(new byte[] { (byte) (length), (byte) (length >>> 8) }, 0, personnalInformation, position, 2);
		position += 2;

		System.arraycopy(userID.getBytes(), 0, personnalInformation, position, userID.length());
		position += userID.length();

		length = userFirstname.length();
		System.arraycopy(new byte[] { (byte) (length), (byte) (length >>> 8) }, 0, personnalInformation, position, 2);
		position += 2;

		System.arraycopy(userFirstname.getBytes(), 0, personnalInformation, position, userFirstname.length());
		position += userFirstname.length();

		length = userLastname.length();
		System.arraycopy(new byte[] { (byte) (length), (byte) (length >>> 8) }, 0, personnalInformation, position, 2);
		position += 2;

		System.arraycopy(userLastname.getBytes(), 0, personnalInformation, position, userLastname.length());
		position += userLastname.length();

		return personnalInformation;
	}

	public byte[] intToByteArray4(int value) {
		return new byte[] { (byte) value, (byte) (value >>> 8), (byte) (value >>> 16), (byte) (value >>> 24), };
	}

	public byte[] intToByteArray2(int value) {
		return new byte[] { (byte) value, (byte) (value >>> 8) };
	}

	private int savePKSFile(String fpFileName, byte[] personnalInformation, MorphoTemplateList mTemplateList, boolean isDataEncryption) {
		int ret = 0;
		String extenstion = "";
		if (isDataEncryption && !MorphoInfo.isFVP) {
			extenstion = ".crypt";
		}

		int response = 0;

		// ---------------------
		// Pks File format
		// ---------------------
		// the .pks file structure :
		// - Personnal Information
		// - Nb of templates: 1 byte (1 or 2 fingers)
		// - Size 1st Tplate: 2 bytes
		// - 1st Template : <Size 1st Tplate> bytes
		// - Size 2nd Tplate: 2 bytes (if exists)
		// - 2nd Template : <Size 2nd Tplate> bytes (if exists)

		JFileChooser fileChooser = new MorphoFileChooser();
		fileChooser.setSelectedFile(new File(fpFileName));
		FileFilter pks = new TemplateFileFilter("SAGEM Pks (.pks" + extenstion + ")", ".pks" + extenstion);
		fileChooser.setAcceptAllFileFilterUsed(false);
		fileChooser.addChoosableFileFilter(pks);
		fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));

		int userSelection = fileChooser.showSaveDialog(msoDemo);
		if (userSelection == JFileChooser.APPROVE_OPTION) {

			FilesMgt.fileChooserDirectory = fileChooser.getCurrentDirectory().getAbsolutePath();

			short[] nbTemplate = { 0 };
			mTemplateList.getNbTemplate(nbTemplate);

			File fileToSave = fileChooser.getSelectedFile();
			String savedFilePath = fileToSave.getAbsolutePath();

			String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
			String savedFilePathTmp = savedFilePath.toLowerCase();
			if (!savedFilePathTmp.endsWith(ext)) {
				savedFilePath += ext;
			}

			// save into file
			FileOutputStream fos;
			try {
				OutputStream outputStream = null;
				fos = new FileOutputStream(savedFilePath);

				if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) { // privacy mode
					outputStream = new ByteArrayOutputStream();

				} else {
					outputStream = fos;
				}

				outputStream.write(personnalInformation);
				outputStream.write((byte) (nbTemplate[0]));

				for (int i = 0; i < nbTemplate[0]; ++i) {
					MorphoTemplateType[] typTemplate = { MorphoTemplateType.MORPHO_NO_PK_FP };
					ArrayList<Byte> dataTemplate = new ArrayList<Byte>();
					short[] pkFpQuality = { 0 };
					short[] dataIndex = { 0 };
					ret = mTemplateList.getTemplate((short) i, typTemplate, dataTemplate, pkFpQuality, dataIndex);
					if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage("Capture",
								"An error occured while calling MorphoTemplateList.getTemplate() function", ret,
								this.msoDemo.getMorphoDeviceInstance().getInternalError());
						fos.close();
						return ret;
					}

					byte[] data = new byte[dataTemplate.size()];
					if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) { // privacy
																											// mode

						for (int j = 0; j < dataTemplate.size(); j++) {
							data[j] = dataTemplate.get(j);
						}

						byte[] privacyKey = msoDemo.getPrivacyModeKey();
						if (null != privacyKey) {
							byte[] decryptedData = msoDemo.decryptAndCheckPrivacyData(data, privacyKey);
							outputStream.write(intToByteArray2(decryptedData.length));
							outputStream.write(decryptedData);
						} else {
							DialogUtils.showErrorMessage("savePKSFile", "Invalid Kprivacy key");
							return -1;
						}

					} else {
						outputStream.write(intToByteArray2(dataTemplate.size()));
						for (int j = 0; j < dataTemplate.size(); j++) {
							data[j] = dataTemplate.get(j);
						}
						outputStream.write(data);
					}
				}
				

				if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) { // privacy mode
					response = DialogUtils.showQuestionMessage("MSO_Demo Java",
							"Privacy mode : template data is crypted.\nWould you like to decrypt it before saving ?");
					
					byte[] outputBuffer = ((ByteArrayOutputStream) outputStream).toByteArray();
					
					if (0 == response) {
						// decrypt outputBuffer
						fos.write(outputBuffer);
					} else {
						byte[] privacyKey = msoDemo.getPrivacyModeKey();
						if (null != privacyKey) {
							byte[] iv = new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
							ArrayList<Byte> cryptedDataArray = new ArrayList<Byte>();
							ret = msoDemo.msoSecu.encryptAes128Cbc(privacyKey, outputBuffer, iv, true, cryptedDataArray);
							if (0 != ret) {
								DialogUtils.showErrorMessage("Privacy Mode ecryption", "An error occured while encrypting data", ret, 0);
								return -1;
							}
							byte[] cryptedData = msoDemo.convertArrayListToByteArray(cryptedDataArray);
							fos.write(cryptedData);
						} else {
							DialogUtils.showErrorMessage("savePKSFile", "Invalid Kprivacy key");
							return -1;
						}
					}
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

	private int saveImagesFiles(String fonction, String imageFilePath, byte[] imageBuffer, MorphoCompressAlgo cAlgorithm, int fonctionRet) {
		int ret = 0;

		JFileChooser fileChooser = new MorphoFileChooser();
		fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));
		fileChooser.setSelectedFile(new File(imageFilePath));
		String extention = "";
		if (msoDemo.isDataEncryption() && !MorphoInfo.isFVP) {
			extention = ".crypt";
		}

		fileChooser.setAcceptAllFileFilterUsed(false);

		if (cAlgorithm == MorphoCompressAlgo.MORPHO_NO_COMPRESS) { // image RAW/BMP
			FileFilter ffRaw = new TemplateFileFilter("Raw Files (.raw" + extention + ")", ".raw" + extention);
			fileChooser.addChoosableFileFilter(ffRaw);
			FileFilter ffBmp = new TemplateFileFilter("Bmp Files (.bmp" + extention + ")", ".bmp" + extention);
			fileChooser.addChoosableFileFilter(ffBmp);
		} else if (cAlgorithm == MorphoCompressAlgo.MORPHO_COMPRESS_V1) { // image V1 
			DialogUtils.showWarningMessage("Save Image File", "Compressed V1 image shall be decompressed after save !");
			FileFilter ffSagemV1 = new TemplateFileFilter("Bin Files (.bin" + extention + ")", ".bin" + extention);
			fileChooser.addChoosableFileFilter(ffSagemV1);			
		} else { // image WSQ
			FileFilter ffWsq = new TemplateFileFilter("Wsq Files (.Wsq" + extention + ")", ".Wsq" + extention);
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
			String savedFilePath = fileToSave.getAbsolutePath();

			String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
			String savedFilePathTmp = savedFilePath.toLowerCase();
			if (!savedFilePathTmp.endsWith(ext)) {
				savedFilePath += ext;
			}

			// save into file
			FileOutputStream fos;
			try {
				fos = new FileOutputStream(savedFilePath);
				fos.write(imageBuffer);
				fos.close();

				resulat = fonction + " Successful";
				message1 = "Image successfully";
				message2 = "exported in file";
				message3 = savedFilePath;

				if ((fonctionRet == MorphoSmartSDK.MORPHOERR_FFD)
					|| (fonctionRet == MorphoSmartSDK.MORPHOERR_MOIST_FINGER)
					|| (fonctionRet == MorphoSmartSDK.MORPHOERR_MOVED_FINGER)
					|| (fonctionRet == MorphoSmartSDK.MORPHOERR_SATURATED_FINGER)
					|| (fonctionRet == MorphoSmartSDK.MORPHOERR_INVALID_FINGER)) {
					message4 = "Warning : Low quality finger saved.";
				} 

				new DialogResultWindow(msoDemo, resulat, message1, message2, message3, message4).setVisible(true);

			} catch (FileNotFoundException fnfe) {
				DialogUtils.showErrorMessage("Save Image File", fnfe.getMessage());
				ret = -1;
			} catch (IOException ioe) {
				DialogUtils.showErrorMessage("Save Image File", ioe.getMessage());
				ret = -1;
			}

			return 0;
		} else {
			resulat = fonction + " Successful";
			message1 = "Saving image file";
			message2 = "aborted by user.";

			if ((fonctionRet == MorphoSmartSDK.MORPHOERR_FFD)
				|| (fonctionRet == MorphoSmartSDK.MORPHOERR_MOIST_FINGER)
				|| (fonctionRet == MorphoSmartSDK.MORPHOERR_MOVED_FINGER)
				|| (fonctionRet == MorphoSmartSDK.MORPHOERR_SATURATED_FINGER)
				|| (fonctionRet == MorphoSmartSDK.MORPHOERR_INVALID_FINGER)){
					message3 = "Saving low quality finger failed.";
			}

			new DialogResultWindow(msoDemo, resulat, message1, message2, message3, "").setVisible(true);
		}

		return ret;
	}
	
	private int saveImagesFiles(String fonction, String imageFilePath, MorphoImage morphoImage, MorphoCompressAlgo cAlgorithm, int fonctionRet) {
		int ret = 0;

		JFileChooser fileChooser = new MorphoFileChooser();
		fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));
		fileChooser.setSelectedFile(new File(imageFilePath));
		String extention = "";
		 if (msoDemo.isDataEncryption() && !MorphoInfo.isFVP) {
			extention = ".crypt";
		}

		fileChooser.setAcceptAllFileFilterUsed(false);

		if (cAlgorithm == MorphoCompressAlgo.MORPHO_NO_COMPRESS) {
			FileFilter ffRaw = new TemplateFileFilter("Raw Files (.raw" + extention + ")", ".raw" + extention);
			fileChooser.addChoosableFileFilter(ffRaw);
			FileFilter ffBmp = new TemplateFileFilter("Bmp Files (.bmp" + extention + ")", ".bmp" + extention);
			fileChooser.addChoosableFileFilter(ffBmp);
		} else if (cAlgorithm == MorphoCompressAlgo.MORPHO_COMPRESS_V1) { // image V1
			DialogUtils.showWarningMessage("Save Image File", "Compressed V1 image shall be decompressed after save !");
			FileFilter ffSagemV1 = new TemplateFileFilter("Bin Files (.bin" + extention + ")", ".bin" + extention);
			fileChooser.addChoosableFileFilter(ffSagemV1);
		} else { // image WSQ
			FileFilter ffWsq = new TemplateFileFilter("Wsq Files (.Wsq" + extention + ")", ".Wsq" + extention);
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
			String savedFilePath = fileToSave.getAbsolutePath();

			if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) { // privacy mode
			savedFilePath += "_pcrypt";
			}
			String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
			String savedFilePathTmp = savedFilePath.toLowerCase();			
			if (!savedFilePathTmp.endsWith(ext)) {
				savedFilePath += ext;
			}

			// save into file
			FileOutputStream fos;
			try {
				fos = new FileOutputStream(savedFilePath);
				if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) { // privacy mode
					fos.write(morphoImage.getImage());
				} else if (cAlgorithm == MorphoCompressAlgo.MORPHO_NO_COMPRESS) { // image RAW/BMP
					if (ext.equalsIgnoreCase(".bmp")) {
						fos.write(morphoImage.getBMPImage());
					} else {
						fos.write(morphoImage.getImage());
					}
				} else if (cAlgorithm == MorphoCompressAlgo.MORPHO_COMPRESS_V1) { // image V1
					fos.write(morphoImage.getCompressedImage());
				} else if (cAlgorithm == MorphoCompressAlgo.MORPHO_COMPRESS_WSQ) { // image WSQ
					if(!msoDemo.isDataEncryption())	{
						fos.write(morphoImage.getCompressedImage());
					} else {
						fos.write(morphoImage.getImage());
					}
				} else {
					fos.write(morphoImage.getImage());
				}
				fos.close();

				resulat = fonction + " Successful";
				message1 = "Image successfully";
				message2 = "exported in file";
				message3 = savedFilePath;

				if ((fonctionRet == MorphoSmartSDK.MORPHOERR_FFD)
					|| (fonctionRet == MorphoSmartSDK.MORPHOERR_MOIST_FINGER)
					|| (fonctionRet == MorphoSmartSDK.MORPHOERR_MOVED_FINGER)
					|| (fonctionRet == MorphoSmartSDK.MORPHOERR_SATURATED_FINGER)
					|| (fonctionRet == MorphoSmartSDK.MORPHOERR_INVALID_FINGER)) {
					message4 = "Warning : Low quality finger saved.";
				} 

				new DialogResultWindow(msoDemo, resulat, message1, message2, message3, message4).setVisible(true);

			} catch (FileNotFoundException fnfe) {
				DialogUtils.showErrorMessage("Save Image File", fnfe.getMessage());
				ret = -1;
			} catch (IOException ioe) {
				DialogUtils.showErrorMessage("Save Image File", ioe.getMessage());
				ret = -1;
			}

			return 0;
		} else {
			resulat = fonction + " Successful";
			message1 = "Saving image file";
			message2 = "aborted by user.";

			if ((fonctionRet == MorphoSmartSDK.MORPHOERR_FFD)
				|| (fonctionRet == MorphoSmartSDK.MORPHOERR_MOIST_FINGER)
				|| (fonctionRet == MorphoSmartSDK.MORPHOERR_MOVED_FINGER)
				|| (fonctionRet == MorphoSmartSDK.MORPHOERR_SATURATED_FINGER)
				|| (fonctionRet == MorphoSmartSDK.MORPHOERR_INVALID_FINGER)){
					message3 = "Saving low quality finger failed.";
			}

			new DialogResultWindow(msoDemo, resulat, message1, message2, message3, "").setVisible(true);
		}

		return ret;
	}

	private int saveTKBFile(String fpFileName, TKBHeader tkbh, ArrayList<Byte> dataX984, byte[] hostCertificate, boolean isDataEncryption) {

		int ret = 0;
		String extention = "";

		if (!MorphoInfo.isFVP && isDataEncryption) {
			extention = ".crypt";
		}

		JFileChooser fileChooser = new MorphoFileChooser();
		fileChooser.setSelectedFile(new File(fpFileName));
		FileFilter tkb = new TemplateFileFilter("Token Bio Files (.tkb" + extention + ")", ".tkb" + extention);
		fileChooser.setAcceptAllFileFilterUsed(false);
		fileChooser.addChoosableFileFilter(tkb);
		fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));

		int userSelection = fileChooser.showSaveDialog(msoDemo);
		if (userSelection == JFileChooser.APPROVE_OPTION) {

			FilesMgt.fileChooserDirectory = fileChooser.getCurrentDirectory().getAbsolutePath();

			File fileToSave = fileChooser.getSelectedFile();
			String savedFilePath = fileToSave.getAbsolutePath();

			String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
			String savedFilePathTmp = savedFilePath.toLowerCase();
			if (!savedFilePathTmp.endsWith(ext)) {
				savedFilePath += ext;
			}

			// save into file
			FileOutputStream fos;
			try {
				fos = new FileOutputStream(savedFilePath);
				fos.write(TKBHeader.magicNbByte);
				fos.write(TKBHeader.versionByte);
				fos.write(intToByteArray4(tkbh.tkbSize));
				fos.write(intToByteArray4(tkbh.certifSize));

				byte[] dataX984Byte = new byte[dataX984.size()];

				for (int i = 0; i < dataX984.size(); i++) {
					dataX984Byte[i] = dataX984.get(i);
				}

				fos.write(dataX984Byte);
				fos.write(hostCertificate);

				fos.close();
			} catch (FileNotFoundException fnfe) {
				DialogUtils.showErrorMessage("Save TKB File", fnfe.getMessage());
				ret = -1;
			} catch (IOException ioe) {
				DialogUtils.showErrorMessage("Save TKB File", ioe.getMessage());
				ret = -1;
			}

			return 0;
		} else {
			ret = -1;
		}

		return ret;
	}

	/**
	 * 
	 * @param matchingThreshold
	 * @return
	 */
	private MorphoFAR getMorphoFarValue(int matchingThreshold) {
		switch (matchingThreshold) {
		case 0:
			return MorphoFAR.MORPHO_FAR_0;
		case 1:
			return MorphoFAR.MORPHO_FAR_1;
		case 2:
			return MorphoFAR.MORPHO_FAR_2;
		case 3:
			return MorphoFAR.MORPHO_FAR_3;
		case 4:
			return MorphoFAR.MORPHO_FAR_4;
		case 5:
			return MorphoFAR.MORPHO_FAR_5;
		case 6:
			return MorphoFAR.MORPHO_FAR_6;
		case 7:
			return MorphoFAR.MORPHO_FAR_7;
		case 8:
			return MorphoFAR.MORPHO_FAR_8;
		case 9:
			return MorphoFAR.MORPHO_FAR_9;
		case 10:
			return MorphoFAR.MORPHO_FAR_10;
		default:
			return MorphoFAR.MORPHO_FAR_5;
		}
	}

	/**
	 * Identify process.
	 * 
	 * @return identify status
	 */
	private int identify() {
		TabBioSettings tabBioSettings = msoDemo.rightPanel.tabBioSettings;
		// timeout
		int timeout = Integer.parseInt(tabBioSettings.txtTimeout.getText().toString());
		// callbackMask
		int callbackCmd = msoDemo.callbackMask;
		callbackCmd &= ~MorphoCallbackCommand.MORPHO_CALLBACK_ENROLLMENT_CMD.swigValue();
		// FAR matchingThreshold
		int matchingThreshold = Integer.parseInt(tabBioSettings.txtMatchingThreshold.getText().toString());
		MorphoFAR far = getMorphoFarValue(matchingThreshold);
		// coderChoice
		String selectedCoder = (String) tabBioSettings.comboCoderChoice.getSelectedItem();
		int coderChoice = Coder.getValue(selectedCoder);
		// detectModeChoice
		int detectModeChoice;
		boolean isForceFingerPlacementOnTop = tabBioSettings.cbForceFingerPlacement.isSelected();
		if (isForceFingerPlacementOnTop) {
			detectModeChoice = DetectionMode.MORPHO_ENROLL_DETECT_MODE.getValue();
			detectModeChoice |= DetectionMode.MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE.getValue();
		} else {
			detectModeChoice = DetectionMode.MORPHO_VERIF_DETECT_MODE.getValue();			
		}
		detectModeChoice |= msoDemo.wakeUpMode;
		msoDemo.setStrategyAcquisitionMode((short) tabBioSettings.comboAcquisitionStrategy.getSelectedIndex());
		// callback function
		IMorphoEventHandler mEventHandlerCallback = new MorphoEventHandler(tabProcess);
		// matching
		long matchingStrategy = tabBioSettings.comboMatchingStrategy.getSelectedIndex();
		// prepare input data
		short[] fingerIndex = { 0 };
		int nbFingersToMatch = 1;
		MorphoUser identifiedUser = new MorphoUser();
		mDevice.setSecurityLevel(msoDemo.getSecurityLevel(EnumTabIndex.TAB_IDENTIFY));

		// identify
		int ret = mDatabase.identify(timeout, far, callbackCmd, mEventHandlerCallback, identifiedUser, null, fingerIndex, coderChoice, detectModeChoice, matchingStrategy, nbFingersToMatch);

		setFVPFingerState(ret);
		
		// response process
		String result = "User identified";
		String msg1 = "";
		String msg2 = "";
		String msg3 = "";
		String msg4 = "";
		if (OK == ret) {
			// privacy mode enable
			if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) { 
				int response = DialogUtils.showQuestionMessage("MSO_Demo Java", "Privacy mode : data is crypted.\nWould you like to decrypt it?");
				
				ArrayList<Byte> userID = new ArrayList<Byte>();
				ArrayList<Byte> firstName = new ArrayList<Byte>();
				ArrayList<Byte> lastName = new ArrayList<Byte>();
				
				// get data field
				identifiedUser.getBufferField(0, userID);
				identifiedUser.getBufferField(1, firstName);
				identifiedUser.getBufferField(2, lastName);
				
				byte[] idCrypted=msoDemo.convertArrayListToByteArray(userID);
				byte[] firstNameCrypted=msoDemo.convertArrayListToByteArray(firstName);
				byte[] lastNameCrypted=msoDemo.convertArrayListToByteArray(lastName);
				
				if (0 == response) {

					byte[] privacyKey = msoDemo.getPrivacyModeKey();
					if (null != privacyKey) {
						// decrypt data field
						byte[] idDecrypted=msoDemo.decryptAndCheckPrivacyData(idCrypted, privacyKey);
						byte[] firstNameDecrypted=msoDemo.decryptAndCheckPrivacyData(firstNameCrypted, privacyKey);
						byte[] lastNameDecrypted=msoDemo.decryptAndCheckPrivacyData(lastNameCrypted, privacyKey);
						
						msg1 = "Firstname : " + new String(firstNameDecrypted);
						msg2 = "Lastname : " + new String(lastNameDecrypted);
						msg4 = "ID : " + new String(idDecrypted);
					} else {
						DialogUtils.showErrorMessage("Identify", "Invalid Kprivacy key");
					}
					
				}else {
					msg1 = "Firstname : " + new String(firstNameCrypted);
					msg2 = "Lastname : " + new String(lastNameCrypted);
					msg4 = "ID : " + new String(idCrypted);
					
				}
				
			}else{
			ArrayList<String> id = new ArrayList<String>();
			ArrayList<String> firstName = new ArrayList<String>();
			ArrayList<String> lastName = new ArrayList<String>();
			identifiedUser.getField(0, id);
			identifiedUser.getField(1, firstName);
			identifiedUser.getField(2, lastName);
			msg1 = "Firstname : " + firstName.get(0);
			msg2 = "Lastname : " + lastName.get(0);
			msg4 = "ID : " + id.get(0);
		}
			beep();
			new DialogResultWindow(msoDemo, result, msg1, msg2, msg3, msg4).setVisible(true);
		} else {
			result = "Identification Failed";
			switch (ret) {
			case MorphoSmartSDK.MORPHOERR_CMDE_ABORTED:
				msg1 = "Command Aborted.";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_HIT:
			case MorphoSmartSDK.MORPHOERR_FFD:
			case MorphoSmartSDK.MORPHOERR_MOIST_FINGER:
			case MorphoSmartSDK.MORPHOERR_MOVED_FINGER:
			case MorphoSmartSDK.MORPHOERR_SATURATED_FINGER:
			case MorphoSmartSDK.MORPHOERR_INVALID_FINGER:
				msg1 = "Person not identified.";
				break;
			case MorphoSmartSDK.MORPHOERR_DB_EMPTY:
				msg1 = "The database is empty !";
				break;
			case MorphoSmartSDK.MORPHOERR_TIMEOUT:
				msg1 = "Timeout has expired.";
				msg2 = "Command Aborted.";
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
				msg2 = "C_MORPHO_Database::Identify() function";
				msg3 = ErrorsMgt.convertSDKError(ret);
				break;
			}
			new DialogResultWindow(msoDemo, result, msg1, msg2, msg3, msg4).setVisible(true);
		}

		return ret;
	}

	/**
	 * Verify.
	 * 
	 * @return
	 */
	private int verify() {
		if (tabVerify.rbFileSimulation.isSelected()) {
			return verityWithFile();
		} else {
			return verifyInBase();
		}
	}

	/**
	 * Verify with file.
	 * 
	 * @return
	 */
	private int verityWithFile() {
		int ret = 0;
		boolean isTkbFile = false;
		ArrayList<String> templateListArray = FilesMgt.getTemplateFiles("Verify", true, msoDemo);
		byte[] privacyKey = null;
		
		if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus)
		{
			privacyKey = msoDemo.getPrivacyModeKey();
		}
		
		byte[] random = new byte[4];
		new Random().nextBytes(random);	
		if (templateListArray.size() == 0)
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
				short[] indexTemplate = { 0 };
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
				if (isTkbFile && templateListArray.size() > 1) {
					DialogUtils.showWarningMessage("Mso_Demo", "You cannot use more than one tkb file.");
					return ret;
				}

				UserData userData;
				if(msoDemo.PrivacyModeStatus!= MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED) {

					if (null != privacyKey) {
						userData = UsersMgt.getUserDataFromFile(filePath, true ,privacyKey , random, msoDemo);
					}else {
						DialogUtils.showErrorMessage("verityWithFile", "Invalid Kprivacy key");
						return -1;
					}
					
				} else {
					
					userData = UsersMgt.getUserDataFromFile(filePath);	
				}
		
				if (userData.getNbFinger() == 0)
					return -1;
				usersData.add(userData);

				if (isTkbFile) {
					ret = morphoTemplateList.putX984(userData.getPkX984TemplateData());
					if (ret != OK) {
						DialogUtils.showErrorMessage("Verify", "An error occured while calling MorphoTemplateList.putX984() function", ret, mDevice.getInternalError());
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

					if (ret != OK) {
						DialogUtils.showErrorMessage("Verify", "An error occured while calling MorphoTemplateList." + fct + "() function", ret, mDevice.getInternalError());
						return ret;
					}
				}
			}
		}
		// Prepare data
		TabBioSettings tabBioSettings = msoDemo.rightPanel.tabBioSettings;
		TabOptions tabOptions = msoDemo.rightPanel.tabOptions;
		// timeout
		int timeout = Integer.parseInt(tabBioSettings.txtTimeout.getText().toString());
		// callback mask
		int callbackMask = msoDemo.callbackMask;
		callbackMask &= ~MorphoCallbackCommand.MORPHO_CALLBACK_ENROLLMENT_CMD.swigValue();
		// FAR matchingThreshold
		int matchingThreshold = Integer.parseInt(tabBioSettings.txtMatchingThreshold.getText().toString());
		MorphoFAR far = getMorphoFarValue(matchingThreshold);
		// coderChoice
		String selectedCoder = (String) tabBioSettings.comboCoderChoice.getSelectedItem();
		int coderChoice = Coder.getValue(selectedCoder);
		// detectModeChoice
		int detectModeChoice;
		boolean isForceFingerPlacementOnTop = tabBioSettings.cbForceFingerPlacement.isSelected();
		if (isForceFingerPlacementOnTop) {
			detectModeChoice = DetectionMode.MORPHO_ENROLL_DETECT_MODE.getValue();
			detectModeChoice |= DetectionMode.MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE.getValue();
		} else {
			detectModeChoice = DetectionMode.MORPHO_VERIF_DETECT_MODE.getValue();			
		}
		
		detectModeChoice |= msoDemo.wakeUpMode;
		// security Level
		mDevice.setSecurityLevel(msoDemo.getSecurityLevel(EnumTabIndex.TAB_VERIFY));
		// Strategy Acquisition Mode
		msoDemo.setStrategyAcquisitionMode((short) tabBioSettings.comboAcquisitionStrategy.getSelectedIndex());
		// callback function
		IMorphoEventHandler mEventHandlerCallback = new MorphoEventHandler(tabProcess);
		// matching
		long matchingStrategy = tabBioSettings.comboMatchingStrategy.getSelectedIndex();
		// matchingScore
		long[] matchingScore = { 0 };
		// exportNumPk
		byte[] exportNumPk = null;
		if (tabOptions.cbExportMatchingPk.isSelected()) {
			exportNumPk = new byte[] { 0 };
		}

		if (msoDemo.isTUNNELING() || msoDemo.isOFFERED_SECURITY()) {
			matchingScore = null;
		}
		// verify
		ret = mDevice.verify(timeout, far, morphoTemplateList, callbackMask, mEventHandlerCallback, matchingScore, exportNumPk, coderChoice, detectModeChoice, matchingStrategy);

		setFVPFingerState(ret);
		
		// response process
		String msg1 = "";
		String msg2 = "";
		String msg3 = "";
		String msg4 = "";
		if (ret != OK) {
			switch (ret) {
			case MorphoSmartSDK.MORPHOERR_CMDE_ABORTED:
				msg1 = "Command Aborted.";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_HIT:
			case MorphoSmartSDK.MORPHOERR_FFD:
			case MorphoSmartSDK.MORPHOERR_MOIST_FINGER:
			case MorphoSmartSDK.MORPHOERR_MOVED_FINGER:
			case MorphoSmartSDK.MORPHOERR_SATURATED_FINGER:
			case MorphoSmartSDK.MORPHOERR_INVALID_FINGER:
				msg1 = "Person not Authenticated.";
				break;
			case MorphoSmartSDK.MORPHOERR_TIMEOUT:
				msg1 = "Timeout has expired.";
				msg2 = "Command Aborted.";
				break;
			case MorphoSmartSDK.MORPHOERR_BADPARAMETER:
				msg1 = "One or more input parameters are out of range";
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
			case MorphoSmartSDK.MORPHOERR_LICENSE_MISSING:
				msg1 = "A required license is missing.";
				break;
			case MorphoSmartSDK.MORPHOERR_CERTIF_UNKNOW:
				// the MSO can not verify the X984 biometric token, the MSO does
				// not have the certificate.
				// the new certificate is stored in the MSO.
				// the new X984 biometric token can be used.
				// TODO extractAndStoreCertif(fileName);
				DialogUtils.showWarningMessage("Verify", "X984 biometric token has just been stored in the MSO.\nPlease redo the Verify process.");
				break;
			default:
				msg1 = "An error occured while calling";
				msg2 = "MorphoDatabase.Verify() function";
				msg3 = ErrorsMgt.convertSDKError(ret);
				break;
			}
			new DialogResultWindow(msoDemo, "Verification Failed", msg1, msg2, msg3, msg4).setVisible(true);
		} else {
			beep();
			// Get authenticated user index
			int indexUser = -1;
			if (exportNumPk != null) {
				if (exportNumPk[0] >= 0 && exportNumPk[0] < 20) {
					indexUser = exportNumPk[0];
				}
				msg4 = "Matching Pk Number : " + exportNumPk[0] + "\n";
			} else {
				if (templateListArray.size() == 1) {
					indexUser = 0;
				}
			}
			// Get user details
			if (indexUser != -1) {
				if(isTkbFile) indexUser = 0;
				UserData userdata = usersData.get(indexUser);
				msg1 = "Firstname : " + userdata.getFirstName();
				msg2 = "Lastname : " + userdata.getLastName();
				msg3 = "ID : " + userdata.getUserID();
				String fileName = FilesMgt.getFileNameFromPath(templateListArray.get(indexUser));
				msg4 += "Matching File : [" + fileName + "]";
				msg4 = DialogResultWindow.formatText(msg4);
			} else {
				msg2 = "You should enable the";
				msg3 = "\"Export Matching Pk Number\"";
				msg4 = "to retrieve the information about the matching file";
			}
			new DialogResultWindow(msoDemo, "User authenticated", msg1, msg2, msg3, msg4).setVisible(true);
		}

		return ret;
	}

	/**
	 * Verify in base.
	 * 
	 * @return
	 */
	private int verifyInBase() {
		String selectedUserID = msoDemo.rightPanel.getSelectedUserId();
		if (selectedUserID.equals("")) {
			DialogUtils.showWarningMessage("MSO_Demo", "Select a user in the list view.");
			return 0;
		}
		int ret = 0;
		// get user data
		MorphoUser mUser = new MorphoUser();
		
		if(MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus){
			byte[] privacyKey = msoDemo.getPrivacyModeKey();
			if (null != privacyKey) {
				byte[] random = new byte[4];
				new Random().nextBytes(random);
				byte[] crypteddata = msoDemo.encryptPrivacyData(selectedUserID.getBytes(),privacyKey,random);
				ret = mDatabase.getUserBuffer(crypteddata, mUser);
			} else {
				DialogUtils.showErrorMessage("Verify in base", "Invalid Kprivacy key");
				return -1;
			}

		}else {
			ret = mDatabase.getUser(selectedUserID, mUser);
		}
		
		if (ret != MorphoSmartSDK.MORPHO_OK) {
			DialogUtils.showErrorMessage("MSO_Demo", "An error occured while calling MorphoDatabase.getUser() function");
			return ret;
		}
		TabBioSettings tabBioSettings = msoDemo.rightPanel.tabBioSettings;
		TabOptions tabOptions = msoDemo.rightPanel.tabOptions;
		// timeout
		int timeout = Integer.parseInt(tabBioSettings.txtTimeout.getText().toString());
		// callback mask
		int callbackMask = msoDemo.callbackMask;
		callbackMask &= ~MorphoCallbackCommand.MORPHO_CALLBACK_ENROLLMENT_CMD.swigValue();
		// FAR matchingThreshold
		int matchingThreshold = Integer.parseInt(tabBioSettings.txtMatchingThreshold.getText().toString());
		MorphoFAR far = getMorphoFarValue(matchingThreshold);
		// coderChoice
		String selectedCoder = (String) tabBioSettings.comboCoderChoice.getSelectedItem();
		int coderChoice = Coder.getValue(selectedCoder);
		// detectModeChoice
		int detectModeChoice;
		boolean isForceFingerPlacementOnTop = tabBioSettings.cbForceFingerPlacement.isSelected();
		if (isForceFingerPlacementOnTop) {
			detectModeChoice = DetectionMode.MORPHO_ENROLL_DETECT_MODE.getValue();
			detectModeChoice |= DetectionMode.MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE.getValue();
		} else {
			detectModeChoice = DetectionMode.MORPHO_VERIF_DETECT_MODE.getValue();			
		}
		detectModeChoice |= msoDemo.wakeUpMode;
		// security Level
		mDevice.setSecurityLevel(msoDemo.getSecurityLevel(EnumTabIndex.TAB_VERIFY));
		// Strategy Acquisition Mode
		msoDemo.setStrategyAcquisitionMode((short) tabBioSettings.comboAcquisitionStrategy.getSelectedIndex());
		// callback function
		IMorphoEventHandler mEventHandlerCallback = new MorphoEventHandler(tabProcess);
		// matching
		long matchingStrategy = tabBioSettings.comboMatchingStrategy.getSelectedIndex();
		// matchingScore
		long[] matchingScore = { 0 };

		if (msoDemo.isTUNNELING() || msoDemo.isOFFERED_SECURITY()) {
			matchingScore = null;
		}

		// exportNumPk
		byte[] exportNumPk = null;
		if (tabOptions.cbExportMatchingPk.isSelected()) {
			exportNumPk = new byte[] { 0 };
		}

		// Verify process
		ret = mUser.verify(timeout, far, callbackMask, mEventHandlerCallback, matchingScore, exportNumPk, coderChoice, detectModeChoice, matchingStrategy);

		setFVPFingerState(ret);
		
		// response
		String result = "User authenticated";
		String msg1 = "";
		String msg2 = "";
		String msg3 = "";
		String msg4 = "";
		if (OK == ret) {
			
			
			if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) { // privacy mode
				int response = DialogUtils.showQuestionMessage("MSO_Demo Java", "Privacy mode : data is crypted.\nWould you like to decrypt it?");
				
				ArrayList<Byte> userID = new ArrayList<Byte>();
				ArrayList<Byte> firstName = new ArrayList<Byte>();
				ArrayList<Byte> lastName = new ArrayList<Byte>();
				
				// get data field
				mUser.getBufferField(0, userID);
				mUser.getBufferField(1, firstName);
				mUser.getBufferField(2, lastName);
				
				byte[] idCrypted=msoDemo.convertArrayListToByteArray(userID);
				byte[] firstNameCrypted=msoDemo.convertArrayListToByteArray(firstName);
				byte[] lastNameCrypted=msoDemo.convertArrayListToByteArray(lastName);
				
				if (0 == response) {

					byte[] privacyKey = msoDemo.getPrivacyModeKey();
					if (null != privacyKey) {
						// decrypt data field
						byte[] idDecrypted=msoDemo.decryptAndCheckPrivacyData(idCrypted, privacyKey);
						byte[] firstNameDecrypted=msoDemo.decryptAndCheckPrivacyData(firstNameCrypted, privacyKey);
						byte[] lastNameDecrypted=msoDemo.decryptAndCheckPrivacyData(lastNameCrypted, privacyKey);
						
						msg1 = "Firstname : " + new String(firstNameDecrypted);
						msg2 = "Lastname : " + new String(lastNameDecrypted);
						msg4 = "ID : " + new String(idDecrypted);
					} else {
						DialogUtils.showErrorMessage("Verify in base", "Invalid Kprivacy key");
					}
					
				}else {
					msg1 = "Firstname : " + new String(firstNameCrypted);
					msg2 = "Lastname : " + new String(lastNameCrypted);
					msg4 = "ID : " + new String(idCrypted);
					
				}
				
			}else{
				
			ArrayList<String> firstName = new ArrayList<String>();
			ArrayList<String> lastName = new ArrayList<String>();
			mUser.getField(1, firstName);
			mUser.getField(2, lastName);
			msg1 = "Firstname : " + firstName.get(0);
			msg2 = "Lastname : " + lastName.get(0);
			msg4 = "ID : " + selectedUserID;
			}
			if (exportNumPk != null) {
				msg4 = "Matching Pk Number : " + exportNumPk[0];
			}
			beep();
			new DialogResultWindow(msoDemo, result, msg1, msg2, msg3, msg4).setVisible(true);
		} else {
			result = "Verification Failed";
			switch (ret) {
			case MorphoSmartSDK.MORPHOERR_CMDE_ABORTED:
				msg1 = "Command Aborted.";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_HIT:
			case MorphoSmartSDK.MORPHOERR_FFD:
			case MorphoSmartSDK.MORPHOERR_MOIST_FINGER:
			case MorphoSmartSDK.MORPHOERR_MOVED_FINGER:
			case MorphoSmartSDK.MORPHOERR_SATURATED_FINGER:
			case MorphoSmartSDK.MORPHOERR_INVALID_FINGER:
				msg1 = "Person not Authenticated.";
				break;
			case MorphoSmartSDK.MORPHOERR_TIMEOUT:
				msg1 = "Timeout has expired.";
				msg2 = "Command Aborted.";
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
			new DialogResultWindow(msoDemo, result, msg1, msg2, msg3, msg4).setVisible(true);
		}

		return ret;
	}

	/**
	 * Capture process.
	 * 
	 * @return
	 */
	private int getImage() {
		int ret = 0;
		int compressValue = Integer.parseInt(tabFpImage.textCompressionRate.getText().toString());
		if (tabFpImage.textCompressionRate.isEnabled() && (compressValue < 2 || compressValue > 255)) {
			DialogUtils.showErrorMessage("Get Image", "Bad Compression Rate parameter.\nCompress Rate should be in range [2 - 255]");
			return ret;
		}

		TabBioSettings tabBioSettings = msoDemo.rightPanel.tabBioSettings;
		// check if "Force Finger Placement on Top" is selected
		if (tabBioSettings.cbForceFingerPlacement.isSelected() && tabFpImage.rbVerifyDetectionMode.isSelected()) {
			DialogUtils.showWarningMessage("Mso_Demo", "GetImage in \"Verify detection mode\"  and \"Force Finger Placement on Top\"" + " option cannot be used together.\nUncheck this option before proceeding ...");
			return ret;
		}
		// timeout
		int timeout = Integer.parseInt(tabBioSettings.txtTimeout.getText().toString());
		// callBack Mask
		int callbackCmd = msoDemo.callbackMask;
		callbackCmd &= ~MorphoCallbackCommand.MORPHO_CALLBACK_ENROLLMENT_CMD.swigValue();
		// acquisitionThreshold
		short acquisitionThreshold = 0;
		if (tabBioSettings.cbFingerQualityThreshold.isSelected()) {
			String quality = tabBioSettings.txtFingerQualityThreshold.getText().toString().trim();
			if (!quality.equals("-"))
				acquisitionThreshold = Short.parseShort(quality);
		}
		// security Level
		mDevice.setSecurityLevel(msoDemo.getSecurityLevel(EnumTabIndex.TAB_FINGERPTINT_IMAGE));
		// get compression type
		MorphoCompressAlgo compressAlgo = MorphoCompressAlgo.MORPHO_NO_COMPRESS;
		short compressRate = 0;
		String imageFormat = tabFpImage.comboImageFormat.getSelectedItem().toString();
		if (imageFormat.equals(EnumImageFormat.WSQ.toString())) {
			compressRate = (short) compressValue;
			compressAlgo = MorphoCompressAlgo.MORPHO_COMPRESS_WSQ;
			if (!MorphoInfo.isWSQSupported) {
				DialogUtils.showErrorMessage("Get Image", "This format is not supported by this software version.");
				return ret;
			}
		} else if (imageFormat.equals(EnumImageFormat.RAW.toString())) {
			compressAlgo = MorphoCompressAlgo.MORPHO_NO_COMPRESS;
		} else {
			compressAlgo = MorphoCompressAlgo.MORPHO_COMPRESS_V1;
		}
		// detectModeChoice
		int detectModeChoice = DetectionMode.MORPHO_ENROLL_DETECT_MODE.getValue();
		if (tabFpImage.rbVerifyDetectionMode.isSelected())
			detectModeChoice = DetectionMode.MORPHO_VERIF_DETECT_MODE.getValue();
		boolean isForceFingerPlacementOnTop = tabBioSettings.cbForceFingerPlacement.isSelected();
		if (isForceFingerPlacementOnTop) {
			detectModeChoice |= DetectionMode.MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE.getValue();
		}
		detectModeChoice |= msoDemo.wakeUpMode;
		// latentDetection
		short latentDetection = (short) (tabFpImage.cbLatentDetect.isSelected() ? LatentDetection.LATENT_DETECT_ENABLE.getValue() : LatentDetection.LATENT_DETECT_DISABLE.getValue());
		// callback function
		IMorphoEventHandler mEventHandlerCallback = new MorphoEventHandler(tabProcess);
		// image
		MorphoImage image = new MorphoImage();

		// Get Image
		ret = mDevice.getImage(timeout, acquisitionThreshold, compressAlgo, compressRate, (long) callbackCmd, mEventHandlerCallback, image, (short) detectModeChoice, latentDetection);

		setFVPFingerState(ret);
		
		// response process
		String result = "";
		String msg1 = "";
		String msg2 = "";
		String msg3 = "";
		String msg4 = "";
		if (ret != OK) {
			result = "GetImage Failed";
			switch (ret) {
			case MorphoSmartSDK.MORPHOERR_NO_HIT:
				msg1 = "Bad GetImage Sequence.";
				break;
			case MorphoSmartSDK.MORPHOERR_CMDE_ABORTED:
				msg1 = "Command aborted by user.";
				break;
			case MorphoSmartSDK.MORPHOERR_TIMEOUT:
				msg1 = "Timeout has expired.";
				msg2 = "Command aborted.";
				break;
			case MorphoSmartSDK.MORPHOERR_FFD:
			case MorphoSmartSDK.MORPHOERR_MOIST_FINGER:
			case MorphoSmartSDK.MORPHOERR_MOVED_FINGER:
			case MorphoSmartSDK.MORPHOERR_SATURATED_FINGER:
			case MorphoSmartSDK.MORPHOERR_INVALID_FINGER:
				msg1 = "Warning : Low quality finger !!!";
				break;
			case MorphoSmartSDK.MORPHOERR_LICENSE_MISSING:
				msg1 = "A required license is missing.";
				break;
			default:
				msg1 = "An error occured while calling";
				msg2 = "MorphoDevice.getImage() function";
				msg3 = ErrorsMgt.convertSDKError(ret);
				break;
			}
		}

		if ( (ret == OK || ret == MorphoSmartSDK.MORPHOERR_FFD || ret == MorphoSmartSDK.MORPHOERR_MOIST_FINGER)
				|| (ret == MorphoSmartSDK.MORPHOERR_MOVED_FINGER || ret == MorphoSmartSDK.MORPHOERR_SATURATED_FINGER || ret == MorphoSmartSDK.MORPHOERR_INVALID_FINGER) ) {
			if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) { // privacy mode
				int response = DialogUtils.showQuestionMessage("MSO_Demo Java", "Privacy mode : image data is crypted.\nWould you like to decrypt it before saving ?");
				if (0 == response) {
					byte[] privacyKey = msoDemo.getPrivacyModeKey();
					if (null != privacyKey) {
						byte[] decryptedImageWithHeader = msoDemo.decryptAndCheckPrivacyData(image.getImage(), privacyKey);
						if (null != decryptedImageWithHeader) {
							byte[] decryptedImage = null;
							if (compressAlgo == MorphoCompressAlgo.MORPHO_NO_COMPRESS) {
								// RAW image header is 12 bytes length, with associated data :
								// Header revision (1 byte)
								// Header size (1 byte)
								// Image row number in pixels (2 bytes)
								// Image column number in pixels (2 bytes)
								// Vertical resolution in dpi (2 bytes)
								// Horizontal resolution in dpi (2 bytes)
								// Compression algorithm used (1 byte)
								// Number of bits per pixel (1 byte)
								int imageNbRow = msoDemo.TwoBytesToIntValue(decryptedImageWithHeader[3], decryptedImageWithHeader[2]);
								int imageNbCol = msoDemo.TwoBytesToIntValue(decryptedImageWithHeader[5], decryptedImageWithHeader[4]);
								decryptedImage = Arrays.copyOfRange(decryptedImageWithHeader, 12, decryptedImageWithHeader.length);
								// display image
								new DialogGetImage(lblCurrentImageInfo.getText(), decryptedImage, imageNbCol, imageNbRow).setVisible(true);
							} else if (compressAlgo == MorphoCompressAlgo.MORPHO_COMPRESS_WSQ) {
								// WSQ image header is 12 bytes length, in addition to RAW header (12 bytes), so ignore first 24 first bytes
								decryptedImage = Arrays.copyOfRange(decryptedImageWithHeader, 24, decryptedImageWithHeader.length);
							} else {
								decryptedImage = Arrays.copyOf(decryptedImageWithHeader, decryptedImageWithHeader.length);
							}
							// save image
							saveImagesFiles("GetImage", "Image", decryptedImage, compressAlgo, ret);
						} else {
							return -1;
						}
					} else {
						DialogUtils.showErrorMessage("Capture", "Invalid Kprivacy key");
						return -1;
					}
				} else if (1 == response) {
					saveImagesFiles("GetImage", "Image", image, compressAlgo, ret);
				}
			} else { // privacy mode not enabled
				// display RAW image
				if (compressAlgo == MorphoCompressAlgo.MORPHO_NO_COMPRESS) {
					//new DialogGetImage(msoDemo, lblCurrentImageInfo.getText(), image).setVisible(true);
					new DialogGetImage(lblCurrentImageInfo.getText(), image).setVisible(true);
				}
				// save image
				saveImagesFiles("GetImage", "Image", image, compressAlgo, ret);
			}
		} else {
			new DialogResultWindow(msoDemo, result, msg1, msg2, msg3, msg4).setVisible(true);
		}

		return ret;
	}

	/**
	 * Enroll process.
	 * 
	 * @return
	 */
	private int enroll() {
		TabBioSettings tabBioSettings = msoDemo.rightPanel.tabBioSettings;
		int ret = 0;
		
		// timeout
		int timeout = Integer.parseInt(tabBioSettings.txtTimeout.getText().toString());
		
		// callback
		int callbackCmd = msoDemo.callbackMask;
		
		// acquisitionThreshold
		short acquisitionThreshold = 0;
		if (tabBioSettings.cbFingerQualityThreshold.isSelected()) {
			String quality = tabBioSettings.txtFingerQualityThreshold.getText().toString().trim();
			if (!quality.equals("-"))
				acquisitionThreshold = Short.parseShort(quality);
		}
		
		// advancedSecurityLevelsRequired
		short advancedSecurityLevelsRequired = (short) (tabBioSettings.cbAdvancedSecurityLevel.isSelected() ? 1 : 0);
		
		// coderChoice
		String selectedCoder = (String) tabBioSettings.comboCoderChoice.getSelectedItem();
		int coderChoice = Coder.getValue(selectedCoder);
		
		// detectModeChoice
		int detectModeChoice = DetectionMode.MORPHO_ENROLL_DETECT_MODE.getValue();
		boolean isForceFingerPlacementOnTop = tabBioSettings.cbForceFingerPlacement.isSelected();
		if (isForceFingerPlacementOnTop) {
			detectModeChoice |= DetectionMode.MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE.getValue();
		}
		detectModeChoice |= msoDemo.wakeUpMode;
		
		// StrategyAcquisitionMode
		msoDemo.setStrategyAcquisitionMode((short) tabBioSettings.comboAcquisitionStrategy.getSelectedIndex());
		
		// callback function
		IMorphoEventHandler mEventHandlerCallback = new MorphoEventHandler(tabProcess);
		
		// security Level
		mDevice.setSecurityLevel(msoDemo.getSecurityLevel(EnumTabIndex.TAB_ENROL));

		MorphoCompressAlgo compressAlgo = MorphoCompressAlgo.MORPHO_NO_COMPRESS;
		short compressRate = 0;
		short exportMinutiae = 0;
		short fingerNumber = (short) (tabEnroll.rbNbOfFinger2.isSelected() ? 2 : 1);
		MorphoTemplateType fpTemplateType = MorphoTemplateType.MORPHO_NO_PK_FP;
		MorphoFVPTemplateType fvpTemplateType = MorphoFVPTemplateType.MORPHO_NO_PK_FVP;
		
		// save PK in Database
		short saveRecord = (short) (tabEnroll.cbSavePkInDB.isSelected() ? 1 : 0);

		// FP Template
		String selectedTemplateType = (String) tabEnroll.comboFpTemplateType.getSelectedItem();
		TemplateType templateFp = TemplateType.getValue(selectedTemplateType);

		if (templateFp != TemplateType.MORPHO_PK_SAGEM_PKS) {
			fpTemplateType = MorphoTemplateType.swigToEnum(templateFp.getCode());
			if (fpTemplateType == MorphoTemplateType.MORPHO_NO_PK_FP) {
				if (!MorphoInfo.isFVP) {
					fpTemplateType = MorphoTemplateType.MORPHO_PK_COMP; // MSO_Demo always exports templates in MORPHO_PK_COMP format
				}
			}
		} else {
			fpTemplateType = MorphoTemplateType.MORPHO_PK_COMP;
		}

		// FVP Template
		String selectedFVPTemplateType = (String) tabEnroll.comboFvpTemplateType.getSelectedItem();
		TemplateFVPType templateFvp = TemplateFVPType.getValue(selectedFVPTemplateType);
		fvpTemplateType = (MorphoInfo.isFVP) ? MorphoFVPTemplateType.MORPHO_PK_FVP : MorphoFVPTemplateType.MORPHO_NO_PK_FVP;
		
		// exportMinutiae
		if (fpTemplateType != MorphoTemplateType.MORPHO_NO_PK_FP || fvpTemplateType != MorphoFVPTemplateType.MORPHO_NO_PK_FVP) {
			exportMinutiae = 1;
		}
		
		// output template
		MorphoTemplateList templateList = new MorphoTemplateList();
		templateList.setActiveFullImageRetrieving(false);
		// Image acquisition enabled
		if (tabEnroll.comboExportImage.getSelectedIndex() != 0) { // Image will be exported
			templateList.setActiveFullImageRetrieving(true);
			String exportImage = tabEnroll.comboExportImage.getSelectedItem().toString();
			if (exportImage.equals(EnumExportImage.WSQ.toString())) {
				compressAlgo = MorphoCompressAlgo.MORPHO_COMPRESS_WSQ;
				compressRate = 15;
			} else if (exportImage.equals(EnumExportImage.RAW.toString())) {
				compressAlgo = MorphoCompressAlgo.MORPHO_NO_COMPRESS;
				compressRate = 0;
			} else if (exportImage.equals(EnumExportImage.SAGEM_V1.toString())) {
				compressAlgo = MorphoCompressAlgo.MORPHO_COMPRESS_V1;
				compressRate = 0;
			}
		}
		
		// checking parameters
		if (!templateList.getActiveFullImageRetrieving() && saveRecord == 0 && fpTemplateType == MorphoTemplateType.MORPHO_NO_PK_FP && fvpTemplateType == MorphoFVPTemplateType.MORPHO_NO_PK_FVP) {
			ret = MorphoSmartSDK.MORPHOERR_BADPARAMETER;
			return ret;
		}

		// create a new user
		MorphoUser user = new MorphoUser();
		
		byte[] privacyKey = null;
		byte[] random = null;
		
		// get user ID
		String userID = tabEnroll.textIdNumber.getText();
		if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) {
			// Generate RND32
			random = new byte[4];
			new Random().nextBytes(random);
			privacyKey = msoDemo.getPrivacyModeKey();
			if (null == privacyKey) {
				DialogUtils.showErrorMessage("MSO_Demo", "Invalid Kprivacy key");
				return -1;
			} 
			byte[] userIDEncrypted = msoDemo.encryptPrivacyData(userID.getBytes(), privacyKey, random);
			if (null == userIDEncrypted) {
				return -1;
			}
			ret = mDatabase.getUserBuffer(userIDEncrypted, user);
			if (ret != OK) {
				DialogUtils.showErrorMessage("MSO_Demo", "An error occured while calling MorphoDatabase.getUserBuffer() function", ret, mDevice.getInternalError());
				return ret;
			}
		} else {
			ret = mDatabase.getUser(userID, user);
			if (ret != OK) {
				DialogUtils.showErrorMessage("MSO_Demo", "An error occured while calling MorphoDatabase.getUser() function", ret, mDevice.getInternalError());
				return ret;
			}
		}

		// adding user fields
		String firstName = tabEnroll.textFirstName.getText();
		String lastName = tabEnroll.textLastName.getText();
		
		if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) {
			byte[] firstNameEncrypted = msoDemo.encryptPrivacyData(firstName.getBytes(), privacyKey, random);
			if (null == firstNameEncrypted) {
				return -1;
			}
			ret = user.putBufferField(MorphoUserFields.FIRSTNAME_FIELD_INDEX.getValue(), firstNameEncrypted);
			if (ret != OK) {
				DialogUtils.showErrorMessage("MSO_Demo", "An error occured while calling MorphoUser.putBufferField() function", ret, mDevice.getInternalError());
				return ret;
			}
			byte[] lastNameEncrypted = msoDemo.encryptPrivacyData(lastName.getBytes(), privacyKey, random);
			if (null == lastNameEncrypted) {
				return -1;
			}
			ret = user.putBufferField(MorphoUserFields.LASTTNAME_FIELD_INDEX.getValue(), lastNameEncrypted);
			if (ret != OK) {
				DialogUtils.showErrorMessage("MSO_Demo", "An error occured while calling MorphoUser.putBufferField() function", ret, mDevice.getInternalError());
				return ret;
			}
		} else {
			ret = user.putField(MorphoUserFields.FIRSTNAME_FIELD_INDEX.getValue(), firstName);
			if (ret != OK) {
				DialogUtils.showErrorMessage("MSO_Demo", "An error occured while calling MorphoUser.putField() function", ret, mDevice.getInternalError());
				return ret;
			}
			ret = user.putField(MorphoUserFields.LASTTNAME_FIELD_INDEX.getValue(), lastName);
			if (ret != OK) {
				DialogUtils.showErrorMessage("MSO_Demo", "An error occured while calling MorphoUser.putField() function", ret, mDevice.getInternalError());
				return ret;
			}
		}
		
		// prepare finger update if necessary
		if (saveRecord == 1 && tabEnroll.cbUpdateTemplate.isSelected()) {
			if (tabEnroll.rbNbOfFinger2.isSelected()) // update both fingers
				ret = user.setTemplateUpdateMask(3);
			else if (tabEnroll.comboFingerIndex.getSelectedIndex() == 0) // update first finger only
				ret = user.setTemplateUpdateMask(1);
			else
				// update second finger
				ret = user.setTemplateUpdateMask(2);
		}
		
		// enroll
		if(ret == OK) {
			ret = user.enroll(timeout, acquisitionThreshold, advancedSecurityLevelsRequired, compressAlgo, compressRate, exportMinutiae, fingerNumber, fpTemplateType, fvpTemplateType, saveRecord, callbackCmd, mEventHandlerCallback, coderChoice, detectModeChoice, templateList);
		}
		
		setFVPFingerState(ret);

		String result = "";
		String msg1 = "";
		String msg2 = "";
		String msg3 = "";
		String msg4 = "";
		if (ret != OK) { // enroll failed
			result = "Enrollment failed";
			switch (ret) {
			case MorphoSmartSDK.MORPHOERR_CMDE_ABORTED:
				msg1 = "Command Aborted.";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_HIT:
				msg1 = "Bad Capture Sequence.";
				break;
			case MorphoSmartSDK.MORPHOERR_ALREADY_ENROLLED:
				msg1 = "User already enrolled.";
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_USER_ID:
				msg1 = "ID " + userID + " already used.";
				break;
			case MorphoSmartSDK.MORPHOERR_TIMEOUT:
				msg1 = "Timeout has expired.";
				msg2 = "Command aborted.";
				break;
			case MorphoSmartSDK.MORPHOERR_DB_FULL:
				msg1 = "Cannot enroll more users.";
				msg2 = "Database is full.";
				break;
			case MorphoSmartSDK.MORPHOERR_FFD:
			case MorphoSmartSDK.MORPHOERR_MOIST_FINGER:
			case MorphoSmartSDK.MORPHOERR_MOVED_FINGER:
			case MorphoSmartSDK.MORPHOERR_SATURATED_FINGER:
			case MorphoSmartSDK.MORPHOERR_INVALID_FINGER:
				msg1 = "Warning : Low quality finger !!!";
				break;
			case MorphoSmartSDK.MORPHOERR_BADPARAMETER:
				msg1 = "One or more input parameters are out of range";
				break;
			case MorphoSmartSDK.MORPHOERR_LICENSE_MISSING:
				msg1 = "A required license is missing.";
				break;
			case MorphoSmartSDK.MORPHOERR_ADVANCED_SECURITY_LEVEL_MISMATCH:
				msg1 = "Failed to make a multimodal template compatible with advanced security levels.";
				break;
			case MorphoSmartSDK.MORPHOERR_BAD_FINAL_FINGER_PRINT_QUALITY:
				msg1 = "Failed to reach the fingerprint quality threshold.";
				break;
			default:
				msg1 = "An error occured while calling";
				msg2 = "MorphoUser.enroll() function";
				msg3 = ErrorsMgt.convertSDKError(ret);
				break;
			}
			new DialogResultWindow(msoDemo, result, msg1, msg2, msg3, msg4).setVisible(true);
		} else { // enroll successful
			if (templateFp == TemplateType.MORPHO_PK_SAGEM_PKS) {
				if (MorphoInfo.isFVP && fvpTemplateType != MorphoFVPTemplateType.MORPHO_NO_PK_FVP) {
					saveFVPFiles("Enroll", userID, fvpTemplateType, templateList);
				}

				byte[] personnalInformation = getPersonalInformation(userID, firstName, lastName);

				String pksFileName = tabEnroll.textIdNumber.getText() + ".pks";
				ret = savePKSFile(pksFileName, personnalInformation, templateList, msoDemo.isDataEncryption());

				if (ret != 0) {
					new DialogResultWindow(msoDemo, "Enroll Successful", "Saving FP template file", "aborted by user.", "", "").setVisible(true);
				} else {
					new DialogResultWindow(msoDemo, "Enroll Successful", "FP Template successfully", "exported in file", FilesMgt.fileChooserDirectory + "/" + pksFileName, "").setVisible(true);
				}
			} else {
				saveTemplatesFiles("Enroll", tabEnroll.textIdNumber.getText(), fingerNumber, templateFvp, templateFp, templateList, msoDemo.isDataEncryption());
			}

			// Image acquisition enabled
			if (templateList.getActiveFullImageRetrieving()) {
				for (int i = 0; i < fingerNumber; i++) {
					MorphoImage morphoImage = new MorphoImage();
					ret = templateList.getFullImageRetrieving((short) i, morphoImage);
					if (ret != OK) {
						new DialogResultWindow(msoDemo, "Enroll Successful", "Error Image Unknown", "", "", "").setVisible(true);
						break;
					}
					if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus) { // privacy mode
						int response = DialogUtils.showQuestionMessage("MSO_Demo Java", "Privacy mode : image data is crypted.\nWould you like to decrypt it before saving ?");
						if (0 == response) {
							byte[] decryptedImageWithHeader = msoDemo.decryptAndCheckPrivacyData(morphoImage.getImage(), privacyKey);
							if (null != decryptedImageWithHeader) {
								byte[] decryptedImage = null;
								if (compressAlgo == MorphoCompressAlgo.MORPHO_NO_COMPRESS) {
									// RAW image header is 12 bytes length, so ignore first 12 first bytes
									decryptedImage = Arrays.copyOfRange(decryptedImageWithHeader, 12, decryptedImageWithHeader.length);
								} else if (compressAlgo == MorphoCompressAlgo.MORPHO_COMPRESS_WSQ) {
									// WSQ image header is 12 bytes length, in addition to RAW header (12 bytes), so ignore first 24 first bytes
									decryptedImage = Arrays.copyOfRange(decryptedImageWithHeader, 24, decryptedImageWithHeader.length);
								} else {
									decryptedImage = Arrays.copyOf(decryptedImageWithHeader, decryptedImageWithHeader.length);
								}
								// save image
								String fileName = userID + "_finger_" + (i + 1);
								ret = saveImagesFiles("Enroll", fileName, decryptedImage, compressAlgo, ret);
								if (ret != OK) {
									break;
								}
							} else {
								break;
							}
						} else if (1 == response) {
							ret = saveImagesFiles("Enroll", "Image", morphoImage, compressAlgo, ret);
							if (ret != OK) {
								break;
							}
						}
					} else { // privacy mode not enabled
						// save image
						String fileName = userID + "_finger_" + (i + 1);
						ret = saveImagesFiles("Enroll", fileName, morphoImage, compressAlgo, ret);
						if (ret != OK) {
							break;
						}
					}
				}
			}
		}
		return ret;
	}

	private void beep() {
		java.awt.Toolkit.getDefaultToolkit().beep();
	}
	
}
