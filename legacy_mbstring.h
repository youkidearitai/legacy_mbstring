/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Tsukada Takuya <tsukada@fminn.nagano.nagano.jp>              |
   |         Hironori Sato <satoh@jpnnet.com>                             |
   |         Shigeru Kanemoto <sgk@happysize.co.jp>                       |
   +----------------------------------------------------------------------+
 */

#ifndef _MBSTRING_H
#define _MBSTRING_H

#include "php_version.h"
#define PHP_MBSTRING_VERSION PHP_VERSION

#ifdef PHP_WIN32
#	undef MBSTRING_API
#	ifdef MBSTRING_EXPORTS
#		define MBSTRING_API __declspec(dllexport)
#	elif defined(COMPILE_DL_MBSTRING)
#		define MBSTRING_API __declspec(dllimport)
#	else
#		define MBSTRING_API /* nothing special */
#	endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#	undef MBSTRING_API
#	define MBSTRING_API __attribute__ ((visibility("default")))
#else
#	undef MBSTRING_API
#	define MBSTRING_API /* nothing special */
#endif

#include "libmbfl_php80/mbfl/mbfilter.h"

#define PHP_MBSTRING_API 20021024

PHP_MINIT_FUNCTION(legacy_mbstring);
PHP_MSHUTDOWN_FUNCTION(legacy_mbstring);
PHP_RINIT_FUNCTION(legacy_mbstring);
PHP_RSHUTDOWN_FUNCTION(legacy_mbstring);
PHP_MINFO_FUNCTION(legacy_mbstring);

ZEND_BEGIN_MODULE_GLOBALS(legacy_mbstring)
	const mbfl_encoding **detect_order_list;
	size_t detect_order_list_size;
	const mbfl_encoding **current_detect_order_list;
	size_t current_detect_order_list_size;
	enum mbfl_no_encoding *default_detect_order_list;
	size_t default_detect_order_list_size;
	zend_bool strict_detection;
ZEND_END_MODULE_GLOBALS(legacy_mbstring)

#define MBSTRG(v) ZEND_MODULE_GLOBALS_ACCESSOR(legacy_mbstring, v)

#if defined(ZTS) && defined(COMPILE_DL_LEGACY_MBSTRING)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif /* _MBSTRING_H */
