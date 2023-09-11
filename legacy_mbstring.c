/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | https://www.php.net/license/3_01.txt                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Tsukada Takuya <tsukada@fminn.nagano.nagano.jp>              |
   |         Rui Hirokawa <hirokawa@php.net>                              |
   |         Hironori Sato <satoh@jpnnet.com>                             |
   |         Shigeru Kanemoto <sgk@happysize.co.jp>                       |
   +----------------------------------------------------------------------+
*/
/* legacy_mbstring extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_legacy_mbstring.h"

/* {{{ includes */
#include "libmbfl/config.h"
#include "php.h"
#include "php_ini.h"
#include "php_variables.h"
#include "legacy_mbstring.h"
#include "ext/standard/php_string.h"
#include "ext/standard/php_mail.h"
#include "ext/standard/exec.h"
#include "ext/standard/url.h"
#include "main/php_output.h"
#include "ext/standard/info.h"
#include "ext/pcre/php_pcre.h"

#include "libmbfl/mbfl/mbfilter_8bit.h"
#include "libmbfl/mbfl/mbfilter_pass.h"
#include "libmbfl/mbfl/mbfilter_wchar.h"
#include "libmbfl/filters/mbfilter_ascii.h"
#include "libmbfl/filters/mbfilter_base64.h"
#include "libmbfl/filters/mbfilter_qprint.h"
#include "libmbfl/filters/mbfilter_ucs4.h"
#include "libmbfl/filters/mbfilter_utf8.h"
#include "libmbfl/filters/mbfilter_tl_jisx0201_jisx0208.h"

#include "php_variables.h"
#include "php_globals.h"
#include "rfc1867.h"
#include "php_content_types.h"
#include "SAPI.h"
#include "php_unicode.h"
#include "TSRM.h"

#include "mb_gpc.h"

#ifdef HAVE_MBREGEX
# include "php_mbregex.h"
#endif

#include "zend_multibyte.h"
#include "legacy_mbstring_arginfo.h"
/* }}} */

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

ZEND_DECLARE_MODULE_GLOBALS(mbstring)

static PHP_GINIT_FUNCTION(mbstring);
static PHP_GSHUTDOWN_FUNCTION(mbstring);

/* {{{ static _php_mb_ini_mbstring_internal_encoding_set */
static int _php_mb_ini_mbstring_internal_encoding_set(const char *new_value, size_t new_value_length)
{
	const mbfl_encoding *encoding;

	if (!new_value || !new_value_length || !(encoding = mbfl_name2encoding(new_value))) {
		/* falls back to UTF-8 if an unknown encoding name is given */
		if (new_value) {
			php_error_docref("ref.mbstring", E_WARNING, "Unknown encoding \"%s\" in ini setting", new_value);
		}
		encoding = &mbfl_encoding_utf8;
	}
	MBSTRG(internal_encoding) = encoding;
	MBSTRG(current_internal_encoding) = encoding;
	return SUCCESS;
}
/* }}} */

static void mbstring_internal_encoding_changed_hook(void) {
	/* One of the internal_encoding / input_encoding / output_encoding ini settings changed. */
	if (!MBSTRG(internal_encoding_set)) {
		const char *encoding = php_get_internal_encoding();
		_php_mb_ini_mbstring_internal_encoding_set(encoding, strlen(encoding));
	}
}

/* {{{ PHP_MINIT_FUNCTION(legacy_mbstring) */
PHP_MINIT_FUNCTION(legacy_mbstring)
{
#if defined(COMPILE_DL_MBSTRING) && defined(ZTS)
ZEND_TSRMLS_CACHE_UPDATE();
#endif
	php_internal_encoding_changed = mbstring_internal_encoding_changed_hook;
	mbstring_internal_encoding_changed_hook();

	return SUCCESS;
}
/* }}} */

