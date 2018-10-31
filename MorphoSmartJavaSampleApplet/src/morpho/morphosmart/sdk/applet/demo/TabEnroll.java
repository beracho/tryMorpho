package morpho.morphosmart.sdk.applet.demo;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.Arrays;

import javax.swing.ButtonGroup;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.UIManager;
import javax.swing.border.TitledBorder;

import morpho.morphosmart.sdk.applet.ennum.EnumExportImage;
import morpho.morphosmart.sdk.applet.ennum.EnumFingerIndex;
import morpho.morphosmart.sdk.applet.ennum.TemplateFVPType;
import morpho.morphosmart.sdk.applet.ennum.TemplateType;
import morpho.morphosmart.sdk.applet.trt.MorphoComboBox;
import morpho.morphosmart.sdk.applet.trt.MorphoInfo;

public class TabEnroll extends JPanel {

	private static final long serialVersionUID = 1L;

	// UI variables
	JTextField textIdNumber;
	JTextField textFirstName;
	JTextField textLastName;
	JComboBox comboFvpTemplateType;
	JComboBox comboFpTemplateType;
	JComboBox comboExportImage;
	JComboBox comboFingerIndex;
	JRadioButton rbNbOfFinger1;
	JRadioButton rbNbOfFinger2;
	JCheckBox cbSavePkInDB;
	JCheckBox cbUpdateTemplate;

