package morpho.morphosmart.sdk.demo.dialog;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.FlowLayout;
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
import javax.swing.JTable;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;

import morpho.morphosmart.sdk.demo.trt.MorphoInfo;

public class DialogReadMsoConf extends JDialog {

	private static final long serialVersionUID = 1L;
	private final JPanel contentPanel = new JPanel();
	// Borders
	private Border lowerBevelBorder = BorderFactory.createBevelBorder(BevelBorder.LOWERED);
	// UI variables
	private JLabel msoSerialNumber = new JLabel(" ");
	private JLabel maxFAFR = new JLabel(" ");
	private JLabel minimumMultimodalSecurity = new JLabel(" ");
	private JTable table = new JTable();
	// variables
	private boolean isProtectedWithSignature = false;
	private boolean isTunneling = false;
	private boolean isOfferedSecurity = false;
	private boolean isAcceptOnlyPkSigned = false;
	private boolean isExportScore = false;

	/**
	 * Create the dialog.
	 */
	public DialogReadMsoConf(JFrame parentFrame) {
		super(parentFrame);
		setTitle("MSO Configuration");
		setSize(350, 350);
		setModalityType(ModalityType.DOCUMENT_MODAL);
		setResizable(false);
		setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		setLocationRelativeTo(null);

		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);

		JLabel lblMsoSerialNumber = new JLabel("MSO Serial number");
		msoSerialNumber.setBorder(lowerBevelBorder);
		JLabel lblMaxFar = new JLabel("Max FAR");
		maxFAFR.setBorder(lowerBevelBorder);
		JLabel lblSecurityOptions = new JLabel("Security Options");

		// show only when FVP is plugged
		JLabel lblMinSecLevel = new JLabel("Minimum Multimodal Security Level");
		minimumMultimodalSecurity.setBorder(lowerBevelBorder);
		if (!MorphoInfo.isFVP) {
			setSize(350, 300);
			lblMinSecLevel.setVisible(false);
			minimumMultimodalSecurity.setVisible(false);
		}

