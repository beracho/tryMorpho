package morpho.morphosmart.sdk.demo.trt;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.StringTokenizer;

import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;

import morpho.morphosmart.sdk.api.MorphoDevice;
import morpho.morphosmart.sdk.demo.TemplateFileFilter;
import morpho.morphosmart.sdk.demo.UserData;
import morpho.morphosmart.sdk.demo.dialog.DialogResultWindow;
import morpho.morphosmart.sdk.demo.dialog.DialogUtils;

/**
 * Files Management.
 */
public class FilesMgt {

	public static String fileChooserDirectory = ".";
	private static String lastChooserFile = "";

	public static boolean saveFFDLog = false;
	public static String ffdLogFilePath = "";

	public static void checkFFDLogParam(String sn) {
		try {
			InputStream is = new FileInputStream("MSO_Demo.ini");
			BufferedReader reader = new BufferedReader(new InputStreamReader(is));
			String line = null;
			String sectionName = null;
			Hashtable<String, String> section = null;
			Hashtable<String, String> sectionTmp = null;
			Hashtable<String, Hashtable<String,String>> _sections = new Hashtable<String, Hashtable<String,String>>();
			while ((line = reader.readLine()) != null) {
				line = line.trim();
				if (null == sectionName) {
					if ((!line.startsWith("[")) || (!line.endsWith("]"))) {
						reader.close();
						return;
					}

					sectionName = line.substring(1, line.length() - 1).trim();
					if (_sections.get(sectionName) == null) {
						section = new Hashtable<String, String>();
						_sections.put(sectionName, section);
						sectionTmp = (Hashtable<String, String>) _sections.get(sectionName);
					}
				} else {
					if (line.startsWith("[")) {
						if (!line.endsWith("]")) {
							reader.close();
							return;
						}
						sectionName = line.substring(1, line.length() - 1).trim();
						if (_sections.get(sectionName) == null) {
							section = new Hashtable<String, String>();
							_sections.put(sectionName, section);
							sectionTmp = (Hashtable<String, String>) _sections.get(sectionName);
						}
					} else {
						line = line.trim();
						if (!line.startsWith(";") && line.length() > 0) {
							StringTokenizer st = new StringTokenizer(line, "=");
							if (st.countTokens() != 2) {
								reader.close();
								return;
							}
							String key = st.nextToken().trim();
							for (int index = 0; index < key.length(); index++) {
								if (Character.isWhitespace(key.charAt(index))) {
									reader.close();
									return;
								}
							}

							String value = st.nextToken().trim();

							sectionTmp.put(key, value);
						}
					}
				}
			}
			is.close();

			sectionTmp = (Hashtable<String, String>) _sections.get("Setup");

			if (sectionTmp != null) {
				ffdLogFilePath = (String) section.get("LogFilePath");
				if (ffdLogFilePath != null && ffdLogFilePath != "") {
					File f = new File(ffdLogFilePath);
					if (f.exists() && f.isDirectory()) {
						saveFFDLog = true;
						ffdLogFilePath = f.getAbsolutePath() + "\\MSO_" + sn + "_Audit.log";
					}
				}
			}

		} catch (FileNotFoundException e) {
		} catch (IOException e) {
		}
	}

