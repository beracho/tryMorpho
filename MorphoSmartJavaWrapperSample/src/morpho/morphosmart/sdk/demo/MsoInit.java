package morpho.morphosmart.sdk.demo;

import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.LayoutStyle.ComponentPlacement;

import morpho.morphosmart.sdk.demo.trt.ImageLoader;

public class MsoInit extends JDialog {

	private static final long serialVersionUID = 1L;
	// UI variables
	JLabel statusOpen = new JLabel(": ");
	JLabel statusMsoConfig = new JLabel(": ");
	JLabel statusMsoCertif = new JLabel(": ");
	JLabel statusSecurityCom = new JLabel(": ");
	JLabel statusDatabaseConf = new JLabel(": ");
	JLabel lblCommunicationInitialisation = new JLabel("Offered communication initialisation");
	JLabel lblOpen = new JLabel("Open USB");
	JLabel lblMsoConfiguration = new JLabel("MSO Configuration");
	JLabel lblTheCertification = new JLabel("The MSO Certification");

	public MsoInit() {
		setIconImage(ImageLoader.load("MSO_Demo.png"));
		setModalExclusionType(ModalExclusionType.APPLICATION_EXCLUDE);
		setTitle("Initialisation, please wait...");
		setResizable(false);
		setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		setSize(450, 240);
		setLocationRelativeTo(null);

		JLabel lblMsoAnalysePlease = new JLabel("MSO analyse, please wait.................");

		JLabel lblTheDatabaseConfiguration = new JLabel("The database configuration");

		GroupLayout groupLayout = new GroupLayout(getContentPane());
		groupLayout.setHorizontalGroup(groupLayout.createParallelGroup(Alignment.LEADING).addGroup(
				Alignment.TRAILING,
				groupLayout
						.createSequentialGroup()
						.addGroup(
								groupLayout.createParallelGroup(Alignment.LEADING).addGroup(groupLayout.createSequentialGroup().addContainerGap().addGroup(groupLayout.createParallelGroup(Alignment.LEADING).addComponent(lblCommunicationInitialisation).addComponent(lblOpen).addComponent(lblMsoConfiguration).addComponent(lblTheCertification))).addGroup(groupLayout.createSequentialGroup().addGap(10).addComponent(lblTheDatabaseConfiguration))
										.addGroup(groupLayout.createSequentialGroup().addGap(10).addComponent(lblMsoAnalysePlease))).addPreferredGap(ComponentPlacement.RELATED, 28, Short.MAX_VALUE)
						.addGroup(groupLayout.createParallelGroup(Alignment.LEADING, false).addGroup(groupLayout.createSequentialGroup().addComponent(statusDatabaseConf).addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)).addGroup(Alignment.TRAILING, groupLayout.createSequentialGroup().addGroup(groupLayout.createParallelGroup(Alignment.LEADING).addComponent(statusSecurityCom).addComponent(statusMsoCertif).addComponent(statusMsoConfig).addComponent(statusOpen)).addGap(151)))));
		groupLayout.setVerticalGroup(groupLayout.createParallelGroup(Alignment.LEADING).addGroup(
				groupLayout
						.createSequentialGroup()
						.addContainerGap()
						.addGroup(
								groupLayout.createParallelGroup(Alignment.TRAILING).addGroup(groupLayout.createSequentialGroup().addComponent(statusOpen).addGap(18).addComponent(statusMsoConfig).addGap(18).addComponent(statusMsoCertif).addGap(18).addComponent(statusSecurityCom))
										.addGroup(groupLayout.createSequentialGroup().addComponent(lblMsoAnalysePlease).addGap(18).addComponent(lblOpen).addGap(18).addComponent(lblMsoConfiguration).addGap(18).addComponent(lblTheCertification).addGap(18).addComponent(lblCommunicationInitialisation))).addGap(18).addGroup(groupLayout.createParallelGroup(Alignment.BASELINE).addComponent(lblTheDatabaseConfiguration).addComponent(statusDatabaseConf)).addContainerGap(27, Short.MAX_VALUE)));
		getContentPane().setLayout(groupLayout);

	}

	//
	// Setters-------------------------------------------------------------------
	//

	public void setOpen(String open) {
		this.lblOpen.setText(open);
		this.repaint();
	}

	public void setStatusOpen(String statusOpen) {
		this.statusOpen.setText(": " + statusOpen);
	}

	public void setStatusMsoConfig(String statusMsoConfig) {
		this.statusMsoConfig.setText(": " + statusMsoConfig);
	}

	public void setStatusMsoCertif(String statusMsoCertif) {
		this.statusMsoCertif.setText(": " + statusMsoCertif);
	}

	public void setCommunicationInitialisation(String securityCom) {
		this.lblCommunicationInitialisation.setText(securityCom);
	}

	public void setStatusSecurityCom(String statusSecurityCom) {
		this.statusSecurityCom.setText(": " + statusSecurityCom);
	}

	public void setStatusDatabaseConf(String statusDatabaseConf) {
		this.statusDatabaseConf.setText(": " + statusDatabaseConf);
	}

	public void setCertification(String certification) {
		this.lblTheCertification.setText(certification);
	}
}
