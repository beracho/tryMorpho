package morpho.morphosmart.sdk.demo;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
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
import javax.swing.SwingConstants;
import javax.swing.UIManager;
import javax.swing.border.EmptyBorder;
import javax.swing.border.TitledBorder;

import morpho.morphosmart.sdk.demo.ennum.TemplateFVPType;
import morpho.morphosmart.sdk.demo.ennum.TemplateType;
import morpho.morphosmart.sdk.demo.trt.MorphoComboBox;
import morpho.morphosmart.sdk.demo.trt.MorphoInfo;

public class TabCapture extends JPanel {
	private static final long serialVersionUID = 1L;

	// UI variables
	JTextField textID;
	JTextField textFirstName;
	JTextField txtLastname;
	JRadioButton rbOneFinger;
	JRadioButton rbTwoFinger;
	JRadioButton rbCaptureTypeEnroll;
	JRadioButton rbCaptureTypeVerif;
	JCheckBox cbLatentDetect;
	JCheckBox cbEmbedInToken;
	JComboBox comboFpTemplateType;
	JComboBox comboFvpTemplateType;

	protected PanelBasicBiometricOperation panelBasicBiometricOperation;

	/**
	 * Create the panel.
	 * 
	 * @param panelBasicBiometricOperation
	 */
	public TabCapture(PanelBasicBiometricOperation panelBasicBiometricOperation) {
		this.panelBasicBiometricOperation = panelBasicBiometricOperation;
		setLayout(new BorderLayout(0, 0));

		JTextArea lblTitle = new JTextArea("This commands extracts the features of 1 or 2 fingers\nand exports the templates in order to store them on an\noutput support.");
		lblTitle.setRows(3);
		lblTitle.setEditable(false);
		lblTitle.setBackground(UIManager.getColor("Panel.background"));
		lblTitle.setFont(new Font("Tahoma", Font.PLAIN, 11));
		lblTitle.setBorder(new EmptyBorder(10, 10, 10, 10));
		add(lblTitle, BorderLayout.NORTH);

		JPanel panel = new JPanel();
		panel.setBorder(new TitledBorder(null, "Code Export", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		add(panel, BorderLayout.CENTER);

		JLabel lblId = new JLabel("User ID");
		JLabel lblNewLabel = new JLabel("First Name");
		JLabel lblLastName = new JLabel("Last Name");
		textID = new JTextField();
		textID.setColumns(10);
		textFirstName = new JTextField();
		textFirstName.setColumns(10);
		txtLastname = new JTextField();
		txtLastname.setColumns(10);

		JLabel lblNumberOfFingers = new JLabel("Number of fingers");
		rbOneFinger = new JRadioButton("1");
		rbOneFinger.setSelected(true);
		rbTwoFinger = new JRadioButton("2");
		// Group the radio buttons.
		ButtonGroup groupFingers = new ButtonGroup();
		groupFingers.add(rbOneFinger);
		groupFingers.add(rbTwoFinger);

		JLabel lblCaptureType = new JLabel("Capture type");
		rbCaptureTypeEnroll = new JRadioButton("Enroll");
		rbCaptureTypeEnroll.setSelected(true);
		rbCaptureTypeVerif = new JRadioButton("Verif");
		// Group the radio buttons.
		ButtonGroup groupEnroll = new ButtonGroup();
		groupEnroll.add(rbCaptureTypeEnroll);
		groupEnroll.add(rbCaptureTypeVerif);
		cbLatentDetect = new JCheckBox("Latent Detect.");
		cbLatentDetect.setEnabled(false);

		GroupLayout gl_panel = new GroupLayout(panel);
		gl_panel.setHorizontalGroup(gl_panel.createParallelGroup(Alignment.LEADING).addGroup(
				gl_panel.createSequentialGroup()
						.addContainerGap()
						.addGroup(
								gl_panel.createParallelGroup(Alignment.LEADING)
										.addGroup(
												gl_panel.createSequentialGroup().addGroup(gl_panel.createParallelGroup(Alignment.LEADING).addGroup(gl_panel.createSequentialGroup().addComponent(textID, GroupLayout.PREFERRED_SIZE, 60, GroupLayout.PREFERRED_SIZE).addGap(18).addComponent(textFirstName, GroupLayout.PREFERRED_SIZE, 69, GroupLayout.PREFERRED_SIZE)).addGroup(gl_panel.createSequentialGroup().addComponent(lblId).addGap(18).addComponent(lblNewLabel))).addGap(18)
														.addGroup(gl_panel.createParallelGroup(Alignment.LEADING, false).addComponent(txtLastname, 0, 0, Short.MAX_VALUE).addComponent(lblLastName, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
										.addGroup(
												gl_panel.createSequentialGroup().addGroup(gl_panel.createParallelGroup(Alignment.LEADING, false).addGroup(gl_panel.createSequentialGroup().addComponent(lblNumberOfFingers).addPreferredGap(ComponentPlacement.RELATED, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addComponent(rbOneFinger)).addGroup(gl_panel.createSequentialGroup().addComponent(lblCaptureType).addGap(18).addComponent(rbCaptureTypeEnroll))).addPreferredGap(ComponentPlacement.RELATED)
														.addGroup(gl_panel.createParallelGroup(Alignment.TRAILING).addComponent(rbCaptureTypeVerif).addComponent(rbTwoFinger)).addPreferredGap(ComponentPlacement.UNRELATED).addComponent(cbLatentDetect))).addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)));
		gl_panel.setVerticalGroup(gl_panel.createParallelGroup(Alignment.LEADING).addGroup(
				gl_panel.createSequentialGroup().addContainerGap().addGroup(gl_panel.createParallelGroup(Alignment.BASELINE).addComponent(lblId).addComponent(lblNewLabel).addComponent(lblLastName)).addPreferredGap(ComponentPlacement.RELATED)
						.addGroup(gl_panel.createParallelGroup(Alignment.BASELINE).addComponent(textID, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).addComponent(textFirstName, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).addComponent(txtLastname, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)).addPreferredGap(ComponentPlacement.RELATED)
						.addGroup(gl_panel.createParallelGroup(Alignment.BASELINE).addComponent(lblNumberOfFingers).addComponent(rbOneFinger).addComponent(rbTwoFinger)).addPreferredGap(ComponentPlacement.RELATED).addGroup(gl_panel.createParallelGroup(Alignment.BASELINE).addComponent(lblCaptureType).addComponent(rbCaptureTypeEnroll).addComponent(rbCaptureTypeVerif).addComponent(cbLatentDetect)).addContainerGap(232, Short.MAX_VALUE)));
		gl_panel.linkSize(SwingConstants.HORIZONTAL, new Component[] { textFirstName, txtLastname });
		gl_panel.linkSize(SwingConstants.HORIZONTAL, new Component[] { lblId, textID });
		panel.setLayout(gl_panel);

		JPanel panel_1 = new JPanel();
		panel_1.setBorder(new TitledBorder(null, "Output", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		add(panel_1, BorderLayout.SOUTH);
		cbEmbedInToken = new JCheckBox("Embed in Token Bio Envelope");
		// not available on Linux
		if (!System.getProperty("os.name").startsWith("Windows")) {
			cbEmbedInToken.setEnabled(false);
		}
		JLabel lblFpTemplate = new JLabel("FP Template Type");
		// Feed combo box with data from enum
		String[] templateData = new String[TemplateType.values().length];
		int i = 0;
		for (TemplateType value : TemplateType.values()) {
			templateData[i++] = value.getLabel();
		}

		Arrays.sort(templateData);

		comboFpTemplateType = new MorphoComboBox(templateData);
		comboFpTemplateType.setSelectedItem(TemplateType.MORPHO_NO_PK_FP.getLabel());
		comboFpTemplateType.setMaximumSize(new Dimension(48, 200));

		JLabel lblFvpTemplateType = new JLabel("FVP Template Type");
		// Feed combo box with data from enum
		templateData = new String[2];
		templateData[0] = TemplateFVPType.MORPHO_NO_PK_FVP.getLabel();
		templateData[1] = TemplateFVPType.MORPHO_PK_FVP.getLabel();
		comboFvpTemplateType = new MorphoComboBox(templateData);
		comboFvpTemplateType.setSelectedItem(TemplateFVPType.MORPHO_PK_FVP.getLabel());
		// enable the combo if a FVP device is connected
		if (!MorphoInfo.isFVP)
		{
			comboFvpTemplateType.setEnabled(false);
			comboFvpTemplateType.setSelectedItem(TemplateFVPType.MORPHO_NO_PK_FVP.getLabel());
			comboFpTemplateType.setSelectedItem(TemplateType.MORPHO_PK_COMP.getLabel());
		}

		GroupLayout gl_panel_1 = new GroupLayout(panel_1);
		gl_panel_1.setHorizontalGroup(gl_panel_1.createParallelGroup(Alignment.LEADING)
				.addGroup(
						gl_panel_1
								.createSequentialGroup()
								.addContainerGap()
								.addGroup(
										gl_panel_1
												.createParallelGroup(Alignment.LEADING)
												.addComponent(cbEmbedInToken)
												.addGroup(
														gl_panel_1.createSequentialGroup().addGroup(gl_panel_1.createParallelGroup(Alignment.LEADING).addComponent(comboFpTemplateType, GroupLayout.PREFERRED_SIZE, 180, GroupLayout.PREFERRED_SIZE).addComponent(lblFpTemplate)).addPreferredGap(ComponentPlacement.RELATED)
																.addGroup(gl_panel_1.createParallelGroup(Alignment.LEADING).addComponent(comboFvpTemplateType, GroupLayout.PREFERRED_SIZE, 135, GroupLayout.PREFERRED_SIZE).addComponent(lblFvpTemplateType)))).addContainerGap(107, Short.MAX_VALUE)));
		gl_panel_1.setVerticalGroup(gl_panel_1.createParallelGroup(Alignment.LEADING).addGroup(
				gl_panel_1.createSequentialGroup().addComponent(cbEmbedInToken).addGap(7).addGroup(gl_panel_1.createParallelGroup(Alignment.BASELINE).addComponent(lblFpTemplate).addComponent(lblFvpTemplateType)).addPreferredGap(ComponentPlacement.RELATED)
						.addGroup(gl_panel_1.createParallelGroup(Alignment.BASELINE).addComponent(comboFpTemplateType, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).addComponent(comboFvpTemplateType, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)).addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)));
		panel_1.setLayout(gl_panel_1);

		//
		// Actions
		//
		rbCaptureTypeVerif.addItemListener(new ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent event) {
				if (event.getStateChange() == ItemEvent.SELECTED) {
					cbLatentDetect.setEnabled(true);
					cbLatentDetect.setSelected(true);
				} else {
					cbLatentDetect.setEnabled(false);
					cbLatentDetect.setSelected(false);
				}
			}
		});

		comboFpTemplateType.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent ae) {
				String selectedTemplateType = (String) comboFpTemplateType.getSelectedItem();
				if (selectedTemplateType.equals(TemplateType.MORPHO_PK_SAGEM_PKS.getLabel())) {
					cbEmbedInToken.setSelected(false);
					cbEmbedInToken.setEnabled(false);
				} else {
					cbEmbedInToken.setEnabled(true);
				}
			}
		});

		rbCaptureTypeEnroll.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent ae) {
				if (rbCaptureTypeEnroll.isSelected()) {
					comboFvpTemplateType.removeAllItems();
					comboFvpTemplateType.addItem(TemplateFVPType.MORPHO_NO_PK_FVP.getLabel());
					comboFvpTemplateType.addItem(TemplateFVPType.MORPHO_PK_FVP.getLabel());
					comboFvpTemplateType.setSelectedItem(TemplateFVPType.MORPHO_PK_FVP.getLabel());
				}
			}
		});

		rbCaptureTypeVerif.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent ae) {
				if (rbCaptureTypeVerif.isSelected()) {
					comboFvpTemplateType.removeAllItems();
					comboFvpTemplateType.addItem(TemplateFVPType.MORPHO_NO_PK_FVP.getLabel());
					comboFvpTemplateType.addItem(TemplateFVPType.MORPHO_PK_FVP_MATCH.getLabel());
				}
			}
		});

	}
}