	public static void appendFFDLog(MorphoDevice mDevice) {
		if (!saveFFDLog)
			return;

		ArrayList<String> ffdLog = new ArrayList<String>();
		int ret = mDevice.getFFDLogs(ffdLog);

		if (ret == 0) {
			try {
				File file = new File(ffdLogFilePath);
				if (!file.exists()) {
					file.createNewFile();
				}

				FileWriter fileWritter = new FileWriter(file.getName(), true);
				BufferedWriter bufferWritter = new BufferedWriter(fileWritter);
				for (int i = 0; i < ffdLog.size(); ++i) {
					bufferWritter.write(ffdLog.get(i) + "\n");
				}
				bufferWritter.close();

			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * 
	 * @param action
	 * @param msoDemo
	 * @param selectionMode
	 * @return
	 */
	public static ArrayList<String> getTemplateFiles(String action, final boolean isMultipleSelectionMode, JFrame msoDemo) {
		ArrayList<String> listFiles = new ArrayList<String>();

		JFileChooser fc = new JFileChooser() {
			private static final long serialVersionUID = 1L;

			@Override
			public void approveSelection() {
				if (isMultipleSelectionMode) {
					File[] f = getSelectedFiles();
					if (f != null) {
						for (int i = 0; i < f.length; ++i) {
							if (!f[i].exists()) {
								DialogUtils.showErrorMessage("File Chooser", "File does not exist");
								super.cancelSelection();
								return;
							}
						}
					}
				} else {
					File f = getSelectedFile();
					if (!f.exists()) {
						DialogUtils.showErrorMessage("File Chooser", "File does not exist");
						super.cancelSelection();
						return;
					}
				}
				super.approveSelection();
			}
		};

		fc.setDialogType(JFileChooser.OPEN_DIALOG);

		if (isMultipleSelectionMode) {
			fc.setMultiSelectionEnabled(isMultipleSelectionMode);
		}

		fc.setCurrentDirectory(new File(fileChooserDirectory));

		FileFilter pks = new TemplateFileFilter("Pks Files (.pks)", ".pks");
		FileFilter tkb = new TemplateFileFilter("Token Bio Files (.tkb)", ".tkb");
		FileFilter pkc = new TemplateFileFilter("PkComp Files (.pkc)", ".pkc");
		FileFilter pkcn = new TemplateFileFilter("PkComp Norm Files (.pkcn)", ".pkcn");
		FileFilter pkl = new TemplateFileFilter("PkLite Files (.pkl)", ".pkl");
		FileFilter pklite = new TemplateFileFilter("PkLite Files (.pklite)", ".pklite");
		FileFilter pkmat = new TemplateFileFilter("PkMat Files (.pkmat)", ".pkmat");
		FileFilter pkm = new TemplateFileFilter("PkMat Files (.pkm)", ".pkm");
		FileFilter pkmn = new TemplateFileFilter("PkMat Norm Files (.pkmn)", ".pkmn");
		FileFilter ansiFmr = new TemplateFileFilter("ANSI 378 FMR Files (.ansi-fmr)", ".ansi-fmr");
		FileFilter ansiFmr2009 = new TemplateFileFilter("ANSI 378 FMR 2009 Files (.ansi-fmr-2009)", ".ansi-fmr-2009");
		FileFilter minexA = new TemplateFileFilter("MINEX-A Files (.minex-a)", ".minex-a");
		FileFilter isoFmr = new TemplateFileFilter("ISO 19794-2 FMR Files (.iso-fmr)", ".iso-fmr");
		FileFilter isoFmr2011 = new TemplateFileFilter("ISO 19794-2 FMR 2011 Files (.iso-fmr-2011)", ".iso-fmr-2011");
		FileFilter isoFmcNs = new TemplateFileFilter("ISO 19794 FMC Normal Size Files (.iso-fmc-ns)", ".iso-fmc-ns");
		FileFilter isoFmcCs = new TemplateFileFilter("ISO 19794 FMC Compact Size Files (.iso-fmc-cs)", ".iso-fmc-cs");
		FileFilter pkv10 = new TemplateFileFilter("PKV10 Files (.pkv10)", ".pkv10");
		FileFilter dinCs = new TemplateFileFilter("DIN-V66400 Compact Size Files (.din-cs)", ".din-cs");
		FileFilter fvp = new TemplateFileFilter("Multimodal Files (.fvp)", ".fvp");
		FileFilter fvpM = new TemplateFileFilter("Multimodal Files (.fvp-m)", ".fvp-m");
		FileFilter all = new FileNameExtensionFilter("All Biometric Files", "iso-fmr-2011", "ansi-fmr-2009", "pks", "tkb", "pkc", "pkcn", "pkmat", "pkm", "pkmn", "pkl", "pklite", "ansi-fmr", "minex-a", "iso-fmr", "iso-fmc-ns", "iso-fmc-cs", "pkv10", "moc", "din-cs", "fvp", "fvp-m");

		fc.setAcceptAllFileFilterUsed(false);
		
		fc.addChoosableFileFilter(pks);
		fc.addChoosableFileFilter(tkb);
		fc.addChoosableFileFilter(pkc);
		fc.addChoosableFileFilter(pkcn);
		fc.addChoosableFileFilter(pkl);
		fc.addChoosableFileFilter(pklite);
		fc.addChoosableFileFilter(pkmat);
		fc.addChoosableFileFilter(pkm);
		fc.addChoosableFileFilter(pkmn);
		fc.addChoosableFileFilter(ansiFmr);
		fc.addChoosableFileFilter(ansiFmr2009);
		fc.addChoosableFileFilter(minexA);
		fc.addChoosableFileFilter(isoFmr);
		fc.addChoosableFileFilter(isoFmr2011);
		fc.addChoosableFileFilter(isoFmcNs);
		fc.addChoosableFileFilter(isoFmcCs);
		fc.addChoosableFileFilter(pkv10);
		fc.addChoosableFileFilter(dinCs);
		fc.addChoosableFileFilter(fvp);
		fc.addChoosableFileFilter(fvpM);
		fc.addChoosableFileFilter(all);

		int ret = fc.showOpenDialog(msoDemo);

		if (ret == JFileChooser.APPROVE_OPTION) {
			if (isMultipleSelectionMode) {
				File[] files = fc.getSelectedFiles();

				if (files != null) {
					for (int i = 0; i < files.length; ++i) {
						listFiles.add(files[i].getAbsolutePath());
					}
				}
			} else {
				listFiles.add(fc.getSelectedFile().getAbsolutePath());
			}
		} else {
			DialogUtils.showInfoMessage(action, action + " aborted. Press OK to continue...");
		}

		fileChooserDirectory = fc.getCurrentDirectory().getAbsolutePath();

		return listFiles;
	}

	/**
	 * Get file name from path.
	 * 
	 * @param path
	 * @return file name
	 */
	public static String getFileNameFromPath(String path) {
		return new File(path).getName();
	}

	/**
	 * Get the selected filter type in FileChooser.
	 * 
	 * @param fileFilter
	 * @return the extension of filter file
	 */
	public static String getSelectedExtension(FileFilter fileFilter) {
		try {
			String desc = fileFilter.getDescription();
			return desc.substring(desc.indexOf("(") + 1, desc.indexOf(")")).toLowerCase();			
		} catch (Exception e) {
			return "";
		}
	}

	/**
	 * 
	 * @param parentFrame
	 * @return
	 */
	public static byte[] getAntireplayCounterFromFile(JFrame parentFrame) {
		FileFilter a12 = new TemplateFileFilter("PKCS#12 Files (.a12)", ".a12");
		return readFile(parentFrame, a12);
	}

	/**
	 * 
	 * @param parentFrame
	 * @param fileFilter
	 * @return
	 */
	private static byte[] readFile(JFrame parentFrame, FileFilter fileFilter) {
		byte[] data = null;

		JFileChooser fc = new MorphoFileChooser();

		fc.setDialogType(JFileChooser.OPEN_DIALOG);
		fc.setCurrentDirectory(new File(fileChooserDirectory));
		fc.setAcceptAllFileFilterUsed(false);
		fc.addChoosableFileFilter(fileFilter);
		int ret = fc.showOpenDialog(parentFrame);

		if (ret == JFileChooser.APPROVE_OPTION) {
			fileChooserDirectory = fc.getCurrentDirectory().getAbsolutePath();
			File f = fc.getSelectedFile();
			lastChooserFile = f.getAbsolutePath();
			try {
				InputStream is = new FileInputStream(lastChooserFile);
				data = new byte[is.available()];
				is.read(data);
				is.close();
			} catch (FileNotFoundException e) {
				DialogUtils.showErrorMessage("Read File", e.getMessage());
			} catch (IOException e) {
				DialogUtils.showErrorMessage("Read File", e.getMessage());
			}
		}

		return data;
	}

	/**
	 * 
	 * @param parentFrame
	 * @return
	 */
	public static byte[] readKSFile(JFrame parentFrame) {
		FileFilter bin = new TemplateFileFilter("Ks Files (.bin)", ".bin");
		return readFile(parentFrame, bin);
	}
	
	/**
	 * 
	 * @param parentFrame
	 * @return
	 */
	public static byte[] readDataFile(String msg,String extension, JFrame parentFrame) {
		FileFilter bin = new TemplateFileFilter(msg, extension);
		return readFile(parentFrame, bin);
	}

	/**
	 * 
	 * @param parentFrame
	 * @return
	 */
	public static byte[] readHostDerFile(JFrame parentFrame) {
		FileFilter der = new TemplateFileFilter("host certificate (der) Files (.der)", ".der");
		return readFile(parentFrame, der);
	}

	/**
	 * 
	 * @param parentFrame
	 * @return
	 */
	public static byte[] readHostKeyFile(JFrame parentFrame) {
		FileFilter key = new TemplateFileFilter("host certificate (key) Files (.key)", ".key");
		return readFile(parentFrame, key);
	}

	/**
	 * 
	 * @param parentFrame
	 * @return
	 */
	public static byte[] readPKCS12File(JFrame parentFrame) {
		FileFilter p12 = new TemplateFileFilter("PKCS#12 Files (.p12)", ".p12");
		return readFile(parentFrame, p12);
	}

	/**
	 * 
	 * @param parentFrame
	 * @return
	 */
	public static UserData readTKBFile(JFrame parentFrame) {
		UserData tkbData = null;

		JFileChooser fc = new MorphoFileChooser();

		fc.setDialogType(JFileChooser.OPEN_DIALOG);
		fc.setCurrentDirectory(new File(fileChooserDirectory));
		FileFilter der = new TemplateFileFilter("Token Bio Files (.tkb)", ".tkb");
		fc.setAcceptAllFileFilterUsed(false);
		fc.addChoosableFileFilter(der);
		int ret = fc.showOpenDialog(parentFrame);

		if (ret == JFileChooser.APPROVE_OPTION) {
			fileChooserDirectory = fc.getCurrentDirectory().getAbsolutePath();
			File f = fc.getSelectedFile();
			if (f.getAbsolutePath().endsWith(".tkb")) {
				tkbData = UsersMgt.getUserDataFromFile(f.getAbsolutePath());
			}
		}

		return tkbData;
	}

	/**
	 * 
	 * @param parentFrame
	 * @return
	 */
	public static byte[] readCertificateFile(JFrame parentFrame) {
		FileFilter der = new TemplateFileFilter("Certificate Files (.der)", ".der");
		return readFile(parentFrame, der);
	}

	/**
	 * 
	 * @param parentFrame
	 * @return
	 */
	public static byte[] readMMIFile(JFrame parentFrame) {
		FileFilter csv = new TemplateFileFilter("Comma-Separated Values (.csv)", ".csv");
		return readFile(parentFrame, csv);
	}

	/**
	 * 
	 * @param parentFrame
	 * @param fileName
	 * @param mmifile
	 * @return
	 */
	public static boolean saveMMIFile(JFrame parentFrame, String fileName, byte[] mmifile) {
		boolean ret = false;

		JFileChooser fileChooser = new JFileChooser();
		fileChooser.setSelectedFile(new File(fileName));
		FileFilter csv = new TemplateFileFilter("Comma-Separated Values (.csv)", ".csv");
		fileChooser.setAcceptAllFileFilterUsed(false);
		fileChooser.addChoosableFileFilter(csv);
		fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));

		int userSelection = fileChooser.showSaveDialog(parentFrame);
		if (userSelection == JFileChooser.APPROVE_OPTION) {
			FilesMgt.fileChooserDirectory = fileChooser.getCurrentDirectory().getAbsolutePath();

			File fileToSave = fileChooser.getSelectedFile();
			String savedFilePath = fileToSave.getAbsolutePath();

			String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
			String savedFilePathTmp = savedFilePath.toLowerCase();
			if (!savedFilePathTmp.endsWith(ext)) {
				savedFilePath += ext;
			}
			// save into file
			FileOutputStream fos;

			try {
				fos = new FileOutputStream(savedFilePath);
				fos.write(mmifile);
				fos.close();
				ret = true;
			} catch (FileNotFoundException fnfe) {
				DialogUtils.showErrorMessage("Save MMI File", fnfe.getMessage());
				ret = false;
			} catch (IOException ioe) {
				DialogUtils.showErrorMessage("Save MMI File", ioe.getMessage());
				ret = false;
			}
		} else {
			new DialogResultWindow(parentFrame, "Failed", "Saving MMI file", "aborted by user.", "", "").setVisible(true);
		}

		return ret;
	}

	/**
	 * 
	 * @param parentFrame
	 * @param fileName
	 * @param certificate
	 * @return
	 */
	public static boolean saveCertificateFile(JFrame parentFrame, String fileName, byte[] certificate) {
		boolean ret = false;

		JFileChooser fileChooser = new JFileChooser();
		fileChooser.setSelectedFile(new File(fileName));
		FileFilter der = new TemplateFileFilter("Certificate Files (.der)", ".der");
		fileChooser.setAcceptAllFileFilterUsed(false);
		fileChooser.addChoosableFileFilter(der);
		fileChooser.setCurrentDirectory(new File(FilesMgt.fileChooserDirectory));

		int userSelection = fileChooser.showSaveDialog(parentFrame);
		if (userSelection == JFileChooser.APPROVE_OPTION) {

			FilesMgt.fileChooserDirectory = fileChooser.getCurrentDirectory().getAbsolutePath();

			File fileToSave = fileChooser.getSelectedFile();
			String savedFilePath = fileToSave.getAbsolutePath();

			String ext = FilesMgt.getSelectedExtension(fileChooser.getFileFilter());
			String savedFilePathTmp = savedFilePath.toLowerCase();
			if (!savedFilePathTmp.endsWith(ext)) {
				savedFilePath += ext;
			}
			// save into file
			try {
				writeFile(savedFilePath, certificate);
				ret = true;
			} catch (IOException ioe) {
				DialogUtils.showErrorMessage("Save PKS File", ioe.getMessage());
				ret = false;
			}
		} else {
			new DialogResultWindow(parentFrame, "Failed", "Saving Certificate file", "aborted by user.", "", "").setVisible(true);
		}

		return ret;
	}
	
	/**
	 * 
	 * @param parentFrame
	 * @return
	 */
	public static byte[] readCryptedFile(JFrame parentFrame) {
		FileFilter csv = new TemplateFileFilter("Crypted files (.crypt)", ".crypt");
		return readFile(parentFrame, csv);
	}
	
	public static String getLastChooserFile() {
		return lastChooserFile;
	}
	
	public static void writeFile(String path, byte[] data) throws IOException {
		FileOutputStream fos = new FileOutputStream(path);
		fos.write(data);
		fos.close();
	}
}
