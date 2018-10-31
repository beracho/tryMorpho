package morpho.morphosmart.sdk.applet.trt;

import java.io.File;

import javax.swing.filechooser.FileFilter;

public class TemplateFileFilter extends FileFilter {

	private String description;

	private String extension;

	public TemplateFileFilter(String description, String extension) {
		if (description == null || extension == null) {
			throw new NullPointerException(
					"Description (or extension) can not be null.");
		}

		this.description = description;
		this.extension = extension;
	}

	@Override
	public boolean accept(File file) {
		if (file.isDirectory()) {
			return true;
		}
		String fileName = file.getName().toLowerCase();

		return fileName.endsWith(extension);
	}

	@Override
	public String getDescription() {
		return description;
	}

}
