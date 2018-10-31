namespace Sample_GenericAcquisitionComponent
{
    partial class Sample_GenericAcquisitionComponent
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Sample_GenericAcquisitionComponent));
			this.tabControlOptions = new System.Windows.Forms.TabControl();
			this.tabPageCoder = new System.Windows.Forms.TabPage();
			this.comboBoxFVPFormat = new System.Windows.Forms.ComboBox();
			this.label10 = new System.Windows.Forms.Label();
			this.textBoxAuthent = new System.Windows.Forms.MaskedTextBox();
			this.textBoxLiveQualityThreshold = new System.Windows.Forms.MaskedTextBox();
			this.comboBoxSecurityLevel = new System.Windows.Forms.ComboBox();
			this.label9 = new System.Windows.Forms.Label();
			this.label8 = new System.Windows.Forms.Label();
			this.checkBoxAccept = new System.Windows.Forms.CheckBox();
			this.label7 = new System.Windows.Forms.Label();
			this.checkBoxRetry = new System.Windows.Forms.CheckBox();
			this.checkBoxJuvenile = new System.Windows.Forms.CheckBox();
			this.comboBoxFPFormat = new System.Windows.Forms.ComboBox();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.comboBoxCoderAlgo = new System.Windows.Forms.ComboBox();
			this.checkBoxConsolidate = new System.Windows.Forms.CheckBox();
			this.tabPageDisplay = new System.Windows.Forms.TabPage();
			this.textBoxShowQualityDuration = new System.Windows.Forms.TextBox();
			this.checkBoxLiveImage = new System.Windows.Forms.CheckBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.buttonSetImagePath = new System.Windows.Forms.Button();
			this.comboBoxFingerPosition = new System.Windows.Forms.ComboBox();
			this.labelShowQualityDuration = new System.Windows.Forms.Label();
			this.checkBoxSaveTemplate = new System.Windows.Forms.CheckBox();
			this.checkBoxSaveBitmap = new System.Windows.Forms.CheckBox();
			this.checkBoxShowLiveQualityBar = new System.Windows.Forms.CheckBox();
			this.checkBoxShowLiveQualityThreshold = new System.Windows.Forms.CheckBox();
			this.tabPageDevice = new System.Windows.Forms.TabPage();
			this.textBoxTimeout = new System.Windows.Forms.MaskedTextBox();
			this.groupBoxMorphoAccess = new System.Windows.Forms.GroupBox();
			this.comboBoxGen = new System.Windows.Forms.ComboBox();
			this.textBoxPort = new System.Windows.Forms.MaskedTextBox();
			this.textBoxIP = new MorphoControls.IPv4InputBox();
			this.label5 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.groupBoxMorphoSmart = new System.Windows.Forms.GroupBox();
			this.buttonRefreshDevice = new System.Windows.Forms.Button();
			this.comboBoxDevice = new System.Windows.Forms.ComboBox();
			this.label6 = new System.Windows.Forms.Label();
			this.tabPageCulture = new System.Windows.Forms.TabPage();
			this.groupBoxCulture = new System.Windows.Forms.GroupBox();
			this.radioButtonCultureDe = new System.Windows.Forms.RadioButton();
			this.radioButtonCultureIt = new System.Windows.Forms.RadioButton();
			this.radioButtonCultureEn = new System.Windows.Forms.RadioButton();
			this.radioButtonCultureAr = new System.Windows.Forms.RadioButton();
			this.radioButtonCulturePt = new System.Windows.Forms.RadioButton();
			this.radioButtonCultureEs = new System.Windows.Forms.RadioButton();
			this.radioButtonCultureFr = new System.Windows.Forms.RadioButton();
			this.radioButtonCultureDefault = new System.Windows.Forms.RadioButton();
			this.buttonEnroll = new System.Windows.Forms.Button();
			this.buttonVerify = new System.Windows.Forms.Button();
			this.buttonCreateObject = new System.Windows.Forms.Button();
			this.ComponentTypes = new System.Windows.Forms.ComboBox();
			this.label11 = new System.Windows.Forms.Label();
			this.comboBoxAcquisitionModeStrategy = new System.Windows.Forms.ComboBox();
			this.tabControlOptions.SuspendLayout();
			this.tabPageCoder.SuspendLayout();
			this.tabPageDisplay.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.tabPageDevice.SuspendLayout();
			this.groupBoxMorphoAccess.SuspendLayout();
			this.groupBoxMorphoSmart.SuspendLayout();
			this.tabPageCulture.SuspendLayout();
			this.groupBoxCulture.SuspendLayout();
			this.SuspendLayout();
			// 
			// tabControlOptions
			// 
			this.tabControlOptions.Controls.Add(this.tabPageCoder);
			this.tabControlOptions.Controls.Add(this.tabPageDisplay);
			this.tabControlOptions.Controls.Add(this.tabPageDevice);
			this.tabControlOptions.Controls.Add(this.tabPageCulture);
			this.tabControlOptions.Location = new System.Drawing.Point(12, 39);
			this.tabControlOptions.Name = "tabControlOptions";
			this.tabControlOptions.SelectedIndex = 0;
			this.tabControlOptions.Size = new System.Drawing.Size(357, 249);
			this.tabControlOptions.TabIndex = 2;
			// 
			// tabPageCoder
			// 
			this.tabPageCoder.Controls.Add(this.comboBoxAcquisitionModeStrategy);
			this.tabPageCoder.Controls.Add(this.label11);
			this.tabPageCoder.Controls.Add(this.comboBoxFVPFormat);
			this.tabPageCoder.Controls.Add(this.label10);
			this.tabPageCoder.Controls.Add(this.textBoxAuthent);
			this.tabPageCoder.Controls.Add(this.textBoxLiveQualityThreshold);
			this.tabPageCoder.Controls.Add(this.comboBoxSecurityLevel);
			this.tabPageCoder.Controls.Add(this.label9);
			this.tabPageCoder.Controls.Add(this.label8);
			this.tabPageCoder.Controls.Add(this.checkBoxAccept);
			this.tabPageCoder.Controls.Add(this.label7);
			this.tabPageCoder.Controls.Add(this.checkBoxRetry);
			this.tabPageCoder.Controls.Add(this.checkBoxJuvenile);
			this.tabPageCoder.Controls.Add(this.comboBoxFPFormat);
			this.tabPageCoder.Controls.Add(this.label4);
			this.tabPageCoder.Controls.Add(this.label3);
			this.tabPageCoder.Controls.Add(this.comboBoxCoderAlgo);
			this.tabPageCoder.Controls.Add(this.checkBoxConsolidate);
			this.tabPageCoder.Location = new System.Drawing.Point(4, 22);
			this.tabPageCoder.Name = "tabPageCoder";
			this.tabPageCoder.Padding = new System.Windows.Forms.Padding(12);
			this.tabPageCoder.Size = new System.Drawing.Size(349, 223);
			this.tabPageCoder.TabIndex = 0;
			this.tabPageCoder.Text = "Coder";
			this.tabPageCoder.UseVisualStyleBackColor = true;
			// 
			// comboBoxFVPFormat
			// 
			this.comboBoxFVPFormat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxFVPFormat.Enabled = false;
			this.comboBoxFVPFormat.FormattingEnabled = true;
			this.comboBoxFVPFormat.Items.AddRange(new object[] {
            "<none>",
            "Morpho PkFVP Finger Vein/Fingerprint Template"});
			this.comboBoxFVPFormat.Location = new System.Drawing.Point(15, 108);
			this.comboBoxFVPFormat.Name = "comboBoxFVPFormat";
			this.comboBoxFVPFormat.Size = new System.Drawing.Size(177, 21);
			this.comboBoxFVPFormat.TabIndex = 5;
			// 
			// label10
			// 
			this.label10.AutoSize = true;
			this.label10.Location = new System.Drawing.Point(15, 92);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(109, 13);
			this.label10.TabIndex = 4;
			this.label10.Text = "FVP Template Format";
			// 
			// textBoxAuthent
			// 
			this.textBoxAuthent.Location = new System.Drawing.Point(213, 148);
			this.textBoxAuthent.Mask = "99999";
			this.textBoxAuthent.Name = "textBoxAuthent";
			this.textBoxAuthent.PromptChar = ' ';
			this.textBoxAuthent.Size = new System.Drawing.Size(121, 20);
			this.textBoxAuthent.TabIndex = 15;
			this.textBoxAuthent.Text = "3500";
			this.textBoxAuthent.ValidatingType = typeof(int);
			// 
			// textBoxLiveQualityThreshold
			// 
			this.textBoxLiveQualityThreshold.Location = new System.Drawing.Point(15, 148);
			this.textBoxLiveQualityThreshold.Mask = "999";
			this.textBoxLiveQualityThreshold.Name = "textBoxLiveQualityThreshold";
			this.textBoxLiveQualityThreshold.PromptChar = ' ';
			this.textBoxLiveQualityThreshold.Size = new System.Drawing.Size(38, 20);
			this.textBoxLiveQualityThreshold.TabIndex = 7;
			this.textBoxLiveQualityThreshold.Text = "60";
			this.textBoxLiveQualityThreshold.Leave += new System.EventHandler(this.textBoxLiveQualityThreshold_Leave);
			// 
			// comboBoxSecurityLevel
			// 
			this.comboBoxSecurityLevel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxSecurityLevel.FormattingEnabled = true;
			this.comboBoxSecurityLevel.Items.AddRange(new object[] {
            "Standard",
            "Medium",
            "High"});
			this.comboBoxSecurityLevel.Location = new System.Drawing.Point(213, 187);
			this.comboBoxSecurityLevel.Name = "comboBoxSecurityLevel";
			this.comboBoxSecurityLevel.Size = new System.Drawing.Size(121, 21);
			this.comboBoxSecurityLevel.TabIndex = 17;
			// 
			// label9
			// 
			this.label9.AutoSize = true;
			this.label9.Location = new System.Drawing.Point(213, 171);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(74, 13);
			this.label9.TabIndex = 16;
			this.label9.Text = "Security Level";
			// 
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Location = new System.Drawing.Point(213, 132);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(83, 13);
			this.label8.TabIndex = 14;
			this.label8.Text = "Authentify score";
			// 
			// checkBoxAccept
			// 
			this.checkBoxAccept.AutoSize = true;
			this.checkBoxAccept.Checked = true;
			this.checkBoxAccept.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxAccept.Location = new System.Drawing.Point(213, 84);
			this.checkBoxAccept.Name = "checkBoxAccept";
			this.checkBoxAccept.Size = new System.Drawing.Size(117, 17);
			this.checkBoxAccept.TabIndex = 13;
			this.checkBoxAccept.Text = "Accept Bad Quality";
			this.checkBoxAccept.UseVisualStyleBackColor = true;
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(15, 132);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(106, 13);
			this.label7.TabIndex = 6;
			this.label7.Text = "LiveQualityThreshold";
			// 
			// checkBoxRetry
			// 
			this.checkBoxRetry.AutoSize = true;
			this.checkBoxRetry.Checked = true;
			this.checkBoxRetry.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxRetry.Location = new System.Drawing.Point(213, 15);
			this.checkBoxRetry.Name = "checkBoxRetry";
			this.checkBoxRetry.Size = new System.Drawing.Size(105, 17);
			this.checkBoxRetry.TabIndex = 10;
			this.checkBoxRetry.Text = "Retry Acquisition";
			this.checkBoxRetry.UseVisualStyleBackColor = true;
			// 
			// checkBoxJuvenile
			// 
			this.checkBoxJuvenile.AutoSize = true;
			this.checkBoxJuvenile.Checked = true;
			this.checkBoxJuvenile.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxJuvenile.Location = new System.Drawing.Point(213, 38);
			this.checkBoxJuvenile.Name = "checkBoxJuvenile";
			this.checkBoxJuvenile.Size = new System.Drawing.Size(65, 17);
			this.checkBoxJuvenile.TabIndex = 11;
			this.checkBoxJuvenile.Text = "Juvenile";
			this.checkBoxJuvenile.UseVisualStyleBackColor = true;
			// 
			// comboBoxFPFormat
			// 
			this.comboBoxFPFormat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxFPFormat.FormattingEnabled = true;
			this.comboBoxFPFormat.Items.AddRange(new object[] {
            "<none>",
            "Morpho CFV Fingerprint Template",
            "Morpho PkMat Fingerprint Template",
            "Morpho PkComp V2 Fingerprint Template",
            "Morpho PkLite Fingerprint Template",
            "ANSI INCITS 378-2004 Finger Minutiae Record",
            "ISO/IEC 19794-2:2005 Finger Minutiae Record",
            "ISO/IEC 19794-2:2005 Finger Minutiae Card Format Normal Size",
            "ISO/IEC 19794-2:2005 Finger Minutiae Card Format Compact Size"});
			this.comboBoxFPFormat.Location = new System.Drawing.Point(15, 68);
			this.comboBoxFPFormat.Name = "comboBoxFPFormat";
			this.comboBoxFPFormat.Size = new System.Drawing.Size(177, 21);
			this.comboBoxFPFormat.TabIndex = 3;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(15, 52);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(102, 13);
			this.label4.TabIndex = 2;
			this.label4.Text = "FP Template Format";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(15, 12);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(59, 13);
			this.label3.TabIndex = 0;
			this.label3.Text = "Coder Algo";
			// 
			// comboBoxCoderAlgo
			// 
			this.comboBoxCoderAlgo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxCoderAlgo.FormattingEnabled = true;
			this.comboBoxCoderAlgo.Items.AddRange(new object[] {
            "Morpho V6 Coder",
            "Morpho V9 Coder",
            "Morpho V10 Coder",
            "Device Embedded Coder"});
			this.comboBoxCoderAlgo.Location = new System.Drawing.Point(15, 28);
			this.comboBoxCoderAlgo.Name = "comboBoxCoderAlgo";
			this.comboBoxCoderAlgo.Size = new System.Drawing.Size(177, 21);
			this.comboBoxCoderAlgo.TabIndex = 1;
			this.comboBoxCoderAlgo.SelectedIndexChanged += new System.EventHandler(this.comboBoxCoderAlgo_SelectedIndexChanged);
			// 
			// checkBoxConsolidate
			// 
			this.checkBoxConsolidate.AutoSize = true;
			this.checkBoxConsolidate.Checked = true;
			this.checkBoxConsolidate.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxConsolidate.Location = new System.Drawing.Point(213, 61);
			this.checkBoxConsolidate.Name = "checkBoxConsolidate";
			this.checkBoxConsolidate.Size = new System.Drawing.Size(81, 17);
			this.checkBoxConsolidate.TabIndex = 12;
			this.checkBoxConsolidate.Text = "Consolidate";
			this.checkBoxConsolidate.UseVisualStyleBackColor = true;
			// 
			// tabPageDisplay
			// 
			this.tabPageDisplay.Controls.Add(this.textBoxShowQualityDuration);
			this.tabPageDisplay.Controls.Add(this.checkBoxLiveImage);
			this.tabPageDisplay.Controls.Add(this.groupBox1);
			this.tabPageDisplay.Controls.Add(this.labelShowQualityDuration);
			this.tabPageDisplay.Controls.Add(this.checkBoxSaveTemplate);
			this.tabPageDisplay.Controls.Add(this.checkBoxSaveBitmap);
			this.tabPageDisplay.Controls.Add(this.checkBoxShowLiveQualityBar);
			this.tabPageDisplay.Controls.Add(this.checkBoxShowLiveQualityThreshold);
			this.tabPageDisplay.Location = new System.Drawing.Point(4, 22);
			this.tabPageDisplay.Name = "tabPageDisplay";
			this.tabPageDisplay.Padding = new System.Windows.Forms.Padding(12);
			this.tabPageDisplay.Size = new System.Drawing.Size(349, 239);
			this.tabPageDisplay.TabIndex = 1;
			this.tabPageDisplay.Text = "Display";
			this.tabPageDisplay.UseVisualStyleBackColor = true;
			// 
			// textBoxShowQualityDuration
			// 
			this.textBoxShowQualityDuration.Location = new System.Drawing.Point(15, 89);
			this.textBoxShowQualityDuration.Name = "textBoxShowQualityDuration";
			this.textBoxShowQualityDuration.Size = new System.Drawing.Size(54, 20);
			this.textBoxShowQualityDuration.TabIndex = 3;
			this.textBoxShowQualityDuration.Text = "500";
			this.textBoxShowQualityDuration.Leave += new System.EventHandler(this.textBoxShowQualityDuration_Leave);
			// 
			// checkBoxLiveImage
			// 
			this.checkBoxLiveImage.AutoSize = true;
			this.checkBoxLiveImage.Checked = true;
			this.checkBoxLiveImage.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxLiveImage.Location = new System.Drawing.Point(236, 61);
			this.checkBoxLiveImage.Name = "checkBoxLiveImage";
			this.checkBoxLiveImage.Size = new System.Drawing.Size(78, 17);
			this.checkBoxLiveImage.TabIndex = 6;
			this.checkBoxLiveImage.Text = "Live Image";
			this.checkBoxLiveImage.UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.buttonSetImagePath);
			this.groupBox1.Controls.Add(this.comboBoxFingerPosition);
			this.groupBox1.Location = new System.Drawing.Point(15, 131);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Padding = new System.Windows.Forms.Padding(10, 9, 10, 8);
			this.groupBox1.Size = new System.Drawing.Size(319, 57);
			this.groupBox1.TabIndex = 7;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Customized Images";
			// 
			// buttonSetImagePath
			// 
			this.buttonSetImagePath.Location = new System.Drawing.Point(221, 25);
			this.buttonSetImagePath.Name = "buttonSetImagePath";
			this.buttonSetImagePath.Size = new System.Drawing.Size(85, 21);
			this.buttonSetImagePath.TabIndex = 1;
			this.buttonSetImagePath.Text = "Associate File";
			this.buttonSetImagePath.UseVisualStyleBackColor = true;
			this.buttonSetImagePath.Click += new System.EventHandler(this.buttonSetImagePath_Click);
			// 
			// comboBoxFingerPosition
			// 
			this.comboBoxFingerPosition.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxFingerPosition.FormattingEnabled = true;
			this.comboBoxFingerPosition.Location = new System.Drawing.Point(13, 25);
			this.comboBoxFingerPosition.Name = "comboBoxFingerPosition";
			this.comboBoxFingerPosition.Size = new System.Drawing.Size(202, 21);
			this.comboBoxFingerPosition.TabIndex = 0;
			// 
			// labelShowQualityDuration
			// 
			this.labelShowQualityDuration.AutoSize = true;
			this.labelShowQualityDuration.Location = new System.Drawing.Point(12, 73);
			this.labelShowQualityDuration.Name = "labelShowQualityDuration";
			this.labelShowQualityDuration.Size = new System.Drawing.Size(188, 13);
			this.labelShowQualityDuration.TabIndex = 2;
			this.labelShowQualityDuration.Text = "Show Coder Quality Duration (millisec.)";
			// 
			// checkBoxSaveTemplate
			// 
			this.checkBoxSaveTemplate.AutoSize = true;
			this.checkBoxSaveTemplate.Checked = true;
			this.checkBoxSaveTemplate.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxSaveTemplate.Location = new System.Drawing.Point(236, 38);
			this.checkBoxSaveTemplate.Name = "checkBoxSaveTemplate";
			this.checkBoxSaveTemplate.Size = new System.Drawing.Size(98, 17);
			this.checkBoxSaveTemplate.TabIndex = 5;
			this.checkBoxSaveTemplate.Text = "Save Template";
			this.checkBoxSaveTemplate.UseVisualStyleBackColor = true;
			// 
			// checkBoxSaveBitmap
			// 
			this.checkBoxSaveBitmap.AutoSize = true;
			this.checkBoxSaveBitmap.Checked = true;
			this.checkBoxSaveBitmap.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxSaveBitmap.Location = new System.Drawing.Point(236, 15);
			this.checkBoxSaveBitmap.Name = "checkBoxSaveBitmap";
			this.checkBoxSaveBitmap.Size = new System.Drawing.Size(83, 17);
			this.checkBoxSaveBitmap.TabIndex = 4;
			this.checkBoxSaveBitmap.Text = "Save Image";
			this.checkBoxSaveBitmap.UseVisualStyleBackColor = true;
			// 
			// checkBoxShowLiveQualityBar
			// 
			this.checkBoxShowLiveQualityBar.AutoSize = true;
			this.checkBoxShowLiveQualityBar.Checked = true;
			this.checkBoxShowLiveQualityBar.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxShowLiveQualityBar.Location = new System.Drawing.Point(15, 15);
			this.checkBoxShowLiveQualityBar.Name = "checkBoxShowLiveQualityBar";
			this.checkBoxShowLiveQualityBar.Size = new System.Drawing.Size(130, 17);
			this.checkBoxShowLiveQualityBar.TabIndex = 0;
			this.checkBoxShowLiveQualityBar.Text = "Show Live Quality Bar";
			this.checkBoxShowLiveQualityBar.UseVisualStyleBackColor = true;
			// 
			// checkBoxShowLiveQualityThreshold
			// 
			this.checkBoxShowLiveQualityThreshold.AutoSize = true;
			this.checkBoxShowLiveQualityThreshold.Location = new System.Drawing.Point(15, 38);
			this.checkBoxShowLiveQualityThreshold.Name = "checkBoxShowLiveQualityThreshold";
			this.checkBoxShowLiveQualityThreshold.Size = new System.Drawing.Size(161, 17);
			this.checkBoxShowLiveQualityThreshold.TabIndex = 1;
			this.checkBoxShowLiveQualityThreshold.Text = "Show Live Quality Threshold";
			this.checkBoxShowLiveQualityThreshold.UseVisualStyleBackColor = true;
			// 
			// tabPageDevice
			// 
			this.tabPageDevice.Controls.Add(this.textBoxTimeout);
			this.tabPageDevice.Controls.Add(this.groupBoxMorphoAccess);
			this.tabPageDevice.Controls.Add(this.label1);
			this.tabPageDevice.Controls.Add(this.groupBoxMorphoSmart);
			this.tabPageDevice.Location = new System.Drawing.Point(4, 22);
			this.tabPageDevice.Name = "tabPageDevice";
			this.tabPageDevice.Padding = new System.Windows.Forms.Padding(12);
			this.tabPageDevice.Size = new System.Drawing.Size(349, 239);
			this.tabPageDevice.TabIndex = 2;
			this.tabPageDevice.Text = "Device";
			this.tabPageDevice.UseVisualStyleBackColor = true;
			// 
			// textBoxTimeout
			// 
			this.textBoxTimeout.Location = new System.Drawing.Point(15, 126);
			this.textBoxTimeout.Mask = "99999";
			this.textBoxTimeout.Name = "textBoxTimeout";
			this.textBoxTimeout.PromptChar = ' ';
			this.textBoxTimeout.Size = new System.Drawing.Size(46, 20);
			this.textBoxTimeout.TabIndex = 3;
			this.textBoxTimeout.Text = "15";
			this.textBoxTimeout.ValidatingType = typeof(int);
			this.textBoxTimeout.Leave += new System.EventHandler(this.textBoxTimeout_Leave);
			// 
			// groupBoxMorphoAccess
			// 
			this.groupBoxMorphoAccess.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.groupBoxMorphoAccess.Controls.Add(this.comboBoxGen);
			this.groupBoxMorphoAccess.Controls.Add(this.textBoxPort);
			this.groupBoxMorphoAccess.Controls.Add(this.textBoxIP);
			this.groupBoxMorphoAccess.Controls.Add(this.label5);
			this.groupBoxMorphoAccess.Controls.Add(this.label2);
			this.groupBoxMorphoAccess.Enabled = false;
			this.groupBoxMorphoAccess.Location = new System.Drawing.Point(92, 110);
			this.groupBoxMorphoAccess.Name = "groupBoxMorphoAccess";
			this.groupBoxMorphoAccess.Padding = new System.Windows.Forms.Padding(10, 9, 10, 8);
			this.groupBoxMorphoAccess.Size = new System.Drawing.Size(242, 78);
			this.groupBoxMorphoAccess.TabIndex = 1;
			this.groupBoxMorphoAccess.TabStop = false;
			this.groupBoxMorphoAccess.Text = "MorphoAccess";
			// 
			// comboBoxGen
			// 
			this.comboBoxGen.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.comboBoxGen.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
			this.comboBoxGen.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
			this.comboBoxGen.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxGen.Items.AddRange(new object[] {
            "MA2G",
            "MA5G"});
			this.comboBoxGen.Location = new System.Drawing.Point(147, 47);
			this.comboBoxGen.Name = "comboBoxGen";
			this.comboBoxGen.Size = new System.Drawing.Size(82, 21);
			this.comboBoxGen.TabIndex = 4;
			// 
			// textBoxPort
			// 
			this.textBoxPort.Location = new System.Drawing.Point(77, 47);
			this.textBoxPort.Mask = "99999";
			this.textBoxPort.Name = "textBoxPort";
			this.textBoxPort.PromptChar = ' ';
			this.textBoxPort.Size = new System.Drawing.Size(63, 20);
			this.textBoxPort.TabIndex = 3;
			this.textBoxPort.Text = "11010";
			this.textBoxPort.ValidatingType = typeof(int);
			// 
			// textBoxIP
			// 
			this.textBoxIP.Address = ((System.Net.IPAddress)(resources.GetObject("textBoxIP.Address")));
			this.textBoxIP.AddressText = "10.126.46.54";
			this.textBoxIP.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxIP.BackColor = System.Drawing.SystemColors.Window;
			this.textBoxIP.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.textBoxIP.Cursor = System.Windows.Forms.Cursors.IBeam;
			this.textBoxIP.ForeColor = System.Drawing.SystemColors.WindowText;
			this.textBoxIP.Location = new System.Drawing.Point(77, 16);
			this.textBoxIP.MinimumSize = new System.Drawing.Size(136, 20);
			this.textBoxIP.Name = "textBoxIP";
			this.textBoxIP.Padding = new System.Windows.Forms.Padding(1, 1, 1, 2);
			this.textBoxIP.Size = new System.Drawing.Size(152, 25);
			this.textBoxIP.TabIndex = 1;
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(13, 50);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(26, 13);
			this.label5.TabIndex = 2;
			this.label5.Text = "Port";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(13, 22);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(58, 13);
			this.label2.TabIndex = 0;
			this.label2.Text = "IP Address";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 110);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(74, 13);
			this.label1.TabIndex = 2;
			this.label1.Text = "Timeout (sec.)";
			// 
			// groupBoxMorphoSmart
			// 
			this.groupBoxMorphoSmart.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.groupBoxMorphoSmart.Controls.Add(this.buttonRefreshDevice);
			this.groupBoxMorphoSmart.Controls.Add(this.comboBoxDevice);
			this.groupBoxMorphoSmart.Controls.Add(this.label6);
			this.groupBoxMorphoSmart.Location = new System.Drawing.Point(15, 15);
			this.groupBoxMorphoSmart.Name = "groupBoxMorphoSmart";
			this.groupBoxMorphoSmart.Padding = new System.Windows.Forms.Padding(10, 9, 10, 8);
			this.groupBoxMorphoSmart.Size = new System.Drawing.Size(319, 70);
			this.groupBoxMorphoSmart.TabIndex = 0;
			this.groupBoxMorphoSmart.TabStop = false;
			this.groupBoxMorphoSmart.Text = "MorphoSmart";
			// 
			// buttonRefreshDevice
			// 
			this.buttonRefreshDevice.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonRefreshDevice.Location = new System.Drawing.Point(252, 38);
			this.buttonRefreshDevice.Name = "buttonRefreshDevice";
			this.buttonRefreshDevice.Size = new System.Drawing.Size(54, 21);
			this.buttonRefreshDevice.TabIndex = 2;
			this.buttonRefreshDevice.Text = "Refresh";
			this.buttonRefreshDevice.UseVisualStyleBackColor = true;
			this.buttonRefreshDevice.Click += new System.EventHandler(this.buttonRefreshDevice_Click);
			// 
			// comboBoxDevice
			// 
			this.comboBoxDevice.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.comboBoxDevice.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxDevice.FormattingEnabled = true;
			this.comboBoxDevice.Location = new System.Drawing.Point(16, 38);
			this.comboBoxDevice.Name = "comboBoxDevice";
			this.comboBoxDevice.Size = new System.Drawing.Size(230, 21);
			this.comboBoxDevice.TabIndex = 1;
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(13, 22);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(107, 13);
			this.label6.TabIndex = 0;
			this.label6.Text = "MorphoSmart Device";
			// 
			// tabPageCulture
			// 
			this.tabPageCulture.Controls.Add(this.groupBoxCulture);
			this.tabPageCulture.Location = new System.Drawing.Point(4, 22);
			this.tabPageCulture.Name = "tabPageCulture";
			this.tabPageCulture.Padding = new System.Windows.Forms.Padding(12);
			this.tabPageCulture.Size = new System.Drawing.Size(349, 239);
			this.tabPageCulture.TabIndex = 3;
			this.tabPageCulture.Text = "Culture";
			this.tabPageCulture.UseVisualStyleBackColor = true;
			// 
			// groupBoxCulture
			// 
			this.groupBoxCulture.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
			this.groupBoxCulture.Controls.Add(this.radioButtonCultureDe);
			this.groupBoxCulture.Controls.Add(this.radioButtonCultureIt);
			this.groupBoxCulture.Controls.Add(this.radioButtonCultureEn);
			this.groupBoxCulture.Controls.Add(this.radioButtonCultureAr);
			this.groupBoxCulture.Controls.Add(this.radioButtonCulturePt);
			this.groupBoxCulture.Controls.Add(this.radioButtonCultureEs);
			this.groupBoxCulture.Controls.Add(this.radioButtonCultureFr);
			this.groupBoxCulture.Controls.Add(this.radioButtonCultureDefault);
			this.groupBoxCulture.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.groupBoxCulture.ForeColor = System.Drawing.SystemColors.ControlText;
			this.groupBoxCulture.Location = new System.Drawing.Point(15, 15);
			this.groupBoxCulture.Name = "groupBoxCulture";
			this.groupBoxCulture.Padding = new System.Windows.Forms.Padding(10, 9, 10, 8);
			this.groupBoxCulture.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.groupBoxCulture.Size = new System.Drawing.Size(319, 99);
			this.groupBoxCulture.TabIndex = 0;
			this.groupBoxCulture.TabStop = false;
			this.groupBoxCulture.Text = "Culture";
			// 
			// radioButtonCultureDe
			// 
			this.radioButtonCultureDe.AutoSize = true;
			this.radioButtonCultureDe.Location = new System.Drawing.Point(223, 48);
			this.radioButtonCultureDe.Name = "radioButtonCultureDe";
			this.radioButtonCultureDe.Size = new System.Drawing.Size(83, 17);
			this.radioButtonCultureDe.TabIndex = 7;
			this.radioButtonCultureDe.Text = "German (de)";
			this.radioButtonCultureDe.UseVisualStyleBackColor = true;
			// 
			// radioButtonCultureIt
			// 
			this.radioButtonCultureIt.AutoSize = true;
			this.radioButtonCultureIt.Location = new System.Drawing.Point(223, 25);
			this.radioButtonCultureIt.Name = "radioButtonCultureIt";
			this.radioButtonCultureIt.Size = new System.Drawing.Size(67, 17);
			this.radioButtonCultureIt.TabIndex = 6;
			this.radioButtonCultureIt.Text = "Italian (it)";
			this.radioButtonCultureIt.UseVisualStyleBackColor = true;
			// 
			// radioButtonCultureEn
			// 
			this.radioButtonCultureEn.AutoSize = true;
			this.radioButtonCultureEn.Location = new System.Drawing.Point(13, 48);
			this.radioButtonCultureEn.Name = "radioButtonCultureEn";
			this.radioButtonCultureEn.Size = new System.Drawing.Size(80, 17);
			this.radioButtonCultureEn.TabIndex = 1;
			this.radioButtonCultureEn.Text = "English (en)";
			this.radioButtonCultureEn.UseVisualStyleBackColor = true;
			// 
			// radioButtonCultureAr
			// 
			this.radioButtonCultureAr.AutoSize = true;
			this.radioButtonCultureAr.Location = new System.Drawing.Point(109, 71);
			this.radioButtonCultureAr.Name = "radioButtonCultureAr";
			this.radioButtonCultureAr.Size = new System.Drawing.Size(73, 17);
			this.radioButtonCultureAr.TabIndex = 5;
			this.radioButtonCultureAr.Text = "Arabic (ar)";
			this.radioButtonCultureAr.UseVisualStyleBackColor = true;
			// 
			// radioButtonCulturePt
			// 
			this.radioButtonCulturePt.AutoSize = true;
			this.radioButtonCulturePt.Location = new System.Drawing.Point(109, 25);
			this.radioButtonCulturePt.Name = "radioButtonCulturePt";
			this.radioButtonCulturePt.Size = new System.Drawing.Size(97, 17);
			this.radioButtonCulturePt.TabIndex = 3;
			this.radioButtonCulturePt.Text = "Portuguese (pt)";
			this.radioButtonCulturePt.UseVisualStyleBackColor = true;
			// 
			// radioButtonCultureEs
			// 
			this.radioButtonCultureEs.AutoSize = true;
			this.radioButtonCultureEs.Location = new System.Drawing.Point(109, 48);
			this.radioButtonCultureEs.Name = "radioButtonCultureEs";
			this.radioButtonCultureEs.Size = new System.Drawing.Size(83, 17);
			this.radioButtonCultureEs.TabIndex = 4;
			this.radioButtonCultureEs.Text = "Spanish (es)";
			this.radioButtonCultureEs.UseVisualStyleBackColor = true;
			// 
			// radioButtonCultureFr
			// 
			this.radioButtonCultureFr.AutoSize = true;
			this.radioButtonCultureFr.Location = new System.Drawing.Point(13, 71);
			this.radioButtonCultureFr.Name = "radioButtonCultureFr";
			this.radioButtonCultureFr.Size = new System.Drawing.Size(73, 17);
			this.radioButtonCultureFr.TabIndex = 2;
			this.radioButtonCultureFr.Text = "French (fr)";
			this.radioButtonCultureFr.UseVisualStyleBackColor = true;
			// 
			// radioButtonCultureDefault
			// 
			this.radioButtonCultureDefault.AutoSize = true;
			this.radioButtonCultureDefault.Checked = true;
			this.radioButtonCultureDefault.Location = new System.Drawing.Point(13, 25);
			this.radioButtonCultureDefault.Name = "radioButtonCultureDefault";
			this.radioButtonCultureDefault.Size = new System.Drawing.Size(63, 17);
			this.radioButtonCultureDefault.TabIndex = 0;
			this.radioButtonCultureDefault.TabStop = true;
			this.radioButtonCultureDefault.Text = "(default)";
			this.radioButtonCultureDefault.UseVisualStyleBackColor = true;
			// 
			// buttonEnroll
			// 
			this.buttonEnroll.Location = new System.Drawing.Point(12, 294);
			this.buttonEnroll.Name = "buttonEnroll";
			this.buttonEnroll.Size = new System.Drawing.Size(85, 21);
			this.buttonEnroll.TabIndex = 3;
			this.buttonEnroll.Text = "Enroll";
			this.buttonEnroll.UseVisualStyleBackColor = true;
			this.buttonEnroll.Click += new System.EventHandler(this.buttonEnroll_Click);
			// 
			// buttonVerify
			// 
			this.buttonVerify.Location = new System.Drawing.Point(284, 294);
			this.buttonVerify.Name = "buttonVerify";
			this.buttonVerify.Size = new System.Drawing.Size(85, 21);
			this.buttonVerify.TabIndex = 4;
			this.buttonVerify.Text = "Verify";
			this.buttonVerify.UseVisualStyleBackColor = true;
			this.buttonVerify.Click += new System.EventHandler(this.buttonVerify_Click);
			// 
			// buttonCreateObject
			// 
			this.buttonCreateObject.Location = new System.Drawing.Point(284, 12);
			this.buttonCreateObject.Name = "buttonCreateObject";
			this.buttonCreateObject.Size = new System.Drawing.Size(85, 21);
			this.buttonCreateObject.TabIndex = 1;
			this.buttonCreateObject.Text = "Create Object";
			this.buttonCreateObject.UseVisualStyleBackColor = true;
			this.buttonCreateObject.Click += new System.EventHandler(this.buttonCreateObject_Click);
			// 
			// ComponentTypes
			// 
			this.ComponentTypes.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.ComponentTypes.FormattingEnabled = true;
			this.ComponentTypes.Location = new System.Drawing.Point(12, 12);
			this.ComponentTypes.Name = "ComponentTypes";
			this.ComponentTypes.Size = new System.Drawing.Size(266, 21);
			this.ComponentTypes.TabIndex = 0;
			// 
			// label11
			// 
			this.label11.AutoSize = true;
			this.label11.Location = new System.Drawing.Point(15, 171);
			this.label11.Name = "label11";
			this.label11.Size = new System.Drawing.Size(130, 13);
			this.label11.TabIndex = 8;
			this.label11.Text = "Acquisition Mode Strategy";
			// 
			// comboBoxAcquisitionModeStrategy
			// 
			this.comboBoxAcquisitionModeStrategy.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxAcquisitionModeStrategy.FormattingEnabled = true;
			this.comboBoxAcquisitionModeStrategy.Location = new System.Drawing.Point(15, 187);
			this.comboBoxAcquisitionModeStrategy.Name = "comboBoxAcquisitionModeStrategy";
			this.comboBoxAcquisitionModeStrategy.Size = new System.Drawing.Size(177, 21);
			this.comboBoxAcquisitionModeStrategy.TabIndex = 9;
			// 
			// Sample_GenericAcquisitionComponent
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(381, 327);
			this.Controls.Add(this.buttonCreateObject);
			this.Controls.Add(this.ComponentTypes);
			this.Controls.Add(this.buttonVerify);
			this.Controls.Add(this.buttonEnroll);
			this.Controls.Add(this.tabControlOptions);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.Name = "Sample_GenericAcquisitionComponent";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Test Generic Acquisition Component";
			this.Load += new System.EventHandler(this.GenericAcquisitionComponent_Load);
			this.Shown += new System.EventHandler(this.GenericAcquisitionComponent_Shown);
			this.tabControlOptions.ResumeLayout(false);
			this.tabPageCoder.ResumeLayout(false);
			this.tabPageCoder.PerformLayout();
			this.tabPageDisplay.ResumeLayout(false);
			this.tabPageDisplay.PerformLayout();
			this.groupBox1.ResumeLayout(false);
			this.tabPageDevice.ResumeLayout(false);
			this.tabPageDevice.PerformLayout();
			this.groupBoxMorphoAccess.ResumeLayout(false);
			this.groupBoxMorphoAccess.PerformLayout();
			this.groupBoxMorphoSmart.ResumeLayout(false);
			this.groupBoxMorphoSmart.PerformLayout();
			this.tabPageCulture.ResumeLayout(false);
			this.groupBoxCulture.ResumeLayout(false);
			this.groupBoxCulture.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControlOptions;
        private System.Windows.Forms.TabPage tabPageCoder;
        private System.Windows.Forms.TabPage tabPageDisplay;
        private System.Windows.Forms.TabPage tabPageDevice;
        private System.Windows.Forms.TabPage tabPageCulture;
        private System.Windows.Forms.CheckBox checkBoxConsolidate;
        private System.Windows.Forms.Button buttonEnroll;
        private System.Windows.Forms.Button buttonVerify;
        private System.Windows.Forms.CheckBox checkBoxRetry;
        private System.Windows.Forms.CheckBox checkBoxJuvenile;
        private System.Windows.Forms.ComboBox comboBoxFPFormat;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox comboBoxCoderAlgo;
        private System.Windows.Forms.CheckBox checkBoxSaveTemplate;
        private System.Windows.Forms.CheckBox checkBoxSaveBitmap;
        private System.Windows.Forms.CheckBox checkBoxShowLiveQualityBar;
        private System.Windows.Forms.CheckBox checkBoxShowLiveQualityThreshold;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBoxCulture;
        private System.Windows.Forms.RadioButton radioButtonCultureDefault;
        private System.Windows.Forms.RadioButton radioButtonCultureFr;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.RadioButton radioButtonCulturePt;
        private System.Windows.Forms.RadioButton radioButtonCultureEs;
        private System.Windows.Forms.RadioButton radioButtonCultureAr;
        private System.Windows.Forms.RadioButton radioButtonCultureEn;
		private System.Windows.Forms.RadioButton radioButtonCultureIt;
        private System.Windows.Forms.RadioButton radioButtonCultureDe;
        private System.Windows.Forms.CheckBox checkBoxAccept;
        private System.Windows.Forms.Button buttonCreateObject;
        private System.Windows.Forms.ComboBox ComponentTypes;
        private System.Windows.Forms.GroupBox groupBoxMorphoAccess;
        private System.Windows.Forms.Label labelShowQualityDuration;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label2;
		private MorphoControls.IPv4InputBox textBoxIP;
        private System.Windows.Forms.GroupBox groupBoxMorphoSmart;
        private System.Windows.Forms.Button buttonRefreshDevice;
        private System.Windows.Forms.ComboBox comboBoxDevice;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox checkBoxLiveImage;
        private System.Windows.Forms.Button buttonSetImagePath;
        private System.Windows.Forms.ComboBox comboBoxFingerPosition;
        private System.Windows.Forms.Label label8;
		private System.Windows.Forms.ComboBox comboBoxSecurityLevel;
		private System.Windows.Forms.Label label9;
        private System.Windows.Forms.MaskedTextBox textBoxLiveQualityThreshold;
        private System.Windows.Forms.MaskedTextBox textBoxAuthent;
        private System.Windows.Forms.MaskedTextBox textBoxTimeout;
        private System.Windows.Forms.MaskedTextBox textBoxPort;
        private System.Windows.Forms.TextBox textBoxShowQualityDuration;
		private System.Windows.Forms.ComboBox comboBoxFVPFormat;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.ComboBox comboBoxGen;
		private System.Windows.Forms.ComboBox comboBoxAcquisitionModeStrategy;
		private System.Windows.Forms.Label label11;
    }
}

