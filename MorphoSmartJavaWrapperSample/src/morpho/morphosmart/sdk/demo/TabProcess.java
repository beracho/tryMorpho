package morpho.morphosmart.sdk.demo;

import java.awt.Color;
import java.awt.Component;
import java.awt.image.BufferedImage;

import javax.swing.BorderFactory;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.SwingConstants;
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;

import morpho.morphosmart.sdk.api.MorphoImage;
import morpho.morphosmart.sdk.demo.trt.MorphoEventHandler;

public class TabProcess extends JPanel {

	private static final long serialVersionUID = 1L;

	protected PanelBasicBiometricOperation panelBasicBiometricOperation;
	private JLabel lblFinger1step1 = new JLabel("");
	private JLabel lblFinger1step2 = new JLabel("");
	private JLabel lblFinger1step3 = new JLabel("");
	private JLabel lblFinger2step1 = new JLabel("");
	private JLabel lblFinger2step2 = new JLabel("");
	private JLabel lblFinger2step3 = new JLabel("");
	private JLabel lblDetectedQuality = new JLabel("...");
	private JLabel[][] lblFinger = { { lblFinger1step1, lblFinger1step2, lblFinger1step3 }, { lblFinger2step1, lblFinger2step2, lblFinger2step3 } };
	// Borders
	private Border lowerBevelBorder = BorderFactory.createBevelBorder(BevelBorder.LOWERED);
	private Border greenLine = BorderFactory.createLineBorder(Color.GREEN);
	private Border orangeLine = BorderFactory.createLineBorder(Color.ORANGE);