/* {{{ php_mb_default_identify_list */
typedef struct _php_mb_nls_ident_list {
	enum mbfl_no_language lang;
	const enum mbfl_no_encoding *list;
	size_t list_size;
} php_mb_nls_ident_list;

static const enum mbfl_no_encoding php_mb_default_identify_list_ja[] = {
	mbfl_no_encoding_ascii,
	mbfl_no_encoding_jis,
	mbfl_no_encoding_utf8,
	mbfl_no_encoding_euc_jp,
	mbfl_no_encoding_sjis
};

static const enum mbfl_no_encoding php_mb_default_identify_list_cn[] = {
	mbfl_no_encoding_ascii,
	mbfl_no_encoding_utf8,
	mbfl_no_encoding_euc_cn,
	mbfl_no_encoding_cp936
};

static const enum mbfl_no_encoding php_mb_default_identify_list_tw_hk[] = {
	mbfl_no_encoding_ascii,
	mbfl_no_encoding_utf8,
	mbfl_no_encoding_euc_tw,
	mbfl_no_encoding_big5
};

static const enum mbfl_no_encoding php_mb_default_identify_list_kr[] = {
	mbfl_no_encoding_ascii,
	mbfl_no_encoding_utf8,
	mbfl_no_encoding_euc_kr,
	mbfl_no_encoding_uhc
};

static const enum mbfl_no_encoding php_mb_default_identify_list_ru[] = {
	mbfl_no_encoding_ascii,
	mbfl_no_encoding_utf8,
	mbfl_no_encoding_koi8r,
	mbfl_no_encoding_cp1251,
	mbfl_no_encoding_cp866
};

static const enum mbfl_no_encoding php_mb_default_identify_list_hy[] = {
	mbfl_no_encoding_ascii,
	mbfl_no_encoding_utf8,
	mbfl_no_encoding_armscii8
};

static const enum mbfl_no_encoding php_mb_default_identify_list_tr[] = {
	mbfl_no_encoding_ascii,
	mbfl_no_encoding_utf8,
	mbfl_no_encoding_cp1254,
	mbfl_no_encoding_8859_9
};

static const enum mbfl_no_encoding php_mb_default_identify_list_ua[] = {
	mbfl_no_encoding_ascii,
	mbfl_no_encoding_utf8,
	mbfl_no_encoding_koi8u
};

static const enum mbfl_no_encoding php_mb_default_identify_list_neut[] = {
	mbfl_no_encoding_ascii,
	mbfl_no_encoding_utf8
};


static const php_mb_nls_ident_list php_mb_default_identify_list[] = {
	{ mbfl_no_language_japanese, php_mb_default_identify_list_ja, sizeof(php_mb_default_identify_list_ja) / sizeof(php_mb_default_identify_list_ja[0]) },
	{ mbfl_no_language_korean, php_mb_default_identify_list_kr, sizeof(php_mb_default_identify_list_kr) / sizeof(php_mb_default_identify_list_kr[0]) },
	{ mbfl_no_language_traditional_chinese, php_mb_default_identify_list_tw_hk, sizeof(php_mb_default_identify_list_tw_hk) / sizeof(php_mb_default_identify_list_tw_hk[0]) },
	{ mbfl_no_language_simplified_chinese, php_mb_default_identify_list_cn, sizeof(php_mb_default_identify_list_cn) / sizeof(php_mb_default_identify_list_cn[0]) },
	{ mbfl_no_language_russian, php_mb_default_identify_list_ru, sizeof(php_mb_default_identify_list_ru) / sizeof(php_mb_default_identify_list_ru[0]) },
	{ mbfl_no_language_armenian, php_mb_default_identify_list_hy, sizeof(php_mb_default_identify_list_hy) / sizeof(php_mb_default_identify_list_hy[0]) },
	{ mbfl_no_language_turkish, php_mb_default_identify_list_tr, sizeof(php_mb_default_identify_list_tr) / sizeof(php_mb_default_identify_list_tr[0]) },
	{ mbfl_no_language_ukrainian, php_mb_default_identify_list_ua, sizeof(php_mb_default_identify_list_ua) / sizeof(php_mb_default_identify_list_ua[0]) },
	{ mbfl_no_language_neutral, php_mb_default_identify_list_neut, sizeof(php_mb_default_identify_list_neut) / sizeof(php_mb_default_identify_list_neut[0]) }
};

