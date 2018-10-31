package morpho.morphosmart.sdk.demo.dialog;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.border.EmptyBorder;

import morpho.morphosmart.sdk.demo.MsoDemo;

public class DialogEnableBioDataEncryption extends JDialog {

	private static final long serialVersionUID = 1L;
	private final JPanel contentPanel = new JPanel();
	private JLabel lblDiversificationData;
	private JTextField txtData;
	private JButton okButton;
	private JButton cancelButton;
	private MsoDemo msoDemo;
	private byte[] diversificationData = null;

	/**
	 * Create the dialog.
	 */
	public DialogEnableBioDataEncryption(JFrame parentFrame, String title) {
		super(parentFrame);
		
		setMsoDemo((MsoDemo) parentFrame);
		setTitle(title);
		setModalityType(ModalityType.DOCUMENT_MODAL);
		setResizable(false);
		setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		setSize(350, 145);
		setLocationRelativeTo(null);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);
		{
			lblDiversificationData = new JLabel("Diversification Data:");
		}
		txtData = new JTextField();
		txtData.setColumns(10);
		GroupLayout gl_contentPanel = new GroupLayout(contentPanel);
		gl_contentPanel.setHorizontalGroup(
			gl_contentPanel.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_contentPanel.createSequentialGroup()
					.addContainerGap()
					.addGroup(gl_contentPanel.createParallelGroup(Alignment.LEADING)
						.addComponent(txtData, GroupLayout.DEFAULT_SIZE, 404, Short.MAX_VALUE)
						.addComponent(lblDiversificationData))
					.addContainerGap())
		);
		gl_contentPanel.setVerticalGroup(
			gl_contentPanel.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_contentPanel.createSequentialGroup()
					.addContainerGap()
					.addComponent(lblDiversificationData)
					.addPreferredGap(ComponentPlacement.RELATED)
					.addComponent(txtData, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
					.addContainerGap(30, Short.MAX_VALUE))
		);
		contentPanel.setLayout(gl_contentPanel);
		{
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
			{
				okButton = new JButton("OK");
				okButton.setActionCommand("OK");
				buttonPane.add(okButton);
				getRootPane().setDefaultButton(okButton);
			}
			{
				cancelButton = new JButton("Cancel");
				cancelButton.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent event) {
						dispose();
					}
				});
				cancelButton.setActionCommand("Cancel");
				buttonPane.add(cancelButton);
			}
		}
		okButton.addActionListener( new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent arg0) {
				msoDemo.setDataEncryption(true);				
				if(txtData.getText() != "")
				{
					diversificationData = txtData.getText().getBytes();
				}
				msoDemo.getMorphoDeviceInstance().enableDataEncryption(true, diversificationData);
				dispose();
			}
		});
	}

	public MsoDemo getMsoDemo() {
		return msoDemo;
	}

	public void setMsoDemo(MsoDemo msoDemo) {
		this.msoDemo = msoDemo;
	}
	
	public byte[] getDiversificationData() {
		return this.diversificationData;
	}

}
