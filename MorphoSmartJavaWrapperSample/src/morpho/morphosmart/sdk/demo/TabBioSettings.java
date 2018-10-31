package morpho.morphosmart.sdk.demo;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Locale;

import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.SwingConstants;

import morpho.morphosmart.sdk.demo.ennum.Coder;
import morpho.morphosmart.sdk.demo.ennum.EnumAcquisitionStrategy;
import morpho.morphosmart.sdk.demo.ennum.EnumMatchingStrategy;
import morpho.morphosmart.sdk.demo.ennum.EnumSecurityLevelFVP;
import morpho.morphosmart.sdk.demo.ennum.EnumSecurityLevelMSO;
import morpho.morphosmart.sdk.demo.trt.MorphoComboBox;
import morpho.morphosmart.sdk.demo.trt.MorphoInfo;

public class TabBioSettings extends JPanel {

	private static final long serialVersionUID = 1L;

	protected PanelDatabaseMgt panelDatabaseMgt;
	// UI variables
	JFormattedTextField txtMatchingThreshold;
	JFormattedTextField txtTimeout;
	JComboBox comboCoderChoice;
	JComboBox comboSecurityLevel;
	JComboBox comboMatchingStrategy;
	JComboBox comboAcquisitionStrategy;
	JCheckBox cbForceFingerPlacement;
	JCheckBox cbAdvancedSecurityLevel;
	JCheckBox cbFingerQualityThreshold;
	JFormattedTextField txtFingerQualityThreshold;

