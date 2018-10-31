package morpho.morphosmart.sdk.demo.dialog;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.BevelBorder;

import morpho.morphosmart.sdk.api.MorphoImage;
import morpho.morphosmart.sdk.demo.trt.MorphoEventHandler;

import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.image.BufferedImage;
import java.awt.Dialog.ModalExclusionType;

public class DialogGetImage extends JFrame {
	private BufferedImage bufferedImage;

	public DialogGetImage(String title, byte[] image, int nbCol, int nbRow) {
		setModalExclusionType(ModalExclusionType.APPLICATION_EXCLUDE);

		bufferedImage = MorphoEventHandler.toBufferedImage(image, nbCol, nbRow);

		if (null != bufferedImage) {
			BorderLayout borderLayout = (BorderLayout) getContentPane().getLayout();
			borderLayout.setVgap(10);
			borderLayout.setHgap(10);
			setAlwaysOnTop(true);
			setTitle(title);
			setSize(750, 750);
			setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
			setLocationRelativeTo(null);

			JPanel panel = new JPanel();
			getContentPane().add(panel, BorderLayout.SOUTH);

			JButton okButton = new JButton("      OK      ");
			okButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					dispose();
				}
			});
			panel.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
			panel.add(okButton);

			final JLabel imageLabel = new JLabel("...");
			getContentPane().add(imageLabel, BorderLayout.CENTER);
			imageLabel.setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));

			getContentPane().addComponentListener(new ComponentAdapter() {
				@Override
				public void componentResized(ComponentEvent e) {
					int width = Math.max(imageLabel.getSize().width - 10, 10);
					int height = Math.max(imageLabel.getSize().height - 10, 10);

					ImageIcon image = new ImageIcon(MorphoEventHandler.resizeImage(bufferedImage,
							width, height));
					imageLabel.setIcon(image);
				}
			});
		} else {
			dispose();
		}
	}
	
	public DialogGetImage(String title, MorphoImage morphoImage) {
		new DialogGetImage(title, morphoImage.getImage(), morphoImage.getImageHeader().getNbCol(), morphoImage.getImageHeader().getNbRow());
	}

	private static final long serialVersionUID = 1L;
}
