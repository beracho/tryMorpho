package morpho.morphosmart.sdk.applet.demo;

import java.awt.BorderLayout;
import java.util.ArrayList;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.border.EmptyBorder;
import javax.swing.table.DefaultTableModel;

import morpho.morphosmart.sdk.api.MorphoDatabase;
import morpho.morphosmart.sdk.api.MorphoDevice;
import morpho.morphosmart.sdk.api.MorphoSmartSDK;
import morpho.morphosmart.sdk.api.MorphoUser;
import morpho.morphosmart.sdk.api.MorphoUserList;
import morpho.morphosmart.sdk.applet.trt.DialogUtils;

public class RightPanel extends JPanel {

	private static final long serialVersionUID = 1L;
	private JLabel lbldeviceName;
	private JLabel lblserialNumber;
	protected JTable dbTable;
	private MorphoDevice mDevice = null;
	private MorphoDatabase mDatabase = null;

	/**
	 * Create the panel.
	 * @param msoDemo 
	 */
	public RightPanel(MsoDemo msoDemo) {
		mDatabase  = msoDemo.mDatabase;
		mDevice = msoDemo.mDevice;
		setLayout(new BorderLayout(0, 0));

		JPanel panelNorth = new JPanel();
		add(panelNorth, BorderLayout.NORTH);
		panelNorth.setLayout(new BorderLayout(0, 0));

		lbldeviceName = new JLabel("Device Name: ");
		lbldeviceName.setBorder(new EmptyBorder(10, 10, 10, 10));
		panelNorth.add(lbldeviceName, BorderLayout.NORTH);

		lblserialNumber = new JLabel("Serial Number: ");
		lblserialNumber.setBorder(new EmptyBorder(10, 10, 10, 10));
		panelNorth.add(lblserialNumber, BorderLayout.SOUTH);

		// table
		JScrollPane scrollPane = new JScrollPane();
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
		
		add(scrollPane, BorderLayout.CENTER);
	}

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
	
	public void setDeviceName(String deviceName, boolean isTUNNELING, boolean isOFFERED_SECURITY)
	{
		String sc = "Device Name: " + deviceName;
		if(isTUNNELING)
		{
			sc += "<br/>MSO configuration : [Tunneling]";
		}
		else if(isOFFERED_SECURITY)
		{
			sc += "<br/>MSO configuration : [Offered]";
		}
		
		sc = DialogUtils.formatText(sc);
		
		lbldeviceName.setText(sc);
	}
	
	public void setDeviceSerailNumber(String serialNumber)
	{
		lblserialNumber.setText("Serial Number: " + serialNumber);
	}
	
	public String getSelectedUserId() {
		DefaultTableModel dbModel = (DefaultTableModel) dbTable.getModel();
		int index = dbTable.getSelectedRow();
		if(index != -1){
			return (String)dbModel.getValueAt(index, 0);
		}
		else
		{
			return "";
		}
	}
	
	public void loadUsers() {
		DefaultTableModel dbModel = (DefaultTableModel) dbTable.getModel();
		dbModel.getDataVector().removeAllElements();
		dbModel.fireTableDataChanged();		

		int[] nbUsedRecord = {0};
		mDatabase.getNbUsedRecord(nbUsedRecord);
		if(nbUsedRecord[0] != 0) {
			byte[] fieldIndexDescriptor = {0,0,0};

			int ret = mDatabase.fillIndexDescriptor(true, (short) 0, fieldIndexDescriptor);

			if(ret == MorphoSmartSDK.MORPHO_OK) {
				ret = mDatabase.fillIndexDescriptor(false, (short) 1, fieldIndexDescriptor);
			}

			if(ret == MorphoSmartSDK.MORPHO_OK) {
				ret = mDatabase.fillIndexDescriptor(false, (short) 2, fieldIndexDescriptor);
			}

			if(ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("Update User","An error occured while calling MorphoDatabase.fillIndexDescriptor() function",ret,mDevice.getInternalError());
				return;
			}

			MorphoUserList mUserList = new MorphoUserList();
			ret = mDatabase.readPublicFields(fieldIndexDescriptor, mUserList);

			if(ret != MorphoSmartSDK.MORPHO_OK &&
					ret != MorphoSmartSDK.MORPHOERR_USER_NOT_FOUND &&
					ret != MorphoSmartSDK.MORPHOERR_FIELD_NOT_FOUND) {
				DialogUtils.showErrorMessage("Update User","An error occured while calling MorphoDatabase.readPublicFields() function",ret,mDevice.getInternalError());
				return;
			}

			int[] nbUser = {0};
			ret = mUserList.getNbUser(nbUser);
			if(ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("Update User",
						"An error occured while calling MorphoUserList.getNbUser() function",
						ret,
						mDevice.getInternalError());
				return;
			}

			for (int i = 0; i < nbUser[0]; ++i) {
				MorphoUser mUser = mUserList.getMorphoUser(i);

				if (mUser != null) {
					ArrayList<String> userID = new ArrayList<String>();
					ArrayList<String> firstName = new ArrayList<String>();
					ArrayList<String> lastName = new ArrayList<String>();

					ret = mUser.getField(0, userID);
					if (ret == MorphoSmartSDK.MORPHOERR_FIELD_NOT_FOUND) {
						userID.clear();
						userID.add("");
					} else if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage("Update User","An error occured while calling MorphoUser.getField() function",ret,mDevice.getInternalError());
						return;
					}

					ret = mUser.getField(1, firstName);
					if (ret == MorphoSmartSDK.MORPHOERR_FIELD_NOT_FOUND) {
						firstName.clear();
						firstName.add("");
					} else if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage("Update User","An error occured while calling MorphoUser.getField() function",ret,mDevice.getInternalError());
						return;
					}

					ret = mUser.getField(2, lastName);
					if (ret == MorphoSmartSDK.MORPHOERR_FIELD_NOT_FOUND) {
						lastName.clear();
						lastName.add("");
					} else if (ret != MorphoSmartSDK.MORPHO_OK) {
						DialogUtils.showErrorMessage("Update User","An error occured while calling MorphoUser.getField() function",ret,mDevice.getInternalError());
						return;
					}

					addTableRow(userID.get(0),firstName.get(0),lastName.get(0));
				}
			}
		}
	}
}
