package morpho.morphosmart.sdk.demo;

import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.LayoutStyle.ComponentPlacement;

public class TabDbInformation extends JPanel {

	private static final long serialVersionUID = 1L;

	protected PanelDatabaseMgt panelDatabaseMgt;

	private JLabel lblMaxNbOfRecords;
	private JLabel lblCurrentNbOfRecords;
	private JLabel lblNbOfFingersPerRecord;
	private JLabel lblEncryptedBb;

	/**
	 * Create the panel.
	 * 
	 * @param panelDatabaseMgt
	 */
	public TabDbInformation(PanelDatabaseMgt panelDatabaseMgt) {
		this.panelDatabaseMgt = panelDatabaseMgt;

		JLabel lblNewLabel = new JLabel("Maximum nb of records");
		lblMaxNbOfRecords = new JLabel("");

		JLabel lblNewLabel_1 = new JLabel("Current nb of records");
		lblCurrentNbOfRecords = new JLabel("");

		JLabel lblNewLabel_2 = new JLabel("Nb of fingers per record");
		lblNbOfFingersPerRecord = new JLabel("");

		JLabel lblNewLabel_3 = new JLabel("Encrypted database");
		lblEncryptedBb = new JLabel("");

		GroupLayout groupLayout = new GroupLayout(this);
		groupLayout.setHorizontalGroup(groupLayout.createParallelGroup(Alignment.LEADING).addGroup(
				groupLayout.createSequentialGroup().addGap(38).addGroup(groupLayout.createParallelGroup(Alignment.LEADING, false).addComponent(lblNewLabel, GroupLayout.DEFAULT_SIZE, 200, Short.MAX_VALUE).addComponent(lblNewLabel_1, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addComponent(lblNewLabel_2, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addComponent(lblNewLabel_3, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
						.addGap(18).addGroup(groupLayout.createParallelGroup(Alignment.LEADING).addComponent(lblMaxNbOfRecords, GroupLayout.PREFERRED_SIZE, 170, GroupLayout.PREFERRED_SIZE).addComponent(lblEncryptedBb, GroupLayout.PREFERRED_SIZE, 170, GroupLayout.PREFERRED_SIZE).addComponent(lblNbOfFingersPerRecord, GroupLayout.PREFERRED_SIZE, 170, GroupLayout.PREFERRED_SIZE).addComponent(lblCurrentNbOfRecords, GroupLayout.PREFERRED_SIZE, 170, GroupLayout.PREFERRED_SIZE))
						.addContainerGap(24, Short.MAX_VALUE)));
		groupLayout.setVerticalGroup(groupLayout.createParallelGroup(Alignment.LEADING).addGroup(
				groupLayout
						.createSequentialGroup()
						.addGap(36)
						.addGroup(groupLayout.createParallelGroup(Alignment.LEADING).addComponent(lblNewLabel, GroupLayout.PREFERRED_SIZE, 32, GroupLayout.PREFERRED_SIZE).addComponent(lblMaxNbOfRecords, GroupLayout.PREFERRED_SIZE, 32, GroupLayout.PREFERRED_SIZE))
						.addPreferredGap(ComponentPlacement.RELATED)
						.addGroup(groupLayout.createParallelGroup(Alignment.LEADING).addComponent(lblCurrentNbOfRecords, GroupLayout.PREFERRED_SIZE, 32, GroupLayout.PREFERRED_SIZE).addComponent(lblNewLabel_1, GroupLayout.PREFERRED_SIZE, 32, GroupLayout.PREFERRED_SIZE))
						.addPreferredGap(ComponentPlacement.RELATED)
						.addGroup(
								groupLayout.createParallelGroup(Alignment.LEADING, false).addGroup(groupLayout.createSequentialGroup().addComponent(lblNbOfFingersPerRecord, GroupLayout.PREFERRED_SIZE, 32, GroupLayout.PREFERRED_SIZE).addPreferredGap(ComponentPlacement.RELATED, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE).addComponent(lblEncryptedBb, GroupLayout.PREFERRED_SIZE, 32, GroupLayout.PREFERRED_SIZE))
										.addGroup(groupLayout.createSequentialGroup().addComponent(lblNewLabel_2, GroupLayout.PREFERRED_SIZE, 32, GroupLayout.PREFERRED_SIZE).addPreferredGap(ComponentPlacement.RELATED).addComponent(lblNewLabel_3, GroupLayout.PREFERRED_SIZE, 32, GroupLayout.PREFERRED_SIZE))).addContainerGap(118, Short.MAX_VALUE)));
		setLayout(groupLayout);

	}

	//
	// Getters and setters
	//

	public void setLblMaxNbOfRecords(int maxNbOfRecords) {
		this.lblMaxNbOfRecords.setText("" + maxNbOfRecords);
	}

	public void setLblCurrentNbOfRecords(int currentNbOfRecords) {
		this.lblCurrentNbOfRecords.setText("" + currentNbOfRecords);
	}

	public void setLblNbOfFingersPerRecord(int nbOfFingersPerRecord) {
		this.lblNbOfFingersPerRecord.setText("" + nbOfFingersPerRecord);
	}

	public void setLblEncryptedBb(String encryptedBb) {
		this.lblEncryptedBb.setText(encryptedBb);
	}

}
