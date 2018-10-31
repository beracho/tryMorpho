package morpho.morphosmart.sdk.demo;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dialog.ModalExclusionType;
import java.awt.EventQueue;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.List;

import javax.swing.DefaultComboBoxModel;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFormattedTextField;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTabbedPane;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.SwingConstants;
import javax.swing.border.EmptyBorder;
import javax.swing.border.TitledBorder;
import javax.swing.text.MaskFormatter;

import com.morpho.morphosmart.pipe.ILog;
import com.morpho.morphosmart.pipe.SimpleLogger;

import morpho.morphosmart.sdk.api.IMsoPipe;
import morpho.morphosmart.sdk.api.IMsoSecu;
import morpho.morphosmart.sdk.api.MorphoDatabase;
import morpho.morphosmart.sdk.api.MorphoDevice;
import morpho.morphosmart.sdk.api.MorphoFAR;
import morpho.morphosmart.sdk.api.MorphoSmartSDK;
import morpho.morphosmart.sdk.demo.dialog.DialogCreateDB;
import morpho.morphosmart.sdk.demo.dialog.DialogUtils;
import morpho.morphosmart.sdk.demo.trt.ImageLoader;
import morpho.morphosmart.sdk.demo.trt.MorphoInfo;
import morpho.morphosmart.sdk.demo.trt.SecurityMgt;
import morpho.msosecu.sdk.api.MsoSecu;


public class MsoConnection extends JFrame {

	private static final long serialVersionUID = 1L;
	// UI variables
	private JPanel contentPane;
	private JTabbedPane tabbedPane;
	private JLabel lblMsoName = new JLabel("Device Name");
	private JLabel lblSerialPort = new JLabel("Serial Port");
	private JLabel lblBaudRate = new JLabel("Baud Rate");
	private JComboBox comboSerialPort;
	private JComboBox comboBaudRate;
	private JComboBox comboDeviceListUSB = new JComboBox();
	
    private String	sensorName	= "";
	// variables
	private List<String> listDeviceNames = new ArrayList<String>();
	// MorphoSmart SDK
	private MorphoDevice mDevice = null;
	private MorphoDatabase mDatabase = null;
	// IMsoSecu
	private IMsoSecu msoSecu = null;
	private String osname = System.getProperty("os.name");
	
