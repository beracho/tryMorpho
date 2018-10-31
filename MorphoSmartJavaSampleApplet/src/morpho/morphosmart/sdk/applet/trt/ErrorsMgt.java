package morpho.morphosmart.sdk.applet.trt;

import morpho.morphosmart.sdk.api.MorphoSmartSDK;

/**
 * Errors Management.
 */
public class ErrorsMgt {

	/**
	 * getSDKErrorText
	 *
	 * @param errorCode
	 * @return error text
	 */
	public static String getSDKErrorText(int errorCode) {
		String message = "";
			switch (errorCode) {
			case MorphoSmartSDK.MORPHO_OK:
				message = "No error";
				break;
			case MorphoSmartSDK.MORPHOERR_INTERNAL:
				message = "Biometrics device performed an internal error";
				break;
			case MorphoSmartSDK.MORPHOERR_PROTOCOLE:
				message = "Communication protocole error";
				break;
			case MorphoSmartSDK.MORPHOERR_CONNECT:
				message = "Can not connect biometrics device";
				break;
			case MorphoSmartSDK.MORPHOERR_CLOSE_COM:
				message = "Error while closing communication port";
				break;
			case MorphoSmartSDK.MORPHOERR_BADPARAMETER:
				message = "Invalid parameter";
				break;
			case MorphoSmartSDK.MORPHOERR_MEMORY_PC:
				message = "Not enough memory (in the PC).";
				break;
			case MorphoSmartSDK.MORPHOERR_MEMORY_DEVICE:
				message = "Not enough memory for the creation of a database in the MSO";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_HIT:
				message = "Authentication or Identification failed";
				break;
			case MorphoSmartSDK.MORPHOERR_STATUS:
				message = "MSO returned an unknown status error";
				break;
			case MorphoSmartSDK.MORPHOERR_DB_FULL:
				message = "The database is full";
				break;
			case MorphoSmartSDK.MORPHOERR_DB_EMPTY:
				message = "The database is empty";
				break;
			case MorphoSmartSDK.MORPHOERR_ALREADY_ENROLLED:
				message = "User has already been enrolled";
				break;
			case MorphoSmartSDK.MORPHOERR_BASE_NOT_FOUND:
				message = "The specified base does not exist";
				break;
			case MorphoSmartSDK.MORPHOERR_BASE_ALREADY_EXISTS:
				message = "The specified base already exist";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_ASSOCIATED_DB:
				message = "User object has been instanciated without MorphoDatabase.getUser";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_ASSOCIATED_DEVICE:
				message = "Database object has been instanciated without MorphoDevice.getDatabase";
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_TEMPLATE:
				message = "The template is not valid";
				break;
			case MorphoSmartSDK.MORPHOERR_NOT_IMPLEMENTED:
				message = "Command not yet implemented in this release";
				break;
			case MorphoSmartSDK.MORPHOERR_TIMEOUT:
				message = "No response after defined time";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_REGISTERED_TEMPLATE:
				message = "No templates have been registered (using MorphoTemplateList.putTemplate).";
				break;
			case MorphoSmartSDK.MORPHOERR_FIELD_NOT_FOUND:
				message = "Field does not exist";
				break;
			case MorphoSmartSDK.MORPHOERR_CORRUPTED_CLASS:
				message = "Class has been corrupted";
				break;
			case MorphoSmartSDK.MORPHOERR_TO_MANY_TEMPLATE:
				message = "There are too many templates";
				break;
			case MorphoSmartSDK.MORPHOERR_TO_MANY_FIELD:
				message = "There are too many fields";
				break;
			case MorphoSmartSDK.MORPHOERR_MIXED_TEMPLATE:
				message = "Templates with differents formats are mixed";
				break;
			case MorphoSmartSDK.MORPHOERR_CMDE_ABORTED:
				message = "Command has been aborted";
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_PK_FORMAT:
				message = "Invalid PK format";
				break;
			case MorphoSmartSDK.MORPHOERR_SAME_FINGER:
				message = "User gave twice the same finger";
				break;
			case MorphoSmartSDK.MORPHOERR_OUT_OF_FIELD:
				message = "The number of the additional field is more than 128";
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_USER_ID:
				message = "UserID is not valid: either the record identifier does not exist in the database (Consulting operation), either it already exists (Enroll operation).";
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_USER_DATA:
				message = "The user data are not valid";
				break;
			case MorphoSmartSDK.MORPHOERR_FIELD_INVALID:
				message = "Additional field name length is more than MORPHO_FIELD_NAME_LEN";
				break;
			case MorphoSmartSDK.MORPHOERR_USER_NOT_FOUND:
				message = "User is not found";
				break;
			case MorphoSmartSDK.MORPHOERR_COM_NOT_OPEN:
				message = "Serial COM has not been opened";
				break;
			case MorphoSmartSDK.MORPHOERR_ELT_ALREADY_PRESENT:
				message = "This element is already present in the list";
				break;
			case MorphoSmartSDK.MORPHOERR_NOCALLTO_DBQUERRYFIRST:
				message = "You have to call MorphoDatabase.dbQueryFirst to initialize the querry";
				break;
			case MorphoSmartSDK.MORPHOERR_USER:
				message = "The communication callback functions returns error between -10000 and -10499";
				break;
			case MorphoSmartSDK.MORPHOERR_BAD_COMPRESSION:
				message = "The Compression is not valid";
				break;
			case MorphoSmartSDK.MORPHOERR_SECU:
				message = "Security error";
				break;
			case MorphoSmartSDK.MORPHOERR_CERTIF_UNKNOW:
				message = "The MSO has not the certificate necessary to verify the signature";
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_CLASS:
				message = "The class has been destroyed";
				break;
			case MorphoSmartSDK.MORPHOERR_USB_DEVICE_NAME_UNKNOWN:
				message = "The specified Usb device is not plugged";
				break;
			case MorphoSmartSDK.MORPHOERR_CERTIF_INVALID:
				message = "The certificate is not valid";
				break;
			case MorphoSmartSDK.MORPHOERR_SIGNER_ID:
				message = "The certificate identity is not the same than the X984 certificate identity";
				break;
			case MorphoSmartSDK.MORPHOERR_SIGNER_ID_INVALID:
				message = "The X984 certificate identity size is different to 20 octets (SHA_1 size).";
				break;
			case MorphoSmartSDK.MORPHOERR_FFD:
				message = "False Finger Detected";
				break;
			case MorphoSmartSDK.MORPHOERR_MOIST_FINGER:
				message = "The finger can be too moist or the scanner is wet";
				break;
			case MorphoSmartSDK.MORPHOERR_MOVED_FINGER :
				message = "The finger moved during acquisition or removed earlier";
				break;
			case MorphoSmartSDK.MORPHOERR_SATURATED_FINGER :
				message = "The finger can be too shiny or too much external light";
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_FINGER :
				message = "The finger is invalid";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_SERVER:
				message = "The Morpho MorphoSmart Service Provider Usb Server is stopped or not installed";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_NOT_INITIALIZED:
				message = "No parameter has been initialized";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_PIN_NEEDED:
				message = "Code pin is needed : it is the first enrollment";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_REENROLL_NOT_ALLOWED:
				message = "User is not allowed to be reenrolled";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_ENROLL_FAILED:
				message = "Enrollment failed";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_IDENT_FAILED:
				message = "Identification failed";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_MORE_OTP:
				message = "No more OTP available (sequence number = 0).";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_NO_HIT:
				message = "Authentication or Identification failed";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_ENROLL_NEEDED:
				message = "Enrollment needed before generating OTP";
				break;
			case MorphoSmartSDK.MORPHOERR_DEVICE_LOCKED:
				message = "The device is locked";
				break;
			case MorphoSmartSDK.MORPHOERR_DEVICE_NOT_LOCK:
				message = "The device is not locked";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_LOCK_GEN_OTP:
				message = "ILV_OTP_GENERATE Locked";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_LOCK_SET_PARAM:
				message = "ILV_OTP_SET_PARAMETERS  Locked";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_LOCK_ENROLL:
				message = "ILV_OTP_ENROLL_USER Locked";
				break;
			case MorphoSmartSDK.MORPHOERR_FVP_MINUTIAE_SECURITY_MISMATCH:
				message = "Security level mismatch: attempt to match fingerprint template in high security level (MorphoSmart&tm; FINGER VP only).";
				break;
			case MorphoSmartSDK.MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN:
				message = "Misplaced or withdrawn finger has been detected during acquisition (MorphoSmart&tm; FINGER VP only).";
				break;
			case MorphoSmartSDK.MORPHOERR_LICENSE_MISSING:
				message = "A required license is missing";
				break;
			case MorphoSmartSDK.MORPHOERR_ADVANCED_SECURITY_LEVEL_MISMATCH:
				message = "The MorphoSmart&tm; FINGER VP was unsuccessful in making a multimodal template compatible with advanced security levels";
				break;
			case MorphoSmartSDK.MORPHOERR_BAD_FINAL_FINGER_PRINT_QUALITY:
				message = "The MorphoSmart&tm; was unsuccessful in capturing the fingerprint with a quality greater than or equal to the specified threshold";
				break;
			case MorphoSmartSDK.MORPHOERR_ISO19794_FIR_IMAGES_MISMATCH_PARAMETER:
				message = "Image information are mismatching for ISO-19794-4 FIR creation";
				break;
			case MorphoSmartSDK.MORPHOERR_FFD_FINGER_MISPLACED:
				message = "Misplaced finger has been detected by FFD during acquisition (MorphoSmart&tm; MSO 3x1 FFD only)";
				break;
			case MorphoSmartSDK.MORPHOERR_KEY_NOT_FOUND:
				message = "A required key is missing";
				break;
			case MorphoSmartSDK.MORPHOWARNING_WSQ_COMPRESSION_RATIO:
				message = "Compression ratio higher than that recommended by the norme  ISO-19794-4 (15).\nThe compression ratio must be less than or equal to 15.";
				break;
			case MorphoSmartSDK.MORPHOERR_ADVANCED_SECURITY_LEVEL_NOT_AVAILABLE:
				message = "The MorphoSmart&tm; FINGER VP was unsuccessful in making a multimodal template compatible with advanced security levels";
				break;
			case MorphoSmartSDK.MORPHOERR_UNAVAILABLE:
				message = "A functionality has been requested, but is not available on the currently connected device";
				break;
			default:
				message = "MORPHOERR";
				break;
		}
		return message;
	}