/* }}} */

/* {{{ module global initialize handler */
static PHP_GINIT_FUNCTION(mbstring)
{
#if defined(COMPILE_DL_MBSTRING) && defined(ZTS)
ZEND_TSRMLS_CACHE_UPDATE();
#endif

	mbstring_globals->language = mbfl_no_language_uni;
	mbstring_globals->internal_encoding = NULL;
	mbstring_globals->current_internal_encoding = mbstring_globals->internal_encoding;
	mbstring_globals->http_output_encoding = &mbfl_encoding_pass;
	mbstring_globals->current_http_output_encoding = &mbfl_encoding_pass;
	mbstring_globals->http_input_identify = NULL;
	mbstring_globals->http_input_identify_get = NULL;
	mbstring_globals->http_input_identify_post = NULL;
	mbstring_globals->http_input_identify_cookie = NULL;
	mbstring_globals->http_input_identify_string = NULL;
	mbstring_globals->http_input_list = NULL;
	mbstring_globals->http_input_list_size = 0;
	mbstring_globals->detect_order_list = NULL;
	mbstring_globals->detect_order_list_size = 0;
	mbstring_globals->current_detect_order_list = NULL;
	mbstring_globals->current_detect_order_list_size = 0;
	mbstring_globals->default_detect_order_list = (enum mbfl_no_encoding *) php_mb_default_identify_list_neut;
	mbstring_globals->default_detect_order_list_size = sizeof(php_mb_default_identify_list_neut) / sizeof(php_mb_default_identify_list_neut[0]);
	mbstring_globals->filter_illegal_mode = MBFL_OUTPUTFILTER_ILLEGAL_MODE_CHAR;
	mbstring_globals->filter_illegal_substchar = 0x3f;	/* '?' */
	mbstring_globals->current_filter_illegal_mode = MBFL_OUTPUTFILTER_ILLEGAL_MODE_CHAR;
	mbstring_globals->current_filter_illegal_substchar = 0x3f;	/* '?' */
	mbstring_globals->illegalchars = 0;
	mbstring_globals->encoding_translation = 0;
	mbstring_globals->strict_detection = 0;
	mbstring_globals->outconv = NULL;
	mbstring_globals->http_output_conv_mimetypes = NULL;
#ifdef HAVE_MBREGEX
	mbstring_globals->mb_regex_globals = php_mb_regex_globals_alloc();
#endif
	mbstring_globals->last_used_encoding_name = NULL;
	mbstring_globals->last_used_encoding = NULL;
	mbstring_globals->internal_encoding_set = 0;
	mbstring_globals->http_output_set = 0;
	mbstring_globals->http_input_set = 0;
}
/* }}} */

/* {{{ PHP_GSHUTDOWN_FUNCTION */
static PHP_GSHUTDOWN_FUNCTION(mbstring)
{
	if (mbstring_globals->http_input_list) {
		free(ZEND_VOIDP(mbstring_globals->http_input_list));
	}
	if (mbstring_globals->detect_order_list) {
		free(ZEND_VOIDP(mbstring_globals->detect_order_list));
	}
#ifdef HAVE_MBREGEX
	php_mb_regex_globals_free(mbstring_globals->mb_regex_globals);
#endif
}
/* }}} */

static size_t count_commas(const char *p, const char *end) {
	size_t count = 0;
	while ((p = memchr(p, ',', end - p))) {
		count++;
		p++;
	}
	return count;
}

static const mbfl_encoding *php_mb_get_encoding_or_pass(const char *encoding_name) {
	if (strcmp(encoding_name, "pass") == 0) {
		return &mbfl_encoding_pass;
	}

	return mbfl_name2encoding(encoding_name);
}