	final int TabSerial = 0, TabUSB = 1, TabPipe = 2;
	// PIPE
	private javax.swing.JFormattedTextField adressIpTextField;
    private javax.swing.JTextField urlTextField, portTextField;
    private javax.swing.JButton getMsoListButton;
    private javax.swing.ButtonGroup hostbuttonGroup;
    private javax.swing.JComboBox<String> msoListComboBox;
	private javax.swing.JLabel portLabel;
    private JRadioButton urlRadioButton, ipRadioButton;
    JPanel panelPipe;
	private IMsoPipe i_msoPipe;
	private boolean isPipe;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					new MsoConnection().setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the frame.
	 */
	public MsoConnection() {
		setIconImage(ImageLoader.load("MSO_Demo.png"));
		setModalExclusionType(ModalExclusionType.APPLICATION_EXCLUDE);
		setTitle("Morpho MSO Demo Java " + MsoDemo.MORPHOSMART_SDK_JAVA_VERSION);
		setResizable(false);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setSize(400, 300);
		setLocationRelativeTo(null);

		// Loading MorphoSDK and MSOSECU libraries
		try {
			System.loadLibrary("MorphoSmartSDKJavaWrapper");
			if (osname.startsWith("Windows")) {
				System.loadLibrary("MSOSECUJavaWrapper");
			} else if (osname.startsWith("Linux")) {
				System.loadLibrary("msosecu");
			}
		} catch (UnsatisfiedLinkError e) {
			DialogUtils.showErrorMessage("Load Library Error", "Native code library failed to load." + " See the chapter on Dynamic Linking Problems in the SWIG Java documentation for help.\nError: " + e.getMessage());
			dispose();
			System.exit(1);
		}

		// fill ComboBoxes with values
		Integer[] baudRate = { 50, 75, 100, 110, 150, 200, 220, 300, 400, 440, 600, 880, 1200, 1440, 2000, 2400, 3600, 4000, 4800, 7200, 8000, 9600, 14400, 16000, 19200, 28800, 38400, 56000, 57600, 76800, 115200, 153600, 230400, 460800, 921600 };
		comboBaudRate = new JComboBox(baudRate);
		comboBaudRate.setSelectedItem(115200);
		String[] serialPort = new String[256];
		for (int i = 0; i < 256; i++)
			serialPort[i] = "COM" + (i + 1);
		comboSerialPort = new JComboBox(serialPort);

		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		contentPane.setLayout(new BorderLayout(0, 0));

		tabbedPane = new JTabbedPane(JTabbedPane.TOP);
		contentPane.add(tabbedPane, BorderLayout.CENTER);

		JPanel panelButtons = new JPanel();
		contentPane.add(panelButtons, BorderLayout.SOUTH);
		panelButtons.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));

		JButton btnOk = new JButton("   OK   ");
		// give the focus to the OK button
		getRootPane().setDefaultButton(btnOk);
		btnOk.requestFocus();
		// btnOk action
		btnOk.addActionListener(btnOkAction);
		panelButtons.add(btnOk);

		JButton btnCancel = new JButton(" Cancel ");
		btnCancel.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				// Close connection JFrame
				mDevice.closeDevice();
				dispose();
			}
		});
		panelButtons.add(btnCancel);

		JPanel panelRS232 = new JPanel();
		panelRS232.setBorder(new TitledBorder(null, "Serial Port Properties", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		tabbedPane.addTab("RS232", panelRS232);

		GroupLayout gl_panelRS232 = new GroupLayout(panelRS232);
		gl_panelRS232.setHorizontalGroup(gl_panelRS232.createParallelGroup(Alignment.LEADING).addGroup(
				gl_panelRS232.createSequentialGroup().addGap(58).addGroup(gl_panelRS232.createParallelGroup(Alignment.LEADING).addComponent(lblSerialPort).addComponent(comboSerialPort, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)).addGap(71).addGroup(gl_panelRS232.createParallelGroup(Alignment.LEADING).addComponent(comboBaudRate, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).addComponent(lblBaudRate))
						.addContainerGap(133, Short.MAX_VALUE)));
		gl_panelRS232.setVerticalGroup(gl_panelRS232.createParallelGroup(Alignment.LEADING).addGroup(
				gl_panelRS232.createSequentialGroup().addGap(55).addGroup(gl_panelRS232.createParallelGroup(Alignment.BASELINE).addComponent(lblSerialPort).addComponent(lblBaudRate)).addPreferredGap(ComponentPlacement.RELATED)
						.addGroup(gl_panelRS232.createParallelGroup(Alignment.BASELINE).addComponent(comboSerialPort, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).addComponent(comboBaudRate, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)).addContainerGap(106, Short.MAX_VALUE)));
		panelRS232.setLayout(gl_panelRS232);

		JPanel panelUSB = new JPanel();
		panelUSB.setBorder(new TitledBorder(null, "MSO Serial Number", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		tabbedPane.addTab("USB", panelUSB);
		lblMsoName.setHorizontalAlignment(SwingConstants.CENTER);
		lblMsoName.setFont(new Font("Tahoma", Font.BOLD, 14));

		GroupLayout gl_panelUSB = new GroupLayout(panelUSB);
		gl_panelUSB.setHorizontalGroup(gl_panelUSB.createParallelGroup(Alignment.LEADING).addGroup(gl_panelUSB.createSequentialGroup().addGap(48).addGroup(gl_panelUSB.createParallelGroup(Alignment.LEADING).addComponent(lblMsoName).addComponent(comboDeviceListUSB, GroupLayout.PREFERRED_SIZE, 258, GroupLayout.PREFERRED_SIZE)).addContainerGap(61, Short.MAX_VALUE)));
		gl_panelUSB.setVerticalGroup(gl_panelUSB.createParallelGroup(Alignment.LEADING).addGroup(gl_panelUSB.createSequentialGroup().addGap(61).addComponent(lblMsoName).addGap(18).addComponent(comboDeviceListUSB, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).addContainerGap(62, Short.MAX_VALUE)));
		gl_panelUSB.linkSize(SwingConstants.HORIZONTAL, new Component[] { comboDeviceListUSB, lblMsoName });
		panelUSB.setLayout(gl_panelUSB);
		
		//--------------------------------------		
		
		panelPipe = new JPanel();
		panelPipe.setBorder(new TitledBorder(null, "Host connection", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		tabbedPane.addTab("DEMO_PIPE", panelPipe);

		hostbuttonGroup = new javax.swing.ButtonGroup();
		ipRadioButton = new javax.swing.JRadioButton();
		urlRadioButton = new javax.swing.JRadioButton();

		urlTextField = new javax.swing.JTextField();
		portLabel = new javax.swing.JLabel();
		portTextField = new javax.swing.JTextField();
		getMsoListButton = new javax.swing.JButton();
		msoListComboBox = new javax.swing.JComboBox<>();

		setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

		hostbuttonGroup.add(ipRadioButton);
		ipRadioButton.setSelected(true);
		ipRadioButton.setText("Ip Format");
		ipRadioButton.setActionCommand("ipRadioButton");
		ipRadioButton.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {
				ipRadioButtonActionPerformed(evt);
			}
		});

		hostbuttonGroup.add(urlRadioButton);

		urlRadioButton.setText("URL Format");
		urlRadioButton.setActionCommand("urlRadioButton");
		urlRadioButton.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {

				urlRadioButtonActionPerformed(evt);

			}
		});

	
        urlTextField.setToolTipText("");
        portTextField.setText("11011");
        urlTextField.setVisible(false);
	        
        MaskFormatter mf1;
		try {
			
			mf1 = new MaskFormatter("###.###.###.###");
			mf1.setPlaceholderCharacter('0');
			adressIpTextField = new JFormattedTextField(mf1);
			
		} catch (ParseException e1) {

			e1.printStackTrace();
		} 
        
        adressIpTextField.setCursor(new java.awt.Cursor(java.awt.Cursor.TEXT_CURSOR));
        portLabel.setText("Port");

        getMsoListButton.setText("Get Mso List");
        getMsoListButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                getMsoListButtonActionPerformed(evt);
            }
        });


        javax.swing.GroupLayout panelPipeLayout = new javax.swing.GroupLayout(panelPipe);
        panelPipe.setLayout(panelPipeLayout);
        panelPipeLayout.setHorizontalGroup(
            panelPipeLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelPipeLayout.createSequentialGroup()
                .addGap(74, 74, 74)
                .addGroup(panelPipeLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(panelPipeLayout.createSequentialGroup()
                        .addGroup(panelPipeLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(getMsoListButton, javax.swing.GroupLayout.PREFERRED_SIZE, 104, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGroup(panelPipeLayout.createSequentialGroup()
                                .addComponent(portLabel)
                                .addGap(26, 26, 26)
                                .addComponent(portTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 93, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, panelPipeLayout.createSequentialGroup()
                        .addGroup(panelPipeLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addGroup(panelPipeLayout.createSequentialGroup()
                                .addComponent(ipRadioButton)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 62, Short.MAX_VALUE)
                                .addComponent(urlRadioButton))
                            .addComponent(msoListComboBox, javax.swing.GroupLayout.Alignment.LEADING, 0, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(adressIpTextField)
                            .addComponent(urlTextField))
                        .addGap(73, 73, 73)
                        )))
        );
        panelPipeLayout.setVerticalGroup(
            panelPipeLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelPipeLayout.createSequentialGroup()
                .addGap(20, 20, 20)
                .addGroup(panelPipeLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(ipRadioButton)
                    .addComponent(urlRadioButton))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(adressIpTextField, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addComponent(urlTextField, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(panelPipeLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(portLabel)
                    .addComponent(portTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(14, 14, 14)
                .addComponent(getMsoListButton)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(msoListComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

	       
      
        panelPipe.setLayout(panelPipeLayout);
		lblMsoName.setHorizontalAlignment(SwingConstants.CENTER);
		lblMsoName.setFont(new Font("Tahoma", Font.BOLD, 14));

		
		//--------------------------------------
		// By default select USB tab
		tabbedPane.setSelectedIndex(1);

		// close action
		addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent we) {
				mDevice.closeDevice();
				System.exit(0);
			}
		});

		// get USB devices
		mDevice = new MorphoDevice();
		long[] nbUsbDevice = { 0 };
		int ret = mDevice.initUsbDevicesNameEnum(nbUsbDevice);
		if (ret == 0) {
			if (nbUsbDevice[0] > 0) {
				for (int i = 0; i < nbUsbDevice[0]; ++i) {
					comboDeviceListUSB.addItem(mDevice.getUsbDeviceName(i));
					listDeviceNames.add(mDevice.getUsbDevicePropertie(i));
				}
				// initaliser le nom du device
				lblMsoName.setText(listDeviceNames.get(comboDeviceListUSB.getSelectedIndex()));
			} else {
				DialogUtils.showWarningMessage("Warning", "No device detected");
			}
		} else {
			DialogUtils.showErrorMessage("Error", "morphoDevice Error");
		}

		// Associate actions
		comboDeviceListUSB.addActionListener(comboDeviceListUSBAction);
	}

	//
	// Listeners-------------------------------------------------------------------------------------
	//
	MsoInit msoInit = new MsoInit();
	private ActionListener btnOkAction = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent event) {
			// Launch MSO initialization
			new Thread(new Runnable() {
				@Override
				public void run() {
					msoInit.setVisible(true);
					msoInit.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
				}
			}).start();

			new Thread(new Runnable() {

				@Override
				public void run() {
					int ret = 0;
			       
					// RS232
					if (tabbedPane.getSelectedIndex() == TabSerial) {
						int baudRate = Integer.parseInt(comboBaudRate.getSelectedItem().toString());
						short serialPort = (short) (comboSerialPort.getSelectedIndex() + 1);
						msoInit.setOpen("Open RS232 COM" + serialPort);
						isPipe = false;
						ret = mDevice.openDevice(serialPort, baudRate);
					}
					// USB
					else if (tabbedPane.getSelectedIndex() == TabUSB) {
						msoInit.setOpen("Open USB");
						// open connection with selected device
						String serialNumber = (String) comboDeviceListUSB.getSelectedItem();
						isPipe = false;
						ret = mDevice.openUsbDevice(serialNumber, 30);
					}
					// PIPE
					else if (tabbedPane.getSelectedIndex() == TabPipe) {
						isPipe = true;

						String serialNumber = (String) msoListComboBox.getSelectedItem();

						String ip = getIp();
						if(ip == null || ip.isEmpty()) {
							msoInit.dispose();
							return;
						}
							
						int port =getport();
						
						if(port <= 0) {
							msoInit.dispose();
							return;
						}

						if(serialNumber == null) {
							msoInit.dispose();
							return;
						}		
						
						msoInit.setOpen("Open Pipe");
						// open connection with selected device
						
				        MsoPipeImpl imsoPipe = new MsoPipeImpl();						
						ILog log = new SimpleLogger();
						log.enableLog();
						imsoPipe.setServer_ip(ip);
						imsoPipe.setServer_port(port);
						imsoPipe.setLogger(log);
						imsoPipe.setSerial(serialNumber);
						imsoPipe.setTimeout(15000);
						i_msoPipe = imsoPipe;

						ret = mDevice.openDevice(imsoPipe, imsoPipe.getServer_ip() , imsoPipe.getServer_port(), (short)4);
					}

					short isOfferedSecurity = 0;
					short isTunneling = 0;
					byte[] hostCertificate = null;

					setVisible(false);
					if (ret == MorphoSmartSDK.MORPHO_OK) {
						msoInit.setStatusOpen("OK");
						// Setup securivty
						String[] secuSerialNumber = { "" };
						byte[] secuConfig = { 0 };
						MorphoFAR[] secuFar = { MorphoFAR.MORPHO_FAR_0 };
						long[] secuMinMSL = { 0 };
						ret = mDevice.getSecuConfig(secuSerialNumber, secuConfig, secuFar, secuMinMSL);

						isOfferedSecurity = (short) (((short) secuConfig[0]) & MorphoSmartSDK.SECU_OFFERED_SECURITY);
						isTunneling = (short) (((short) secuConfig[0]) & MorphoSmartSDK.SECU_TUNNELING);

						msoSecu = new MsoSecu();
						// User shall call setOpenSSLPath() with correct keys folder location
						String currentDir = System.getProperty("user.dir");
						if (osname.startsWith("Windows")) {
							msoSecu.setOpenSSLPath(currentDir+"/OpenSSL/Keys/");
						} else if (osname.startsWith("Linux")) {
							msoSecu.setOpenSSLPath(currentDir+"/../openssl/Keys/");
						}
						
						if (isTunneling != 0) {
							msoInit.setCertification("Host Certification");
							msoInit.setCommunicationInitialisation("Tunneling communication initialisation");
							msoInit.setStatusMsoConfig("Tunneling");
							msoInit.setStatusSecurityCom("please wait a few seconds...");

							if (isMultiProtectStarted()) {
								int q = DialogUtils.showQuestionMessage("MSO Connection", "The service \"Sagem Licence Service\" is not compatible with products with Tunneling configuration ,\n it may cause problems in the enrollment process. \nPlease stop this service before using MSO_Demo.\n\nContinue anyway ?");
								if (q == JOptionPane.NO_OPTION) {
									dispose();
									msoInit.dispose();
									return;
								}
							}

							ArrayList<Byte> certifHost = new ArrayList<Byte>();
							ret = msoSecu.getHostCertif(certifHost);

							if (ret != MorphoSmartSDK.MORPHO_OK) {
								DialogUtils.showErrorMessage("Connection", "Cannot read host certificate (error " + ret + ").\nPlease check that:\n1/ You have generated the HOST keys (with the MKMS or other)\n2/ You have copied host.der (HOST public certificate), host.key (HOST private key),\n ca.crt (CA public key that can be found in the MKMS station) in the directory .\\openssl\\keys.");
								dispose();
								msoInit.dispose();
								return;
							} else {
								msoInit.setStatusMsoCertif("OK");
								hostCertificate = new byte[certifHost.size()];
								for (int i = 0; i < certifHost.size(); i++) {
									hostCertificate[i] = certifHost.get(i);
								}

								ret = SecurityMgt.tunnelingOpen(mDevice, msoSecu, hostCertificate);
								if (ret != MorphoSmartSDK.MORPHO_OK) {
									DialogUtils.showErrorMessage("Connection", "Security Tunneling Initialization failed.\nPlease check that:\n1/ You have generated the HOST keys (with the MKMS or other)\n2/ You have copied host.der (HOST public certificate), host.key (HOST private key), ca.crt (CA public key that can be found in the MKMS station) in the directory .\\openssl\\keys.");
									dispose();
									msoInit.dispose();
									return;
								} else {
									msoInit.setStatusSecurityCom("OK");
								}
							}
						} else if (isOfferedSecurity != 0) {
							msoInit.setCommunicationInitialisation("Offered communication initialisation");
							msoInit.setStatusMsoConfig("Offered");
							msoInit.setCertification("The MSO Certification");

							ret = SecurityMgt.offeredSecuOpen(mDevice, msoSecu);
							if (ret != MorphoSmartSDK.MORPHO_OK) {
								DialogUtils.showErrorMessage("Connection", "Offered Security Initialization failed", ret, mDevice.getInternalError());
								int q = DialogUtils.showQuestionMessage("Connection", "The MorphoSmart Certificate has not been authenticated.\nPlease check that you have copied ca.crt (CA public key that can be found in the MKMS station) in the directory .\\openssl\\keys.\nContinue anyway ?");
								if (q == JOptionPane.NO_OPTION) {
									dispose();
									msoInit.dispose();
									return;
								}
							} else {
								msoInit.setStatusMsoCertif("OK");
								msoInit.setStatusSecurityCom("OK");
							}
						}

						ArrayList<String> productName = new ArrayList<String>();
						ret = mDevice.getDescriptorBin((short) MorphoSmartSDK.BINDESC_PRODUCT_NAME, productName);

						if (ret == MorphoSmartSDK.MORPHO_OK && productName.size() > 0) {
							// The device is FVP ?
							if (productName.get(0).endsWith("FINGER VP")) {
								MorphoInfo.isFVP = true;
							}
						} else {
							DialogUtils.showErrorMessage("Connection", "An error occured while calling MorphoDevice.getDescriptorBin() function", ret, mDevice.getInternalError());
						}

						boolean isBaseAvailable = false;
						mDatabase = new MorphoDatabase();
						ret = mDevice.getDatabase((short) 0, "", mDatabase);
						if (ret != MorphoSmartSDK.MORPHO_OK && ret != MorphoSmartSDK.MORPHOERR_BASE_NOT_FOUND) {
							DialogUtils.showErrorMessage("Connection", "An error occured while calling MorphoDevice.getDatabase() function", ret, mDevice.getInternalError());
						}

						int[] nbUsedRecord = { 0 };
						ret = mDatabase.getNbUsedRecord(nbUsedRecord);

						if (ret == MorphoSmartSDK.MORPHOERR_BASE_NOT_FOUND) {
							msoInit.setStatusDatabaseConf("no detected database");
							ret = DialogUtils.showQuestionMessage("Connection", "There is currently no internal database available in the MorphoSmartSDK.\nWould you like to create one?");
							if (ret == JOptionPane.YES_OPTION) {
								DialogCreateDB dialogCreateDB = new DialogCreateDB(MsoConnection.this);
								dialogCreateDB.setDevice(mDevice);
								dialogCreateDB.setDatabase(mDatabase);
								dialogCreateDB.setVisible(true);
								if (dialogCreateDB.getSdkError() == 0) {
									isBaseAvailable = true;
									msoInit.setStatusDatabaseConf("OK");
								}
							}
						} else if (ret != MorphoSmartSDK.MORPHO_OK) {
							DialogUtils.showErrorMessage("Connection", "An error occured while calling MorphoDatabase.getNbUsedRecord() function", ret, mDevice.getInternalError());
						} else {
							isBaseAvailable = true;
							msoInit.setStatusDatabaseConf("OK");
						}

						// Close connection JFrame
						dispose();
						// Launch MSO Demo
						boolean offeredSecurity = (isOfferedSecurity != 0) ? true : false;
						boolean tunneling = (isTunneling != 0) ? true : false;
						MsoDemo msoDemoFrame = null;
						if(isPipe)
							msoDemoFrame = new MsoDemo(mDevice, (String) comboDeviceListUSB.getSelectedItem(), mDatabase, isBaseAvailable, hostCertificate, offeredSecurity, tunneling, msoSecu, i_msoPipe);
						else
							msoDemoFrame = new MsoDemo(mDevice, (String) comboDeviceListUSB.getSelectedItem(), mDatabase, isBaseAvailable, hostCertificate, offeredSecurity, tunneling, msoSecu);
							
						msoDemoFrame.setVisible(true);
					} else {
						DialogUtils.showErrorMessage("Connection Error", "The connection with the MSO failed. Please check the device is correctly plugged.");
						dispose();
					}
					msoInit.dispose();
				}
			}).start();
		}
	};

	
	
	//-----------------------  pipe -----------------------------------------------------------
	private void ipRadioButtonActionPerformed(java.awt.event.ActionEvent evt) {
		adressIpTextField.setVisible(true);
		urlTextField.setVisible(false);
		
		panelPipe.revalidate();
		panelPipe.repaint();
	}
    
    private void urlRadioButtonActionPerformed(java.awt.event.ActionEvent evt) {
    	adressIpTextField.setVisible(false);
    	
    	urlTextField.setVisible(true);
    	String hostName;
		try {
			hostName = InetAddress.getLocalHost().getCanonicalHostName();
			urlTextField.setText(hostName);
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
    	
		panelPipe.revalidate();
    	panelPipe.repaint();
    	
    }
    
    private String getIp() {
		String ip = "";
		if (urlRadioButton.isSelected()) {
			ip = urlTextField.getText().toString();
			if (ip.isEmpty()) {
				JOptionPane.showMessageDialog(rootPane, "Tapez l'URL", "Message d'erreur", JOptionPane.ERROR_MESSAGE);
				return null;
			}
		} else if (ipRadioButton.isSelected()) {
			ip = adressIpTextField.getText().toString();
		}
		return ip;
    }
    
    private int getport() {
		int port = 0;
		try {
			String portField = portTextField.getText();
			if (portField == null || portField.trim().isEmpty()) {
				throw new NumberFormatException();
			} else {
				port = Integer.parseInt(portField);
			}
		} catch (NumberFormatException nfe) {
			JOptionPane.showMessageDialog(rootPane, "Port : Tapez un nombre entier", "Message d'erreur",
					JOptionPane.ERROR_MESSAGE);
			return -1;
		}
		return port;
    }
    
	private void getMsoListButtonActionPerformed(java.awt.event.ActionEvent evt) { 

		String ip = getIp();
		if (ip == null)
			return;
		int port = getport();
		if(port == -1)
			return;	
		
		MsoPipeImpl imsoPipe = new MsoPipeImpl();

		ArrayList<String> devices = new ArrayList<String>();
		ILog log = new SimpleLogger();
		log.enableLog();
		imsoPipe.setServer_ip(ip).setServer_port(port).setLogger(log).enumerate(devices);
		String[] sensorsName = new String[devices.size()];

		if (devices.size() > 0) {
			for (int i = 0; i < devices.size(); i++) {
				sensorsName[i] = devices.get(i);
			}
			msoListComboBox.setModel(new DefaultComboBoxModel<>(sensorsName));
		}

	}
    
	//-------------------------------------------------------------


    
	private ActionListener comboDeviceListUSBAction = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			JComboBox cb = (JComboBox) e.getSource();
			int index = cb.getSelectedIndex();
			lblMsoName.setText(listDeviceNames.get(index));
		}
	};

	private boolean isMultiProtectStarted() {
		if (!System.getProperty("os.name").startsWith("Windows")) {
			return false;
		}

		boolean ret = false;
		try {
			String process1 = "SagemSecurite_Licence_Protection.exe";
			String process2 = "SagemSecurite_License_Protection.exe";

			File file = File.createTempFile("realhowto", ".vbs");
			file.deleteOnExit();
			FileWriter fw = new java.io.FileWriter(file);

			String vbs = "Set WshShell = WScript.CreateObject(\"WScript.Shell\")\n" + "Set locator = CreateObject(\"WbemScripting.SWbemLocator\")\n" + "Set service = locator.ConnectServer()\n" + "Set processes = service.ExecQuery _\n" + " (\"select * from Win32_Process where name='" + process1 + "' or name='" + process2 + "'\")\n" + "For Each process in processes\n" + "wscript.echo process.Name \n" + "Next\n" + "Set WSHShell = Nothing\n";

			fw.write(vbs);
			fw.close();
			Process p = Runtime.getRuntime().exec("cscript //NoLogo " + file.getPath());
			BufferedReader input = new BufferedReader(new InputStreamReader(p.getInputStream()));
			String line;
			line = input.readLine();
			if (line != null) {
				if (line.equals(process1) || line.equals(process2)) {
					ret = true;
				}
			}
			input.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return ret;
	}

}
