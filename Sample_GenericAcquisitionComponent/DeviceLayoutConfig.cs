using Morpho.MorphoAcquisition;
using System.Collections.Generic;
using System;

namespace Sample_GenericAcquisitionComponent
{
    internal class DeviceLayoutConfig
    {
        public const string NoneItemString = "<none>";

        #region Private Members
        private DeviceType m_e_Type = DeviceType.NO_DEVICE;

        private bool m_b_FVPTemplatesSupported = false;
        private bool m_b_ConsolidateSupported = false;
        private bool m_b_SecurityLevelSupported = false;
		private bool m_b_AcquisitionModeStrategySupported = false;
        private bool m_b_ShowLiveQualityThresholdSupported = false;

        private string m_x_CoderAlgorithmDefaultValue = string.Empty;
        private string m_x_FPTemplateDefaultValue = string.Empty;
        private string m_x_FVPTemplateDefaultValue = string.Empty;
        private bool m_b_ConsolidateDefaultValue = false;
        private string m_x_SecurityLevelDefaultValue = string.Empty;
		private string m_x_AcquisitionModeStrategyDefaultValue = string.Empty;
        private bool m_b_ShowLiveQualityThresholdDefaultValue = false;

        private AssociativeTuple<string, CoderAlgorithm> m_x_CoderAlgorithmMap = new AssociativeTuple<string, CoderAlgorithm>();
        private AssociativeTuple<string, TemplateFormat> m_x_FPTemplateFormatMap = new AssociativeTuple<string, TemplateFormat>();
        private AssociativeTuple<string, TemplateFormat> m_x_FVPTemplateFormatMap = new AssociativeTuple<string, TemplateFormat>();
        private AssociativeTuple<string, ExtendedSecurityLevel> m_x_SecurityLevelMap = new AssociativeTuple<string, ExtendedSecurityLevel>();
		private AssociativeTuple<string, AcquisitionModeStrategy> m_x_AcquisitionModeStrategyMap = new AssociativeTuple<string, AcquisitionModeStrategy>();
        private AssociativeTuple<string, TemplateFormat> m_x_VerifyTemplateFormatMap = new AssociativeTuple<string, TemplateFormat>();
        #endregion

        #region Public Properties
        public DeviceType Type { get { return this.m_e_Type; } }

        public bool FVPTemplatesSupported { get { return this.m_b_FVPTemplatesSupported; } }
        public bool ConsolidateSupported { get { return this.m_b_ConsolidateSupported; } }
        public bool SecurityLevelSupported { get { return this.m_b_SecurityLevelSupported; } }
		public bool AcquisitionModeStrategySupported { get { return this.m_b_AcquisitionModeStrategySupported; } }
        public bool ShowLiveQualityThresholdSupported { get { return this.m_b_ShowLiveQualityThresholdSupported; } }

        public string CoderAlgorithmDefaultValue { get { return this.m_x_CoderAlgorithmDefaultValue; } }
        public string FPTemplateDefaultValue { get { return this.m_x_FPTemplateDefaultValue; } }
        public string FVPTemplateDefaultValue { get { return this.m_x_FVPTemplateDefaultValue; } }
        public bool ConsolidateDefaultValue { get { return this.m_b_ConsolidateDefaultValue; } }
        public string SecurityLevelDefaultValue { get { return this.m_x_SecurityLevelDefaultValue; } }
		public string AcquisitionModeStrategyDefaultValue { get { return this.m_x_AcquisitionModeStrategyDefaultValue; } }
        public bool ShowLiveQualityThresholdDefaultValue { get { return this.m_b_ShowLiveQualityThresholdDefaultValue; } }

        public AssociativeTuple<string, CoderAlgorithm> CoderAlgorithms { get { return this.m_x_CoderAlgorithmMap; } }
        public AssociativeTuple<string, TemplateFormat> FPTemplateFormats { get { return this.m_x_FPTemplateFormatMap; } }
        public AssociativeTuple<string, TemplateFormat> FVPTemplateFormats { get { return this.m_x_FVPTemplateFormatMap; } }
        public AssociativeTuple<string, ExtendedSecurityLevel> SecurityLevels { get { return this.m_x_SecurityLevelMap; } }
		public AssociativeTuple<string, AcquisitionModeStrategy> AcquisitionModeStrategies { get { return this.m_x_AcquisitionModeStrategyMap; } }
		public AssociativeTuple<string, TemplateFormat> VerifyTemplateFormats { get { return this.m_x_VerifyTemplateFormatMap; } }
        #endregion

