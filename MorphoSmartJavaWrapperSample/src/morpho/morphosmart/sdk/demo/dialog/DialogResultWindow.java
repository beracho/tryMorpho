package morpho.morphosmart.sdk.demo.dialog;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.SwingConstants;
import javax.swing.border.BevelBorder;
import javax.swing.border.EmptyBorder;

public class DialogResultWindow extends JDialog {

	private static final long serialVersionUID = 1L;
	private final JPanel contentPanel = new JPanel();

	/**
	 * Create the dialog.
	 */
	public DialogResultWindow(JFrame parentFrame, String result, String message1, String message2, String message3, String message4) {
		super(parentFrame);

		if (result == null || result.trim().length() == 0)
			result = "-";
		if (message1 == null || message1.trim().length() == 0)
			message1 = "-";
		if (message2 == null || message2.trim().length() == 0)
			message2 = "-";
		if (message3 == null || message3.trim().length() == 0)
			message3 = "-";
		if (message4 == null || message4.trim().length() == 0)
			message4 = "-";

		setTitle("Result Window");
		setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		setModalityType(ModalityType.DOCUMENT_MODAL);
		setResizable(false);
		setSize(700, 430);
		setLocationRelativeTo(null);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);

		JLabel lblResultStatus = new JLabel(result);
		lblResultStatus.setHorizontalAlignment(SwingConstants.CENTER);
		lblResultStatus.setFont(new Font("Arial", Font.BOLD, 28));
		lblResultStatus.setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));
		JLabel lblMessage1 = new JLabel(message1);
		lblMessage1.setHorizontalAlignment(SwingConstants.CENTER);
		lblMessage1.setFont(new Font("Arial", Font.PLAIN, 18));
		lblMessage1.setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));
		JLabel lblMessage2 = new JLabel(message2);
		lblMessage2.setHorizontalAlignment(SwingConstants.CENTER);
		lblMessage2.setFont(new Font("Arial", Font.PLAIN, 18));
		lblMessage2.setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));
		JLabel lblMessage3 = new JLabel(formatText(message3, 50));
		lblMessage3.setHorizontalAlignment(SwingConstants.CENTER);
		lblMessage3.setFont(new Font("Arial", Font.PLAIN, 18));
		lblMessage3.setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));
		JLabel lblMessage4 = new JLabel(message4);
		lblMessage4.setHorizontalAlignment(SwingConstants.CENTER);
		lblMessage4.setFont(new Font("Arial", Font.PLAIN, 18));
		lblMessage4.setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));


		GroupLayout gl_contentPanel = new GroupLayout(contentPanel);
		gl_contentPanel.setHorizontalGroup(
			gl_contentPanel.createParallelGroup(Alignment.TRAILING)
				.addGroup(gl_contentPanel.createSequentialGroup()
					.addContainerGap()
					.addGroup(gl_contentPanel.createParallelGroup(Alignment.TRAILING)
						.addComponent(lblMessage4, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, 454, Short.MAX_VALUE)
						.addComponent(lblMessage3, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, 454, Short.MAX_VALUE)
						.addComponent(lblMessage1, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, 454, Short.MAX_VALUE)
						.addComponent(lblMessage2, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, 454, Short.MAX_VALUE)
						.addComponent(lblResultStatus, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, 454, Short.MAX_VALUE))
					.addContainerGap())
		);
		gl_contentPanel.setVerticalGroup(
			gl_contentPanel.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_contentPanel.createSequentialGroup()
					.addContainerGap()
					.addComponent(lblResultStatus, GroupLayout.PREFERRED_SIZE, 50, GroupLayout.PREFERRED_SIZE)
					.addPreferredGap(ComponentPlacement.UNRELATED)
					.addComponent(lblMessage1, GroupLayout.PREFERRED_SIZE, 60, GroupLayout.PREFERRED_SIZE)
					.addPreferredGap(ComponentPlacement.RELATED)
					.addComponent(lblMessage2, GroupLayout.PREFERRED_SIZE, 40, GroupLayout.PREFERRED_SIZE)
					.addPreferredGap(ComponentPlacement.UNRELATED)
					.addComponent(lblMessage3, GroupLayout.PREFERRED_SIZE, 70, GroupLayout.PREFERRED_SIZE)
					.addPreferredGap(ComponentPlacement.UNRELATED)
					.addComponent(lblMessage4, GroupLayout.DEFAULT_SIZE, 70, Short.MAX_VALUE)
					.addContainerGap())
		);
		contentPanel.setLayout(gl_contentPanel);
		{
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
			{
				JButton okButton = new JButton("   OK   ");
				okButton.setActionCommand("OK");
				buttonPane.add(okButton);
				getRootPane().setDefaultButton(okButton);
				okButton.addActionListener(new ActionListener() {
					@Override
					public void actionPerformed(ActionEvent ae) {
						dispose();
					}
				});
			}
		}
	}
	
	public static String formatText(String message, int size) {
		String html = "<html><body style='text-align: center;'>";
		if(message.length() > size) {
			html += message.substring(0, size) + "<br/>" + message.substring(size);
		} else {
			html += message;
		}
		return html + "</body></html>";
	}

	public static String formatText(String message) {
		String html = "<html><body style='text-align: center;'>";		
		html +=  message.replace("\n", "<br/>");		
		return html + "</body></html>";
	}

}
