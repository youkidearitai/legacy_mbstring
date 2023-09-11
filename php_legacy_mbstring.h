/* legacy_mbstring extension for PHP */

#ifndef PHP_LEGACY_MBSTRING_H
# define PHP_LEGACY_MBSTRING_H

extern zend_module_entry legacy_mbstring_module_entry;
# define phpext_legacy_mbstring_ptr &legacy_mbstring_module_entry

# define PHP_LEGACY_MBSTRING_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_LEGACY_MBSTRING)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_LEGACY_MBSTRING_H */
