package morpho.morphosmart.sdk.applet.demo;

import java.awt.BorderLayout;
import java.util.ArrayList;

import javax.swing.JApplet;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.border.EmptyBorder;

import morpho.morphosmart.sdk.api.IMsoSecu;
import morpho.morphosmart.sdk.api.MorphoCallbackCommand;
import morpho.morphosmart.sdk.api.MorphoDatabase;
import morpho.morphosmart.sdk.api.MorphoDevice;
import morpho.morphosmart.sdk.api.MorphoFAR;
import morpho.morphosmart.sdk.api.MorphoSmartSDK;
import morpho.morphosmart.sdk.applet.trt.DialogUtils;
import morpho.morphosmart.sdk.applet.trt.MorphoInfo;
import morpho.morphosmart.sdk.applet.trt.SecurityMgt;
import morpho.msosecu.sdk.api.MsoSecu;

public class MsoDemo extends JApplet {

	private static final long serialVersionUID = 1L;

	protected JPanel panelLeft = new JPanel();
	public RightPanel panelRight = null;
	public PanelLive panelLive = null;
	protected JTabbedPane tabbedPane = new JTabbedPane(JTabbedPane.TOP);

	private TabEnroll tabEnroll;
	private TabVerify tabVerify;
	public TabProcess tabProcess;

	public MorphoDevice mDevice = null;
	public MorphoDatabase mDatabase = null;

	public static boolean isBaseAvailable;
	
	public String serialNumber = "";
	public String msoName = "";
	
	public int callbackMask = 0; 
	
	private short isOfferedSecurity = 0;
	private short isTunneling = 0;

	/**
	 * Create the applet.
	 */
	public MsoDemo() {

		// Loading MorphoSDK and MSOSECU libraries
		try {
			System.loadLibrary("MorphoSmartSDKJavaWrapper");
			if (System.getProperty("os.name").startsWith("Windows")) {
				// load MSOSECU library on Windows ONLY
				System.loadLibrary("MSOSECUJavaWrapper");
			}
		} catch (UnsatisfiedLinkError e) {
			DialogUtils
					.showErrorMessage(
							"Load Library Error",
							"Native code library failed to load."
									+ " See the chapter on Dynamic Linking Problems in the SWIG Java documentation for help.\nError: "
									+ e.getMessage());
			System.exit(1);
		}

		callbackMask = MorphoCallbackCommand.MORPHO_CALLBACK_IMAGE_CMD.swigValue() | MorphoCallbackCommand.MORPHO_CALLBACK_ENROLLMENT_CMD.swigValue() | MorphoCallbackCommand.MORPHO_CALLBACK_COMMAND_CMD.swigValue() | MorphoCallbackCommand.MORPHO_CALLBACK_CODEQUALITY.swigValue() | MorphoCallbackCommand.MORPHO_CALLBACK_DETECTQUALITY.swigValue();
		
		openConnection();
		
		panelLive = new PanelLive(this);
		panelRight = new RightPanel(this);
		
		panelRight.setDeviceName(msoName,isTUNNELING(),isOFFERED_SECURITY());
		panelRight.setDeviceSerailNumber(serialNumber);
		
		if(isBaseAvailable)
		{
			panelRight.loadUsers();
		}
		
		setSize(800, 600);
		getContentPane().setLayout(new BorderLayout(0, 0));

		panelLeft.setLayout(new BorderLayout(0, 0));
		panelLeft.add(tabbedPane, BorderLayout.CENTER);
		panelLeft.add(panelLive, BorderLayout.SOUTH);

		// Verify
		tabVerify = new TabVerify(isBaseAvailable);
		tabbedPane.addTab("Verify", tabVerify);
		// Enroll
		tabEnroll = new TabEnroll();
		if(isBaseAvailable)
		{
			tabbedPane.addTab("Enroll", tabEnroll);
		}
		// Process
		tabProcess = new TabProcess(this);
		//tabbedPane.addTab("Process", tabProcess);

		panelLeft.setBorder(new EmptyBorder(10, 10, 10, 10));
		panelRight.setBorder(new EmptyBorder(10, 10, 10, 10));

		getContentPane().add(panelLeft, BorderLayout.WEST);
		getContentPane().add(panelRight, BorderLayout.CENTER);	
	}

