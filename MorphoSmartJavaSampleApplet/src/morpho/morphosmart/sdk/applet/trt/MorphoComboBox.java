package morpho.morphosmart.sdk.applet.trt;

import java.awt.Dimension;
import java.util.Vector;

import javax.swing.ComboBoxModel;
import javax.swing.JComboBox;

public class MorphoComboBox extends JComboBox {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public MorphoComboBox() {
	}

	public MorphoComboBox(final Object items[]) {
		super(items);
	}

	public MorphoComboBox(Vector<?> items) {
		super(items);
	}

	public MorphoComboBox(ComboBoxModel aModel) {
		super(aModel);
	}

	private boolean layingOut = false;

	public void doLayout() {
		try {
			layingOut = true;
			super.doLayout();
		} finally {
			layingOut = false;
		}
	}

	public Dimension getSize() {
		Dimension dim = super.getSize();
		if (!layingOut)
			dim.width = Math.max(dim.width, getPreferredSize().width);
		return dim;
	}
}
