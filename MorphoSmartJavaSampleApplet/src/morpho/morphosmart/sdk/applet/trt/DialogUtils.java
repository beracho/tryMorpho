package morpho.morphosmart.sdk.applet.trt;

import javax.swing.JOptionPane;

public class DialogUtils {
	/**
	 * 
	 * @param title
	 * @param message
	 * @param messageType
	 */
	private static void showMessage(String title, String message, int messageType) {
		JOptionPane.showMessageDialog(null, message, title, messageType);
	}

	/**
	 * 
	 * @param title
	 * @param message
	 */
	public static void showErrorMessage(String title, String message) {
		showMessage(title, message, JOptionPane.ERROR_MESSAGE);
	}

	/**
	 * 
	 * @param title
	 * @param message
	 */
	public static void showWarningMessage(String title, String message) {
		showMessage(title, message, JOptionPane.WARNING_MESSAGE);
	}

	/**
	 * 
	 * @param title
	 * @param message
	 */
	public static void showInfoMessage(String title, String message) {
		showMessage(title, message, JOptionPane.INFORMATION_MESSAGE);
	}

	/**
	 * 
	 * @param title
	 * @param message
	 * @param errorCode
	 * @param internalError
	 */
	public static void showErrorMessage(String title, String message, int errorCode, int internalError) {
		String errorText = ErrorsMgt.convertSDKError(errorCode);
		String msg = String.format(message +
				"\n" + errorCode + "\t: " + errorText +
				"\n" + internalError + "\t: Internal error");
		showMessage(title, msg, JOptionPane.ERROR_MESSAGE);
	}

	/**
	 * 
	 * @param title
	 * @param message
	 * @return
	 */
	public static int showQuestionMessage(String title, String message) {
		return JOptionPane.showConfirmDialog(null, message, title, JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE);	
	}
	
	public static String formatText(String message) {
		String html = "<html><body style='text-align: center;'>" + message + "</body></html>";		
		return html;
	}

}
