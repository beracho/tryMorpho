package morpho.morphosmart.sdk.demo;

import java.awt.BorderLayout;

import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.UIManager;

public class TabIdentify extends JPanel {

	private static final long serialVersionUID = 1L;

	/**
	 * Create the panel.
	 * 
	 * @param panelBasicBiometricOperation
	 */
	public TabIdentify() {
		setLayout(new BorderLayout(0, 0));

		JTextArea txtrThisOperationAllows = new JTextArea();
		txtrThisOperationAllows.setEditable(false);
		txtrThisOperationAllows.setBackground(UIManager.getColor("Button.background"));
		String str = "\n  This operation allows you to identify a user in the\n" + "  database. Press \"Start\" and put your finger on\n" + "  the MSO sensor. If you have already been enrolled,\n" + "  the device will identify you.";
		txtrThisOperationAllows.setText(str);
		add(txtrThisOperationAllows, BorderLayout.NORTH);
	}

}
