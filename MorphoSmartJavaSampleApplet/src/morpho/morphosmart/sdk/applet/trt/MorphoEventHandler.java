package morpho.morphosmart.sdk.applet.trt;

import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.awt.image.WritableRaster;

import morpho.morphosmart.sdk.api.IMorphoEventHandler;
import morpho.morphosmart.sdk.api.MorphoCallbackEnrollmentStatus;
import morpho.morphosmart.sdk.api.MorphoCommandStatus;
import morpho.morphosmart.sdk.api.MorphoImage;
import morpho.morphosmart.sdk.api.MorphoImageHeader;
import morpho.morphosmart.sdk.applet.demo.PanelLive;
import morpho.morphosmart.sdk.applet.demo.TabProcess;
import morpho.morphosmart.sdk.applet.ennum.EnumMoveFinger;

public class MorphoEventHandler implements IMorphoEventHandler {

	private TabProcess tabProcess;
	private short fingerNumber, step;
	private boolean isEnroll;
	private PanelLive panelLive = null;

	/**
	 * Constructor.
	 * 
	 * @param tabProcess
	 */
	public MorphoEventHandler(TabProcess tabProcess, boolean isEnroll) {
		this.tabProcess = tabProcess;	
		this.isEnroll = isEnroll;
		panelLive = tabProcess.msoDemo.panelLive;
	}

	@Override
	public void onCommandStatusEvent(MorphoCommandStatus fingerStatus) {
		if (fingerStatus == null)
			return;

		String intruction = null;
		EnumMoveFinger move = null;
		switch (fingerStatus) {
		case MORPHO_MOVE_NO_FINGER:
			intruction = "NO FINGER";
			break;
		case MORPHO_PRESS_FINGER_HARDER:
			intruction = "PRESS HARDER";
			break;
		case MORPHO_LATENT:
			intruction = "LATENT";
			break;
		case MORPHO_FINGER_MISPLACED:
			intruction = "FINGER MISPLACED";
			break;
		case MORPHO_LAST_MESSAGE:
			intruction = "LAST MESSAGE";
			break;
		case MORPHO_FINGER_OK:
			intruction = "FINGER OK";
			break;
		case MORPHO_FINGER_DETECTED:
			intruction = "FINGER DETECTED";
			if (MorphoInfo.isFVP)
			break;
		case MORPHO_LIVE_OK:
			intruction = "LIVE OK";
			break;
		case MORPHO_REMOVE_FINGER:
			intruction = "REMOVE FINGER";
			if (isEnroll) {
				tabProcess.setBorderColorGreen(fingerNumber, step);
			}
			break;
		case MORPHO_MOVE_FINGER_UP:
			intruction = "MOVE UP";
			move = EnumMoveFinger.MOVE_UP;
			break;
		case MORPHO_MOVE_FINGER_DOWN:
			intruction = "MOVE DOWN";
			move = EnumMoveFinger.MOVE_DOWN;
			break;
		case MORPHO_MOVE_FINGER_LEFT:
			intruction = "MOVE LEFT";
			move = EnumMoveFinger.MOVE_LEFT;
			break;
		case MORPHO_MOVE_FINGER_RIGHT:
			intruction = "MOVE RIGHT";
			move = EnumMoveFinger.MOVE_RIGHT;
			break;
		default:
			move = null;
			break;
		}

		panelLive.setInstruction(intruction);
		panelLive.setStepsImage(move);		
	}

	@Override
	public void onImageEvent(MorphoImage morphoImage) {
		if (morphoImage != null) {
			if (isEnroll) {
				tabProcess.setLiveImage(morphoImage, fingerNumber, step);
			}
			
			panelLive.setLiveImage(morphoImage);

			MorphoImageHeader imgHeader = morphoImage.getImageHeader();
			int nbCol = imgHeader.getNbCol();
			int nbRow = imgHeader.getNbRow();
			int resX = imgHeader.getResX();
			int resY = imgHeader.getResY();
			int bitPerPixel = imgHeader.getNbBitsPerPixel();
			panelLive.setCurrentImageInfo(nbCol, nbRow,
					resX, resY, bitPerPixel);
		} else {
			System.err.println("onImageEvent, MorphoImage is null");
		}
	}

	@Override
	public void onEnrolmentEvent(MorphoCallbackEnrollmentStatus enrolmentEvent) {
		if (enrolmentEvent != null) {
			fingerNumber = enrolmentEvent.getNbFinger();
			step = enrolmentEvent.getNbCapture();
		} else {
			System.err.println("onEnrolmentEvent, enrolmentEvent is null");
		}
	}

	@Override
	public void onImageLastEvent(MorphoImage morphoImage) {
		if (morphoImage != null) {
			panelLive.setLiveImage(morphoImage);
		} else {
			System.err.println("onImageLastEvent MorphoImage is null");
		}

	}

	@Override
	public void onCodeQualityEvent(short quality) {
		if (isEnroll) {
			tabProcess.setCodeQuality(quality);
		}
		panelLive.setScore(quality);
	}

	@Override
	public void onDetectQualityEvent(short quality) {
		if (isEnroll) {
			tabProcess.setDetectedQuality(quality);
		}
		panelLive.setScore(quality);
	}

	public static BufferedImage toBufferedImage(final byte[] buffer, final int width,
			final int height) {
		BufferedImage bufferedImage = null;
		try {
			bufferedImage = new BufferedImage(width, height,
					BufferedImage.TYPE_BYTE_GRAY);
			final WritableRaster raster = bufferedImage.getRaster();
			final int[] gray = new int[1];
			int i = 0, x;
			for (int y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					gray[0] = buffer[i++];
					raster.setPixel(x, y, gray);
				}
			}
		} catch (final NullPointerException e) {
			bufferedImage = null;
			e.printStackTrace();
		} catch (final ArrayIndexOutOfBoundsException e) {
			bufferedImage = null;
		}
		return bufferedImage;
	}
	
	public static BufferedImage resizeImage(BufferedImage bufferedImage, int width, int height) {
		
		int bufferedImageWidth = bufferedImage.getWidth();
		int bufferedImageHeight = bufferedImage.getHeight();
		double ratio = 0;
		int widthZM;
		int heightZM;
		
		if(
			(height >= bufferedImageHeight && width >= bufferedImageWidth) ||
			(height < bufferedImageHeight && width < bufferedImageWidth)
		)
		{
			ratio = Math.min(height/(double)bufferedImageHeight, width/(double)bufferedImageWidth);
		}
		else if (height < bufferedImageHeight)
		{
			ratio = height/(double)bufferedImageHeight;
		}
		else
		{
			ratio = width/(double)bufferedImageWidth;
		}
		
		widthZM = (int) (bufferedImageWidth * ratio);
		heightZM = (int) (bufferedImageHeight * ratio);
		
		int x = Math.abs(widthZM - width)/2;
		int y = Math.abs(heightZM - height)/2;
		
        BufferedImage bi = new BufferedImage(width, height, BufferedImage.TRANSLUCENT);
        Graphics2D g2d = (Graphics2D) bi.createGraphics();
        g2d.addRenderingHints(new RenderingHints(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY));
        g2d.drawImage(bufferedImage, x, y, widthZM, heightZM, null);
        g2d.dispose();
        return bi;
    }

}