	/**
	 * Create the panel.
	 * @param isBaseAvailable 
	 * @param panelBasicBiometricOperation 
	 */
	public TabEnroll() {
		setLayout(new BorderLayout(0, 0));

		JTextArea txtrThisOperationAllows = new JTextArea();
		txtrThisOperationAllows.setEditable(false);
		txtrThisOperationAllows.setBackground(UIManager.getColor("Button.background"));
		String str = "  This operation allows you to add a new user in the database.\n"
				+ "  Fill the following fields and press \"Start\". The MSO will light\n"
				+ "  up to capture your fingers.";
		txtrThisOperationAllows.setText(str);
		add(txtrThisOperationAllows, BorderLayout.NORTH);
		
		JPanel panelCenter = new JPanel();
		panelCenter.setBorder(new TitledBorder(null, "New Enrollement", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		add(panelCenter, BorderLayout.CENTER);
		
		JLabel lblIdNumber = new JLabel("User ID");
		textIdNumber = new JTextField();
		textIdNumber.setColumns(12);
		JLabel lblFirstName = new JLabel("First Name");
		textFirstName = new JTextField();
		textFirstName.setColumns(12);
		JLabel lblLastName = new JLabel("Last Name");
		textLastName = new JTextField();
		textLastName.setColumns(12);
		JLabel lblNumberOfFingers = new JLabel("Number of fingers");
		rbNbOfFinger1 = new JRadioButton("1");
		rbNbOfFinger1.setSelected(true);
		rbNbOfFinger2 = new JRadioButton("2");
		// group radio buttons
		ButtonGroup group = new ButtonGroup();
		group.add(rbNbOfFinger1);
		group.add(rbNbOfFinger2);

		GroupLayout gl_panelCenter = new GroupLayout(panelCenter);
		gl_panelCenter.setHorizontalGroup(
			gl_panelCenter.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_panelCenter.createSequentialGroup()
					.addGroup(gl_panelCenter.createParallelGroup(Alignment.LEADING)
						.addGroup(gl_panelCenter.createSequentialGroup()
							.addGap(10)
							.addGroup(gl_panelCenter.createParallelGroup(Alignment.LEADING)
								.addComponent(lblIdNumber)
								.addComponent(lblFirstName)
								.addComponent(lblLastName))
							.addGap(17)
							.addGroup(gl_panelCenter.createParallelGroup(Alignment.LEADING)
								.addComponent(textIdNumber, Alignment.TRAILING, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
								.addComponent(textFirstName, Alignment.TRAILING, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
								.addComponent(textLastName, Alignment.TRAILING, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
						.addGroup(gl_panelCenter.createSequentialGroup()
							.addContainerGap()
							.addComponent(lblNumberOfFingers)
							.addPreferredGap(ComponentPlacement.UNRELATED)
							.addComponent(rbNbOfFinger1)
							.addGap(18)
							.addComponent(rbNbOfFinger2)))
					.addContainerGap(196, Short.MAX_VALUE))
		);
		gl_panelCenter.setVerticalGroup(
			gl_panelCenter.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_panelCenter.createSequentialGroup()
					.addGap(11)
					.addGroup(gl_panelCenter.createParallelGroup(Alignment.LEADING)
						.addGroup(gl_panelCenter.createSequentialGroup()
							.addGap(3)
							.addComponent(lblIdNumber))
						.addComponent(textIdNumber, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
					.addGap(6)
					.addGroup(gl_panelCenter.createParallelGroup(Alignment.LEADING)
						.addGroup(gl_panelCenter.createSequentialGroup()
							.addGap(3)
							.addComponent(lblFirstName))
						.addComponent(textFirstName, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
					.addGap(6)
					.addGroup(gl_panelCenter.createParallelGroup(Alignment.LEADING)
						.addComponent(lblLastName)
						.addComponent(textLastName, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
					.addPreferredGap(ComponentPlacement.UNRELATED)
					.addGroup(gl_panelCenter.createParallelGroup(Alignment.BASELINE)
						.addComponent(lblNumberOfFingers)
						.addComponent(rbNbOfFinger1)
						.addComponent(rbNbOfFinger2))
					.addContainerGap(303, Short.MAX_VALUE))
		);
		panelCenter.setLayout(gl_panelCenter);


		JPanel panelSouth = new JPanel();
		add(panelSouth, BorderLayout.SOUTH);
		
		cbUpdateTemplate = new JCheckBox("Update template");
		cbSavePkInDB = new JCheckBox("Save PK in DataBase");
		cbSavePkInDB.setSelected(true);
		JLabel lblFingerIndex = new JLabel("Finger index");
		comboFingerIndex = new MorphoComboBox(EnumFingerIndex.toStringArray());
		comboFingerIndex.setEnabled(false);
		JLabel lblExportImage = new JLabel("Export image");
		comboExportImage = new MorphoComboBox(EnumExportImage.toStringArray());
		comboExportImage.setSelectedItem(EnumExportImage.NO_IMAGE.toString());
		JLabel lblFpTemplateType = new JLabel("FP Template Type");
		// Feed combo box with data from enum
		String[] templateData = new String[TemplateType.values().length];
		int i = 0;
		for (TemplateType value : TemplateType.values()) {
			templateData[i++] = value.getLabel();
		}
		Arrays.sort(templateData);
		comboFpTemplateType = new MorphoComboBox(templateData);
		comboFpTemplateType.setSelectedItem(TemplateType.MORPHO_NO_PK_FP.getLabel());
		JLabel lblFvpTemplateType = new JLabel("FVP Template Type");
		// Feed combo box with data from enum
		templateData = new String[2];
		templateData[0] = TemplateFVPType.MORPHO_NO_PK_FVP.getLabel();
		templateData[1] = TemplateFVPType.MORPHO_PK_FVP.getLabel();
		comboFvpTemplateType = new MorphoComboBox(templateData);
		// enable ONLY if a FVP device is connected
		if(!MorphoInfo.isFVP)
			comboFvpTemplateType.setEnabled(false);
		

		GroupLayout gl_panelSouth = new GroupLayout(panelSouth);
		gl_panelSouth.setHorizontalGroup(
			gl_panelSouth.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_panelSouth.createSequentialGroup()
					.addContainerGap()
					.addGroup(gl_panelSouth.createParallelGroup(Alignment.LEADING)
						.addComponent(lblFingerIndex)
						.addComponent(cbUpdateTemplate)
						.addComponent(lblExportImage)
						.addComponent(lblFpTemplateType)
						.addComponent(lblFvpTemplateType))
					.addGap(18)
					.addGroup(gl_panelSouth.createParallelGroup(Alignment.LEADING, false)
						.addComponent(comboFvpTemplateType, 0, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
						.addComponent(comboExportImage, 0, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
						.addComponent(cbSavePkInDB)
						.addComponent(comboFpTemplateType, 0, 0, Short.MAX_VALUE)
						.addComponent(comboFingerIndex, 0, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
					.addContainerGap(96, Short.MAX_VALUE))
		);
		gl_panelSouth.setVerticalGroup(
			gl_panelSouth.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_panelSouth.createSequentialGroup()
					.addContainerGap()
					.addGroup(gl_panelSouth.createParallelGroup(Alignment.BASELINE)
						.addComponent(cbUpdateTemplate)
						.addComponent(cbSavePkInDB))
					.addPreferredGap(ComponentPlacement.RELATED, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
					.addGroup(gl_panelSouth.createParallelGroup(Alignment.BASELINE)
						.addComponent(lblFingerIndex)
						.addComponent(comboFingerIndex, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
					.addPreferredGap(ComponentPlacement.RELATED)
					.addGroup(gl_panelSouth.createParallelGroup(Alignment.BASELINE)
						.addComponent(lblExportImage)
						.addComponent(comboExportImage, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
					.addPreferredGap(ComponentPlacement.RELATED)
					.addGroup(gl_panelSouth.createParallelGroup(Alignment.BASELINE)
						.addComponent(lblFpTemplateType)
						.addComponent(comboFpTemplateType, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
					.addPreferredGap(ComponentPlacement.RELATED)
					.addGroup(gl_panelSouth.createParallelGroup(Alignment.BASELINE)
						.addComponent(lblFvpTemplateType)
						.addComponent(comboFvpTemplateType, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)))
		);
		panelSouth.setLayout(gl_panelSouth);

		// some actions
		cbUpdateTemplate.addItemListener(new ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent event) {
				if(event.getStateChange() == ItemEvent.SELECTED && rbNbOfFinger1.isSelected())
					comboFingerIndex.setEnabled(true);
				else
					comboFingerIndex.setEnabled(false);
			}
		});
		
		rbNbOfFinger1.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent ae) {
				if (cbUpdateTemplate.isSelected()) {
					if (rbNbOfFinger1.isSelected())
						comboFingerIndex.setEnabled(true);
					else
						comboFingerIndex.setEnabled(false);
				}
			}
		});
		
		rbNbOfFinger2.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent ae) {
				if (cbUpdateTemplate.isSelected()) {
					if (rbNbOfFinger2.isSelected())
						comboFingerIndex.setEnabled(false);
					else
						comboFingerIndex.setEnabled(true);
				}
			}
		});		
	}

}
