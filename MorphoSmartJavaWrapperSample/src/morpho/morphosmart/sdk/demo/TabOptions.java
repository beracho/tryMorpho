package morpho.morphosmart.sdk.demo;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.LayoutStyle.ComponentPlacement;

import morpho.morphosmart.sdk.api.MorphoCallbackCommand;
import morpho.morphosmart.sdk.api.MorphoSmartSDK;
import morpho.morphosmart.sdk.demo.dialog.DialogUtils;
import morpho.morphosmart.sdk.demo.ennum.EnumSensorWindowPosition;
import morpho.morphosmart.sdk.demo.trt.MorphoComboBox;
import morpho.morphosmart.sdk.demo.trt.MorphoInfo;

public class TabOptions extends JPanel {

	private static final long serialVersionUID = 1L;

	protected PanelDatabaseMgt panelDatabaseMgt;

	JCheckBox cbImageFullResolution;
	JCheckBox cbImageViewer;
	JCheckBox cbAsyncPositioningCommand;
	JCheckBox cbAsyncEnrollmentCommand;
	JCheckBox cbAsyncDetectQuality;
	JCheckBox cbAsyncCodeQuality;
	JCheckBox cbExportMatchingPk;
	JCheckBox cbWakeupWithLed;
	JCheckBox cbConfigKeyUser;
	JComboBox comboSensorWindowPosition;

	/**
	 * Create the panel.
	 * 
	 * @param panelDatabaseMgt
	 */
	public TabOptions(PanelDatabaseMgt panelDatabaseMgt) {
		this.panelDatabaseMgt = panelDatabaseMgt;

		cbImageFullResolution = new JCheckBox("Image Full Resolution Viewer");
		cbImageFullResolution.setSelected(true);
		cbImageFullResolution.setEnabled(false);

		cbImageViewer = new JCheckBox("Image Viewer");
		cbImageViewer.setSelected(true);

		cbAsyncPositioningCommand = new JCheckBox("Async Positioning Command"); 
		cbAsyncPositioningCommand.setSelected(true);

		cbAsyncEnrollmentCommand = new JCheckBox("Async Enrollment Command");
		cbAsyncEnrollmentCommand.setSelected(true);

		cbAsyncDetectQuality = new JCheckBox("Async Detect Quality");
		cbAsyncDetectQuality.setSelected(true);

		cbAsyncCodeQuality = new JCheckBox("Async Code Quality");
		cbAsyncCodeQuality.setSelected(true);

		cbConfigKeyUser  = new JCheckBox("Power Supply Config");
		cbExportMatchingPk = new JCheckBox("Export Matching Pk Number");
		cbWakeupWithLed = new JCheckBox("WakeUp with led off");

		JLabel lbl1 = new JLabel("Sensor Window Position");
		if (MorphoInfo.isFVP) {
			comboSensorWindowPosition = new MorphoComboBox();  
			comboSensorWindowPosition.setEnabled(false);
		} else {
			comboSensorWindowPosition = new MorphoComboBox(EnumSensorWindowPosition.toStringArray());
		}

		GroupLayout groupLayout = new GroupLayout(this);
		groupLayout.setHorizontalGroup(groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(
						groupLayout
								.createSequentialGroup()
								.addContainerGap()
								.addGroup(
										groupLayout
												.createParallelGroup(Alignment.LEADING)
												.addGroup(
														groupLayout
																.createSequentialGroup()
																.addGroup(
																		groupLayout
																				.createParallelGroup(Alignment.LEADING)
																				.addComponent(cbImageFullResolution)
																				.addGroup(
																						groupLayout.createSequentialGroup().addGroup(groupLayout.createParallelGroup(Alignment.LEADING).addComponent(cbExportMatchingPk).addComponent(cbWakeupWithLed)).addPreferredGap(ComponentPlacement.UNRELATED) 
																								.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
																										
																										.addComponent(cbConfigKeyUser , GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
																										.addComponent(lbl1, -1, 150, Short.MAX_VALUE)
																										.addComponent(comboSensorWindowPosition, 0, 150, Short.MAX_VALUE))))
																.addGap(155)).addGroup(groupLayout.createSequentialGroup().addComponent(cbImageViewer).addContainerGap(233, Short.MAX_VALUE))
												.addGroup(groupLayout.createSequentialGroup().addComponent(cbAsyncPositioningCommand).addContainerGap(165, Short.MAX_VALUE)).addGroup(groupLayout.createSequentialGroup().addComponent(cbAsyncEnrollmentCommand).addContainerGap(167, Short.MAX_VALUE)).addGroup(groupLayout.createSequentialGroup().addComponent(cbAsyncDetectQuality).addContainerGap(197, Short.MAX_VALUE))
												.addGroup(groupLayout.createSequentialGroup().addComponent(cbAsyncCodeQuality).addContainerGap(205, Short.MAX_VALUE)))));
		
		
		groupLayout.setVerticalGroup(groupLayout.createParallelGroup(Alignment.LEADING).addGroup(
				groupLayout.createSequentialGroup().addContainerGap().addComponent(cbImageFullResolution).addPreferredGap(ComponentPlacement.RELATED).addComponent(cbImageViewer).addPreferredGap(ComponentPlacement.RELATED).addComponent(cbAsyncPositioningCommand).addPreferredGap(ComponentPlacement.RELATED).addComponent(cbAsyncEnrollmentCommand).addPreferredGap(ComponentPlacement.RELATED).addComponent(cbAsyncDetectQuality).addPreferredGap(ComponentPlacement.RELATED)
						.addGroup(
								
								groupLayout.createParallelGroup(Alignment.BASELINE).
								addComponent(cbAsyncCodeQuality).
								addComponent(cbConfigKeyUser)).
								addGap(10).
								addGroup(
										groupLayout.createParallelGroup(Alignment.BASELINE).
										addComponent(cbExportMatchingPk).
										addComponent(lbl1)).
										addPreferredGap(ComponentPlacement.RELATED).
										addGroup(
												groupLayout.createParallelGroup(Alignment.LEADING).
												addComponent(comboSensorWindowPosition, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE).
												addComponent(cbWakeupWithLed)).
										addGap(19)));
		setLayout(groupLayout);

		// add listeners
		cbImageViewer.addActionListener(cbImageViewerItemListener);
		cbImageFullResolution.addActionListener(cbImageFullResolutionItemListener);
		comboSensorWindowPosition.addActionListener(comboSensorWindowPositionChange);
		cbWakeupWithLed.addActionListener(cbWakeupWithLedItemListener);
		cbAsyncPositioningCommand.addActionListener(cbAsyncPositioningCommandItemListener);
		cbAsyncEnrollmentCommand.addActionListener(cbAsyncEnrollmentCommandItemListener);
		cbAsyncDetectQuality.addActionListener(cbAsyncDetectQualityItemListener);
		cbAsyncCodeQuality.addActionListener(cbAsyncCodeQualityItemListener);
		cbConfigKeyUser.addActionListener(cbConfigKeyUserItemListener);
	}