/* {{{ static zend_result php_mb_parse_encoding_list()
 *  Return FAILURE if input contains any illegal encoding, otherwise SUCCESS.
 * 	Emits a ValueError in function context and a warning in INI context, in INI context arg_num must be 0.
 */
static zend_result php_mb_parse_encoding_list(const char *value, size_t value_length,
	const mbfl_encoding ***return_list, size_t *return_size, bool persistent, uint32_t arg_num,
	zend_bool allow_pass_encoding)
{
	if (value == NULL || value_length == 0) {
		*return_list = NULL;
		*return_size = 0;
		return SUCCESS;
	} else {
		zend_bool included_auto;
		size_t n, size;
		char *p1, *endp, *tmpstr;
		const mbfl_encoding **entry, **list;

		/* copy the value string for work */
		if (value[0]=='"' && value[value_length-1]=='"' && value_length>2) {
			tmpstr = (char *)estrndup(value+1, value_length-2);
			value_length -= 2;
		} else {
			tmpstr = (char *)estrndup(value, value_length);
		}

		endp = tmpstr + value_length;
		size = 1 + count_commas(tmpstr, endp) + MBSTRG(default_detect_order_list_size);
		list = (const mbfl_encoding **)pecalloc(size, sizeof(mbfl_encoding*), persistent);
		entry = list;
		n = 0;
		included_auto = 0;
		p1 = tmpstr;
		while (1) {
			char *comma = (char *) php_memnstr(p1, ",", 1, endp);
			char *p = comma ? comma : endp;
			*p = '\0';
			/* trim spaces */
			while (p1 < p && (*p1 == ' ' || *p1 == '\t')) {
				p1++;
			}
			p--;
			while (p > p1 && (*p == ' ' || *p == '\t')) {
				*p = '\0';
				p--;
			}
			/* convert to the encoding number and check encoding */
			if (strcasecmp(p1, "auto") == 0) {
				if (!included_auto) {
					const enum mbfl_no_encoding *src = MBSTRG(default_detect_order_list);
					const size_t identify_list_size = MBSTRG(default_detect_order_list_size);
					size_t i;
					included_auto = 1;
					for (i = 0; i < identify_list_size; i++) {
						*entry++ = mbfl_no2encoding(*src++);
						n++;
					}
				}
			} else {
				const mbfl_encoding *encoding =
					allow_pass_encoding ? php_mb_get_encoding_or_pass(p1) : mbfl_name2encoding(p1);
				if (!encoding) {
					/* Called from an INI setting modification */
					if (arg_num == 0) {
						php_error_docref("ref.mbstring", E_WARNING, "INI setting contains invalid encoding \"%s\"", p1);
					} else {
						zend_argument_value_error(arg_num, "contains invalid encoding \"%s\"", p1);
					}
					efree(tmpstr);
					pefree(ZEND_VOIDP(list), persistent);
					return FAILURE;
				}

				*entry++ = encoding;
				n++;
			}
			if (n >= size || comma == NULL) {
				break;
			}
			p1 = comma + 1;
		}
		*return_list = list;
		*return_size = n;
		efree(tmpstr);
	}

	return SUCCESS;
}
/* }}} */


/* {{{ static int php_mb_parse_encoding_array()
 *  Return FAILURE if input contains any illegal encoding, otherwise SUCCESS.
 * 	Emits a ValueError in function context and a warning in INI context, in INI context arg_num must be 0.
 */
