package morpho.morphosmart.sdk.demo.dialog;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ButtonGroup;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextField;
import javax.swing.border.EmptyBorder;

import morpho.morphosmart.sdk.api.MorphoDatabase;
import morpho.morphosmart.sdk.api.MorphoDevice;
import morpho.morphosmart.sdk.api.MorphoFieldAttribute;
import morpho.morphosmart.sdk.api.MorphoSmartSDK;
import morpho.morphosmart.sdk.api.MorphoTemplateType;

public class DialogCreateDB extends JDialog {

	private static final long serialVersionUID = 1L;
	private final JPanel contentPanel = new JPanel();
	private JTextField txtMaxnbofrecords;
	private JTextField txtNboffingers;
	private JRadioButton rbNo;
	private JRadioButton rbYes;
	private int sdkError = 0;
	private MorphoDevice mDevice;
	private MorphoDatabase mDatabase;

	/**
	 * Create the dialog.
	 */
	public DialogCreateDB(JFrame parentFrame) {
		super(parentFrame);
		setTitle("Create DataBase");
		setModalityType(ModalityType.DOCUMENT_MODAL);
		setResizable(false);
		setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		setSize(400, 220);
		setLocationRelativeTo(null);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);
		JLabel lblMaximumNumberOf = new JLabel("Maximum number of records");
		JLabel lblNumberOfFinger = new JLabel("Number of finger per record");
		JLabel lblEncryptDatabase = new JLabel("Encrypt database");
		txtMaxnbofrecords = new JTextField();
		txtMaxnbofrecords.setText("500");
		txtMaxnbofrecords.setColumns(10);
		txtNboffingers = new JTextField();
		txtNboffingers.setText("2");
		txtNboffingers.setColumns(10);

		rbNo = new JRadioButton("No");
		rbNo.setSelected(true);
		rbYes = new JRadioButton("Yes");

		ButtonGroup group = new ButtonGroup();
		group.add(rbYes);
		group.add(rbNo);

		GroupLayout gl_contentPanel = new GroupLayout(contentPanel);
		gl_contentPanel.setHorizontalGroup(
			gl_contentPanel.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_contentPanel.createSequentialGroup()
					.addContainerGap()
					.addGroup(gl_contentPanel.createParallelGroup(Alignment.LEADING)
						.addGroup(gl_contentPanel.createSequentialGroup()
							.addComponent(lblMaximumNumberOf)
							.addGap(18)
							.addComponent(txtMaxnbofrecords, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
						.addGroup(gl_contentPanel.createSequentialGroup()
							.addGroup(gl_contentPanel.createParallelGroup(Alignment.LEADING)
								.addComponent(lblNumberOfFinger)
								.addComponent(lblEncryptDatabase))
							.addGap(18)
							.addGroup(gl_contentPanel.createParallelGroup(Alignment.LEADING)
								.addGroup(gl_contentPanel.createSequentialGroup()
									.addComponent(rbNo)
									.addGap(18)
									.addComponent(rbYes))
								.addComponent(txtNboffingers, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))))
					.addContainerGap(172, Short.MAX_VALUE))
		);
		gl_contentPanel.setVerticalGroup(
			gl_contentPanel.createParallelGroup(Alignment.LEADING)
				.addGroup(Alignment.TRAILING, gl_contentPanel.createSequentialGroup()
					.addContainerGap(27, Short.MAX_VALUE)
					.addGroup(gl_contentPanel.createParallelGroup(Alignment.BASELINE)
						.addComponent(lblMaximumNumberOf)
						.addComponent(txtMaxnbofrecords, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
					.addGap(18)
					.addGroup(gl_contentPanel.createParallelGroup(Alignment.BASELINE)
						.addComponent(lblNumberOfFinger)
						.addComponent(txtNboffingers, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
					.addGap(18)
					.addGroup(gl_contentPanel.createParallelGroup(Alignment.BASELINE)
						.addComponent(lblEncryptDatabase)
						.addComponent(rbNo)
						.addComponent(rbYes))
					.addGap(23))
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
						sdkError = -1; //no base
						dispose();
					}
				});
			}
		}
	}

	private ActionListener btnOkAction = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			int[] fieldIndex = {0};
			sdkError = mDatabase.putField(MorphoFieldAttribute.MORPHO_PUBLIC_FIELD, 15, "First", fieldIndex);

			if(sdkError == MorphoSmartSDK.MORPHO_OK) {
				sdkError = mDatabase.putField(MorphoFieldAttribute.MORPHO_PUBLIC_FIELD, 15, "Last", fieldIndex);
			}
			if(sdkError != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("Create Database","An error occured while calling MorphoDatabase.putField() function",sdkError,mDevice.getInternalError());
				dispose();
			}

			long nbRecord = Integer.parseInt(txtMaxnbofrecords.getText());
			short nbFinger = Short.parseShort(txtNboffingers.getText());
			short dataEncryption = (short)((rbNo.isSelected() == true ? 0 : 1));

			sdkError = mDatabase.dbCreate(nbRecord, nbFinger, MorphoTemplateType.MORPHO_PK_COMP, (short) 0, dataEncryption);
			if(sdkError != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("Create Database","An error occured while calling MorphoDatabase.dbCreate() function",sdkError,mDevice.getInternalError());
			}
			dispose();
		}
	};

	public int getSdkError() {
		return sdkError;
	}

	public MorphoDevice getDevice() {
		return mDevice;
	}

	public void setDevice(MorphoDevice mDevice) {
		this.mDevice = mDevice;
	}

	public MorphoDatabase getDatabase() {
		return mDatabase;
	}

	public void setDatabase(MorphoDatabase mDatabase) {
		this.mDatabase = mDatabase;
	}
}