	//
	// Listeners
	//

	private ActionListener comboSensorWindowPositionChange = new ActionListener() {

		@Override
		public void actionPerformed(ActionEvent event) {
			int paramValue = comboSensorWindowPosition.getSelectedIndex();
			int ret = panelDatabaseMgt.msoDemo.getMorphoDeviceInstance().setConfigParam(MorphoSmartSDK.CONFIG_SENSOR_WIN_POSITION_TAG, paramValue);

			if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("SetConfigParam", "An error occured while calling MorphoDevice.setConfigParam() function", ret, panelDatabaseMgt.msoDemo.getMorphoDeviceInstance().getInternalError());
			}
		}

	};

	private ActionListener cbAsyncCodeQualityItemListener = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent event) {
			if (cbAsyncCodeQuality.isSelected()) {
				panelDatabaseMgt.msoDemo.callbackMask |= MorphoCallbackCommand.MORPHO_CALLBACK_CODEQUALITY.swigValue();
			} else {
				panelDatabaseMgt.msoDemo.callbackMask &= ~MorphoCallbackCommand.MORPHO_CALLBACK_CODEQUALITY.swigValue();
			}
		}
	};

	private ActionListener cbAsyncDetectQualityItemListener = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent event) {
			if (cbAsyncDetectQuality.isSelected()) {
				panelDatabaseMgt.msoDemo.callbackMask |= MorphoCallbackCommand.MORPHO_CALLBACK_DETECTQUALITY.swigValue();
			} else {
				panelDatabaseMgt.msoDemo.callbackMask &= ~MorphoCallbackCommand.MORPHO_CALLBACK_DETECTQUALITY.swigValue();
			}
		}
	};

	private ActionListener cbAsyncEnrollmentCommandItemListener = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent event) {
			if (cbAsyncEnrollmentCommand.isSelected()) {
				panelDatabaseMgt.msoDemo.callbackMask |= MorphoCallbackCommand.MORPHO_CALLBACK_ENROLLMENT_CMD.swigValue();
			} else {
				panelDatabaseMgt.msoDemo.callbackMask &= ~MorphoCallbackCommand.MORPHO_CALLBACK_ENROLLMENT_CMD.swigValue();
			}
		}
	};

	private ActionListener cbAsyncPositioningCommandItemListener = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent event) {
			if (cbAsyncPositioningCommand.isSelected()) {
				panelDatabaseMgt.msoDemo.callbackMask |= MorphoCallbackCommand.MORPHO_CALLBACK_COMMAND_CMD.swigValue();
			} else {
				panelDatabaseMgt.msoDemo.callbackMask &= ~MorphoCallbackCommand.MORPHO_CALLBACK_COMMAND_CMD.swigValue();
			}
		}
	};

	private ActionListener cbImageViewerItemListener = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent event) {
			if (cbImageViewer.isSelected()) {
				cbImageFullResolution.setSelected(true);
				cbImageFullResolution.setEnabled(false);
				panelDatabaseMgt.msoDemo.callbackMask |= MorphoCallbackCommand.MORPHO_CALLBACK_IMAGE_CMD.swigValue();
			} else {
				cbImageFullResolution.setSelected(false);
				cbImageFullResolution.setEnabled(true);
				panelDatabaseMgt.msoDemo.callbackMask &= ~MorphoCallbackCommand.MORPHO_CALLBACK_IMAGE_CMD.swigValue();
			}
		}
	};

	private ActionListener cbWakeupWithLedItemListener = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent event) {
			if (cbWakeupWithLed.isSelected()) {
				panelDatabaseMgt.msoDemo.wakeUpMode = MorphoSmartSDK.MORPHO_WAKEUP_LED_OFF;
			} else {
				panelDatabaseMgt.msoDemo.wakeUpMode = 0;
			}
		}
	};

	private ActionListener cbImageFullResolutionItemListener = new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent event) {
			if (cbImageFullResolution.isSelected()) {
				cbImageViewer.setSelected(false);
				cbImageViewer.setEnabled(false);
				panelDatabaseMgt.msoDemo.callbackMask |= MorphoCallbackCommand.MORPHO_CALLBACK_LAST_IMAGE_FULL_RES_CMD.swigValue();
			} else {
				cbImageViewer.setSelected(false);
				cbImageViewer.setEnabled(true);
				panelDatabaseMgt.msoDemo.callbackMask &= ~MorphoCallbackCommand.MORPHO_CALLBACK_LAST_IMAGE_FULL_RES_CMD.swigValue();
			}
		}
	};
	
	private ActionListener cbConfigKeyUserItemListener = new ActionListener() {
				
		@Override
		public void actionPerformed(ActionEvent event) {
			int ret;
			if (cbConfigKeyUser.isSelected())
			{
				DialogUtils.showInfoMessage("INFORMATION",  "Caution, for this value, the device should be powered over 4,5V.\n You must restart the sensor through \"Reboot soft\" function before using this parameter!");
				cbConfigKeyUser.setSelected(true);
				ret = panelDatabaseMgt.msoDemo.getMorphoDeviceInstance().setConfigParam(MorphoSmartSDK.CONFIG_KEY_USER_TAG, 1);
				
			}
			else
			{
				DialogUtils.showInfoMessage("INFORMATION",  "You must restart the sensor through \"Reboot soft\" function before using this parameter!");
				cbConfigKeyUser.setSelected(false);
				ret = panelDatabaseMgt.msoDemo.getMorphoDeviceInstance().setConfigParam(MorphoSmartSDK.CONFIG_KEY_USER_TAG, 0);
				
			}			
			if (ret != MorphoSmartSDK.MORPHO_OK) {
				DialogUtils.showErrorMessage("SetConfigParam", "An error occured while calling MorphoDevice.setConfigParam() function", ret, panelDatabaseMgt.msoDemo.getMorphoDeviceInstance().getInternalError());
			
	
	};

	};
};
}
