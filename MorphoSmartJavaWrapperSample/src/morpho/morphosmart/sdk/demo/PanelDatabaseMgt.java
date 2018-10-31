package morpho.morphosmart.sdk.demo;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
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
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTable;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.ListSelectionModel;
import javax.swing.SwingConstants;
import javax.swing.border.BevelBorder;
import javax.swing.border.TitledBorder;
import javax.swing.table.DefaultTableModel;

import morpho.morphosmart.sdk.api.MorphoDatabase;
import morpho.morphosmart.sdk.api.MorphoDatabase.MorphoTypeDeletion;
import morpho.morphosmart.sdk.api.MorphoDevice;
import morpho.morphosmart.sdk.api.MorphoDevice.MorphoDevicePrivacyModeStatus;
import morpho.morphosmart.sdk.api.MorphoFAR;
import morpho.morphosmart.sdk.api.MorphoFVPTemplateType;
import morpho.morphosmart.sdk.api.MorphoSmartSDK;
import morpho.morphosmart.sdk.api.MorphoTemplateList;
import morpho.morphosmart.sdk.api.MorphoTemplateType;
import morpho.morphosmart.sdk.api.MorphoUser;
import morpho.morphosmart.sdk.api.MorphoUserList;
import morpho.morphosmart.sdk.demo.dialog.DialogAddUpdateUser;
import morpho.morphosmart.sdk.demo.dialog.DialogCreateDB;
import morpho.morphosmart.sdk.demo.dialog.DialogResultWindow;
import morpho.morphosmart.sdk.demo.dialog.DialogUtils;
import morpho.morphosmart.sdk.demo.ennum.EnumTabIndex;
import morpho.morphosmart.sdk.demo.trt.FilesMgt;
import morpho.morphosmart.sdk.demo.trt.ImageLoader;
import morpho.morphosmart.sdk.demo.trt.UsersMgt;

public class PanelDatabaseMgt extends JPanel implements ActionListener {

	private static final long serialVersionUID = 1L;
	private JTable dbTable;

	// right tabs of MSO Demo
	protected TabDbInformation tabDbInfo;
	protected TabBioSettings tabBioSettings;
	protected TabOptions tabOptions;

	// JTable buttons and label
	JLabel lblBaseStatus;
	JCheckBox cbNoCheck;
	JButton btnIdentifyMatch;
	JButton btnVerifyMatch;
	JButton btnAddUser;
	JButton btnUpdateUser;
	JButton btnRemoveUser;
	JButton btnRemoveAll;
	JButton btnCreateBase;
	JButton btnDestroyBase;
	// icon for base status
	Icon iconGreen = new ImageIcon(ImageLoader.load("bitmap_g.png"));
	Icon iconRed = new ImageIcon(ImageLoader.load("bitmap_r.png"));
	Icon iconYellow = new ImageIcon(ImageLoader.load("bitmap_y.png"));
	// MSO_Demo
	protected MsoDemo msoDemo;
	protected MorphoDevice mDevice;
	// variables
	private boolean btnIdentify;
	private boolean btnVerify;
	private boolean btnDestroy;
	private boolean btnCreate;
	private boolean btnRemove;
	private boolean btnUpdate;
	private boolean btnAdd;
	private boolean noCheck;

