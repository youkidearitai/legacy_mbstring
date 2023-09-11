/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 2f26840b896b3349164c02e99fc68d6a0562694e */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_legacy_mb_detect_encoding, 0, 1, MAY_BE_STRING|MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
	ZEND_ARG_TYPE_MASK(0, encodings, MAY_BE_ARRAY|MAY_BE_STRING|MAY_BE_NULL, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, strict, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()


ZEND_FUNCTION(legacy_mb_detect_encoding);


static const zend_function_entry ext_functions[] = {
	ZEND_FE(legacy_mb_detect_encoding, arginfo_legacy_mb_detect_encoding)
	ZEND_FE_END
};