	/**
	 * Create the panel.
	 * 
	 * @param panelDatabaseMgt
	 */
	public TabBioSettings(PanelDatabaseMgt panelDatabaseMgt) {
		this.panelDatabaseMgt = panelDatabaseMgt;

		JLabel lblMatchingThreshold = new JLabel("Matching Threshold (0 - 10)");
		NumberFormat format = NumberFormat.getIntegerInstance();
		format.setMaximumFractionDigits(0);
		format.setMinimumIntegerDigits(0);
		format.setMaximumIntegerDigits(10);
		txtMatchingThreshold = new JFormattedTextField(format);
		txtMatchingThreshold.setColumns(3);
		txtMatchingThreshold.setText("5");

		JLabel lblTimeoutsec = new JLabel("Timeout (sec)");
		NumberFormat format2 = NumberFormat.getIntegerInstance();
		format2.setMinimumIntegerDigits(0);
		txtTimeout = new JFormattedTextField(format2);
		txtTimeout.setColumns(3);
		txtTimeout.setText("0");

		JLabel lblCoderChoice = new JLabel("Coder Choice");
		JLabel lblSecuritylevel = new JLabel("Security Level");
		JLabel lblMatchingStrategy = new JLabel("Matching Strategy");
		comboMatchingStrategy = new MorphoComboBox(EnumMatchingStrategy.toStringArray());
		if (MorphoInfo.isFVP) {
			comboSecurityLevel = new MorphoComboBox(EnumSecurityLevelFVP.toStringArray());
		} else {
			comboSecurityLevel = new MorphoComboBox(EnumSecurityLevelMSO.toStringArray());
		}
		// feed combo box with data from enum
		String[] data = new String[Coder.values().length];
		int i = 0;
		for (Coder value : Coder.values()) {
			data[i++] = value.getLabel();
		}
		comboCoderChoice = new JComboBox(data);
		cbForceFingerPlacement = new JCheckBox("Force Finger Placement on Top");
		cbForceFingerPlacement.setSelected(true);
		cbForceFingerPlacement.setToolTipText("Force the finger to cover the top of the capture area to increase quality");
		cbAdvancedSecurityLevel = new JCheckBox("Advanced Security Level Compatibility Required");
		if (!MorphoInfo.isFVP) {
			cbAdvancedSecurityLevel.setEnabled(false);
		}
		cbFingerQualityThreshold = new JCheckBox("Finger Quality Threshold");
		NumberFormat numberFormat = NumberFormat.getNumberInstance(Locale.getDefault());
		DecimalFormat decimalFormat = (DecimalFormat) numberFormat;
		decimalFormat.setGroupingUsed(false);
		txtFingerQualityThreshold = new JFormattedTextField(decimalFormat);
		txtFingerQualityThreshold.setEnabled(false);
		txtFingerQualityThreshold.setText(" - ");
		txtFingerQualityThreshold.setColumns(3);

		JLabel lblAcquisitionStrategy = new JLabel("Acquisition Strategy");
		comboAcquisitionStrategy = new MorphoComboBox(EnumAcquisitionStrategy.toStringArray());

		GroupLayout groupLayout = new GroupLayout(this);
		groupLayout.setHorizontalGroup(groupLayout.createParallelGroup(Alignment.LEADING).addGroup(
				groupLayout
						.createSequentialGroup()
						.addContainerGap()
						.addGroup(
								groupLayout
										.createParallelGroup(Alignment.LEADING)
										.addGroup(groupLayout.createSequentialGroup().addComponent(cbFingerQualityThreshold).addPreferredGap(ComponentPlacement.RELATED).addComponent(txtFingerQualityThreshold, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
										.addComponent(cbAdvancedSecurityLevel)
										.addComponent(cbForceFingerPlacement)
										.addGroup(groupLayout.createSequentialGroup().addComponent(lblMatchingThreshold).addPreferredGap(ComponentPlacement.RELATED).addComponent(txtMatchingThreshold, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).addGap(18).addComponent(lblTimeoutsec).addPreferredGap(ComponentPlacement.RELATED).addComponent(txtTimeout, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
										.addGroup(
												groupLayout
														.createSequentialGroup()
														.addGroup(groupLayout.createParallelGroup(Alignment.LEADING).addComponent(lblMatchingStrategy).addComponent(lblSecuritylevel).addComponent(lblCoderChoice))
														.addPreferredGap(ComponentPlacement.RELATED)
														.addGroup(
																groupLayout
																		.createParallelGroup(Alignment.LEADING)
																		.addComponent(comboCoderChoice, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
																		.addGroup(
																				groupLayout.createSequentialGroup().addGroup(groupLayout.createParallelGroup(Alignment.LEADING).addComponent(comboSecurityLevel, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).addComponent(comboMatchingStrategy, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)).addGap(18)
																						.addGroup(groupLayout.createParallelGroup(Alignment.LEADING).addComponent(comboAcquisitionStrategy, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).addComponent(lblAcquisitionStrategy)))))).addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)));
		groupLayout.setVerticalGroup(groupLayout.createParallelGroup(Alignment.LEADING).addGroup(
				groupLayout.createSequentialGroup().addGroup(groupLayout.createParallelGroup(Alignment.BASELINE).addComponent(txtMatchingThreshold, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).addComponent(lblTimeoutsec).addComponent(txtTimeout, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).addComponent(lblMatchingThreshold)).addPreferredGap(ComponentPlacement.RELATED)
						.addGroup(groupLayout.createParallelGroup(Alignment.BASELINE).addComponent(lblCoderChoice).addComponent(comboCoderChoice, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)).addPreferredGap(ComponentPlacement.RELATED)
						.addGroup(groupLayout.createParallelGroup(Alignment.BASELINE).addComponent(lblSecuritylevel).addComponent(comboSecurityLevel, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).addComponent(lblAcquisitionStrategy)).addPreferredGap(ComponentPlacement.RELATED)
						.addGroup(groupLayout.createParallelGroup(Alignment.BASELINE).addComponent(lblMatchingStrategy).addComponent(comboMatchingStrategy, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).addComponent(comboAcquisitionStrategy, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)).addGap(18).addComponent(cbForceFingerPlacement).addPreferredGap(ComponentPlacement.UNRELATED).addComponent(cbAdvancedSecurityLevel)
						.addPreferredGap(ComponentPlacement.UNRELATED).addGroup(groupLayout.createParallelGroup(Alignment.BASELINE).addComponent(cbFingerQualityThreshold).addComponent(txtFingerQualityThreshold, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)).addContainerGap(31, Short.MAX_VALUE)));
		groupLayout.linkSize(SwingConstants.HORIZONTAL, new Component[] { comboSecurityLevel, comboCoderChoice, comboMatchingStrategy });
		setLayout(groupLayout);

		comboSecurityLevel.addActionListener(comboSecurityLevelChange);
		comboAcquisitionStrategy.addActionListener(comboAcquisitionStrategyChange);
		cbFingerQualityThreshold.addActionListener(cbFingerQualityThresholdChange);
	}

	private ActionListener comboSecurityLevelChange = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent event) {
			panelDatabaseMgt.msoDemo.setSecurityLevel((String) comboSecurityLevel.getSelectedItem());
		}
	};

	private ActionListener comboAcquisitionStrategyChange = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent event) {
			panelDatabaseMgt.msoDemo.setStrategyAcquisitionMode((short) comboAcquisitionStrategy.getSelectedIndex());
		}
	};

	private ActionListener cbFingerQualityThresholdChange = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent ae) {
			if (cbFingerQualityThreshold.isSelected())
				txtFingerQualityThreshold.setEnabled(true);
			else
				txtFingerQualityThreshold.setEnabled(false);
		}
	};
}
