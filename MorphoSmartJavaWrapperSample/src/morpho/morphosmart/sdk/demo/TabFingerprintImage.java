package morpho.morphosmart.sdk.demo;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.NumberFormat;

import javax.swing.ButtonGroup;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextArea;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.UIManager;

import morpho.morphosmart.sdk.demo.ennum.EnumImageFormat;
import morpho.morphosmart.sdk.demo.trt.MorphoComboBox;

public class TabFingerprintImage extends JPanel {

	private static final long serialVersionUID = 1L;
	// UI variables
	JRadioButton rbVerifyDetectionMode;
	JRadioButton rbEnrollmentDetectionMode;
	JCheckBox cbLatentDetect;
	JComboBox comboImageFormat;
	JLabel lblCompressionRate;
	JFormattedTextField textCompressionRate;

	PanelBasicBiometricOperation panelBasicBiometricOperation;

	/**
	 * Create the panel.
	 * 
	 * @param panelBasicBiometricOperation
	 */
	public TabFingerprintImage(PanelBasicBiometricOperation panelBasicBiometricOperation) {
		this.panelBasicBiometricOperation = panelBasicBiometricOperation;
		setLayout(new BorderLayout(0, 0));

		JTextArea txtrThisOperationAllows = new JTextArea();
		txtrThisOperationAllows.setEditable(false);
		txtrThisOperationAllows.setBackground(UIManager.getColor("Button.background"));
		String str = "\n   This operation allows you to capture an image\n" + "  in order to store it on an output support.\n";
		txtrThisOperationAllows.setText(str);
		add(txtrThisOperationAllows, BorderLayout.NORTH);

		JPanel panel = new JPanel();
		add(panel, BorderLayout.CENTER);

		rbVerifyDetectionMode = new JRadioButton("Verify detection mode");
		rbEnrollmentDetectionMode = new JRadioButton("Enrollment detection mode");
		ButtonGroup group = new ButtonGroup();
		group.add(rbEnrollmentDetectionMode);
		group.add(rbVerifyDetectionMode);

		rbEnrollmentDetectionMode.setSelected(true);
		cbLatentDetect = new JCheckBox("Latent Dect.");
		JLabel lblImageFormat = new JLabel("Image format");
		comboImageFormat = new MorphoComboBox(EnumImageFormat.toStringArray());
		comboImageFormat.setSelectedItem(EnumImageFormat.RAW.toString());
		lblCompressionRate = new JLabel("Compression rate");
		lblCompressionRate.setEnabled(false);
		NumberFormat format = NumberFormat.getIntegerInstance();
		format.setMaximumFractionDigits(0);
		format.setMinimumIntegerDigits(1);
		format.setMaximumIntegerDigits(3);
		format.setParseIntegerOnly(true);
		textCompressionRate = new JFormattedTextField(format);
		textCompressionRate.setEnabled(false);
		textCompressionRate.setText("10");
		textCompressionRate.setColumns(10);

		GroupLayout gl_panel = new GroupLayout(panel);
		gl_panel.setHorizontalGroup(gl_panel.createParallelGroup(Alignment.LEADING).addGroup(
				gl_panel.createSequentialGroup()
						.addGap(59)
						.addGroup(
								gl_panel.createParallelGroup(Alignment.LEADING).addGroup(gl_panel.createSequentialGroup().addComponent(lblCompressionRate).addPreferredGap(ComponentPlacement.UNRELATED).addComponent(textCompressionRate, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)).addComponent(rbEnrollmentDetectionMode).addComponent(rbVerifyDetectionMode)
										.addGroup(gl_panel.createSequentialGroup().addGroup(gl_panel.createParallelGroup(Alignment.LEADING).addComponent(lblImageFormat).addComponent(cbLatentDetect)).addPreferredGap(ComponentPlacement.UNRELATED).addComponent(comboImageFormat, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))).addContainerGap(211, Short.MAX_VALUE)));
		gl_panel.setVerticalGroup(gl_panel.createParallelGroup(Alignment.LEADING).addGroup(
				gl_panel.createSequentialGroup().addContainerGap().addComponent(rbVerifyDetectionMode).addPreferredGap(ComponentPlacement.UNRELATED).addComponent(rbEnrollmentDetectionMode).addPreferredGap(ComponentPlacement.UNRELATED).addComponent(cbLatentDetect).addPreferredGap(ComponentPlacement.RELATED)
						.addGroup(gl_panel.createParallelGroup(Alignment.BASELINE).addComponent(lblImageFormat).addComponent(comboImageFormat, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)).addPreferredGap(ComponentPlacement.RELATED).addGroup(gl_panel.createParallelGroup(Alignment.BASELINE).addComponent(lblCompressionRate).addComponent(textCompressionRate, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
						.addContainerGap(100, Short.MAX_VALUE)));
		panel.setLayout(gl_panel);

		comboImageFormat.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent ae) {
				if (comboImageFormat.getSelectedItem().toString().equals(EnumImageFormat.WSQ.toString())) {
					lblCompressionRate.setEnabled(true);
					textCompressionRate.setEnabled(true);
				} else {
					lblCompressionRate.setEnabled(false);
					textCompressionRate.setEnabled(false);
				}
			}
		});

	}
}