	/**
	 * Create the panel.
	 * 
	 * @param msoDemo
	 */
	public PanelDatabaseMgt(MsoDemo msoDemo) {
		this.msoDemo = msoDemo;
		this.mDevice = msoDemo.getMorphoDeviceInstance();
		setLayout(new BorderLayout(0, 0));

		JLabel lblSafranLogo = new JLabel();
		lblSafranLogo.setHorizontalAlignment(SwingConstants.CENTER);
		lblSafranLogo.setBorder(BorderFactory.createBevelBorder(BevelBorder.RAISED));
		lblSafranLogo.setIcon(new ImageIcon(ImageLoader.load("Logo.png")));
		add(lblSafranLogo, BorderLayout.NORTH);

		JPanel panel = new JPanel();
		panel.setBorder(new TitledBorder(null, "Database and Files Management", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		add(panel, BorderLayout.SOUTH);

		btnIdentifyMatch = new JButton("Identify Match");
		btnVerifyMatch = new JButton("Verify Match");
		btnAddUser = new JButton("Add User");
		cbNoCheck = new JCheckBox("No Check");
		btnUpdateUser = new JButton("Update User");
		btnRemoveUser = new JButton("Remove User");
		btnRemoveAll = new JButton("Remove All");
		btnCreateBase = new JButton("Create Base");
		btnDestroyBase = new JButton("Destroy Base");
		lblBaseStatus = new JLabel("Base Status");
		lblBaseStatus.setIcon(iconGreen);
		// set actions
		btnIdentifyMatch.addActionListener(this);
		btnVerifyMatch.addActionListener(this);
		btnAddUser.addActionListener(this);
		btnUpdateUser.addActionListener(this);
		btnRemoveUser.addActionListener(this);
		btnRemoveAll.addActionListener(this);
		btnCreateBase.addActionListener(this);
		btnDestroyBase.addActionListener(this);

		JScrollPane scrollPane = new JScrollPane();

		GroupLayout gl_panel = new GroupLayout(panel);
		gl_panel.setHorizontalGroup(gl_panel.createParallelGroup(Alignment.LEADING).addGroup(
				Alignment.TRAILING,
				gl_panel.createSequentialGroup().addComponent(scrollPane, GroupLayout.DEFAULT_SIZE, 297, Short.MAX_VALUE).addPreferredGap(ComponentPlacement.RELATED)
						.addGroup(gl_panel.createParallelGroup(Alignment.TRAILING).addComponent(btnIdentifyMatch).addGroup(gl_panel.createParallelGroup(Alignment.LEADING).addComponent(btnAddUser).addComponent(btnVerifyMatch).addComponent(cbNoCheck).addComponent(btnUpdateUser).addComponent(btnRemoveUser).addComponent(btnRemoveAll).addComponent(btnCreateBase).addComponent(btnDestroyBase)).addComponent(lblBaseStatus)).addContainerGap()));
		gl_panel.setVerticalGroup(gl_panel.createParallelGroup(Alignment.LEADING).addGroup(
				Alignment.TRAILING,
				gl_panel.createSequentialGroup()
						.addGroup(
								gl_panel.createParallelGroup(Alignment.TRAILING)
										.addGroup(gl_panel.createSequentialGroup().addGap(13).addComponent(scrollPane, GroupLayout.DEFAULT_SIZE, 268, Short.MAX_VALUE))
										.addGroup(
												gl_panel.createSequentialGroup().addComponent(lblBaseStatus).addPreferredGap(ComponentPlacement.RELATED, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addComponent(btnIdentifyMatch).addPreferredGap(ComponentPlacement.RELATED).addComponent(btnVerifyMatch).addPreferredGap(ComponentPlacement.RELATED).addComponent(btnAddUser).addPreferredGap(ComponentPlacement.UNRELATED).addComponent(cbNoCheck).addPreferredGap(ComponentPlacement.RELATED)
														.addComponent(btnUpdateUser).addPreferredGap(ComponentPlacement.RELATED).addComponent(btnRemoveUser).addPreferredGap(ComponentPlacement.RELATED).addComponent(btnRemoveAll).addPreferredGap(ComponentPlacement.RELATED).addComponent(btnCreateBase).addPreferredGap(ComponentPlacement.RELATED).addComponent(btnDestroyBase))).addContainerGap()));
		gl_panel.linkSize(SwingConstants.VERTICAL, new Component[] { btnIdentifyMatch, btnVerifyMatch, btnAddUser, cbNoCheck, btnUpdateUser, btnRemoveUser, btnRemoveAll, btnCreateBase, btnDestroyBase, lblBaseStatus });
		gl_panel.linkSize(SwingConstants.HORIZONTAL, new Component[] { btnIdentifyMatch, btnVerifyMatch, btnAddUser, cbNoCheck, btnUpdateUser, btnRemoveUser, btnRemoveAll, btnCreateBase, btnDestroyBase, lblBaseStatus });

		// table
		dbTable = new JTable();
		dbTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		Object[][] columns = null;
		Object[] rows = new String[] { "ID", "Firstname", "Lastname" };
		dbTable.setModel(new DefaultTableModel(columns, rows) {
			private static final long serialVersionUID = 1L;
			boolean[] columnEditables = new boolean[] { false, false, false };

			public boolean isCellEditable(int row, int column) {
				return columnEditables[column];
			}
		});
		dbTable.setFillsViewportHeight(true);
		scrollPane.setViewportView(dbTable);
		panel.setLayout(gl_panel);

		loadUsers();

		// tabs
		JTabbedPane tabbedPane = new JTabbedPane(JTabbedPane.TOP);
		add(tabbedPane, BorderLayout.CENTER);
		// tab DB info
		tabDbInfo = new TabDbInformation(this);
		tabbedPane.addTab("Database Information", tabDbInfo);
		// tab BIO settings
		tabBioSettings = new TabBioSettings(this);
		tabbedPane.addTab("Biometric Settings", tabBioSettings);
		// tab option
		tabOptions = new TabOptions(this);
		tabbedPane.addTab("Options", tabOptions);
		
		
		dbTable.addMouseListener(new MouseAdapter() {
	        public void mousePressed(MouseEvent e) {
	          if (e.getClickCount() == 2) {
	        	  String userID = getSelectedUserId();
	        	  if(userID != "")
	        	  {
	        		  PanelDatabaseMgt.this.msoDemo.leftPanel.selectTabVerify();
	        		  Thread startStopThread = new Thread(new Runnable() {
	      				@Override
	      				public void run() {
	      					PanelDatabaseMgt.this.msoDemo.leftPanel.startStop();					
	      				}
	      			});
	      			startStopThread.start();
	        	  }
	          }
	        }
	      });
	}

	/**
	 *
	 */
	public void loadUsers() {
		DefaultTableModel dbModel = (DefaultTableModel) dbTable.getModel();
		dbModel.getDataVector().removeAllElements();
		dbModel.fireTableDataChanged();

		MorphoDatabase mDatabase = msoDemo.getMorphoDatabaseInstance();

		int[] nbUsedRecord = { 0 };
		mDatabase.getNbUsedRecord(nbUsedRecord);
		if (nbUsedRecord[0] != 0) {
			byte[] fieldIndexDescriptor = { 0, 0, 0 };

			int ret = mDatabase.fillIndexDescriptor(true, (short) 0, fieldIndexDescriptor);

			if (ret == MorphoSmartSDK.MORPHO_OK) {
				ret = mDatabase.fillIndexDescriptor(false, (short) 1, fieldIndexDescriptor);
			}

			if (ret == MorphoSmartSDK.MORPHO_OK) {
				ret = mDatabase.fillIndexDescriptor(false, (short) 2, fieldIndexDescriptor);
			}

			if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("Update User", "An error occured while calling MorphoDatabase.fillIndexDescriptor() function", ret, mDevice.getInternalError());
				return;
			}

			MorphoUserList mUserList = new MorphoUserList();
			ret = mDatabase.readPublicFields(fieldIndexDescriptor, mUserList);

			if (ret != MorphoSmartSDK.MORPHO_OK && ret != MorphoSmartSDK.MORPHOERR_USER_NOT_FOUND && ret != MorphoSmartSDK.MORPHOERR_FIELD_NOT_FOUND) {
				DialogUtils.showErrorMessage("Update User", "An error occured while calling MorphoDatabase.readPublicFields() function", ret, mDevice.getInternalError());
				return;
			}

			int[] nbUser = { 0 };
			ret = mUserList.getNbUser(nbUser);
			if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("Update User", "An error occured while calling MorphoUserList.getNbUser() function", ret, msoDemo.getMorphoDeviceInstance().getInternalError());
				return;
			}

			for (int i = 0; i < nbUser[0]; ++i) {
				MorphoUser mUser = mUserList.getMorphoUser(i);

				if (mUser != null) {
				
					if(MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus){
						
						
						ArrayList<Byte> userID = new ArrayList<Byte>();
						ArrayList<Byte> firstName = new ArrayList<Byte>();
						ArrayList<Byte> lastName = new ArrayList<Byte>();

						
						String id_User;
						String first_Name;
						String last_name;
						
						byte[] privacyKey = msoDemo.getPrivacyModeKey();
						if (null != privacyKey) {
						
						
						ret = mUser.getBufferField(0, userID);
						if (ret == MorphoSmartSDK.MORPHOERR_FIELD_NOT_FOUND) {
							id_User="";
							} else if (ret != MorphoSmartSDK.MORPHO_OK) {
							DialogUtils.showErrorMessage("Update User", "An error occured while calling MorphoUser.getBufferField() function", ret, mDevice.getInternalError());
							return;
						}else {
							
							byte[] idCrypted=msoDemo.convertArrayListToByteArray(userID);
							
							byte[] idFieldDecrypted = msoDemo.decryptAndCheckPrivacyData(idCrypted,privacyKey);
							byte[] idDecrypted = Arrays.copyOfRange(idFieldDecrypted, 8, idFieldDecrypted.length);
							id_User=new String(idDecrypted);
						}
						
						ret = mUser.getBufferField(1, firstName);
						if (ret == MorphoSmartSDK.MORPHOERR_FIELD_NOT_FOUND) {
							first_Name="";
						} else if (ret != MorphoSmartSDK.MORPHO_OK) {
							DialogUtils.showErrorMessage("Update User", "An error occured while calling MorphoUser.getBufferField() function", ret, mDevice.getInternalError());
							return;
						}else {
							
							byte[] firstNameCrypted = msoDemo.convertArrayListToByteArray(firstName);
							
							byte[] firstNameFieldDecrypted =  msoDemo.decryptAndCheckPrivacyData(firstNameCrypted,privacyKey);
							byte[] firstNameDecrypted = Arrays.copyOfRange(firstNameFieldDecrypted, 8, firstNameFieldDecrypted.length);
							first_Name=new String(firstNameDecrypted);
						}
						
						
						ret = mUser.getBufferField(2, lastName);
						if (ret == MorphoSmartSDK.MORPHOERR_FIELD_NOT_FOUND) {
							last_name="";
						} else if (ret != MorphoSmartSDK.MORPHO_OK) {
							DialogUtils.showErrorMessage("Update User", "An error occured while calling MorphoUser.getBufferField() function", ret, mDevice.getInternalError());
							return;
						}else {
							byte[] lastNameCrypted = msoDemo.convertArrayListToByteArray(lastName);

							byte[] lastNameFieldDecrypted =  msoDemo.decryptAndCheckPrivacyData(lastNameCrypted,privacyKey);
							byte[] lastNameDecrypted = Arrays.copyOfRange(lastNameFieldDecrypted, 8, lastNameFieldDecrypted.length);
							last_name=new String(lastNameDecrypted);
						}
						
						addTableRow(id_User, first_Name, last_name);
						
						} else {
							DialogUtils.showErrorMessage("loadUsers", "Invalid Kprivacy key");
						}
						
					}
				else{	
					ArrayList<String> userID = new ArrayList<String>();
					ArrayList<String> firstName = new ArrayList<String>();
					ArrayList<String> lastName = new ArrayList<String>();

					ret = mUser.getField(0, userID);
					if (ret == MorphoSmartSDK.MORPHOERR_FIELD_NOT_FOUND) {
						userID.clear();
						userID.add("");
					} else if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage("Update User", "An error occured while calling MorphoUser.getField() function", ret, mDevice.getInternalError());
						return;
					}

					ret = mUser.getField(1, firstName);
					if (ret == MorphoSmartSDK.MORPHOERR_FIELD_NOT_FOUND) {
						firstName.clear();
						firstName.add("");
					} else if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage("Update User", "An error occured while calling MorphoUser.getField() function", ret, mDevice.getInternalError());
						return;
					}

					ret = mUser.getField(2, lastName);
					if (ret == MorphoSmartSDK.MORPHOERR_FIELD_NOT_FOUND) {
						lastName.clear();
						lastName.add("");
					} else if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage("Update User", "An error occured while calling MorphoUser.getField() function", ret, mDevice.getInternalError());
						return;
					}

					addTableRow(userID.get(0), firstName.get(0), lastName.get(0));
				}
				}
			}
		}

		if (nbUsedRecord[0] != 0) {
			btnRemoveUser.setEnabled(true);
			btnRemoveAll.setEnabled(true);
			btnUpdateUser.setEnabled(true);
			btnIdentifyMatch.setEnabled(true);
			lblBaseStatus.setIcon(iconGreen);
		} else {
			btnRemoveUser.setEnabled(false);
			btnRemoveAll.setEnabled(false);
			btnUpdateUser.setEnabled(false);
			btnIdentifyMatch.setEnabled(false);
			lblBaseStatus.setIcon(iconYellow);
		}
	}

	/**
	 * Add row to Database Table
	 * 
	 * @param id
	 * @param firstName
	 * @param lastName
	 */
	public void addTableRow(String userId, String firstName, String lastName) {
		firstName = firstName.trim();
		if (firstName.equals(""))
			firstName = "<None>";

		lastName = lastName.trim();
		if (lastName.equals(""))
			lastName = "<None>";

		DefaultTableModel dbModel = (DefaultTableModel) dbTable.getModel();
		String[] data = new String[] { userId, firstName, lastName };
		dbModel.addRow(data);
	}

	/**
	 * return the ID of the selected user from DB
	 * 
	 * @return
	 */
	public String getSelectedUserId() {
		DefaultTableModel dbModel = (DefaultTableModel) dbTable.getModel();
		int index = dbTable.getSelectedRow();
		if (index != -1) {
			return (String) dbModel.getValueAt(index, 0);
		} else {
			return "";
		}
	}

	@Override
	public void actionPerformed(ActionEvent event) {
		if ("Add User".equals(event.getActionCommand())) {
			AddUser();
		} else if ("Update User".equals(event.getActionCommand())) {
			updateUser();
		} else if ("Identify Match".equals(event.getActionCommand())) {
			identifyMatch();
		} else if ("Verify Match".equals(event.getActionCommand())) {
			verifyMatch();
		} else if ("Remove User".equals(event.getActionCommand())) {
			removeUser();
		} else if ("Remove All".equals(event.getActionCommand())) {
			removeAll();
		} else if ("Create Base".equals(event.getActionCommand())) {
			createBase();
		} else if ("Destroy Base".equals(event.getActionCommand())) {
			destroyBase(true);
		}
	}

	public void identifyMatch() {
		int ret = 0;
		String fct = "";
		ArrayList<String> templateList1 = FilesMgt.getTemplateFiles("IdentifyMatch", false, msoDemo); // select
																										// one
																										// template
																										// file

		DialogResultWindow resultWindow = null;
		byte[] random = new byte[4];
		new Random().nextBytes(random);
		byte[] privacyKey = null;

		if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus)
			privacyKey = msoDemo.getPrivacyModeKey();

		if (templateList1.size() == 1) {
			UserData userData;
			if(msoDemo.PrivacyModeStatus!= MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED) {

				if (null != privacyKey) {
					userData = UsersMgt.getUserDataFromFile(templateList1.get(0), true ,privacyKey , random, msoDemo);
				}else {
					DialogUtils.showErrorMessage("identifyMatch", "Invalid Kprivacy key");
					return;
				}
				
			}else {
				
				userData = UsersMgt.getUserDataFromFile(templateList1.get(0));
			}
			
			if (userData.getNbFinger() == 0)
				return;
			if (userData.getNbFinger() > 1) {
				DialogUtils.showInfoMessage("IdentifyMatch", "This file contains two templates.\nOnly the first one will be used\nfor the identification.\n\nPress OK to continue...");
				if (userData.getMorphoTemplateType() == MorphoTemplateType.MORPHO_PK_ANSI_378) {
					userData.setNbFinger(1);
				}
			}

			resultWindow = new DialogResultWindow(msoDemo, "The file data :", "Firstname : " + userData.getFirstName(), "Lastname : " + userData.getLastName(), "ID : " + userData.getUserID(), "");
			resultWindow.setVisible(true);

			MorphoTemplateList morphoTemplateList = new MorphoTemplateList();

			short[] indexTemplate = { 0 };

			for (int i = 0; i < userData.getNbFinger(); ++i) {
				if (userData.getMorphoTemplateType() != MorphoTemplateType.MORPHO_NO_PK_FP) {
					ret = morphoTemplateList.putTemplate(userData.getMorphoTemplateType(), userData.getTemplateData(i), (short) 0xFF, (short) 0, indexTemplate);
					fct = "putTemplate";
				} else if (userData.getMorphoFVPTemplateType() != MorphoFVPTemplateType.MORPHO_NO_PK_FVP) {
					ret = morphoTemplateList.putFVPTemplate(userData.getMorphoFVPTemplateType(), userData.getTemplateData(i), (short) 0, (short) 0, indexTemplate);
					fct = "putFVPTemplate";
				} else {
					break;
				}

				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils.showErrorMessage("IdentifyMatch", "An error occured while calling MorphoTemplateList." + fct + "() function", ret, mDevice.getInternalError());
					return;
				}
			}

			byte[] pkX984 = userData.getPkX984TemplateData();
			if (pkX984 != null) {
				ret = morphoTemplateList.putX984(pkX984);

				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils.showErrorMessage("IdentifyMatch", "An error occured while calling MorphoTemplateList.putX984() function", ret, mDevice.getInternalError());
					return;
				}
			}

			int securityLevel = this.msoDemo.getSecurityLevel(EnumTabIndex.TAB_IDENTIFY);
			mDevice.setSecurityLevel(securityLevel);

			// The MSO doesn't accept several PK. Only the first one is used.
			morphoTemplateList.setPkX984Index((short) 1);

			MorphoDatabase mDatabase = this.msoDemo.getMorphoDatabaseInstance();
			MorphoUser mUser = new MorphoUser();

			MorphoFAR FAR = MorphoFAR.swigToEnum(Integer.parseInt(tabBioSettings.txtMatchingThreshold.getText()));

			long[] matchingScore = { 0 };
			if (msoDemo.isTUNNELING() || msoDemo.isOFFERED_SECURITY()) {
				matchingScore = null;
			}
			short[] fingerIndex = { 0 };

			msoDemo.waitCursor();
			ret = mDatabase.identifyMatch(FAR, morphoTemplateList, mUser, matchingScore, fingerIndex);
			msoDemo.defaultCursor();
			String message1 = "";
			String message2 = "";
			String message3 = "";
			String message4 = "";

			if (ret != MorphoSmartSDK.MORPHO_OK) {
				message1 = "Identification Failed";

				if (ret == MorphoSmartSDK.MORPHOERR_NO_HIT) {
					message2 = "Person not Identified";
				} else if (ret == MorphoSmartSDK.MORPHOERR_LICENSE_MISSING) {
					message2 = "A required license is missing";
				} else {
					DialogUtils.showErrorMessage("IdentifyMatch", "An error occured while calling MorphoDatabase.identifyMatch() function", ret, mDevice.getInternalError());
					return;
				}
			} else {
				ArrayList<Byte> userID = new ArrayList<Byte>();
				ArrayList<Byte> firstName = new ArrayList<Byte>();
				ArrayList<Byte> lastName = new ArrayList<Byte>();
				
				
				if(msoDemo.PrivacyModeStatus!= MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED) {
					
					
					
					if (null != privacyKey) {
						
						
						if (mUser.getBufferField(0, userID) == MorphoSmartSDK.MORPHO_OK && mUser.getBufferField(1, firstName) == MorphoSmartSDK.MORPHO_OK && mUser.getBufferField(2, lastName) == MorphoSmartSDK.MORPHO_OK) {
							byte[] crypteduserID = msoDemo.convertArrayListToByteArray(userID);
							byte[] cryptedfirstName = msoDemo.convertArrayListToByteArray(firstName);
							byte[] cryptedlastName = msoDemo.convertArrayListToByteArray(lastName);
							int response = DialogUtils.showQuestionMessage("MSO_Demo Java", "Privacy mode : data is crypted.\nWould you like to decrypt it?");
							if (0 == response) {
								
								byte[] decrypteduserID = msoDemo.decryptAndCheckPrivacyData(crypteduserID, privacyKey);
								byte[] decryptedfirstName = msoDemo.decryptAndCheckPrivacyData(cryptedfirstName, privacyKey);
								byte[] decryptedcryptedlastName = msoDemo.decryptAndCheckPrivacyData(cryptedlastName, privacyKey);
								
								message1 = "User identified";
								message4 = "ID : " + new String(decrypteduserID);
								message2 = "Firstname : " + new String(decryptedfirstName);
								message3 = "Lastname : " + new String(decryptedcryptedlastName);
							}else {
								message1 = "User identified";
								message4 = "ID : " + new String(crypteduserID);
								message2 = "Firstname : " + new String(cryptedfirstName);
								message3 = "Lastname : " + new String(cryptedlastName);
								
							}
							
						
						}else {
							DialogUtils.showErrorMessage("IdentifyMatch", "An error occured while calling MorphoUser.getBufferField() function", ret, mDevice.getInternalError());
							return;
						}
						
		
					}else {
						DialogUtils.showErrorMessage("identifyMatch", "Invalid Kprivacy key");
						return;
					}
					
					
				}else{	
					ArrayList<String> dataField = new ArrayList<String>();
					ret = mUser.getField(0, dataField); // User ID

					if (ret == MorphoSmartSDK.MORPHO_OK) {
						message1 = "User identified";
						message4 = "ID : " + dataField.get(0);

						ret = mUser.getField(1, dataField); // Firstname

						if (ret == MorphoSmartSDK.MORPHO_OK) {
							message2 = "Firstname : " + dataField.get(0);

							ret = mUser.getField(2, dataField); // Lastname

							if (ret == MorphoSmartSDK.MORPHO_OK) {
								message3 = "Lastname : " + dataField.get(0);
							}
						}
					}
	
					if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage("IdentifyMatch", "An error occured while calling MorphoUser.getField() function", ret, mDevice.getInternalError());
						return;
					}
				}
			}

			resultWindow = new DialogResultWindow(msoDemo, message1, message2, message3, message4, "");
			resultWindow.setVisible(true);
		}
	}

	public void verifyMatch() {
		int ret = 0;
		String fct = "";
		byte[] random = new byte[4];
		new Random().nextBytes(random);
		byte[] privacyKey = null;

		if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus)
			privacyKey = msoDemo.getPrivacyModeKey();

		ArrayList<String> templateList1 = FilesMgt.getTemplateFiles("VerifyMatch", false, msoDemo); // select
																									// one
																									// template
																									// file

		if (templateList1.size() == 1) {
			UserData userData;
			if (msoDemo.PrivacyModeStatus != MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED) {

				if (null != privacyKey) {
					userData = UsersMgt.getUserDataFromFile(templateList1.get(0), true, privacyKey, random, msoDemo);
				} else {
					DialogUtils.showErrorMessage("verifyMatch", "Invalid Kprivacy key");
					return;
				}
			} else {

				userData = UsersMgt.getUserDataFromFile(templateList1.get(0));
			}

			if (userData.getNbFinger() == 0)
				return;
			if (userData.getNbFinger() > 1) {
				DialogUtils.showInfoMessage("VerifyMatch", "This file contains two templates.\nOnly the first one will be used\nfor the verification.\n\nPress OK to continue...");
				if (userData.getMorphoTemplateType() == MorphoTemplateType.MORPHO_PK_ANSI_378) {
					userData.setNbFinger(1);
				}
			}

			MorphoTemplateList morphoTemplateList1 = new MorphoTemplateList();

			short[] indexTemplate = { 0 };

			for (int i = 0; i < userData.getNbFinger(); ++i) {
				if (userData.getMorphoFVPTemplateType() != MorphoFVPTemplateType.MORPHO_NO_PK_FVP) {
					ret = morphoTemplateList1.putFVPTemplate(userData.getMorphoFVPTemplateType(), userData.getTemplateData(i), (short) 0, (short) 0, indexTemplate);
					fct = "putFVPTemplate";
				} else if (userData.getMorphoTemplateType() != MorphoTemplateType.MORPHO_NO_PK_FP) {
					ret = morphoTemplateList1.putTemplate(userData.getMorphoTemplateType(), userData.getTemplateData(i), (short) 0xFF, (short) 0, indexTemplate);
					fct = "putTemplate";
				} else {
					break;
				}

				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils.showErrorMessage("VerifyMatch", "An error occured while calling MorphoTemplateList." + fct + "() function", ret, mDevice.getInternalError());
					return;
				}
			}

			byte[] pkX984 = userData.getPkX984TemplateData();
			if (pkX984 != null) {
				ret = morphoTemplateList1.putX984(pkX984);

				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils.showErrorMessage("VerifyMatch", "An error occured while calling MorphoTemplateList.putX984() function", ret, mDevice.getInternalError());
					return;
				}
			}

			ArrayList<String> templateList2 = FilesMgt.getTemplateFiles("VerifyMatch", true, msoDemo);

			if (templateList2.size() == 0) {
				return;
			}

			MorphoTemplateList morphoTemplateList2 = new MorphoTemplateList();

			boolean isTkbFile = false;
			if (templateList2.size() == 0)
				return;
			else if (templateList2.size() > 20) {
				DialogUtils.showWarningMessage("Mso_Demo", "The number of templates must be less or equal to 20.");
				return;
			}

			List<UserData> usersData = new ArrayList<UserData>();
			if (templateList2.size() > 0) {
				for (int i = 0; i < templateList2.size(); ++i) {
					String filePath = templateList2.get(i);
					if (i == 0) {
						if (filePath.endsWith(".tkb"))
							isTkbFile = true;
					} else {
						if ((isTkbFile && !filePath.endsWith(".tkb")) || (!isTkbFile && filePath.endsWith(".tkb"))) {
							DialogUtils.showWarningMessage("Mso_Demo", "You can not mix tkb template with standard templates.");
							return;
						}
					}

					if (isTkbFile && templateList2.size() > 1) {
						DialogUtils.showWarningMessage("Mso_Demo", "You cannot use more than one tkb file.");
						return;
					}

					if (msoDemo.PrivacyModeStatus != MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED) {

						if (null != privacyKey) {
							userData = UsersMgt.getUserDataFromFile(filePath, true, privacyKey, random, msoDemo);
						} else {
							DialogUtils.showErrorMessage("verifyMatch", "Invalid Kprivacy key");
							return;
						}
					} else {

						userData = UsersMgt.getUserDataFromFile(filePath);
					}

					if (userData.getNbFinger() == 0)
						return;
					usersData.add(userData);

					if (isTkbFile) {
						ret = morphoTemplateList2.putX984(userData.getPkX984TemplateData());

						if (ret != MorphoSmartSDK.MORPHO_OK) {
							DialogUtils.showErrorMessage("Verify", "An error occured while calling morphoTemplateList2.putX984() function", ret, mDevice.getInternalError());
							return;
						}
					} else {
						if (userData.getMorphoTemplateType() != MorphoTemplateType.MORPHO_NO_PK_FP) {
							ret = morphoTemplateList2.putTemplate(userData.getMorphoTemplateType(), userData.getTemplateData(0), (short) 0xFF, (short) 0, indexTemplate);
							fct = "putTemplate";
						} else if (userData.getMorphoFVPTemplateType() != MorphoFVPTemplateType.MORPHO_NO_PK_FVP) {
							ret = morphoTemplateList2.putFVPTemplate(userData.getMorphoFVPTemplateType(), userData.getTemplateData(0), (short) 0, (short) 0, indexTemplate);
							fct = "putFVPTemplate";
						}

						if (ret != MorphoSmartSDK.MORPHO_OK) {
							DialogUtils.showErrorMessage("Verify", "An error occured while calling morphoTemplateList2." + fct + "() function", ret, mDevice.getInternalError());
							return;
						}
					}
				}
			}

			int securityLevel = this.msoDemo.getSecurityLevel(EnumTabIndex.TAB_VERIFY);
			mDevice.setSecurityLevel(securityLevel);

			// The MSO doesn't accept several PK. Only the first one is used.
			morphoTemplateList1.setPkX984Index((short) 1);

			MorphoFAR FAR = MorphoFAR.swigToEnum(Integer.parseInt(tabBioSettings.txtMatchingThreshold.getText()));

			long[] matchingScore = { 0 };

			if (msoDemo.isTUNNELING() || msoDemo.isOFFERED_SECURITY()) {
				matchingScore = null;
			}

			msoDemo.waitCursor();
			ret = mDevice.verifyMatch(FAR, morphoTemplateList1, morphoTemplateList2, matchingScore);
			msoDemo.defaultCursor();

			DialogResultWindow resultWindow = null;

			if (ret != MorphoSmartSDK.MORPHO_OK) {
				if (ret == MorphoSmartSDK.MORPHOERR_NO_HIT) {
					resultWindow = new DialogResultWindow(msoDemo, "Authentication Failed", "Person not authenticated", "", "", "");
				} else if (ret == MorphoSmartSDK.MORPHOERR_LICENSE_MISSING) {
					resultWindow = new DialogResultWindow(msoDemo, "Verification Failed", "A required license is missing.", "", "", "");
				} else {
					DialogUtils.showErrorMessage("VerifyMatch", "An error occured while calling MorphoDevice.verifyMatch() function", ret, mDevice.getInternalError());
					return;
				}
			} else {
				resultWindow = new DialogResultWindow(msoDemo, "User authenticated", "Firstname : " + userData.getFirstName(), "Lastname : " + userData.getLastName(), "ID : " + userData.getUserID(), "");
			}

			resultWindow.setVisible(true);
		}
	}

	public void createBase() {
		DialogCreateDB dialogCreateDB = new DialogCreateDB(this.msoDemo);
		dialogCreateDB.setDevice(msoDemo.getMorphoDeviceInstance());
		dialogCreateDB.setDatabase(msoDemo.getMorphoDatabaseInstance());
		dialogCreateDB.setVisible(true);

		this.msoDemo.updateDataBaseInformation();
		loadUsers();

		if (dialogCreateDB.getSdkError() == 0) {
			this.msoDemo.enableDataBaseFunction(true);
			lblBaseStatus.setIcon(iconYellow);
		} else {
			this.msoDemo.enableDataBaseFunction(false);
		}
	}

	public void destroyBase(boolean askConfirmation) {
		if (askConfirmation) {
			int ret = DialogUtils.showQuestionMessage("Destroy Base", "This command is going to destroy the current database,\nand the whole content will be erased. Most biometrics operations\nwon't work anymore, unless you create a new base.\n\nConfirm ?");
			if (ret == JOptionPane.NO_OPTION)
				return;
		}
		
		MorphoDatabase mDatabase = msoDemo.getMorphoDatabaseInstance();
		msoDemo.waitCursor();
		int ret = mDatabase.dbDelete(MorphoTypeDeletion.MORPHO_DESTROY_BASE);
		msoDemo.defaultCursor();
		if (ret != MorphoSmartSDK.MORPHO_OK) {
			DialogUtils.showErrorMessage("Destroy Base", "An error occured while calling MorphoDatabase.dbDelete(MORPHO_DESTROY_BASE) function", ret, mDevice.getInternalError());
		} else {
			loadUsers();

			tabDbInfo.setLblEncryptedBb("N/A");
			tabDbInfo.setLblMaxNbOfRecords(0);
			tabDbInfo.setLblNbOfFingersPerRecord(0);
			tabDbInfo.setLblCurrentNbOfRecords(0);

			btnDestroyBase.setEnabled(false);
			btnCreateBase.setEnabled(true);
			btnAddUser.setEnabled(false);
			cbNoCheck.setEnabled(false);
			lblBaseStatus.setIcon(iconRed);
			msoDemo.leftPanel.AddTabsEnrollIdentify(false);
		}
	}

	/**
	 *
	 */
	public void removeUser() {
		DefaultTableModel dbModel = (DefaultTableModel) dbTable.getModel();
		int index = dbTable.getSelectedRow();
		if (index != -1) {
			String userID = (String) dbModel.getValueAt(index, 0);
			MorphoDatabase mDatabase = this.msoDemo.getMorphoDatabaseInstance();

			MorphoUser mUser = new MorphoUser();
			int ret;
			if(MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus){
				byte[] privacyKey = msoDemo.getPrivacyModeKey();
				if (null != privacyKey) {
					byte[] random = new byte[4];
					new Random().nextBytes(random);
					byte[] crypteddata = msoDemo.encryptPrivacyData(userID.getBytes(), privacyKey, random);
					ret = mDatabase.getUserBuffer(crypteddata, mUser);
				} else {
					DialogUtils.showErrorMessage("removeUser", "Invalid Kprivacy key");
					return;
				}

			} else {
				ret = mDatabase.getUser(userID, mUser);
			}

			if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("Remove User",
						"An error occured while calling MorphoDatabase.getUser() function", ret,
						mDevice.getInternalError());
			} else {
				msoDemo.waitCursor();
				ret = mUser.dbDelete();
				msoDemo.defaultCursor();

				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils.showErrorMessage("Remove User",
							"An error occured while calling MorphoUser.dbDelete() function", ret,
							mDevice.getInternalError());
				} else {
					loadUsers();
					this.msoDemo.updateDataBaseInformation();
				}
			}
		} else {
			DialogUtils.showWarningMessage("Remove User", "Select a user first");
		}
	}

	public void removeAll() {
		int ret = DialogUtils.showQuestionMessage("Remove All Users",
				"This operation is going to erase the whole Database. Confirm ?");
		if (ret == JOptionPane.YES_OPTION) {
			MorphoDatabase mDatabase = msoDemo.getMorphoDatabaseInstance();
			msoDemo.waitCursor();
			ret = mDatabase.dbDelete(MorphoTypeDeletion.MORPHO_ERASE_BASE);
			msoDemo.defaultCursor();
			if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("Remove All Users",
						"An error occured while calling MorphoDatabase.dbDelete(MORPHO_ERASE_BASE) function", ret,
						mDevice.getInternalError());
			} else {
				DialogUtils.showInfoMessage("Remove All Users", "Database Empty...");
				msoDemo.updateDataBaseInformation();
				loadUsers();
			}
		}
	}

	/**
	 *
	 */
	public void updateUser() {
		DefaultTableModel dbModel = (DefaultTableModel) dbTable.getModel();
		int index = dbTable.getSelectedRow();
		if (index != -1) {
			String userID = (String) dbModel.getValueAt(index, 0);
			String firstName = (String) dbModel.getValueAt(index, 1);
			String lastName = (String) dbModel.getValueAt(index, 2);

			new DialogAddUpdateUser(msoDemo, "Update", userID, firstName, lastName).setVisible(true);
		} else {
			DialogUtils.showWarningMessage("Update User", "Select a user first");
		}
	}

	/**
	 *
	 */
	public void AddUser() {
		byte[] random = new byte[4];
		new Random().nextBytes(random);
		byte[] privacyKey = null;

		if (MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED != msoDemo.PrivacyModeStatus)
			privacyKey = msoDemo.getPrivacyModeKey();

		ArrayList<String> listFiles = FilesMgt.getTemplateFiles("Add User", true, msoDemo);
		int nbTemplate = listFiles.size();

		if (nbTemplate == 0)
			return;

		MorphoUser mUser = new MorphoUser();

		int ret = 0;
		boolean isNoCkeck = msoDemo.getPanelDatabaseMgtInstance().isNoCheck();
		boolean tkb_or_pks = false;
	
		for (int i = 0; i < nbTemplate; ++i) {
			String file = listFiles.get(i);
			if (i == 0) {
				if (file.endsWith("pks") || file.endsWith("tkb")) { // tkb or
																	// pks
																	// template
					tkb_or_pks = true;
				}
			} else {
				if ((!tkb_or_pks && (file.endsWith("pks") || file.endsWith("tkb")))
						|| (tkb_or_pks && (!file.endsWith("pks") && !file.endsWith("tkb")))) {
					DialogUtils.showErrorMessage("Add User",
							"You can not mix tkb or pks templates with standard templates");
					return;
				}
			}
		}

		short[] nbFinger = { 0 };
		MorphoDatabase mDatabase = msoDemo.getMorphoDatabaseInstance();
		mDatabase.getNbFinger(nbFinger);
		if (!tkb_or_pks && nbTemplate > nbFinger[0]) {
			DialogUtils.showErrorMessage("Add User",
					"The number of selected templates does not match the database format");
			return;
		}

		String userID = "";
		String firstName = "";
		String lastName = "";

		DialogAddUpdateUser dialogAddUpdateUser = null;

		if (!tkb_or_pks) {
			dialogAddUpdateUser = new DialogAddUpdateUser(PanelDatabaseMgt.this.msoDemo, "Add", "", "", "");
			dialogAddUpdateUser.setVisible(true);

			if (!dialogAddUpdateUser.isOK()) {
				return;
			}
			userID = dialogAddUpdateUser.getUserID();
			firstName = dialogAddUpdateUser.getFirstName();
			lastName = dialogAddUpdateUser.getLastName();

			if (msoDemo.PrivacyModeStatus != MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED) {

				if (null != privacyKey) {

					byte[] idCrypted = msoDemo.encryptPrivacyData(userID.getBytes(), privacyKey, random);
					byte[] firstNameCrypted = msoDemo.encryptPrivacyData(firstName.getBytes(), privacyKey, random);
					byte[] lastNameCrypted = msoDemo.encryptPrivacyData(lastName.getBytes(), privacyKey, random);
					ret = mDatabase.getUserBuffer(idCrypted, mUser);

					if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage("Add User",
								"An error occured while calling MorphoDatabase.getUserBuffer() function", ret,
								this.msoDemo.getMorphoDeviceInstance().getInternalError());
						return;
					}

					// Firstname
					ret = mUser.putBufferField(1, firstNameCrypted);
					if (ret == MorphoSmartSDK.MORPHO_OK) {
						// Lastname
						ret = mUser.putBufferField(2, lastNameCrypted);
					}

					if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage("Add User",
								"An error occured while calling MorphoUser.putBufferField() function", ret,
								this.msoDemo.getMorphoDeviceInstance().getInternalError());
						return;
					}
				} else {

					DialogUtils.showErrorMessage("AddUser", "Invalid Kprivacy key");
					return;
				}

			} else {

				ret = mDatabase.getUser(userID, mUser);
				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils.showErrorMessage("Add User",
							"An error occured while calling MorphoDatabase.getUser() function", ret,
							this.msoDemo.getMorphoDeviceInstance().getInternalError());
					return;
				}

				// Firstname
				ret = mUser.putField(1, firstName);
				if (ret == MorphoSmartSDK.MORPHO_OK) {
					// Lastname
					ret = mUser.putField(2, lastName);
				}

				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils.showErrorMessage("Add User",
							"An error occured while calling MorphoUser.putField() function", ret,
							this.msoDemo.getMorphoDeviceInstance().getInternalError());
					return;
				}
			}
		}

		for (int i = 0; i < nbTemplate; ++i) {
			UserData userData;
			if (msoDemo.PrivacyModeStatus != MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED) {

				if (null != privacyKey) {
					userData = UsersMgt.getUserDataFromFile(listFiles.get(i), true, privacyKey, random, msoDemo);
				} else {
					DialogUtils.showErrorMessage("AddUser", "Invalid Kprivacy key");
					return;
				}
			} else {
				userData = UsersMgt.getUserDataFromFile(listFiles.get(i));
			}

			if (userData.getNbFinger() == 0)
				return;
			if (!tkb_or_pks) {
				if (userData.getNbFinger() > 1
						&& userData.getMorphoTemplateType() == MorphoTemplateType.MORPHO_PK_ANSI_378) {
					userData.setNbFinger(1);
				}

				if (userData.getNbFinger() > nbFinger[0]) {
					DialogUtils.showErrorMessage("Add User",
							"This file (" + listFiles.get(i) + ") contains more than " + nbFinger[0] + " template");
					return;
				}

				short[] indexTemplate = { 0 };
				if (userData.getMorphoFVPTemplateType() != MorphoFVPTemplateType.MORPHO_NO_PK_FVP) {
					ret = mUser.putFVPTemplate(userData.getMorphoFVPTemplateType(), userData.getTemplateData(0),
							(short) 0, (short) 0, indexTemplate);
				} else {
					ret = mUser.putTemplate(userData.getMorphoTemplateType(), userData.getTemplateData(0), (short) 0,
							(short) 0, indexTemplate);
				}

				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils.showErrorMessage("Add User",
							"An error occured while calling MorphoUser.putTemplate() function", ret,
							this.msoDemo.getMorphoDeviceInstance().getInternalError());
					return;
				}

			} else {

				if (msoDemo.PrivacyModeStatus != MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED) {

					if (null != privacyKey) {
						byte[] idCrypted = msoDemo.encryptPrivacyData(userData.getUserID().getBytes(), privacyKey, random);
						byte[] firstNameCrypted = msoDemo.encryptPrivacyData(userData.getFirstName().getBytes(), privacyKey, random);
						byte[] lastNameCrypted = msoDemo.encryptPrivacyData(userData.getLastName().getBytes(), privacyKey, random);
						ret = mDatabase.getUserBuffer(idCrypted, mUser);

						if (ret != MorphoSmartSDK.MORPHO_OK) {
							DialogUtils.showErrorMessage("Add User",
									"An error occured while calling MorphoDatabase.getUserBuffer() function", ret,
									this.msoDemo.getMorphoDeviceInstance().getInternalError());
							return;
						}

						// Firstname
						ret = mUser.putBufferField(1, firstNameCrypted);
						if (ret == MorphoSmartSDK.MORPHO_OK) {
							// Lastname
							ret = mUser.putBufferField(2, lastNameCrypted);
						}

						if (ret != MorphoSmartSDK.MORPHO_OK) {
							DialogUtils.showErrorMessage("Add User",
									"An error occured while calling MorphoUser.putBufferField() function", ret,
									this.msoDemo.getMorphoDeviceInstance().getInternalError());
							return;
						}
					} else {

						DialogUtils.showErrorMessage("AddUser", "Invalid Kprivacy key");
						return;
					}
				} else {
					ret = mDatabase.getUser(userData.getUserID(), mUser);
					if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage("Add User",
								"An error occured while calling MorphoDatabase.getUser() function", ret,
								this.msoDemo.getMorphoDeviceInstance().getInternalError());
						return;
					}

					userID = userData.getUserID();
					firstName = userData.getFirstName();
					lastName = userData.getLastName();

					// Firstname
					ret = mUser.putField(1, userData.getFirstName());
					if (ret == MorphoSmartSDK.MORPHO_OK) {
						// Lastname
						ret = mUser.putField(2, userData.getLastName());
					}

					if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage("Add User",
								"An error occured while calling MorphoUser.putField() function", ret,
								this.msoDemo.getMorphoDeviceInstance().getInternalError());
						return;
					}
				}
				if (listFiles.get(i).endsWith(".tkb")) // template tkb
				{
					ret = mUser.putX984(userData.getPkX984TemplateData());
					if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage("Add User",
								"An error occured while calling MorphoUser.putX984() function", ret,
								this.msoDemo.getMorphoDeviceInstance().getInternalError());
						return;
					}
				} else // template pks
				{
					for (int j = 0; j < userData.getNbFinger(); ++j) {
						short[] indexTemplate = { 0 };
						ret = mUser.putTemplate(userData.getMorphoTemplateType(), userData.getTemplateData(i),
								(short) 0, (short) 0, indexTemplate);
					}
				}

				mUser.setNoCheckOnTemplateForDBStore(isNoCkeck);

				msoDemo.waitCursor();
				ret = mUser.dbStore();
				msoDemo.defaultCursor();
				if (ret != MorphoSmartSDK.MORPHO_OK) {
					break;
				}
			}
		}

		if (ret == MorphoSmartSDK.MORPHO_OK && !tkb_or_pks) {
			mUser.setNoCheckOnTemplateForDBStore(isNoCkeck);
			msoDemo.waitCursor();
			ret = mUser.dbStore();
			msoDemo.defaultCursor();
		}

		if (ret != MorphoSmartSDK.MORPHO_OK) {
			switch (ret) {
			case MorphoSmartSDK.MORPHOERR_ALREADY_ENROLLED:
				if (nbTemplate == 1) {
					ret = DialogUtils.showQuestionMessage("Add User",
							"The templates of this user are already in the database !"
									+ "\nWould you like to enroll him again with other fingers ?");
					if (ret == JOptionPane.YES_OPTION) {
						DialogUtils.showInfoMessage("Add User",
								"Choose 1 or 2 fingers in the Enroll Page and press start.");
						PanelBasicBiometricOperation pbbo = msoDemo.getPanelBasicBiometricOperationInstance();
						pbbo.tabEnroll.textIdNumber.setText(userID);
						pbbo.tabEnroll.textFirstName.setText(firstName);
						pbbo.tabEnroll.textLastName.setText(lastName);
						pbbo.tabbedPane.setSelectedComponent(pbbo.tabEnroll);
						return;
					}
				} else {
					DialogUtils.showErrorMessage("Add User", "User " + firstName + " " + lastName + " (ID " + userID
							+ ") is already enrolled.\nThe file won't be stored.");
					return;
				}
				break;
			case MorphoSmartSDK.MORPHOERR_DB_FULL:
				DialogUtils.showErrorMessage("Add User", "Cannot add more user.\nDatabase is full.");
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_USER_ID:
				DialogUtils.showErrorMessage("Add User",
						"The ID " + userID + " is already used for a user in the Database.");
				break;
			default:
				DialogUtils.showErrorMessage("Add User", "An error occured while calling MorphoUser.dbStore() function",
						ret, msoDemo.getMorphoDeviceInstance().getInternalError());
			}
		} else {
			msoDemo.getPanelDatabaseMgtInstance().loadUsers();
			msoDemo.updateDataBaseInformation();
		}
	}

	/**
	 * 
	 * @return
	 */
	public boolean isNoCheck() {
		return cbNoCheck.isSelected();
	}

	/**
	 *
	 */
	public void disableAllButons() {
		btnIdentify = btnIdentifyMatch.isEnabled();
		btnVerify = btnVerifyMatch.isEnabled();
		btnAdd = btnAddUser.isEnabled();
		btnUpdate = btnUpdateUser.isEnabled();
		btnRemove = btnRemoveAll.isEnabled();
		btnCreate = btnCreateBase.isEnabled();
		btnDestroy = btnDestroyBase.isEnabled();
		noCheck = cbNoCheck.isEnabled();

		btnIdentifyMatch.setEnabled(false);
		btnVerifyMatch.setEnabled(false);
		btnAddUser.setEnabled(false);
		btnUpdateUser.setEnabled(false);
		btnRemoveUser.setEnabled(false);
		btnRemoveAll.setEnabled(false);
		btnCreateBase.setEnabled(false);
		btnDestroyBase.setEnabled(false);
		cbNoCheck.setEnabled(false);
	}

	/**
	 *
	 */
	public void enableAllButtons() {
		btnIdentifyMatch.setEnabled(btnIdentify);
		btnVerifyMatch.setEnabled(btnVerify);
		btnAddUser.setEnabled(btnAdd);
		btnUpdateUser.setEnabled(btnUpdate);
		btnRemoveUser.setEnabled(btnRemove);
		btnRemoveAll.setEnabled(btnRemove);
		btnCreateBase.setEnabled(btnCreate);
		btnDestroyBase.setEnabled(btnDestroy);
		cbNoCheck.setEnabled(noCheck);
	}

}