	private void openConnection() {
		// get USB devices
		mDevice = new MorphoDevice();
		long[] nbUsbDevice = { 0 };
		int ret = mDevice.initUsbDevicesNameEnum(nbUsbDevice);		
		if (ret == 0) {
			if (nbUsbDevice[0] > 0) {
				serialNumber = mDevice.getUsbDeviceName(0);
				msoName = mDevice.getUsbDevicePropertie(0);				
			} else {
				DialogUtils.showWarningMessage("Warning", "No device detected");
			}
		} else {
			DialogUtils.showErrorMessage("Error", "morphoDevice Error");
		}

		byte[] hostCertificate = null;

		if (ret == 0) {
			ret = mDevice.openUsbDevice(serialNumber, 30);
		}

		if (ret == 0) {
			
			IMsoSecu msoSecu = new MsoSecu();
			
			String openSSLFolder = System.getenv("MORPHOSMART_SDK");
			if(openSSLFolder != null)
			{
				msoSecu.setOpenSSLPath(openSSLFolder);
			}
			
			// Setup security
			String[] secuSerialNumber = { "" };
			byte[] secuConfig = { 0 };
			MorphoFAR[] secuFar = { MorphoFAR.MORPHO_FAR_0 };
			long[] secuMinMSL = { 0 };
			ret = mDevice.getSecuConfig(secuSerialNumber, secuConfig, secuFar,
					secuMinMSL);

			isOfferedSecurity = (short) (((short) secuConfig[0]) & MorphoSmartSDK.SECU_OFFERED_SECURITY);
			isTunneling = (short) (((short) secuConfig[0]) & MorphoSmartSDK.SECU_TUNNELING);

			if (!System.getProperty("os.name").startsWith("Windows")) {
				// linux is not supported with secured devices
				if (isOfferedSecurity != 0 || isTunneling != 0) {
					DialogUtils.showErrorMessage("MSO Connection",
							"Secured devices are not supported on Linux");
					System.exit(1);
				}
			}			

			if (isTunneling != 0) {

				ArrayList<Byte> certifHost = new ArrayList<Byte>();
				ret = msoSecu.getHostCertif(certifHost);

				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils
							.showErrorMessage(
									"Connection",
									"Cannot read host certificate (error "
											+ ret
											+ ").\nPlease check that:\n1/ You have generated the HOST keys (with the MKMS or other)\n2/ You have copied host.der (HOST public certificate), host.key (HOST private key),\n ca.crt (CA public key that can be found in the MKMS station) in the directory .\\openssl\\keys.");
				} else {
					hostCertificate = new byte[certifHost.size()];
					for (int i = 0; i < certifHost.size(); i++) {
						hostCertificate[i] = certifHost.get(i);
					}

					ret = SecurityMgt.tunnelingOpen(mDevice, msoSecu,
							hostCertificate);
					if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils
								.showErrorMessage(
										"Connection",
										"Security Tunneling Initialization failed.\nPlease check that:\n1/ You have generated the HOST keys (with the MKMS or other)\n2/ You have copied host.der (HOST public certificate), host.key (HOST private key), ca.crt (CA public key that can be found in the MKMS station) in the directory .\\openssl\\keys.");
						return;
					}
				}
			} else if (isOfferedSecurity != 0) {
				ret = SecurityMgt.offeredSecuOpen(mDevice, msoSecu);
				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils.showErrorMessage("Connection",
							"Offered Security Initialization failed", ret,
							mDevice.getInternalError());
					int q = DialogUtils
							.showQuestionMessage(
									"Connection",
									"The MorphoSmart Certificate has not been authenticated.\nPlease check that you have copied ca.crt (CA public key that can be found in the MKMS station) in the directory .\\openssl\\keys.\nContinue anyway ?");
					if (q == JOptionPane.NO_OPTION) {
					}
				}
			}

			// The device is FVP ?
			if (msoName.endsWith("FINGER VP")) {
				MorphoInfo.isFVP = true;
			}

			isBaseAvailable = false;
			mDatabase = new MorphoDatabase();
			ret = mDevice.getDatabase((short) 0, "", mDatabase);
			if (ret != MorphoSmartSDK.MORPHO_OK
					&& ret != MorphoSmartSDK.MORPHOERR_BASE_NOT_FOUND) {
				DialogUtils
						.showErrorMessage(
								"Connection",
								"An error occured while calling MorphoDevice.getDatabase() function",
								ret, mDevice.getInternalError());
			}

			int[] nbUsedRecord = { 0 };
			ret = mDatabase.getNbUsedRecord(nbUsedRecord);

			if (ret == MorphoSmartSDK.MORPHOERR_BASE_NOT_FOUND) {
				DialogUtils
						.showErrorMessage("Connection",
								"There is currently no internal database available in the MorphoSmart Device");
			} else if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils
						.showErrorMessage(
								"Connection",
								"An error occured while calling MorphoDatabase.getNbUsedRecord() function",
								ret, mDevice.getInternalError());
			} else {
				isBaseAvailable = true;
			}
		}
	}
	
	public boolean isTabEnroll()
	{
		if(tabbedPane.getSelectedIndex() == 1)
		{
			return true;
		}
		return false;
	}
	
	public int getSelectedTabIndex()
	{
		return tabbedPane.getSelectedIndex();
	}
	
	public void addTabProcess()
	{
		tabbedPane.addTab("Process", tabProcess);
		tabbedPane.setSelectedComponent(tabProcess);
	}
	
	public void selectTab(int tabIndex)
	{
		tabbedPane.setSelectedIndex(tabIndex);
	}
	
	public void finishCapture(int currentTabIndex)
	{
		// remove Process tab
		tabbedPane.remove(tabProcess);
		tabbedPane.setSelectedIndex(currentTabIndex);
		// Clear images from tab process
		tabProcess.clearLive();		
	}
	
	public TabEnroll getTabEnroll()
	{
		return tabEnroll;
	}
	
	public TabVerify getTabVerify()
	{
		return tabVerify;
	}
	
	public int getSecurityLevel(int tab) {
		int securityLevel = 0;		

		if (MorphoInfo.isFVP) {
			if (tab == 1) { // enroll
				securityLevel = MorphoSmartSDK.FFD_SECURITY_LEVEL_DEFAULT_HOST;
			} else {
				securityLevel = MorphoSmartSDK.MULTIMODAL_SECURITY_STANDARD;				
			}
		} else {
			securityLevel = MorphoSmartSDK.FFD_SECURITY_LEVEL_LOW_HOST;			
		}

		return securityLevel;
	}

	public boolean isTUNNELING() {
		return (isTunneling != 0 ?true:false);
	}

	public boolean isOFFERED_SECURITY() {
		return (isOfferedSecurity != 0?true:false);
	}
}