        public DeviceLayoutConfig(DeviceType i_e_Type)
        {
            // Save the device type
            this.m_e_Type = i_e_Type;

            // Adding the "None" values for optional template format lists for all device type
            this.m_x_FPTemplateFormatMap[DeviceLayoutConfig.NoneItemString] = TemplateFormat.PK_FVP;
            this.m_x_FVPTemplateFormatMap[DeviceLayoutConfig.NoneItemString] = TemplateFormat.CFV;

            // Configure the device layout config depending on its type
            switch (this.m_e_Type)
            {
                case DeviceType.ACTIVE_MACI:
                    {
                        // Setup coder algorithm list and default value
                        AddCoderAlgorithm(ref this.m_x_CoderAlgorithmMap, CoderAlgorithm.EmbeddedCoder);
                        this.m_x_CoderAlgorithmDefaultValue = this.m_x_CoderAlgorithmMap[CoderAlgorithm.EmbeddedCoder];

                        // Setup Fingerprint template format list and default value
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKCOMPV2);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKMAT);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKLITE);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ANSI_378);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ISO_19794_2_FMR);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Normal_Size);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Compact_Size);
                        this.m_x_FPTemplateDefaultValue = this.m_x_FPTemplateFormatMap[TemplateFormat.PKCOMPV2];

                        // Disable and setup the Finger VP template format list and default value
                        this.m_b_FVPTemplatesSupported = false;
                        this.m_x_FVPTemplateDefaultValue = DeviceLayoutConfig.NoneItemString;

                        // Enable and setup the consolidate default value
                        this.m_b_ConsolidateSupported = true;
                        this.m_b_ConsolidateDefaultValue = true;

                        // Disable and setup the security level list and default value
                        AddSecurityLevel(ref this.m_x_SecurityLevelMap, ExtendedSecurityLevel.STANDARD);
                        this.m_b_SecurityLevelSupported = false;
                        this.m_x_SecurityLevelDefaultValue = this.m_x_SecurityLevelMap[ExtendedSecurityLevel.STANDARD];

						// Disable and setup the acquisition mode strategy list and default value
						AddAcquisitionModeStrategy(ref this.m_x_AcquisitionModeStrategyMap, AcquisitionModeStrategy.EXPERT);
						this.m_b_AcquisitionModeStrategySupported = false;
						this.m_x_AcquisitionModeStrategyDefaultValue = this.m_x_AcquisitionModeStrategyMap[AcquisitionModeStrategy.EXPERT];

                        // Disable and setup the show live quality threshold default value
                        this.m_b_ShowLiveQualityThresholdSupported = false;
                        this.m_b_ShowLiveQualityThresholdDefaultValue = false;

                        // Setup the verify template format list
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.PKCOMPV2);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.PKMAT);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.PKLITE);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.ANSI_378);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.ISO_19794_2_FMR);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Normal_Size);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Compact_Size);

                        break;
                    }



                case DeviceType.MORPHOSMART:
                    {
                        // Setup coder algorithm list and default value
                        AddCoderAlgorithm(ref this.m_x_CoderAlgorithmMap, CoderAlgorithm.EmbeddedCoder);
                        this.m_x_CoderAlgorithmDefaultValue = this.m_x_CoderAlgorithmMap[CoderAlgorithm.EmbeddedCoder];

                        // Setup Fingerprint template format list and default value
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKMAT);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKCOMPV2);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKLITE);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ANSI_378);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ISO_19794_2_FMR);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Normal_Size);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Compact_Size);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKCOMPV2_NORM);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKMAT_NORM);
                        this.m_x_FPTemplateDefaultValue = this.m_x_FPTemplateFormatMap[TemplateFormat.PKMAT];

                        // Enable and setup the Finger VP template format list and default value
                        AddTemplateFormat(ref this.m_x_FVPTemplateFormatMap, TemplateFormat.PK_FVP);
                        this.m_b_FVPTemplatesSupported = true;
                        this.m_x_FVPTemplateDefaultValue = DeviceLayoutConfig.NoneItemString;

                        // Enable and setup the consolidate default value
                        this.m_b_ConsolidateSupported = true;
                        this.m_b_ConsolidateDefaultValue = true;

                        // Enable and setup the security level list and default value
                        AddSecurityLevel(ref this.m_x_SecurityLevelMap, ExtendedSecurityLevel.STANDARD);
                        AddSecurityLevel(ref this.m_x_SecurityLevelMap, ExtendedSecurityLevel.MEDIUM);
                        AddSecurityLevel(ref this.m_x_SecurityLevelMap, ExtendedSecurityLevel.HIGH);
                        AddSecurityLevel(ref this.m_x_SecurityLevelMap, ExtendedSecurityLevel.CRITICAL);
                        this.m_b_SecurityLevelSupported = true;
                        this.m_x_SecurityLevelDefaultValue = this.m_x_SecurityLevelMap[ExtendedSecurityLevel.STANDARD];

						// Disable and setup the acquisition mode strategy list and default value
						AddAcquisitionModeStrategy(ref this.m_x_AcquisitionModeStrategyMap, AcquisitionModeStrategy.EXPERT);
						this.m_b_AcquisitionModeStrategySupported = false;
						this.m_x_AcquisitionModeStrategyDefaultValue = this.m_x_AcquisitionModeStrategyMap[AcquisitionModeStrategy.EXPERT];

                        // Enable and setup the show live quality threshold default value
                        this.m_b_ShowLiveQualityThresholdSupported = true;
                        this.m_b_ShowLiveQualityThresholdDefaultValue = false;

                        // Setup the verify template format list
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.PKMAT);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.PKCOMPV2);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.PKLITE);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.ANSI_378);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.ISO_19794_2_FMR);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Normal_Size);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Compact_Size);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.PK_FVP);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.PKCOMPV2_NORM);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.PKMAT_NORM);

                        break;
                    }



                case DeviceType.MORPHOKIT:
                    {
                        // Setup coder algorithm list and default value
                        AddCoderAlgorithm(ref this.m_x_CoderAlgorithmMap, CoderAlgorithm.V6);
                        AddCoderAlgorithm(ref this.m_x_CoderAlgorithmMap, CoderAlgorithm.V9);
                        AddCoderAlgorithm(ref this.m_x_CoderAlgorithmMap, CoderAlgorithm.V10);
                        this.m_x_CoderAlgorithmDefaultValue = this.m_x_CoderAlgorithmMap[CoderAlgorithm.V10];

                        // Setup Fingerprint template format list and default value
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.CFV);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKMAT);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKCOMPV2);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKLITE);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ANSI_378);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ISO_19794_2_FMR);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Normal_Size);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Compact_Size);
                        this.m_x_FPTemplateDefaultValue = this.m_x_FPTemplateFormatMap[TemplateFormat.CFV];

                        // Disable and setup the Finger VP template format list and default value
                        this.m_b_FVPTemplatesSupported = false;
                        this.m_x_FVPTemplateDefaultValue = DeviceLayoutConfig.NoneItemString;

                        // Enable and setup the consolidate default value
                        this.m_b_ConsolidateSupported = true;
                        this.m_b_ConsolidateDefaultValue = true;

                        // Disable and setup the security level list and default value
                        AddSecurityLevel(ref this.m_x_SecurityLevelMap, ExtendedSecurityLevel.STANDARD);
                        this.m_b_SecurityLevelSupported = false;
                        this.m_x_SecurityLevelDefaultValue = this.m_x_SecurityLevelMap[ExtendedSecurityLevel.STANDARD];

						// Disable and setup the acquisition mode strategy list and default value
						AddAcquisitionModeStrategy(ref this.m_x_AcquisitionModeStrategyMap, AcquisitionModeStrategy.EXPERT);
						this.m_b_AcquisitionModeStrategySupported = false;
						this.m_x_AcquisitionModeStrategyDefaultValue = this.m_x_AcquisitionModeStrategyMap[AcquisitionModeStrategy.EXPERT];

                        // Enable and setup the show live quality threshold default value
                        this.m_b_ShowLiveQualityThresholdSupported = true;
                        this.m_b_ShowLiveQualityThresholdDefaultValue = false;

                        // Setup the verify template format list
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.CFV);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.PKMAT);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.PKCOMPV2);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.PKLITE);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.ANSI_378);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.ISO_19794_2_FMR);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Normal_Size);
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Compact_Size);

                        break;
                    }



                case DeviceType.MORPHOKIT_FVP:
                    {
                        // Setup coder algorithm list and default value
                        AddCoderAlgorithm(ref this.m_x_CoderAlgorithmMap, CoderAlgorithm.V6);
                        AddCoderAlgorithm(ref this.m_x_CoderAlgorithmMap, CoderAlgorithm.V10);
                        this.m_x_CoderAlgorithmDefaultValue = this.m_x_CoderAlgorithmMap[CoderAlgorithm.V10];

                        // Setup Fingerprint template format list and default value
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.CFV);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKMAT);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKCOMPV2);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.PKLITE);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ANSI_378);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ISO_19794_2_FMR);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Normal_Size);
                        AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, TemplateFormat.ISO_19794_2_Card_Format_Compact_Size);
                        this.m_x_FPTemplateDefaultValue = DeviceLayoutConfig.NoneItemString;

                        // Enable and setup the Finger VP template format list and default value
                        AddTemplateFormat(ref this.m_x_FVPTemplateFormatMap, TemplateFormat.PK_FVP);
                        this.m_b_FVPTemplatesSupported = true;
                        this.m_x_FVPTemplateDefaultValue = this.m_x_FVPTemplateFormatMap[TemplateFormat.PK_FVP];

                        // Disable and setup the consolidate default value
                        this.m_b_ConsolidateSupported = false;
                        this.m_b_ConsolidateDefaultValue = true;

                        // Enable and setup the security level list and default value
                        AddSecurityLevel(ref this.m_x_SecurityLevelMap, ExtendedSecurityLevel.STANDARD);
                        AddSecurityLevel(ref this.m_x_SecurityLevelMap, ExtendedSecurityLevel.MEDIUM);
                        AddSecurityLevel(ref this.m_x_SecurityLevelMap, ExtendedSecurityLevel.HIGH);
                        //AddSecurityLevel(ref this.m_x_SecurityLevelMap, ExtendedSecurityLevel.CRITICAL);
                        this.m_b_SecurityLevelSupported = true;
                        this.m_x_SecurityLevelDefaultValue = this.m_x_SecurityLevelMap[ExtendedSecurityLevel.STANDARD];

						// Enable and setup the acquisition mode strategy list and default value
						AddAcquisitionModeStrategy(ref this.m_x_AcquisitionModeStrategyMap, AcquisitionModeStrategy.EXPERT);
						AddAcquisitionModeStrategy(ref this.m_x_AcquisitionModeStrategyMap, AcquisitionModeStrategy.UNIVERSAL_FAST);
						AddAcquisitionModeStrategy(ref this.m_x_AcquisitionModeStrategyMap, AcquisitionModeStrategy.UNIVERSAL_ACCURATE);
						AddAcquisitionModeStrategy(ref this.m_x_AcquisitionModeStrategyMap, AcquisitionModeStrategy.FULL_MULTIMODAL);
						AddAcquisitionModeStrategy(ref this.m_x_AcquisitionModeStrategyMap, AcquisitionModeStrategy.ANTI_SPOOFING);
						this.m_b_AcquisitionModeStrategySupported = true;
						this.m_x_AcquisitionModeStrategyDefaultValue = this.m_x_AcquisitionModeStrategyMap[AcquisitionModeStrategy.EXPERT];

                        // Enable and setup the show live quality threshold default value
                        this.m_b_ShowLiveQualityThresholdSupported = true;
                        this.m_b_ShowLiveQualityThresholdDefaultValue = false;

                        // Setup the verify template format list
                        AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, TemplateFormat.PK_FVP);

                        break;
                    }



                case DeviceType.DUMMYDEVICE:
                    {
                        // Setup coder algorithm list and default value
                        foreach (CoderAlgorithm l_e_CoderAlgorithm in Enum.GetValues(typeof(CoderAlgorithm)))
                            AddCoderAlgorithm(ref this.m_x_CoderAlgorithmMap, l_e_CoderAlgorithm);
                        this.m_x_CoderAlgorithmDefaultValue = this.m_x_CoderAlgorithmMap.Firsts[0];

                        // Setup Fingerprint template format list and default value
                        foreach (TemplateFormat l_e_TemplateFormat in Enum.GetValues(typeof(TemplateFormat)))
                            AddTemplateFormat(ref this.m_x_FPTemplateFormatMap, l_e_TemplateFormat);
                        this.m_x_FPTemplateDefaultValue = this.m_x_FPTemplateFormatMap.Firsts[1];

                        // Enable and setup the Finger VP template format list and default value
                        foreach (TemplateFormat l_e_TemplateFormat in Enum.GetValues(typeof(TemplateFormat)))
                            AddTemplateFormat(ref this.m_x_FVPTemplateFormatMap, l_e_TemplateFormat);
                        this.m_b_FVPTemplatesSupported = true;
                        this.m_x_FVPTemplateDefaultValue = DeviceLayoutConfig.NoneItemString;

                        // Enable and setup the consolidate default value
                        this.m_b_ConsolidateSupported = true;
                        this.m_b_ConsolidateDefaultValue = true;

                        // Enable and setup the security level list and default value
                        foreach (ExtendedSecurityLevel l_e_SecurityLevel in Enum.GetValues(typeof(ExtendedSecurityLevel)))
                            AddSecurityLevel(ref this.m_x_SecurityLevelMap, l_e_SecurityLevel);
                        this.m_b_SecurityLevelSupported = true;
                        this.m_x_SecurityLevelDefaultValue = this.m_x_SecurityLevelMap.Firsts[0];

                        // Enable and setup the show live quality threshold default value
                        this.m_b_ShowLiveQualityThresholdSupported = true;
                        this.m_b_ShowLiveQualityThresholdDefaultValue = false;

                        // Setup the verify template format list
                        foreach (TemplateFormat l_e_TemplateFormat in Enum.GetValues(typeof(TemplateFormat)))
                            AddTemplateFormat(ref this.m_x_VerifyTemplateFormatMap, l_e_TemplateFormat);

                        break;
                    }



                default:
                    throw new InvalidParameterException("Unknown device layout type", ErrorCodes.IED_ERR_INTERNAL);
            }
        }

        private static void AddCoderAlgorithm(ref AssociativeTuple<string, CoderAlgorithm> io_x_CoderAlgorithmMap, CoderAlgorithm i_e_CoderAlgorithm)
        {
            switch (i_e_CoderAlgorithm)
            {
                case CoderAlgorithm.EmbeddedCoder:
                    io_x_CoderAlgorithmMap["Device Embedded Coder"] = CoderAlgorithm.EmbeddedCoder;
                    break;
                case CoderAlgorithm.V6:
                    io_x_CoderAlgorithmMap["Morpho V6 Coder"] = CoderAlgorithm.V6;
                    break;
                case CoderAlgorithm.V9:
                    io_x_CoderAlgorithmMap["Morpho V9 Coder"] = CoderAlgorithm.V9;
                    break;
                case CoderAlgorithm.V10:
                    io_x_CoderAlgorithmMap["Morpho V10 Coder"] = CoderAlgorithm.V10;
                    break;
                default:
                    throw new InvalidParameterException("Unknown coder algorithm value", ErrorCodes.IED_ERR_INTERNAL);
            }
        }

        private static void AddTemplateFormat(ref AssociativeTuple<string, TemplateFormat> io_x_TemplateFormatMap, TemplateFormat i_e_TemplateFormat)
        {
            switch (i_e_TemplateFormat)
            {
                case TemplateFormat.CFV:
                    io_x_TemplateFormatMap["Morpho CFV Fingerprint Template"] = TemplateFormat.CFV;
                    break;
                case TemplateFormat.PKMAT:
                    io_x_TemplateFormatMap["Morpho PkMat Fingerprint Template"] = TemplateFormat.PKMAT;
                    break;
                case TemplateFormat.PKCOMPV2:
                    io_x_TemplateFormatMap["Morpho PkComp V2 Fingerprint Template"] = TemplateFormat.PKCOMPV2;
                    break;
                case TemplateFormat.PKLITE:
                    io_x_TemplateFormatMap["Morpho PkLite Fingerprint Template"] = TemplateFormat.PKLITE;
                    break;
                case TemplateFormat.PK_FVP:
                    io_x_TemplateFormatMap["Morpho PkFVP Finger Vein/Fingerprint Template"] = TemplateFormat.PK_FVP;
                    break;
                case TemplateFormat.ANSI_378:
                    io_x_TemplateFormatMap["ANSI INCITS 378-2004 Finger Minutiae Record"] = TemplateFormat.ANSI_378;
                    break;
                case TemplateFormat.ISO_19794_2_FMR:
                    io_x_TemplateFormatMap["ISO/IEC 19794-2:2005 Finger Minutiae Record"] = TemplateFormat.ISO_19794_2_FMR;
                    break;
                case TemplateFormat.ISO_19794_2_Card_Format_Normal_Size:
                    io_x_TemplateFormatMap["ISO/IEC 19794-2:2005 Finger Minutiae Card Format Normal Size"] = TemplateFormat.ISO_19794_2_Card_Format_Normal_Size;
                    break;
                case TemplateFormat.ISO_19794_2_Card_Format_Compact_Size:
                    io_x_TemplateFormatMap["ISO/IEC 19794-2:2005 Finger Minutiae Card Format Compact Size"] = TemplateFormat.ISO_19794_2_Card_Format_Compact_Size;
                    break;
                case TemplateFormat.PKMAT_NORM:
                    io_x_TemplateFormatMap["Morpho PkMat Norm Fingerprint Template"] = TemplateFormat.PKMAT_NORM;
                    break;
                case TemplateFormat.PKCOMPV2_NORM:
                    io_x_TemplateFormatMap["Morpho PkComp V2 Norm Fingerprint Template"] = TemplateFormat.PKCOMPV2_NORM;
                    break;
                default:
                    throw new InvalidParameterException("Unknown template format value", ErrorCodes.IED_ERR_INTERNAL);
            }
        }

        private static void AddSecurityLevel(ref AssociativeTuple<string, ExtendedSecurityLevel> io_x_SecurityLevelMap, ExtendedSecurityLevel i_e_SecurityLevel)
        {
            switch (i_e_SecurityLevel)
            {
                case ExtendedSecurityLevel.STANDARD:
                    io_x_SecurityLevelMap["Standard"] = ExtendedSecurityLevel.STANDARD;
                    break;
                case ExtendedSecurityLevel.MEDIUM:
                    io_x_SecurityLevelMap["Medium"] = ExtendedSecurityLevel.MEDIUM;
                    break;
                case ExtendedSecurityLevel.HIGH:
                    io_x_SecurityLevelMap["High"] = ExtendedSecurityLevel.HIGH;
                    break;
                case ExtendedSecurityLevel.CRITICAL:
                    io_x_SecurityLevelMap["Critical"] = ExtendedSecurityLevel.CRITICAL;
                    break;
                default:
                    throw new InvalidParameterException("Unknown security level value", ErrorCodes.IED_ERR_INTERNAL);
            }
        }

		private static void AddAcquisitionModeStrategy(ref AssociativeTuple<string, AcquisitionModeStrategy> io_x_AcquisitionModeStrategyMap, AcquisitionModeStrategy i_e_AcquisitionModeStrategy)
		{
			switch(i_e_AcquisitionModeStrategy)
			{
				case AcquisitionModeStrategy.EXPERT:
					io_x_AcquisitionModeStrategyMap["Expert"] = AcquisitionModeStrategy.EXPERT;
					break;
				case AcquisitionModeStrategy.UNIVERSAL_FAST:
					io_x_AcquisitionModeStrategyMap["Universal (fast)"] = AcquisitionModeStrategy.UNIVERSAL_FAST;
					break;
				case AcquisitionModeStrategy.UNIVERSAL_ACCURATE:
					io_x_AcquisitionModeStrategyMap["Universal (accurate)"] = AcquisitionModeStrategy.UNIVERSAL_ACCURATE;
					break;
				case AcquisitionModeStrategy.FULL_MULTIMODAL:
					io_x_AcquisitionModeStrategyMap["Full multimodal"] = AcquisitionModeStrategy.FULL_MULTIMODAL;
					break;
				case AcquisitionModeStrategy.ANTI_SPOOFING:
					io_x_AcquisitionModeStrategyMap["Anti-spoofing"] = AcquisitionModeStrategy.ANTI_SPOOFING;
					break;
				default:
					throw new InvalidParameterException("Unknown acquisition mode strategy value", ErrorCodes.IED_ERR_INTERNAL);
			}
		}
    }
}
