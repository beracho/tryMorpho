package morpho.morphosmart.sdk.applet.trt;

import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;

import morpho.morphosmart.sdk.applet.demo.MsoDemo;


final public class ImageLoader {

	public static Image load(String imageName) {
		URL url = null;
		if (imageName.endsWith(".gif")) {
			url = MsoDemo.class.getClassLoader().getResource(imageName);
			return new ImageIcon(url).getImage();
		}
		
		InputStream in = MsoDemo.class.getClassLoader().getResourceAsStream(imageName);
		BufferedImage bi = null;
		try {
			bi = ImageIO.read(in);
		} catch (IOException e) {
			DialogUtils.showErrorMessage("", e.getMessage());
		}
		Image image = new ImageIcon(bi).getImage();
		return image;
	}
}
