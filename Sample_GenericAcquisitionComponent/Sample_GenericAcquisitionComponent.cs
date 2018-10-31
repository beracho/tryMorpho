using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Windows.Forms;
using Morpho.MorphoAcquisition;

namespace Sample_GenericAcquisitionComponent
{
    public partial class Sample_GenericAcquisitionComponent : Form
    {
        private MorphoAcquisitionComponent GenericAcqComponent = null;
        private DeviceType CurrentComponentType = DeviceType.NO_DEVICE;
        private DeviceLayoutConfig CurrentDeviceLayoutConfig = null;

        #region Static members and methods
        private static readonly Dictionary<DeviceType, DeviceLayoutConfig> DeviceLayoutConfigMap = new Dictionary<DeviceType, DeviceLayoutConfig>();

        static Sample_GenericAcquisitionComponent()
        {
            // Create one device layout config for each known device type
            DeviceLayoutConfigMap[DeviceType.ACTIVE_MACI] = new DeviceLayoutConfig(DeviceType.ACTIVE_MACI);
            DeviceLayoutConfigMap[DeviceType.MORPHOSMART] = new DeviceLayoutConfig(DeviceType.MORPHOSMART);
            DeviceLayoutConfigMap[DeviceType.MORPHOKIT] = new DeviceLayoutConfig(DeviceType.MORPHOKIT);
            DeviceLayoutConfigMap[DeviceType.MORPHOKIT_FVP] = new DeviceLayoutConfig(DeviceType.MORPHOKIT_FVP);
            DeviceLayoutConfigMap[DeviceType.DUMMYDEVICE] = new DeviceLayoutConfig(DeviceType.DUMMYDEVICE);
        }
        #endregion

        public Sample_GenericAcquisitionComponent()
        {
            InitializeComponent();
        }