static int php_mb_parse_encoding_array(HashTable *target_hash, const mbfl_encoding ***return_list,
	size_t *return_size, uint32_t arg_num)
{
	/* Allocate enough space to include the default detect order if "auto" is used. */
	size_t size = zend_hash_num_elements(target_hash) + MBSTRG(default_detect_order_list_size);
	const mbfl_encoding **list = ecalloc(size, sizeof(mbfl_encoding*));
	const mbfl_encoding **entry = list;
	zend_bool included_auto = 0;
	size_t n = 0;
	zval *hash_entry;
	ZEND_HASH_FOREACH_VAL(target_hash, hash_entry) {
		zend_string *encoding_str = zval_try_get_string(hash_entry);
		if (UNEXPECTED(!encoding_str)) {
			efree(ZEND_VOIDP(list));
			return FAILURE;
		}

		if (strcasecmp(ZSTR_VAL(encoding_str), "auto") == 0) {
			if (!included_auto) {
				const enum mbfl_no_encoding *src = MBSTRG(default_detect_order_list);
				const size_t identify_list_size = MBSTRG(default_detect_order_list_size);
				size_t j;

				included_auto = 1;
				for (j = 0; j < identify_list_size; j++) {
					*entry++ = mbfl_no2encoding(*src++);
					n++;
				}
			}
		} else {
			const mbfl_encoding *encoding = mbfl_name2encoding(ZSTR_VAL(encoding_str));
			if (encoding) {
				*entry++ = encoding;
				n++;
			} else {
				zend_argument_value_error(arg_num, "contains invalid encoding \"%s\"", ZSTR_VAL(encoding_str));
				zend_string_release(encoding_str);
				efree(ZEND_VOIDP(list));
				return FAILURE;
			}
		}
		zend_string_release(encoding_str);
	} ZEND_HASH_FOREACH_END();
	*return_list = list;
	*return_size = n;
	return SUCCESS;
}
/* }}} */


/* {{{ Encodings of the given string is returned (as a string) */
PHP_FUNCTION(legacy_mb_detect_encoding)
{
	char *str;
	size_t str_len;
	zend_string *encoding_str = NULL;
	HashTable *encoding_ht = NULL;
	zend_bool strict = 0;

	mbfl_string string;
	const mbfl_encoding *ret;
	const mbfl_encoding **elist;
	size_t size;
	zend_bool free_elist;

	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_STRING(str, str_len)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY_HT_OR_STR_OR_NULL(encoding_ht, encoding_str)
		Z_PARAM_BOOL(strict)
	ZEND_PARSE_PARAMETERS_END();

	/* make encoding list */
	if (encoding_ht) {
		if (FAILURE == php_mb_parse_encoding_array(encoding_ht, &elist, &size, 2)) {
			RETURN_THROWS();
		}
		free_elist = 1;
	} else if (encoding_str) {
		if (FAILURE == php_mb_parse_encoding_list(ZSTR_VAL(encoding_str), ZSTR_LEN(encoding_str), &elist, &size, /* persistent */ 0, /* arg_num */ 2, /* allow_pass_encoding */ 0)) {
			RETURN_THROWS();
		}
		free_elist = 1;
	} else {
		elist = MBSTRG(current_detect_order_list);
		size = MBSTRG(current_detect_order_list_size);
		free_elist = 0;
	}

	if (size == 0) {
		efree(ZEND_VOIDP(elist));
		zend_argument_value_error(2, "must specify at least one encoding");
		RETURN_THROWS();
	}

	if (ZEND_NUM_ARGS() < 3) {
		strict = MBSTRG(strict_detection);
	}

	mbfl_string_init(&string);
	string.val = (unsigned char *)str;
	string.len = str_len;
	ret = mbfl_identify_encoding(&string, elist, size, strict);

	if (free_elist) {
		efree(ZEND_VOIDP(elist));
	}

	if (ret == NULL) {
		RETURN_FALSE;
	}

	RETVAL_STRING((char *)ret->name);
}
/* }}} */

