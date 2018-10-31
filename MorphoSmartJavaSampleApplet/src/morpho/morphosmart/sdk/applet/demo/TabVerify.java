package morpho.morphosmart.sdk.applet.demo;

import java.awt.BorderLayout;

import javax.swing.ButtonGroup;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextArea;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.UIManager;
import javax.swing.border.TitledBorder;

public class TabVerify extends JPanel {

	private static final long serialVersionUID = 1L;

	protected JRadioButton rbFileSimulation;
	protected JRadioButton rbCheckInLocal;

	/**
	 * Create the panel.
	 * @param isBaseAvailable 
	 * @param panelBasicBiometricOperation
	 */
	public TabVerify(boolean isBaseAvailable) {
		setLayout(new BorderLayout(0, 0));
		
		JTextArea txtrTheVerifyCommand = new JTextArea();
		txtrTheVerifyCommand.setEditable(false);
		txtrTheVerifyCommand.setBackground(UIManager.getColor("Button.background"));
		String str = "\n  The verify command allows you to authenticate someone\n"
				+ "  thanks to an input item. The MSO captures\n"
				+ "  finger and verifies of the template matches.\n";
		txtrTheVerifyCommand.setText(str);
		add(txtrTheVerifyCommand, BorderLayout.NORTH);
		
		JPanel panel = new JPanel();
		panel.setBorder(new TitledBorder(null, "Authentication Mode", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		add(panel, BorderLayout.CENTER);
		
		// Radio group
		rbFileSimulation = new JRadioButton("File Simulation");
		rbFileSimulation.setSelected(true);
		rbCheckInLocal = new JRadioButton("Check in local base (select a user first)");
		ButtonGroup groupFingers = new ButtonGroup();
	    groupFingers.add(rbFileSimulation);
	    groupFingers.add(rbCheckInLocal);
		
		GroupLayout gl_panel = new GroupLayout(panel);
		gl_panel.setHorizontalGroup(
			gl_panel.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_panel.createSequentialGroup()
					.addContainerGap()
					.addGroup(gl_panel.createParallelGroup(Alignment.LEADING)
						.addComponent(rbFileSimulation)
						.addComponent(rbCheckInLocal))
					.addContainerGap(223, Short.MAX_VALUE))
		);
		gl_panel.setVerticalGroup(
			gl_panel.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_panel.createSequentialGroup()
					.addContainerGap()
					.addComponent(rbFileSimulation)
					.addPreferredGap(ComponentPlacement.UNRELATED)
					.addComponent(rbCheckInLocal)
					.addContainerGap(127, Short.MAX_VALUE))
		);
		
		panel.setLayout(gl_panel);

		if(!isBaseAvailable)
		{
			rbCheckInLocal.setEnabled(false);
		}
	}
}