		GroupLayout gl_contentPanel = new GroupLayout(contentPanel);
		gl_contentPanel.setHorizontalGroup(
			gl_contentPanel.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_contentPanel.createSequentialGroup()
					.addContainerGap()
					.addGroup(gl_contentPanel.createParallelGroup(Alignment.LEADING)
						.addGroup(gl_contentPanel.createSequentialGroup()
							.addComponent(lblMsoSerialNumber)
							.addContainerGap(274, Short.MAX_VALUE))
						.addGroup(gl_contentPanel.createSequentialGroup()
							.addComponent(msoSerialNumber, GroupLayout.DEFAULT_SIZE, 292, Short.MAX_VALUE)
							.addGap(72))
						.addGroup(gl_contentPanel.createSequentialGroup()
							.addComponent(lblMaxFar)
							.addContainerGap(321, Short.MAX_VALUE))
						.addGroup(gl_contentPanel.createSequentialGroup()
							.addComponent(lblSecurityOptions)
							.addContainerGap(285, Short.MAX_VALUE))
						.addGroup(gl_contentPanel.createSequentialGroup()
							.addComponent(table, GroupLayout.PREFERRED_SIZE, 273, GroupLayout.PREFERRED_SIZE)
							.addContainerGap(91, Short.MAX_VALUE))
						.addGroup(gl_contentPanel.createSequentialGroup()
							.addComponent(lblMinSecLevel)
							.addContainerGap(318, Short.MAX_VALUE))
						.addGroup(Alignment.TRAILING, gl_contentPanel.createSequentialGroup()
							.addGroup(gl_contentPanel.createParallelGroup(Alignment.TRAILING)
								.addComponent(minimumMultimodalSecurity, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, 292, Short.MAX_VALUE)
								.addComponent(maxFAFR, GroupLayout.DEFAULT_SIZE, 292, Short.MAX_VALUE))
							.addGap(72))))
		);
		
		gl_contentPanel.setVerticalGroup(
			gl_contentPanel.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_contentPanel.createSequentialGroup()
					.addContainerGap()
					.addComponent(lblMsoSerialNumber)
					.addPreferredGap(ComponentPlacement.RELATED)
					.addComponent(msoSerialNumber)
					.addPreferredGap(ComponentPlacement.UNRELATED)
					.addComponent(lblMaxFar)
					.addPreferredGap(ComponentPlacement.RELATED)
					.addComponent(maxFAFR)
					.addPreferredGap(ComponentPlacement.UNRELATED)
					.addComponent(lblSecurityOptions)
					.addPreferredGap(ComponentPlacement.RELATED)
					.addComponent(table, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
					.addGap(18)
					.addComponent(lblMinSecLevel)
					.addPreferredGap(ComponentPlacement.RELATED)
					.addComponent(minimumMultimodalSecurity)
					.addContainerGap(18, Short.MAX_VALUE))
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
				okButton.requestFocus();
				okButton.addActionListener(new ActionListener() {
					@Override
					public void actionPerformed(ActionEvent ae) {
						dispose();
					}
				});
			}
		}
	}

	/**
	 * Custom renderer class.
	 * 
	 */
	private class ColoredRenderer extends DefaultTableCellRenderer {
		private static final long serialVersionUID = 652852277993293545L;
		public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) { 
		    Component comp = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column); 
		    // test only the second column
		    String cellValue = (String) value;
		    if(column == 1 && cellValue.equalsIgnoreCase("Yes")) {
	    		comp.setBackground(Color.GREEN);
		    } else if(column == 1 && cellValue.equalsIgnoreCase("No")) {
		    	comp.setBackground(new Color(255, 100, 100));
		    } else {
		    	comp.setBackground(Color.WHITE);
		    }
		    return comp; 
		} 
	}

	/**
	 * Fill tables with the correct values.
	 */
	public void fillTable() {
		table.setCellSelectionEnabled(false);
		table.setRowSelectionAllowed(false);
		table.setBorder(lowerBevelBorder);
		table.setDefaultRenderer(Object.class, new ColoredRenderer());
		table.setModel(new DefaultTableModel(
			new Object[][] {
				{" Download is protected with a signature", (isProtectedWithSignature ? "Yes" : "No")},
				{" Mode Tunneling", (isTunneling ? "Yes" : "No")},
				{" Mode Offered Security", (isOfferedSecurity ? "Yes" : "No")},
				{" MSO accepts only signed templates", (isAcceptOnlyPkSigned ? "Yes" : "No")},
				{" Export score", (isExportScore ? "Yes" : "No")},
				},
				new String[] {"", ""}
			){
				private static final long serialVersionUID = 1820122219145872634L;
				boolean[] columnEditables = new boolean[] {
					false, false
				};
				public boolean isCellEditable(int row, int column) {
					return columnEditables[column];
				}
			}
		);
		table.getColumnModel().getColumn(0).setResizable(false);
		table.getColumnModel().getColumn(1).setResizable(false);
		table.getColumnModel().getColumn(0).setPreferredWidth(210);
		table.getColumnModel().getColumn(1).setPreferredWidth(35);
	}

	/**
	 * 
	 * @param msoSerialNumber
	 */
	public void setMsoSerialNumber(String msoSerialNumber) {
		this.msoSerialNumber.setText(msoSerialNumber);
	}

	/**
	 * 
	 * @param maxFAFR
	 */
	public void setMaxFAFR(String maxFAFR) {
		this.maxFAFR.setText(maxFAFR);
	}

	/**
	 * 
	 * @param minimumMultimodalSecurity
	 */
	public void setMinimumMultimodalSecurity(String minimumMultimodalSecurity) {
		this.minimumMultimodalSecurity.setText(minimumMultimodalSecurity);
	}

	/**
	 * 
	 * @param isProtectedWithSignature
	 */
	public void setIsProtectedWithSignature(boolean isProtectedWithSignature) {
		this.isProtectedWithSignature =isProtectedWithSignature;
	}

	/**
	 * 
	 * @param isTunneling
	 */
	public void setIsTunneling(boolean isTunneling) {
		this.isTunneling = isTunneling;
	}

	/**
	 * 
	 * @param isOfferedSecurity
	 */
	public void setIsOfferedSecurity(boolean isOfferedSecurity) {
		this.isOfferedSecurity = isOfferedSecurity;
	}

	/**
	 * 
	 * @param isAcceptOnlyPkSigned
	 */
	public void setIsAcceptOnlyPkSigned(boolean isAcceptOnlyPkSigned) {
		this.isAcceptOnlyPkSigned  = isAcceptOnlyPkSigned;
	}

	/**
	 * 
	 * @param isExportScore
	 */
	public void setIsExportScore(boolean isExportScore) {
		this.isExportScore  = isExportScore;
	}
}