/* {{{ php_mb_populate_current_detect_order_list */
static void php_mb_populate_current_detect_order_list(void)
{
	const mbfl_encoding **entry = 0;
	size_t nentries;

	if (MBSTRG(detect_order_list) && MBSTRG(detect_order_list_size)) {
		nentries = MBSTRG(detect_order_list_size);
		entry = (const mbfl_encoding **)safe_emalloc(nentries, sizeof(mbfl_encoding*), 0);
		memcpy(ZEND_VOIDP(entry), MBSTRG(detect_order_list), sizeof(mbfl_encoding*) * nentries);
	} else {
		const enum mbfl_no_encoding *src = MBSTRG(default_detect_order_list);
		size_t i;
		nentries = MBSTRG(default_detect_order_list_size);
		entry = (const mbfl_encoding **)safe_emalloc(nentries, sizeof(mbfl_encoding*), 0);
		for (i = 0; i < nentries; i++) {
			entry[i] = mbfl_no2encoding(src[i]);
		}
	}
	MBSTRG(current_detect_order_list) = entry;
	MBSTRG(current_detect_order_list_size) = nentries;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(legacy_mbstring)
{
#if defined(ZTS) && defined(COMPILE_DL_LEGACY_MBSTRING)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	MBSTRG(current_internal_encoding) = MBSTRG(internal_encoding);
	MBSTRG(current_http_output_encoding) = MBSTRG(http_output_encoding);
	MBSTRG(current_filter_illegal_mode) = MBSTRG(filter_illegal_mode);
	MBSTRG(current_filter_illegal_substchar) = MBSTRG(filter_illegal_substchar);

	MBSTRG(illegalchars) = 0;

	php_mb_populate_current_detect_order_list();
	zend_multibyte_set_internal_encoding((const zend_encoding *)MBSTRG(internal_encoding));

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(legacy_mbstring)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "legacy_mbstring support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION(legacy_mbstring) */
PHP_MSHUTDOWN_FUNCTION(legacy_mbstring)
{
	UNREGISTER_INI_ENTRIES();

	zend_multibyte_restore_functions();

	php_internal_encoding_changed = NULL;

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION(legacy_mbstring) */
PHP_RSHUTDOWN_FUNCTION(legacy_mbstring)
{
	if (MBSTRG(current_detect_order_list) != NULL) {
		efree(ZEND_VOIDP(MBSTRG(current_detect_order_list)));
		MBSTRG(current_detect_order_list) = NULL;
		MBSTRG(current_detect_order_list_size) = 0;
	}
	if (MBSTRG(outconv) != NULL) {
		MBSTRG(illegalchars) += mbfl_buffer_illegalchars(MBSTRG(outconv));
		mbfl_buffer_converter_delete(MBSTRG(outconv));
		MBSTRG(outconv) = NULL;
	}

	/* clear http input identification. */
	MBSTRG(http_input_identify) = NULL;
	MBSTRG(http_input_identify_post) = NULL;
	MBSTRG(http_input_identify_get) = NULL;
	MBSTRG(http_input_identify_cookie) = NULL;
	MBSTRG(http_input_identify_string) = NULL;

	if (MBSTRG(last_used_encoding_name)) {
		zend_string_release(MBSTRG(last_used_encoding_name));
		MBSTRG(last_used_encoding_name) = NULL;
	}

	MBSTRG(internal_encoding_set) = 0;
	MBSTRG(http_output_set) = 0;
	MBSTRG(http_input_set) = 0;

	return SUCCESS;
}
/* }}} */

/* {{{ legacy_mbstring_module_entry */
zend_module_entry legacy_mbstring_module_entry = {
	STANDARD_MODULE_HEADER,
	"legacy_mbstring",					/* Extension name */
	ext_functions,					/* zend_function_entry */
	PHP_MINIT(legacy_mbstring),							/* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(legacy_mbstring),							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(legacy_mbstring),			/* PHP_RINIT - Request initialization */
	PHP_RSHUTDOWN(legacy_mbstring),							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(legacy_mbstring),			/* PHP_MINFO - Module info */
	PHP_LEGACY_MBSTRING_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_LEGACY_MBSTRING
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(legacy_mbstring)
#endif