	/**
	 * Convert SDK error to a readable error.
	 *
	 * @param errorCode
	 * @return
	 */
	public static String convertSDKError(int ret) {
		String message = "";
		switch (ret) {
			case MorphoSmartSDK.MORPHO_OK:
				message = "MORPHO_OK";
				break;
			case MorphoSmartSDK.MORPHOERR_INTERNAL:
				message = "MORPHOERR_INTERNAL";
				break;
			case MorphoSmartSDK.MORPHOERR_PROTOCOLE:
				message = "MORPHOERR_PROTOCOLE";
				break;
			case MorphoSmartSDK.MORPHOERR_CONNECT:
				message = "MORPHOERR_CONNECT";
				break;
			case MorphoSmartSDK.MORPHOERR_CLOSE_COM:
				message = "MORPHOERR_CLOSE_COM";
				break;
			case MorphoSmartSDK.MORPHOERR_BADPARAMETER:
				message = "MORPHOERR_BADPARAMETER";
				break;
			case MorphoSmartSDK.MORPHOERR_MEMORY_PC:
				message = "MORPHOERR_MEMORY_PC";
				break;
			case MorphoSmartSDK.MORPHOERR_MEMORY_DEVICE:
				message = "MORPHOERR_MEMORY_DEVICE";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_HIT:
				message = "MORPHOERR_NO_HIT";
				break;
			case MorphoSmartSDK.MORPHOERR_STATUS:
				message = "MORPHOERR_STATUS";
				break;
			case MorphoSmartSDK.MORPHOERR_DB_FULL:
				message = "MORPHOERR_DB_FULL";
				break;
			case MorphoSmartSDK.MORPHOERR_DB_EMPTY:
				message = "MORPHOERR_DB_EMPTY";
				break;
			case MorphoSmartSDK.MORPHOERR_ALREADY_ENROLLED:
				message = "MORPHOERR_ALREADY_ENROLLED";
				break;
			case MorphoSmartSDK.MORPHOERR_BASE_NOT_FOUND:
				message = "MORPHOERR_BASE_NOT_FOUND";
				break;
			case MorphoSmartSDK.MORPHOERR_BASE_ALREADY_EXISTS:
				message = "MORPHOERR_BASE_ALREADY_EXISTS";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_ASSOCIATED_DB:
				message = "MORPHOERR_NO_ASSOCIATED_DB";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_ASSOCIATED_DEVICE:
				message = "MORPHOERR_NO_ASSOCIATED_DEVICE";
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_TEMPLATE:
				message = "MORPHOERR_INVALID_TEMPLATE";
				break;
			case MorphoSmartSDK.MORPHOERR_NOT_IMPLEMENTED:
				message = "MORPHOERR_NOT_IMPLEMENTED";
				break;
			case MorphoSmartSDK.MORPHOERR_TIMEOUT:
				message = "MORPHOERR_TIMEOUT";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_REGISTERED_TEMPLATE:
				message = "MORPHOERR_NO_REGISTERED_TEMPLATE";
				break;
			case MorphoSmartSDK.MORPHOERR_FIELD_NOT_FOUND:
				message = "MORPHOERR_FIELD_NOT_FOUND";
				break;
			case MorphoSmartSDK.MORPHOERR_CORRUPTED_CLASS:
				message = "MORPHOERR_CORRUPTED_CLASS";
				break;
			case MorphoSmartSDK.MORPHOERR_TO_MANY_TEMPLATE:
				message = "MORPHOERR_TO_MANY_TEMPLATE";
				break;
			case MorphoSmartSDK.MORPHOERR_TO_MANY_FIELD:
				message = "MORPHOERR_TO_MANY_FIELD";
				break;
			case MorphoSmartSDK.MORPHOERR_MIXED_TEMPLATE:
				message = "MORPHOERR_MIXED_TEMPLATE";
				break;
			case MorphoSmartSDK.MORPHOERR_CMDE_ABORTED:
				message = "MORPHOERR_CMDE_ABORTED";
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_PK_FORMAT:
				message = "MORPHOERR_INVALID_PK_FORMAT";
				break;
			case MorphoSmartSDK.MORPHOERR_SAME_FINGER:
				message = "MORPHOERR_SAME_FINGER";
				break;
			case MorphoSmartSDK.MORPHOERR_OUT_OF_FIELD:
				message = "MORPHOERR_OUT_OF_FIELD";
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_USER_ID:
				message = "MORPHOERR_INVALID_USER_ID";
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_USER_DATA:
				message = "MORPHOERR_INVALID_USER_DATA";
				break;
			case MorphoSmartSDK.MORPHOERR_FIELD_INVALID:
				message = "MORPHOERR_FIELD_INVALID";
				break;
			case MorphoSmartSDK.MORPHOERR_USER_NOT_FOUND:
				message = "MORPHOERR_USER_NOT_FOUND";
				break;
			case MorphoSmartSDK.MORPHOERR_COM_NOT_OPEN:
				message = "MORPHOERR_COM_NOT_OPEN";
				break;
			case MorphoSmartSDK.MORPHOERR_ELT_ALREADY_PRESENT:
				message = "MORPHOERR_ELT_ALREADY_PRESENT";
				break;
			case MorphoSmartSDK.MORPHOERR_NOCALLTO_DBQUERRYFIRST:
				message = "MORPHOERR_NOCALLTO_DBQUERRYFIRST";
				break;
			case MorphoSmartSDK.MORPHOERR_USER:
				message = "MORPHOERR_USER";
				break;
			case MorphoSmartSDK.MORPHOERR_BAD_COMPRESSION:
				message = "MORPHOERR_BAD_COMPRESSION";
				break;
			case MorphoSmartSDK.MORPHOERR_SECU:
				message = "MORPHOERR_SECU";
				break;
			case MorphoSmartSDK.MORPHOERR_CERTIF_UNKNOW:
				message = "MORPHOERR_CERTIF_UNKNOW";
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_CLASS:
				message = "MORPHOERR_INVALID_CLASS";
				break;
			case MorphoSmartSDK.MORPHOERR_USB_DEVICE_NAME_UNKNOWN:
				message = "MORPHOERR_USB_DEVICE_NAME_UNKNOWN";
				break;
			case MorphoSmartSDK.MORPHOERR_CERTIF_INVALID:
				message = "MORPHOERR_CERTIF_INVALID";
				break;
			case MorphoSmartSDK.MORPHOERR_SIGNER_ID:
				message = "MORPHOERR_SIGNER_ID";
				break;
			case MorphoSmartSDK.MORPHOERR_SIGNER_ID_INVALID:
				message = "MORPHOERR_SIGNER_ID_INVALID";
				break;
			case MorphoSmartSDK.MORPHOERR_FFD:
				message = "MORPHOERR_FFD";
				break;
			case MorphoSmartSDK.MORPHOERR_MOIST_FINGER:
				message = "MORPHOERR_MOIST_FINGER";
				break;
			case MorphoSmartSDK.MORPHOERR_MOVED_FINGER :
				message = "MORPHOERR_MOVED_FINGER";
				break;
			case MorphoSmartSDK.MORPHOERR_SATURATED_FINGER :
				message = "MORPHOERR_SATURATED_FINGER";
				break;
			case MorphoSmartSDK.MORPHOERR_INVALID_FINGER :
				message = "MORPHOERR_INVALID_FINGER";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_SERVER:
				message = "MORPHOERR_NO_SERVER";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_NOT_INITIALIZED:
				message = "MORPHOERR_OTP_NOT_INITIALIZED";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_PIN_NEEDED:
				message = "MORPHOERR_OTP_PIN_NEEDED";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_REENROLL_NOT_ALLOWED:
				message = "MORPHOERR_OTP_REENROLL_NOT_ALLOWED";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_ENROLL_FAILED:
				message = "MORPHOERR_OTP_ENROLL_FAILED";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_IDENT_FAILED:
				message = "MORPHOERR_OTP_IDENT_FAILED";
				break;
			case MorphoSmartSDK.MORPHOERR_NO_MORE_OTP:
				message = "MORPHOERR_NO_MORE_OTP";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_NO_HIT:
				message = "MORPHOERR_OTP_NO_HIT";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_ENROLL_NEEDED:
				message = "MORPHOERR_OTP_ENROLL_NEEDED";
				break;
			case MorphoSmartSDK.MORPHOERR_DEVICE_LOCKED:
				message = "MORPHOERR_DEVICE_LOCKED";
				break;
			case MorphoSmartSDK.MORPHOERR_DEVICE_NOT_LOCK:
				message = "MORPHOERR_DEVICE_NOT_LOCK";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_LOCK_GEN_OTP:
				message = "MORPHOERR_OTP_LOCK_GEN_OTP";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_LOCK_SET_PARAM:
				message = "MORPHOERR_OTP_LOCK_SET_PARAM";
				break;
			case MorphoSmartSDK.MORPHOERR_OTP_LOCK_ENROLL:
				message = "MORPHOERR_OTP_LOCK_ENROLL";
				break;
			case MorphoSmartSDK.MORPHOERR_FVP_MINUTIAE_SECURITY_MISMATCH:
				message = "MORPHOERR_FVP_MINUTIAE_SECURITY_MISMATCH";
				break;
			case MorphoSmartSDK.MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN:
				message = "MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN";
				break;
			case MorphoSmartSDK.MORPHOERR_LICENSE_MISSING:
				message = "MORPHOERR_LICENSE_MISSING";
				break;
			case MorphoSmartSDK.MORPHOERR_ADVANCED_SECURITY_LEVEL_MISMATCH:
				message = "MORPHOERR_ADVANCED_SECURITY_LEVEL_MISMATCH";
				break;
			case MorphoSmartSDK.MORPHOERR_BAD_FINAL_FINGER_PRINT_QUALITY:
				message = "MORPHOERR_BAD_FINAL_FINGER_PRINT_QUALITY";
				break;
			case MorphoSmartSDK.MORPHOERR_ISO19794_FIR_IMAGES_MISMATCH_PARAMETER:
				message = "MORPHOERR_ISO19794_FIR_IMAGES_MISMATCH_PARAMETER";
				break;
			case MorphoSmartSDK.MORPHOERR_FFD_FINGER_MISPLACED:
				message = "MORPHOERR_FFD_FINGER_MISPLACED";
				break;
			case MorphoSmartSDK.MORPHOERR_KEY_NOT_FOUND:
				message = "MORPHOERR_KEY_NOT_FOUND";
				break;
			case MorphoSmartSDK.MORPHOWARNING_WSQ_COMPRESSION_RATIO:
				message = "MORPHOWARNING_WSQ_COMPRESSION_RATIO";
				break;
			case MorphoSmartSDK.MORPHOERR_ADVANCED_SECURITY_LEVEL_NOT_AVAILABLE:
				message = "MORPHOERR_ADVANCED_SECURITY_LEVEL_NOT_AVAILABLE";
				break;
			case MorphoSmartSDK.MORPHOERR_UNAVAILABLE:
				message = "MORPHOERR_UNAVAILABLE";
				break;
			default:
				message = "MORPHOERR";
				break;
		}
		return message;
	}

}