        private void InitDialog()
        {
            try
            {
                comboBoxCoderAlgo.SelectedItem = CurrentDeviceLayoutConfig.CoderAlgorithms[GenericAcqComponent.CoderAlgorithm];
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                checkBoxJuvenile.Checked = GenericAcqComponent.JuvenileMode;
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                checkBoxRetry.Checked = GenericAcqComponent.RetryAcquisition;
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                checkBoxAccept.Checked = GenericAcqComponent.AcceptBadQualityEnrollment;
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                textBoxLiveQualityThreshold.Text = GenericAcqComponent.LiveQualityThreshold.ToString();
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                if (CurrentDeviceLayoutConfig.FPTemplateFormats.Contains(GenericAcqComponent.TemplateFormat))
                {
                    this.comboBoxFPFormat.SelectedItem = CurrentDeviceLayoutConfig.FPTemplateFormats[GenericAcqComponent.TemplateFormat];

                    if (CurrentDeviceLayoutConfig.FVPTemplatesSupported)
                        this.comboBoxFVPFormat.SelectedItem = CurrentDeviceLayoutConfig.FVPTemplateFormats[TemplateFormat.PK_FVP];
                }
                else
                {
                    this.comboBoxFPFormat.SelectedItem = DeviceLayoutConfig.NoneItemString;

                    if (CurrentDeviceLayoutConfig.FVPTemplatesSupported && CurrentDeviceLayoutConfig.FVPTemplateFormats.Contains(GenericAcqComponent.TemplateFormat))
                        this.comboBoxFVPFormat.SelectedItem = CurrentDeviceLayoutConfig.FVPTemplateFormats[GenericAcqComponent.TemplateFormat];
                    else
                        this.comboBoxFVPFormat.SelectedItem = DeviceLayoutConfig.NoneItemString;
                }
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                textBoxTimeout.Text = GenericAcqComponent.Timeout.ToString();
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }
            catch (Exception)
            {
                textBoxTimeout.Text = "15";
            }

            try
            {
                comboBoxSecurityLevel.SelectedItem = CurrentDeviceLayoutConfig.SecurityLevels[GenericAcqComponent.ExtSecurityLevel];
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

			try
			{
				comboBoxAcquisitionModeStrategy.SelectedItem = CurrentDeviceLayoutConfig.AcquisitionModeStrategies[GenericAcqComponent.AcquisitionModeStrategy];
			}
			catch { }
        }

        private void SetDropDownAutoWidth(ComboBox i_x_ComboBox)
        {
            Label l_x_Label = new Label();
            int l_i_LabelWidth = 0;
            int l_i_MaxWidth = 0;

            foreach (object l_x_Item in i_x_ComboBox.Items)
            {
                l_x_Label.Text = l_x_Item.ToString() + "      ";
                l_i_LabelWidth = l_x_Label.PreferredWidth;

                if (l_i_LabelWidth > l_i_MaxWidth)
                    l_i_MaxWidth = l_i_LabelWidth;
            }

            l_x_Label.Dispose();
            i_x_ComboBox.DropDownWidth = l_i_MaxWidth;
        }

        private void buttonEnroll_Click(object sender, EventArgs e)
        {
            try
            {

                //Init Acquisition Component
                InitAcquisition(ref GenericAcqComponent);

                //verify the checkbox of consolidation
                try
                {
                    GenericAcqComponent.Consolidation = checkBoxConsolidate.Checked;
                }
                catch (MethodNotImplementedException) { }
                catch (NullReferenceException) { }

                //Run Acquisition
                EnrollmentResult result = GenericAcqComponent.RunEnroll();

                if (result == null)
                    return;

                if ((result.Status != ErrorCodes.IED_NO_ERROR) && (result.Status != ErrorCodes.WARNING_BAD_QUALITY_ACCEPTED))
                    return;
                string info = "Quality Score: " + result.TemplateQuality + "." + Environment.NewLine;
                if (CurrentDeviceLayoutConfig.SecurityLevelSupported)
                {
                    string str = result.IsAdvancedSecurityLevelsCompatible ? "Yes" : "No";
                    info = info + "Advanced Security Level Compatible: " + str + ".";
                }
                MessageBox.Show(info,
                    "Acquisition info",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
                if ((result.ImageList != null) && (checkBoxSaveBitmap.Checked))
                {
                    int index = 0;
                    if (checkBoxConsolidate.Checked)
                        index = result.BestImageIndex - 1;

                    SaveFingerPrint(
                        result.ImageList[index].Image as byte[],
                        result.ImageList[index].Width,
                        result.ImageList[index].Height);
                }

                if (checkBoxSaveTemplate.Checked)
                {
                    SaveTemplate(result.Template, result.TemplateFormat);
                    SaveTemplate(result.AdditionalFPTemplate, result.AdditionalFPTemplateFormat);
                }
            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message,
                    exc.GetType().Name,
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
            }
        }

        private void buttonVerify_Click(object sender, EventArgs e)
        {
            try
            {
                //Select File
                OpenFileDialog dlg = new OpenFileDialog();

                dlg.Multiselect = false;
                dlg.Title = "Select reference template";

                Dictionary<TemplateFormat, string> l_x_TemplateFormatFilterMap = new Dictionary<TemplateFormat, string>();

                l_x_TemplateFormatFilterMap[TemplateFormat.CFV] = "Morpho CFV Fingerprint Template (*.cfv)|*.cfv";
                l_x_TemplateFormatFilterMap[TemplateFormat.PKMAT] = "Morpho PkMat Fingerprint Template (*.pkmat)|*.pkmat";
                l_x_TemplateFormatFilterMap[TemplateFormat.PKCOMPV2] = "Morpho PkComp V2 Fingerprint Template (*.pkc)|*.pkc";
                l_x_TemplateFormatFilterMap[TemplateFormat.PKLITE] = "Morpho PkLite Fingerprint Template (*.pklite)|*.pklite";
                l_x_TemplateFormatFilterMap[TemplateFormat.PK_FVP] = "Morpho PkFVP Finger Vein/Fingerprint Template (*.fvp)|*.fvp";
                l_x_TemplateFormatFilterMap[TemplateFormat.ANSI_378] = "ANSI INCITS 378-2004 Finger Minutiae Record (*.ansi-fmr)|*.ansi-fmr";
                l_x_TemplateFormatFilterMap[TemplateFormat.ISO_19794_2_FMR] = "ISO/IEC 19794-2:2005 Finger Minutiae Record (*.iso-fmr)|*.iso-fmr";
                l_x_TemplateFormatFilterMap[TemplateFormat.ISO_19794_2_Card_Format_Normal_Size] = "ISO/IEC 19794-2:2005 Finger Minutiae Card Format Normal Size (*.iso-fmc-ns)|*.iso-fmc-ns";
                l_x_TemplateFormatFilterMap[TemplateFormat.ISO_19794_2_Card_Format_Compact_Size] = "ISO/IEC 19794-2:2005 Finger Minutiae Card Format Compact Size (*.iso-fmc-cs)|*.iso-fmc-cs";
                l_x_TemplateFormatFilterMap[TemplateFormat.PKMAT_NORM] = "Morpho PkMat Norm Fingerprint Template (*.pkmn)|*.pkmn";
                l_x_TemplateFormatFilterMap[TemplateFormat.PKCOMPV2_NORM] = "Morpho PkComp V2 Norm Fingerprint Template (*.pkcn)|*.pkcn";


                AssociativeTuple<int, TemplateFormat> l_x_FilterTemplates = new AssociativeTuple<int, TemplateFormat>();
                string l_x_Filter = string.Empty;
                int l_i_FilterIndex = -1;
                int l_i_FilterCount = 1;

                foreach (TemplateFormat l_x_TemplateFormat in CurrentDeviceLayoutConfig.VerifyTemplateFormats.Seconds)
                {
                    l_x_FilterTemplates[l_i_FilterCount] = l_x_TemplateFormat;
                    l_x_Filter += l_x_TemplateFormatFilterMap[l_x_TemplateFormat] + "|";

                    if (l_x_TemplateFormat == CurrentDeviceLayoutConfig.FPTemplateFormats[this.comboBoxFPFormat.SelectedItem.ToString()] ||
                        (CurrentDeviceLayoutConfig.FVPTemplatesSupported && (l_x_TemplateFormat == CurrentDeviceLayoutConfig.FVPTemplateFormats[this.comboBoxFVPFormat.SelectedItem.ToString()])))
                        l_i_FilterIndex = l_i_FilterCount;

                    l_i_FilterCount++;
                }

                if (CurrentDeviceLayoutConfig.FVPTemplatesSupported)
                {
                    l_x_FilterTemplates[l_i_FilterCount] = CurrentDeviceLayoutConfig.FVPTemplateFormats[CurrentDeviceLayoutConfig.FVPTemplateDefaultValue];

                    if (l_i_FilterIndex == -1)
                        l_i_FilterIndex = l_x_FilterTemplates[CurrentDeviceLayoutConfig.FVPTemplateFormats[CurrentDeviceLayoutConfig.FVPTemplateDefaultValue]];
                }
                else
                {
                    l_x_FilterTemplates[l_i_FilterCount] = CurrentDeviceLayoutConfig.FPTemplateFormats[CurrentDeviceLayoutConfig.FPTemplateDefaultValue];

                    if (l_i_FilterIndex == -1)
                        l_i_FilterIndex = l_x_FilterTemplates[CurrentDeviceLayoutConfig.FPTemplateFormats[CurrentDeviceLayoutConfig.FPTemplateDefaultValue]];
                }

                l_x_Filter += "All Files (*.*)|*.*";

                dlg.Filter = l_x_Filter;
                dlg.FilterIndex = l_i_FilterIndex;

                if (dlg.ShowDialog() != DialogResult.OK)
                {
                    //Cancel by User
                    return;
                }

                //Open Template
                FileStream fs = new FileStream(dlg.FileName, FileMode.Open, FileAccess.Read);
                Byte[] reference_template = new Byte[(int)fs.Length];
                fs.Read(reference_template as Byte[], 0, (int)fs.Length);
                fs.Close();

                //Init Acquisition Component
                InitAcquisition(ref GenericAcqComponent);

                // Retrieve the template format selected by user for the reference template
                TemplateFormat l_e_ReferenceTemplateFormat = l_x_FilterTemplates[dlg.FilterIndex];

                // Backup the previous template format value
                TemplateFormat l_e_PreviousTemplateFormat = GenericAcqComponent.TemplateFormat;

                // Set MorphoAcquisition template format to the reference template format
                GenericAcqComponent.TemplateFormat = l_e_ReferenceTemplateFormat;

                //Run Acquisition
                VerifyResult result = GenericAcqComponent.RunCaptureVerify(reference_template);

                // Restore MorphoAcquisition previous template format
                GenericAcqComponent.TemplateFormat = l_e_PreviousTemplateFormat;

                if ((result.Status != ErrorCodes.IED_NO_ERROR) && (result.Status != ErrorCodes.IED_ERR_NO_HIT))
                {
                    // Error happened (matching not done)
                    MessageBox.Show(String.Format("Verification failed with error {0}.", result.Status), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                else if (result.Status == ErrorCodes.IED_ERR_NO_HIT)
                {
                    // Sensor returned NO_HIT
                    MessageBox.Show("Authentication failed.", "Authentication failed", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else if (this.CurrentComponentType == DeviceType.ACTIVE_MACI)
                {
                    // MorphoAccess does not indicate the matching score so it is not pertinent to check it
                    MessageBox.Show("Authentication succeeded.", "Authentication succeeded", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else if (result.AuthenticationScore >= Convert.ToInt16(textBoxAuthent.Text))
                {
                    // Other wise, we check the matching score
                    MessageBox.Show(String.Format("Authentication succeeded, score : {0}", result.AuthenticationScore),
                        "Authentication succeeded", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    // This should never happen
                    MessageBox.Show(String.Format("Authentication failed, score : {0}", result.AuthenticationScore),
                        "Authentication failed", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message, exc.GetType().Name, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void InitAcquisition(ref MorphoAcquisitionComponent AcqComponent)
        {
            // For Active_MACI, device name are: "TCP;IP_Address=<value>;Port=<value>"
            if (this.CurrentComponentType == DeviceType.ACTIVE_MACI)
                AcqComponent.DeviceName = "TCP;IP_Address=" + textBoxIP.AddressText + ";Port=" + textBoxPort.Text + ";Application=APPLICATION_" + comboBoxGen.SelectedItem.ToString();
            else
            {
                if (comboBoxDevice.Items.Count > 0)
                    AcqComponent.DeviceName = comboBoxDevice.SelectedItem.ToString();
                else
                    throw new Exception("No device name was assigned.");
            }

            try
            {
                AcqComponent.CoderAlgorithm = CurrentDeviceLayoutConfig.CoderAlgorithms[this.comboBoxCoderAlgo.SelectedItem.ToString()];
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                if (this.comboBoxFPFormat.SelectedItem.ToString() != DeviceLayoutConfig.NoneItemString)
                    AcqComponent.TemplateFormat = CurrentDeviceLayoutConfig.FPTemplateFormats[this.comboBoxFPFormat.SelectedItem.ToString()];
                else if ((this.comboBoxFVPFormat.Enabled) && (this.comboBoxFVPFormat.SelectedItem.ToString() != DeviceLayoutConfig.NoneItemString))
                    AcqComponent.TemplateFormat = CurrentDeviceLayoutConfig.FVPTemplateFormats[this.comboBoxFVPFormat.SelectedItem.ToString()];
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                AcqComponent.JuvenileMode = checkBoxJuvenile.Checked;
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                AcqComponent.LiveQualityThreshold = int.Parse(textBoxLiveQualityThreshold.Text);
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                AcqComponent.RetryAcquisition = checkBoxRetry.Checked;
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                AcqComponent.AcceptBadQualityEnrollment = checkBoxAccept.Checked;
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                AcqComponent.ShowLiveQualityBar = checkBoxShowLiveQualityBar.Checked;
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                AcqComponent.ShowLiveQualityThreshold = checkBoxShowLiveQualityThreshold.Checked;
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                AcqComponent.Timeout = ushort.Parse(textBoxTimeout.Text);
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                AcqComponent.TimeoutQualityCoder = Convert.ToInt32(textBoxShowQualityDuration.Text);
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

            try
            {
                AcqComponent.ExtSecurityLevel = CurrentDeviceLayoutConfig.SecurityLevels[comboBoxSecurityLevel.SelectedItem.ToString()];
            }
            catch (MethodNotImplementedException) { }
            catch (NullReferenceException) { }

			try
			{
				AcqComponent.AcquisitionModeStrategy = CurrentDeviceLayoutConfig.AcquisitionModeStrategies[comboBoxAcquisitionModeStrategy.SelectedItem.ToString()];
			}
			catch { }

            GenericAcqComponent.LiveImage = checkBoxLiveImage.Checked;

            if (radioButtonCultureFr.Checked)
                AcqComponent.SetCulture("fr-FR");
            else if (radioButtonCulturePt.Checked)
                AcqComponent.SetCulture("pt-PT");
            else if (radioButtonCultureEs.Checked)
                AcqComponent.SetCulture("es-ES");
            else if (radioButtonCultureEn.Checked)
                AcqComponent.SetCulture("en-US");
            else if (radioButtonCultureAr.Checked)
                AcqComponent.SetCulture("ar-SA");
            else if (radioButtonCultureIt.Checked)
                AcqComponent.SetCulture("it-IT");
            else if (radioButtonCultureDe.Checked)
                AcqComponent.SetCulture("de-DE");
            else if (radioButtonCultureDefault.Checked)
                AcqComponent.SetCulture("en-US");
        }

        private void buttonRefreshDevice_Click(object sender, EventArgs e)
        {
            //Get Serial Numbers
            comboBoxDevice.Items.Clear();
            String[] DeviceList = GenericAcqComponent.GetConnectedDevices();

            if (DeviceList != null)
            {
                for (int i = 0; i < DeviceList.Length; i++)
                    comboBoxDevice.Items.Add(DeviceList[i]);

                SetDropDownAutoWidth(comboBoxDevice);
            }

            //Select First
            if (comboBoxDevice.Items.Count > 0)
                comboBoxDevice.SelectedIndex = 0;
        }

        private void SaveFingerPrint(byte[] picture, int width, int height)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.AddExtension = true;

            dlg.FileName = "fingerprint";
            dlg.Title = "Save picture";
            dlg.DefaultExt = "png";
            dlg.Filter = "PNG Image (*.png)|*.png|All Files (*.*)|*.*";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                CreateGreyscaleBitmap(picture, width, height).Save(dlg.FileName, ImageFormat.Png);
            }
        }

        private void SaveTemplate(byte[] template, TemplateFormat format)
        {
            if(template != null)
            {
                if(format == TemplateFormat.PK_FVP)
                    SaveFVPTemplate(template, format);
                else
                    SaveFPTemplate(template, format);
            }
        }

        private void SaveFPTemplate(byte[] template, TemplateFormat format)
        {
            if(this.comboBoxFPFormat.SelectedItem.ToString() != DeviceLayoutConfig.NoneItemString)
            {
                SaveFileDialog dlg = new SaveFileDialog();
                dlg.AddExtension = false;

                dlg.FileName = "fingerprint";
                dlg.Title = "Save fingerprint template";
                dlg.AddExtension = true;

                string l_s_Filter;

                switch(format)
                {
                    case TemplateFormat.CFV:
                        l_s_Filter = "Morpho CFV Fingerprint Template (*.cfv)|*.cfv|";
                        dlg.DefaultExt = "cfv";
                        break;

                    case TemplateFormat.PKMAT:
                        l_s_Filter = "Morpho PkMat Fingerprint Template (*.pkmat)|*.pkmat|";
                        dlg.DefaultExt = "pkmat";
                        break;

                    case TemplateFormat.PKCOMPV2:
                        l_s_Filter = "Morpho PkComp V2 Fingerprint Template (*.pkc)|*.pkc|";
                        dlg.DefaultExt = "pkc";
                        break;

                    case TemplateFormat.PKLITE:
                        l_s_Filter = "Morpho PkLite Fingerprint Template (*.pklite)|*.pklite|";
                        dlg.DefaultExt = "pklite";
                        break;

                    case TemplateFormat.ANSI_378:
                        l_s_Filter = "ANSI INCITS 378-2004 Finger Minutiae Record (*.ansi-fmr)|*.ansi-fmr|";
                        dlg.DefaultExt = "ansi-fmr";
                        break;

                    case TemplateFormat.ISO_19794_2_FMR:
                        l_s_Filter = "ISO/IEC 19794-2:2005 Finger Minutiae Record (*.iso-fmr)|*.iso-fmr|";
                        dlg.DefaultExt = "iso-fmr";
                        break;

                    case TemplateFormat.ISO_19794_2_Card_Format_Normal_Size:
                        l_s_Filter = "ISO/IEC 19794-2:2005 Finger Minutiae Card Format Normal Size (*.iso-fmc-ns)|*.iso-fmc-ns|";
                        dlg.DefaultExt = "iso-fmc-ns";
                        break;

                    case TemplateFormat.ISO_19794_2_Card_Format_Compact_Size:
                        l_s_Filter = "ISO/IEC 19794-2:2005 Finger Minutiae Card Format Compact Size (*.iso-fmc-cs)|*.iso-fmc-cs|";
                        dlg.DefaultExt = "iso-fmc-cs";
                        break;
                    
                    case TemplateFormat.PKMAT_NORM:
                        l_s_Filter = "Morpho PkMat Norm Fingerprint Template (*.pkmn)|*.pkmn|";
                        dlg.DefaultExt = "pkmn";
                        break;
                    
                    case TemplateFormat.PKCOMPV2_NORM:
                        l_s_Filter = "Morpho PkComp V2 Norm Fingerprint Template (*.pkcn)|*.pkcn|";
                        dlg.DefaultExt = "pkcn";
                        break;

                    default:
                        l_s_Filter = string.Empty;
                        break;
                }

                dlg.Filter = l_s_Filter + "All Files (*.*)|*.*";

                if(dlg.ShowDialog() == DialogResult.OK)
                {
                    System.IO.FileStream fs = new System.IO.FileStream(dlg.FileName, FileMode.Create, FileAccess.Write);
                    fs.Write(template, 0, template.Length);
                    fs.Close();
                }
            }
        }

        private void SaveFVPTemplate(byte[] template, TemplateFormat format)
        {
            if(this.comboBoxFVPFormat.SelectedItem.ToString() != DeviceLayoutConfig.NoneItemString)
            {
                SaveFileDialog dlg = new SaveFileDialog();
                dlg.AddExtension = false;

                dlg.FileName = "fvp_template";
                dlg.Title = "Save finger vein/fingerprint template";
                dlg.AddExtension = true;

                string l_s_Filter;

                switch(format)
                {
                    case TemplateFormat.PK_FVP:
                        l_s_Filter = "Morpho PK_FVP Finger Vein/Fingerprint Template (*.fvp)|*.fvp|";
                        dlg.DefaultExt = "fvp";
                        break;

                    default:
                        l_s_Filter = string.Empty;
                        break;
                }

                dlg.Filter = l_s_Filter + "All Files (*.*)|*.*";

                if(dlg.ShowDialog() == DialogResult.OK)
                {
                    System.IO.FileStream fs = new System.IO.FileStream(dlg.FileName, FileMode.Create, FileAccess.Write);
                    fs.Write(template, 0, template.Length);
                    fs.Close();
                }
            }
        }

        private Bitmap CreateGreyscaleBitmap(byte[] buffer, int width, int height)
        {
            Bitmap bmp = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format8bppIndexed);
            // copy the acquire image data to our bitmap
            // this works because the width of all MSO images is a multiple of 4
            BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.ReadWrite, PixelFormat.Format8bppIndexed);
            System.Runtime.InteropServices.Marshal.Copy(buffer, 0, bmpData.Scan0, width * height);
            bmp.UnlockBits(bmpData);

            // set up a greyscale palette
            ColorPalette pal = bmp.Palette;
            for (int i = 0; i < 256; i++)
            {
                pal.Entries[i] = Color.FromArgb(i, i, i);
            }
            bmp.Palette = pal;

            return bmp;
        }

        private void GenericAcquisitionComponent_Shown(object sender, EventArgs e)
        {
        }

        private void comboBoxCoderAlgo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (CurrentDeviceLayoutConfig.CoderAlgorithms[this.comboBoxCoderAlgo.SelectedItem.ToString()] == CoderAlgorithm.V9)
            {
                checkBoxConsolidate.Checked = false;
                checkBoxConsolidate.Enabled = false;
            }
            else
            {
                if (!checkBoxConsolidate.Enabled)
                    checkBoxConsolidate.Checked = CurrentDeviceLayoutConfig.ConsolidateDefaultValue;

                checkBoxConsolidate.Enabled = CurrentDeviceLayoutConfig.ConsolidateSupported;
            }
        }

        private void buttonCreateObject_Click(object sender, EventArgs e)
        {
            if (ComponentTypes.SelectedItem == null)
            {
                MessageBox.Show("No component was selected.\nPlease choose a component from the list.", "Sample generic acquisition component", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            try
            {
                string l_x_DeviceName = ComponentTypes.SelectedItem.ToString();

                if (Enum.IsDefined(typeof(DeviceType), l_x_DeviceName))
                    CurrentComponentType = (DeviceType)Enum.Parse(typeof(DeviceType), l_x_DeviceName, true);
                else
                    CurrentComponentType = DeviceType.NO_DEVICE;

                GenericAcqComponent = new MorphoAcquisitionComponent(this.CurrentComponentType);
                CurrentDeviceLayoutConfig = DeviceLayoutConfigMap[this.CurrentComponentType];

                this.tabControlOptions.Enabled = true;
                this.buttonEnroll.Enabled = true;
                this.buttonVerify.Enabled = true;

                this.buttonRefreshDevice_Click(this, EventArgs.Empty);

                this.groupBoxMorphoAccess.Enabled = (CurrentDeviceLayoutConfig.Type == DeviceType.ACTIVE_MACI);
                this.groupBoxMorphoSmart.Enabled = (CurrentDeviceLayoutConfig.Type != DeviceType.ACTIVE_MACI);

                this.comboBoxCoderAlgo.Enabled = true;
                this.comboBoxFPFormat.Enabled = true;
                this.comboBoxFVPFormat.Enabled = true;
                this.checkBoxConsolidate.Enabled = true;
                this.comboBoxSecurityLevel.Enabled = true;
				this.comboBoxAcquisitionModeStrategy.Enabled = true;
                this.checkBoxShowLiveQualityThreshold.Enabled = true;

                this.comboBoxCoderAlgo.Items.Clear();
                this.comboBoxFPFormat.Items.Clear();
                this.comboBoxFVPFormat.Items.Clear();
                this.comboBoxSecurityLevel.Items.Clear();
				this.comboBoxAcquisitionModeStrategy.Items.Clear();

                this.comboBoxCoderAlgo.Items.AddRange(CurrentDeviceLayoutConfig.CoderAlgorithms.Firsts);
                this.comboBoxFPFormat.Items.AddRange(CurrentDeviceLayoutConfig.FPTemplateFormats.Firsts);
                this.comboBoxFVPFormat.Items.AddRange(CurrentDeviceLayoutConfig.FVPTemplateFormats.Firsts);
                this.comboBoxSecurityLevel.Items.AddRange(CurrentDeviceLayoutConfig.SecurityLevels.Firsts);
				this.comboBoxAcquisitionModeStrategy.Items.AddRange(CurrentDeviceLayoutConfig.AcquisitionModeStrategies.Firsts);

                SetDropDownAutoWidth(this.comboBoxCoderAlgo);
                SetDropDownAutoWidth(this.comboBoxFPFormat);
                SetDropDownAutoWidth(this.comboBoxFVPFormat);
                SetDropDownAutoWidth(this.comboBoxSecurityLevel);
				SetDropDownAutoWidth(this.comboBoxAcquisitionModeStrategy);

                this.comboBoxCoderAlgo.SelectedItem = CurrentDeviceLayoutConfig.CoderAlgorithmDefaultValue;
                this.comboBoxFPFormat.SelectedItem = CurrentDeviceLayoutConfig.FPTemplateDefaultValue;
                this.comboBoxFVPFormat.SelectedItem = CurrentDeviceLayoutConfig.FVPTemplateDefaultValue;
                this.checkBoxConsolidate.Checked = CurrentDeviceLayoutConfig.ConsolidateDefaultValue;
                this.comboBoxSecurityLevel.SelectedItem = CurrentDeviceLayoutConfig.SecurityLevelDefaultValue;
				this.comboBoxAcquisitionModeStrategy.SelectedItem = CurrentDeviceLayoutConfig.AcquisitionModeStrategyDefaultValue;
                this.checkBoxShowLiveQualityThreshold.Checked = CurrentDeviceLayoutConfig.ShowLiveQualityThresholdDefaultValue;

                this.comboBoxFVPFormat.Enabled = CurrentDeviceLayoutConfig.FVPTemplatesSupported;
                this.checkBoxConsolidate.Enabled = CurrentDeviceLayoutConfig.ConsolidateSupported;
                this.comboBoxSecurityLevel.Enabled = CurrentDeviceLayoutConfig.SecurityLevelSupported;
				this.comboBoxAcquisitionModeStrategy.Enabled = CurrentDeviceLayoutConfig.AcquisitionModeStrategySupported;
                this.checkBoxShowLiveQualityThreshold.Enabled = CurrentDeviceLayoutConfig.ShowLiveQualityThresholdSupported;

                this.InitDialog();
            }
            catch (Exception exc)
            {
                this.tabControlOptions.Enabled = false;
                this.buttonEnroll.Enabled = false;
                this.buttonVerify.Enabled = false;
                this.comboBoxSecurityLevel.Enabled = false;
				this.comboBoxAcquisitionModeStrategy.Enabled = false;
                this.groupBoxMorphoAccess.Enabled = false;
                this.groupBoxMorphoSmart.Enabled = false;
                this.comboBoxFVPFormat.Enabled = false;
                this.checkBoxShowLiveQualityThreshold.Enabled = true;
                this.checkBoxConsolidate.Enabled = true;

                MessageBox.Show(exc.Message, exc.GetType().ToString());
            }
        }

        private void GenericAcquisitionComponent_Load(object sender, EventArgs e)
        {
            tabControlOptions.Enabled = false;
            buttonEnroll.Enabled = false;
            buttonVerify.Enabled = false;

            GenericAcqComponent = new MorphoAcquisitionComponent();
            DeviceType[] availableTypes = GenericAcqComponent.CheckAvailableComponents();

            if ((availableTypes == null) || (availableTypes.Length == 0))
                ComponentTypes.Items.Add(DeviceType.NO_DEVICE);

            foreach (DeviceType item in availableTypes)
                ComponentTypes.Items.Add(item);

            SetDropDownAutoWidth(ComponentTypes);

            GenericAcqComponent = null;

            foreach (FingerEventStatus item in Enum.GetValues(typeof(FingerEventStatus)))
                comboBoxFingerPosition.Items.Add(item.ToString());

            SetDropDownAutoWidth(comboBoxFingerPosition);
            comboBoxFingerPosition.SelectedIndex = 0;
        }

        private void buttonSetImagePath_Click(object sender, EventArgs e)
        {
            //Select File
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Multiselect = false;
            dlg.Title = "Select customized image for " + this.comboBoxFingerPosition.SelectedItem.ToString() + " status";

            string l_x_Filter = "All Files (*.*)|*.*";
            l_x_Filter += "|All Image types (*.bmp, *.gif, *.jpg, *.jpeg, *.jpe, *.png)|*.bmp;*.gif;*.jpg;*.jpeg;*.jpe;*.png";
            l_x_Filter += "|Windows BMP Image (*.bmp)|*.bmp";
            l_x_Filter += "|GIF Image (*.gif)|*.gif";
            l_x_Filter += "|JPEG Image (*.jpg, *.jpeg, *.jpe)|*.jpg;*.jpeg;*.jpe";
            l_x_Filter += "|PNG Image (*.png)|*.png";

            dlg.Filter = l_x_Filter;
            dlg.FilterIndex = 2;

            if (dlg.ShowDialog() != DialogResult.OK)
            {
                //Cancel by User
                return;
            }

            GenericAcqComponent.SetImagePath((FingerEventStatus)comboBoxFingerPosition.SelectedIndex, dlg.FileName);
        }

        private void textBoxLiveQualityThreshold_Leave(object sender, EventArgs e)
        {
            int l_i_LiveQualityThreshold = 0;

            if (String.IsNullOrEmpty(textBoxLiveQualityThreshold.Text)
                || !(Int32.TryParse(textBoxLiveQualityThreshold.Text, out l_i_LiveQualityThreshold))
                || (l_i_LiveQualityThreshold < 0)
                || (l_i_LiveQualityThreshold > 0x64))
            {
                textBoxLiveQualityThreshold.BackColor = Color.Red;
                textBoxLiveQualityThreshold.Focus();
                MessageBox.Show("Invalid value");
            }
            else
                textBoxLiveQualityThreshold.BackColor = Color.White;
        }

        private void textBoxTimeout_Leave(object sender, EventArgs e)
        {
            int l_i_Timeout = 0;

            if (String.IsNullOrEmpty(textBoxTimeout.Text)
                || !(Int32.TryParse(textBoxTimeout.Text, out l_i_Timeout))
                || (l_i_Timeout < 0)
                || (l_i_Timeout > 0xFFFF))
            {
                textBoxTimeout.BackColor = Color.Red;
                textBoxTimeout.Focus();
                MessageBox.Show("Invalid value");
            }
            else
                textBoxTimeout.BackColor = Color.White;
        }

        private void textBoxShowQualityDuration_Leave(object sender, EventArgs e)
        {
            int l_i_ShowQualityDuration = 0;

            if (String.IsNullOrEmpty(textBoxShowQualityDuration.Text)
                || !(Int32.TryParse(textBoxShowQualityDuration.Text, out l_i_ShowQualityDuration))
                || (l_i_ShowQualityDuration < 0))
            {
                textBoxShowQualityDuration.BackColor = Color.Red;
                textBoxShowQualityDuration.Focus();
                MessageBox.Show("Invalid value");
            }
            else
                textBoxShowQualityDuration.BackColor = Color.White;
        }
    }
}