	/**
	 * Create the panel.
	 * 
	 * @param panelBasicBiometricOperation
	 */
	public TabProcess(PanelBasicBiometricOperation panelBasicBiometricOperation) {
		this.panelBasicBiometricOperation = panelBasicBiometricOperation;
		JPanel panelFinger1 = new JPanel();

		panelFinger1.setBorder(new TitledBorder(null, "Finger #1", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		lblFinger1step1.setBorder(lowerBevelBorder);
		lblFinger1step2.setBorder(lowerBevelBorder);
		lblFinger1step3.setBorder(lowerBevelBorder);

		JPanel panelFinger2 = new JPanel();
		panelFinger2.setBorder(new TitledBorder(null, "Finger #2", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		lblFinger2step1.setBorder(lowerBevelBorder);
		lblFinger2step2.setBorder(lowerBevelBorder);
		lblFinger2step3.setBorder(lowerBevelBorder);

		lblDetectedQuality.setBorder(lowerBevelBorder);

		GroupLayout groupLayout = new GroupLayout(this);
		groupLayout.setHorizontalGroup(groupLayout.createParallelGroup(Alignment.LEADING).addComponent(panelFinger1, GroupLayout.DEFAULT_SIZE, 390, Short.MAX_VALUE).addComponent(panelFinger2, Alignment.TRAILING, GroupLayout.DEFAULT_SIZE, 390, Short.MAX_VALUE).addGroup(Alignment.TRAILING, groupLayout.createSequentialGroup().addContainerGap().addComponent(lblDetectedQuality, GroupLayout.DEFAULT_SIZE, 370, Short.MAX_VALUE).addContainerGap()));
		groupLayout.setVerticalGroup(groupLayout.createParallelGroup(Alignment.LEADING).addGroup(groupLayout.createSequentialGroup().addComponent(panelFinger1, GroupLayout.PREFERRED_SIZE, 140, GroupLayout.PREFERRED_SIZE).addPreferredGap(ComponentPlacement.RELATED).addComponent(panelFinger2, GroupLayout.PREFERRED_SIZE, 137, GroupLayout.PREFERRED_SIZE).addPreferredGap(ComponentPlacement.RELATED).addComponent(lblDetectedQuality).addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)));
		groupLayout.linkSize(SwingConstants.VERTICAL, new Component[] { panelFinger1, panelFinger2 });
		GroupLayout gl_panelFinger2 = new GroupLayout(panelFinger2);
		gl_panelFinger2.setHorizontalGroup(gl_panelFinger2.createParallelGroup(Alignment.LEADING).addGroup(gl_panelFinger2.createSequentialGroup().addContainerGap().addComponent(lblFinger2step1, GroupLayout.PREFERRED_SIZE, 100, GroupLayout.PREFERRED_SIZE).addGap(18).addComponent(lblFinger2step2, GroupLayout.PREFERRED_SIZE, 100, GroupLayout.PREFERRED_SIZE).addGap(18).addComponent(lblFinger2step3, GroupLayout.PREFERRED_SIZE, 100, GroupLayout.PREFERRED_SIZE).addContainerGap(32, Short.MAX_VALUE)));
		gl_panelFinger2.setVerticalGroup(gl_panelFinger2.createParallelGroup(Alignment.LEADING).addGroup(
				gl_panelFinger2.createSequentialGroup().addGroup(gl_panelFinger2.createParallelGroup(Alignment.LEADING).addComponent(lblFinger2step1, GroupLayout.PREFERRED_SIZE, 115, GroupLayout.PREFERRED_SIZE).addComponent(lblFinger2step2, GroupLayout.PREFERRED_SIZE, 115, GroupLayout.PREFERRED_SIZE).addComponent(lblFinger2step3, GroupLayout.PREFERRED_SIZE, 115, GroupLayout.PREFERRED_SIZE)).addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)));
		panelFinger2.setLayout(gl_panelFinger2);

		GroupLayout gl_panelFinger1 = new GroupLayout(panelFinger1);
		gl_panelFinger1.setHorizontalGroup(gl_panelFinger1.createParallelGroup(Alignment.LEADING).addGroup(gl_panelFinger1.createSequentialGroup().addContainerGap().addComponent(lblFinger1step1, GroupLayout.PREFERRED_SIZE, 100, GroupLayout.PREFERRED_SIZE).addGap(18).addComponent(lblFinger1step2, GroupLayout.PREFERRED_SIZE, 100, GroupLayout.PREFERRED_SIZE).addGap(18).addComponent(lblFinger1step3, GroupLayout.PREFERRED_SIZE, 100, GroupLayout.PREFERRED_SIZE).addContainerGap(23, Short.MAX_VALUE)));
		gl_panelFinger1.setVerticalGroup(gl_panelFinger1.createParallelGroup(Alignment.LEADING).addGroup(
				gl_panelFinger1.createSequentialGroup().addGroup(gl_panelFinger1.createParallelGroup(Alignment.LEADING).addComponent(lblFinger1step1, GroupLayout.PREFERRED_SIZE, 115, GroupLayout.PREFERRED_SIZE).addComponent(lblFinger1step2, GroupLayout.PREFERRED_SIZE, 115, GroupLayout.PREFERRED_SIZE).addComponent(lblFinger1step3, GroupLayout.PREFERRED_SIZE, 115, GroupLayout.PREFERRED_SIZE)).addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)));
		panelFinger1.setLayout(gl_panelFinger1);
		setLayout(groupLayout);

	}

	public void setDetectedQuality(short detectedQuality) {
		this.lblDetectedQuality.setText("DETECTED QUALITY : " + detectedQuality);
	}

	public PanelBasicBiometricOperation getPanelBasicBiometricOperation() {
		return panelBasicBiometricOperation;
	}

	/**
	 * Set the live image to the appropriate Label based on fingerNumber and
	 * step
	 * 
	 * @param morphoImage
	 * @param fingerNumber
	 * @param step
	 */
	public void setLiveImage(MorphoImage morphoImage, short fingerNumber, short step) {
		if (fingerNumber == -1 || step == -1) {
			return;
		}
		int row = fingerNumber - 1;
		int col = step - 1;
		JLabel label = lblFinger[row][col];

		BufferedImage bufferedImage = MorphoEventHandler.toBufferedImage(morphoImage.getImage(), morphoImage.getImageHeader().getNbCol(), morphoImage.getImageHeader().getNbRow());

		ImageIcon image = new ImageIcon(MorphoEventHandler.resizeImage(bufferedImage, label.getWidth(), label.getHeight()));

		label.setIcon(image);
		label.setBorder(BorderFactory.createCompoundBorder(orangeLine, lowerBevelBorder));
	}

	/**
	 * Colorify the done captures
	 * 
	 * @param fingerNumber
	 * @param step
	 */
	public void setBorderColorGreen(short fingerNumber, short step) {
		if (fingerNumber == -1 || step == -1) {
			return;
		}
		lblFinger[fingerNumber - 1][step - 1].setBorder(BorderFactory.createCompoundBorder(greenLine, lowerBevelBorder));
	}

	/**
	 * Clear JLabels for further uses
	 */
	public void clearLive() {
		int rows = 2;
		int cols = 3;
		for (int col = 0; col < cols; ++col) {
			for (int row = 0; row < rows; ++row) {
				lblFinger[row][col].setIcon(null);
				lblFinger[row][col].setBorder(lowerBevelBorder);
			}
		}
		panelBasicBiometricOperation.reinitUI();
		this.lblDetectedQuality.setText("...");
	}

	public void setCodeQuality(short codeQuality) {
		this.lblDetectedQuality.setText("CODED QUALITY : " + codeQuality);
	}
}
