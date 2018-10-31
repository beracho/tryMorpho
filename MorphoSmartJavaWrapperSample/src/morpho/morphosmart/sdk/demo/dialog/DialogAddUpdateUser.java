package morpho.morphosmart.sdk.demo.dialog;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.SwingConstants;
import javax.swing.border.EmptyBorder;

import morpho.morphosmart.sdk.api.MorphoDatabase;
import morpho.morphosmart.sdk.api.MorphoSmartSDK;
import morpho.morphosmart.sdk.api.MorphoUser;
import morpho.morphosmart.sdk.api.MorphoDevice.MorphoDevicePrivacyModeStatus;
import morpho.morphosmart.sdk.demo.MsoDemo;

public class DialogAddUpdateUser extends JDialog {

	private static final long serialVersionUID = 1L;

	private final JPanel contentPanel = new JPanel();
	private JTextField txtIdNumber = new JTextField();
	private JTextField txtFirstName = new JTextField();
	private JTextField txtLastName = new JTextField();
	private boolean isAddUser = false;
	protected MsoDemo msoDemo;
	private ArrayList<String> templatesFiles = new ArrayList<String>();
	private boolean isOK = false;

	private String userID = "";
	private String firstName = "";
	private String lastName = "";

	/**
	 *
	 * Create the dialog.
	 *
	 * @param parentFrame
	 * @param title should be 'Add' or 'Update'
	 * @param userId
	 * @param lastName
	 * @param firstName
	 */
	public DialogAddUpdateUser(MsoDemo msoDemo, String title, String userId, String firstName, String lastName) {
		super(msoDemo);
		this.msoDemo = msoDemo;
		if(title == "Add"){
			isAddUser = true; // add user
		} else {
			isAddUser = false; // update user
		}

		setTitle(title + " User");
		setModalityType(ModalityType.DOCUMENT_MODAL);
		setResizable(false);
		setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		setSize(260, 230);
		setLocationRelativeTo(null);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);

		JLabel lblId = new JLabel("ID Number");
		JLabel lblFirstName = new JLabel("First Name");
		JLabel lblLastName = new JLabel("Last Name");
		txtIdNumber.setColumns(8);
		txtFirstName.setColumns(8);
		txtLastName.setColumns(8);

