package morpho.morphosmart.sdk.demo.trt;

import java.io.File;
import java.io.IOException;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;


public class MorphoFileChooser extends JFileChooser{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	@Override
    public void approveSelection(){
        String savedFilePath = getSelectedFile().getAbsolutePath();
        String ext = FilesMgt.getSelectedExtension(this.getFileFilter());
        String savedFilePathTmp = savedFilePath.toLowerCase();
		if (!savedFilePathTmp.endsWith(ext)) {
	    	savedFilePath += ext;
	    }	   
	    
	    File f =  new File(savedFilePath);

	    if(!isFilenameValid(f))
 	    {
 	    	JOptionPane.showMessageDialog(this,f.getName() + " : \nThe filename is not valid.");
 	    	return;
 	    }
	    
        if(f.exists() && getDialogType() == SAVE_DIALOG)
        {        	
            int result = JOptionPane.showConfirmDialog(this,"The file "+f.getName()+" already exists, do you want to replace it?","Existing file",JOptionPane.YES_NO_OPTION);
            switch(result){
                case JOptionPane.YES_OPTION:
                    super.approveSelection();
                    return;
                case JOptionPane.NO_OPTION:
                    return;
                case JOptionPane.CLOSED_OPTION:
                    return;                    
            }
        }
        super.approveSelection();
    }      
	
	public static boolean isFilenameValid(File f) {
	    try {
	       f.getCanonicalPath();
	       return true;
	    }
	    catch (IOException e) {
	       return false;
	    }
	  }

}