		GroupLayout gl_contentPanel = new GroupLayout(contentPanel);
		gl_contentPanel.setHorizontalGroup(
			gl_contentPanel.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_contentPanel.createSequentialGroup()
					.addContainerGap()
					.addGroup(gl_contentPanel.createParallelGroup(Alignment.TRAILING)
						.addGroup(gl_contentPanel.createParallelGroup(Alignment.LEADING)
							.addComponent(lblId)
							.addComponent(lblFirstName))
						.addComponent(lblLastName))
					.addPreferredGap(ComponentPlacement.UNRELATED)
					.addGroup(gl_contentPanel.createParallelGroup(Alignment.LEADING)
						.addComponent(txtIdNumber, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
						.addComponent(txtFirstName, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
						.addComponent(txtLastName, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
					.addContainerGap(17, Short.MAX_VALUE))
		);

		gl_contentPanel.setVerticalGroup(
			gl_contentPanel.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_contentPanel.createSequentialGroup()
					.addGap(20)
					.addGroup(gl_contentPanel.createParallelGroup(Alignment.BASELINE)
						.addComponent(lblId)
						.addComponent(txtIdNumber, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
					.addPreferredGap(ComponentPlacement.UNRELATED)
					.addGroup(gl_contentPanel.createParallelGroup(Alignment.BASELINE)
						.addComponent(lblFirstName)
						.addComponent(txtFirstName, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
					.addPreferredGap(ComponentPlacement.UNRELATED)
					.addGroup(gl_contentPanel.createParallelGroup(Alignment.BASELINE)
						.addComponent(txtLastName, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
						.addComponent(lblLastName))
					.addContainerGap(18, Short.MAX_VALUE))
		);

		contentPanel.setLayout(gl_contentPanel);
		{
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
			{
				JButton okButton = new JButton("OK");
				okButton.setActionCommand("OK");
				buttonPane.add(okButton);
				getRootPane().setDefaultButton(okButton);
				okButton.addActionListener(btnOkAction);
			}
			{
				JButton cancelButton = new JButton("Cancel");
				cancelButton.setActionCommand("Cancel");
				buttonPane.add(cancelButton);
				cancelButton.addActionListener(new ActionListener() {

					@Override
					public void actionPerformed(ActionEvent e) {
						dispose();
					}
				});
			}
		}
		{
			JLabel label = new JLabel("\nType the ID of the user to " + title + "\n");
			label.setBorder(new EmptyBorder(15,0,0,0));
			label.setHorizontalAlignment(SwingConstants.CENTER);
			if ("Update".equals(title)) {
				txtIdNumber.setEnabled(false);
			}
			getContentPane().add(label, BorderLayout.NORTH);
		}

		txtIdNumber.setText(userId);
		txtFirstName.setText(firstName);
		txtLastName.setText(lastName);

		setUserID(userId);
		setFirstName(firstName);
		setLastName(lastName);
	}

	private ActionListener btnOkAction = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			if(isAddUser) {
				isOK = true;
				setUserID(txtIdNumber.getText());
				setFirstName(txtFirstName.getText());
				setLastName(txtLastName.getText());
				dispose();
			} else {
				updateUser();
			}
		}
	};

	/**
	 *
	 */
	private void updateUser() {

		MorphoDatabase mDatabase = msoDemo.getMorphoDatabaseInstance();
		MorphoUser mUser = new MorphoUser();

		int ret;
		if (msoDemo.PrivacyModeStatus != MorphoDevicePrivacyModeStatus.PRIVACY_MODE_DISABLED) {

			byte[] privacyKey = msoDemo.getPrivacyModeKey();
			if (null != privacyKey) {

				byte[] random = new byte[4];
				new Random().nextBytes(random);

				byte[] firstNameFieldDecrypted = new byte[8 + txtFirstName.getText().getBytes().length];
				byte[] lastNameFieldDecrypted = new byte[8 + txtLastName.getText().getBytes().length];

				// first name decrypted: index (4 bit) + txtFirstName lenght (4 bit)  +  txtFirstName data
				
				byte[] indexFirstname = { 1, 0, 0, 0 }; // firstname index

				
				System.arraycopy(indexFirstname, 0, firstNameFieldDecrypted, 0, 4);

				byte[] firstNameLength = msoDemo.LongToUnsignedFourBytesBuffer(txtFirstName.getText().getBytes().length,true);

				System.arraycopy(firstNameLength, 0, firstNameFieldDecrypted, 4, 4);
				System.arraycopy(txtFirstName.getText().getBytes(), 0, firstNameFieldDecrypted, 8,
						txtFirstName.getText().getBytes().length);

				
				// last name decrypted: index (4 bit) + txtLastName lenght (4 bit)  +  txtLastName data
				
				
				byte[] indexLastname = { 2, 0, 0, 0 }; // lastname index

				System.arraycopy(indexLastname, 0, lastNameFieldDecrypted, 0, 4);

				byte[] lastNameLength = msoDemo.LongToUnsignedFourBytesBuffer(txtLastName.getText().getBytes().length,true);

				System.arraycopy(lastNameLength, 0, lastNameFieldDecrypted, 4, 4);
				System.arraycopy(txtLastName.getText().getBytes(), 0, lastNameFieldDecrypted, 8,
						txtLastName.getText().getBytes().length);

				byte[] cryptedtxtIdNumber = msoDemo.encryptPrivacyData(txtIdNumber.getText().getBytes(), privacyKey,
						random);
				byte[] cryptedtxtFirstName = msoDemo.encryptPrivacyData(firstNameFieldDecrypted, privacyKey, random);
				byte[] cryptedtxtLastName = msoDemo.encryptPrivacyData(lastNameFieldDecrypted, privacyKey, random);

				ret = mDatabase.getUserBuffer(cryptedtxtIdNumber, mUser);
				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils.showErrorMessage("Update User",
							"An error occured while calling MorphoDatabase.getUserBuffer() function", ret,
							this.msoDemo.getMorphoDeviceInstance().getInternalError());
					dispose();
				}
				// Firstname

				ret = mUser.putBufferField(1, cryptedtxtFirstName);
				if (ret == MorphoSmartSDK.MORPHO_OK) {
					// Lastname
					ret = mUser.putBufferField(2, cryptedtxtLastName);
				}
				if (ret != MorphoSmartSDK.MORPHO_OK) {
					DialogUtils.showErrorMessage("Update User",
							"An error occured while calling MorphoUser.putBufferField() function", ret,
							this.msoDemo.getMorphoDeviceInstance().getInternalError());
					dispose();
				}
			} else {
				DialogUtils.showErrorMessage("updateUser", "Invalid Kprivacy key");
				dispose();

			}
		} else {

			ret = mDatabase.getUser(txtIdNumber.getText(), mUser);
			if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("Update User",
						"An error occured while calling MorphoDatabase.getUser() function", ret,
						this.msoDemo.getMorphoDeviceInstance().getInternalError());
				dispose();
			}
			// Firstname
			ret = mUser.putField(1, txtFirstName.getText());
			if (ret == MorphoSmartSDK.MORPHO_OK) {
				// Lastname
				ret = mUser.putField(2, txtLastName.getText());
			}
			if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("Update User",
						"An error occured while calling MorphoUser.putField() function", ret,
						this.msoDemo.getMorphoDeviceInstance().getInternalError());
				dispose();
			}
		}
		ret = mUser.dbUpdatePublicFields();
		if (ret != MorphoSmartSDK.MORPHO_OK) {
			DialogUtils.showErrorMessage("Update User",
					"An error occured while calling MorphoUser.dbUpdatePublicFields() function", ret,
					this.msoDemo.getMorphoDeviceInstance().getInternalError());
			dispose();
		} else {
			this.msoDemo.getPanelDatabaseMgtInstance().loadUsers();
		}

		dispose();
	}

	/**
	 *
	 * @param templateFile
	 */
	public void addTemplateFile(String templateFile) {
		templatesFiles.add(templateFile);
	}

	public boolean isOK() {
		return isOK;
	}

	public String getUserID() {
		return userID;
	}

	public void setUserID(String userID) {
		this.userID = userID;
	}

	public String getFirstName() {
		return firstName;
	}

	public void setFirstName(String firstName) {
		this.firstName = firstName;
	}

	public String getLastName() {
		return lastName;
	}

	public void setLastName(String lastName) {
		this.lastName = lastName;
	}
}
