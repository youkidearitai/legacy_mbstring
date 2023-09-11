dnl config.m4 for extension legacy_mbstring

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use 'with':

AC_DEFUN([PHP_MBSTRING_ADD_SOURCES], [
  PHP_MBSTRING_SOURCES="$PHP_MBSTRING_SOURCES $1"
])

AC_DEFUN([PHP_MBSTRING_ADD_BASE_SOURCES], [
  PHP_MBSTRING_BASE_SOURCES="$PHP_MBSTRING_BASE_SOURCES $1"
])

AC_DEFUN([PHP_MBSTRING_ADD_BUILD_DIR], [
  PHP_MBSTRING_EXTRA_BUILD_DIRS="$PHP_MBSTRING_EXTRA_BUILD_DIRS $1"
])

AC_DEFUN([PHP_MBSTRING_ADD_INCLUDE], [
  PHP_MBSTRING_EXTRA_INCLUDES="$PHP_MBSTRING_EXTRA_INCLUDES $1"
])

AC_DEFUN([PHP_MBSTRING_ADD_CFLAG], [
  PHP_MBSTRING_CFLAGS="$PHP_MBSTRING_CFLAGS $1"
])

AC_DEFUN([PHP_MBSTRING_ADD_INSTALL_HEADERS], [
  PHP_MBSTRING_INSTALL_HEADERS="$PHP_MBSTRING_INSTALL_HEADERS $1"
])

AC_DEFUN([PHP_MBSTRING_EXTENSION], [
  PHP_NEW_EXTENSION(legacy_mbstring, $PHP_MBSTRING_BASE_SOURCES $PHP_MBSTRING_SOURCES, $ext_shared,, $PHP_MBSTRING_CFLAGS -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
  PHP_SUBST(MBSTRING_SHARED_LIBADD)

  for dir in $PHP_MBSTRING_EXTRA_BUILD_DIRS; do
    PHP_ADD_BUILD_DIR([$ext_builddir/$dir], 1)
  done

  for dir in $PHP_MBSTRING_EXTRA_INCLUDES; do
    PHP_ADD_INCLUDE([$ext_srcdir/$dir])
    PHP_ADD_INCLUDE([$ext_builddir/$dir])
  done

  out="php_config.h"

  if test "$ext_shared" != "no" && test -f "$ext_builddir/config.h.in"; then
    out="$abs_builddir/config.h"
  fi

  cat > $ext_builddir/libmbfl/config.h <<EOF
#include "$out"
EOF

  PHP_MBSTRING_ADD_INSTALL_HEADERS([mbstring.h])
  PHP_INSTALL_HEADERS([ext/legacy_mbstring], [$PHP_MBSTRING_INSTALL_HEADERS])
])

AC_DEFUN([PHP_MBSTRING_SETUP_LIBMBFL], [
  dnl
  dnl Bundled libmbfl is required and can not be disabled
  dnl
  PHP_MBSTRING_ADD_BUILD_DIR([libmbfl])
  PHP_MBSTRING_ADD_BUILD_DIR([libmbfl/mbfl])
  PHP_MBSTRING_ADD_BUILD_DIR([libmbfl/filters])
  PHP_MBSTRING_ADD_BUILD_DIR([libmbfl/nls])
  PHP_MBSTRING_ADD_INCLUDE([libmbfl])
  PHP_MBSTRING_ADD_INCLUDE([libmbfl/mbfl])

  PHP_MBSTRING_ADD_SOURCES([
    libmbfl/filters/html_entities.c
    libmbfl/filters/mbfilter_7bit.c
    libmbfl/filters/mbfilter_ascii.c
    libmbfl/filters/mbfilter_base64.c
    libmbfl/filters/mbfilter_big5.c
    libmbfl/filters/mbfilter_byte2.c
    libmbfl/filters/mbfilter_byte4.c
    libmbfl/filters/mbfilter_cp1251.c
    libmbfl/filters/mbfilter_cp1252.c
    libmbfl/filters/mbfilter_cp1254.c
    libmbfl/filters/mbfilter_cp5022x.c
    libmbfl/filters/mbfilter_cp51932.c
    libmbfl/filters/mbfilter_cp850.c
    libmbfl/filters/mbfilter_cp866.c
    libmbfl/filters/mbfilter_cp932.c
    libmbfl/filters/mbfilter_cp936.c
    libmbfl/filters/mbfilter_gb18030.c
    libmbfl/filters/mbfilter_euc_cn.c
    libmbfl/filters/mbfilter_euc_jp.c
    libmbfl/filters/mbfilter_euc_jp_2004.c
    libmbfl/filters/mbfilter_euc_jp_win.c
    libmbfl/filters/mbfilter_euc_kr.c
    libmbfl/filters/mbfilter_euc_tw.c
    libmbfl/filters/mbfilter_htmlent.c
    libmbfl/filters/mbfilter_hz.c
    libmbfl/filters/mbfilter_iso2022_jp_ms.c
    libmbfl/filters/mbfilter_iso2022jp_2004.c
    libmbfl/filters/mbfilter_iso2022jp_mobile.c
    libmbfl/filters/mbfilter_iso2022_kr.c
    libmbfl/filters/mbfilter_iso8859_1.c
    libmbfl/filters/mbfilter_iso8859_10.c
    libmbfl/filters/mbfilter_iso8859_13.c
    libmbfl/filters/mbfilter_iso8859_14.c
    libmbfl/filters/mbfilter_iso8859_15.c
    libmbfl/filters/mbfilter_iso8859_16.c
    libmbfl/filters/mbfilter_iso8859_2.c
    libmbfl/filters/mbfilter_iso8859_3.c
    libmbfl/filters/mbfilter_iso8859_4.c
    libmbfl/filters/mbfilter_iso8859_5.c
    libmbfl/filters/mbfilter_iso8859_6.c
    libmbfl/filters/mbfilter_iso8859_7.c
    libmbfl/filters/mbfilter_iso8859_8.c
    libmbfl/filters/mbfilter_iso8859_9.c
    libmbfl/filters/mbfilter_jis.c
    libmbfl/filters/mbfilter_koi8r.c
    libmbfl/filters/mbfilter_armscii8.c
    libmbfl/filters/mbfilter_qprint.c
    libmbfl/filters/mbfilter_sjis.c
    libmbfl/filters/mbfilter_sjis_open.c
    libmbfl/filters/mbfilter_sjis_mobile.c
    libmbfl/filters/mbfilter_sjis_mac.c
    libmbfl/filters/mbfilter_sjis_2004.c
    libmbfl/filters/mbfilter_tl_jisx0201_jisx0208.c
    libmbfl/filters/mbfilter_ucs2.c
    libmbfl/filters/mbfilter_ucs4.c
    libmbfl/filters/mbfilter_uhc.c
    libmbfl/filters/mbfilter_utf16.c
    libmbfl/filters/mbfilter_utf32.c
    libmbfl/filters/mbfilter_utf7.c
    libmbfl/filters/mbfilter_utf7imap.c
    libmbfl/filters/mbfilter_utf8.c
    libmbfl/filters/mbfilter_utf8_mobile.c
    libmbfl/filters/mbfilter_uuencode.c
    libmbfl/filters/mbfilter_koi8u.c
    libmbfl/mbfl/mbfilter.c
    libmbfl/mbfl/mbfilter_8bit.c
    libmbfl/mbfl/mbfilter_pass.c
    libmbfl/mbfl/mbfilter_wchar.c
    libmbfl/mbfl/mbfl_convert.c
    libmbfl/mbfl/mbfl_encoding.c
    libmbfl/mbfl/mbfl_filter_output.c
    libmbfl/mbfl/mbfl_ident.c
    libmbfl/mbfl/mbfl_language.c
    libmbfl/mbfl/mbfl_memory_device.c
    libmbfl/mbfl/mbfl_string.c
    libmbfl/nls/nls_de.c
    libmbfl/nls/nls_en.c
    libmbfl/nls/nls_ja.c
    libmbfl/nls/nls_kr.c
    libmbfl/nls/nls_neutral.c
    libmbfl/nls/nls_ru.c
    libmbfl/nls/nls_uni.c
    libmbfl/nls/nls_zh.c
    libmbfl/nls/nls_hy.c
    libmbfl/nls/nls_tr.c
    libmbfl/nls/nls_ua.c
  ])
  PHP_MBSTRING_ADD_INSTALL_HEADERS([libmbfl/config.h libmbfl/mbfl/eaw_table.h libmbfl/mbfl/mbfilter.h libmbfl/mbfl/mbfilter_8bit.h libmbfl/mbfl/mbfilter_pass.h libmbfl/mbfl/mbfilter_wchar.h libmbfl/mbfl/mbfl_consts.h libmbfl/mbfl/mbfl_convert.h libmbfl/mbfl/mbfl_defs.h libmbfl/mbfl/mbfl_encoding.h libmbfl/mbfl/mbfl_filter_output.h libmbfl/mbfl/mbfl_ident.h libmbfl/mbfl/mbfl_language.h libmbfl/mbfl/mbfl_memory_device.h libmbfl/mbfl/mbfl_string.h])
])


dnl PHP_ARG_WITH([legacy_mbstring],
dnl   [for legacy_mbstring support],
dnl   [AS_HELP_STRING([--with-legacy_mbstring],
dnl     [Include legacy_mbstring support])])

dnl Otherwise use 'enable':

PHP_ARG_ENABLE([legacy_mbstring],
  [whether to enable legacy_mbstring support],
  [AS_HELP_STRING([--enable-legacy_mbstring],
    [Enable legacy_mbstring support])],
  [no])

if test "$PHP_LEGACY_MBSTRING" != "no"; then
  dnl Write more examples of tests here...

  dnl Remove this code block if the library does not support pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBS, LEGACY_MBSTRING_SHARED_LIBADD)

  dnl If you need to check for a particular library version using PKG_CHECK_MODULES,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-legacy_mbstring -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/legacy_mbstring.h"  # you most likely want to change this
  dnl if test -r $PHP_LEGACY_MBSTRING/$SEARCH_FOR; then # path given as parameter
  dnl   LEGACY_MBSTRING_DIR=$PHP_LEGACY_MBSTRING
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for legacy_mbstring files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       LEGACY_MBSTRING_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$LEGACY_MBSTRING_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the legacy_mbstring distribution])
  dnl fi

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-legacy_mbstring -> add include path
  dnl PHP_ADD_INCLUDE($LEGACY_MBSTRING_DIR/include)

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-legacy_mbstring -> check for lib and symbol presence
  dnl LIBNAME=LEGACY_MBSTRING # you may want to change this
  dnl LIBSYMBOL=LEGACY_MBSTRING # you most likely want to change this

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   AC_DEFINE(HAVE_LEGACY_MBSTRING_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your legacy_mbstring library.])
  dnl ], [
  dnl   $LIBFOO_LIBS
  dnl ])

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $LEGACY_MBSTRING_DIR/$PHP_LIBDIR, LEGACY_MBSTRING_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_LEGACY_MBSTRING_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your legacy_mbstring library.])
  dnl ],[
  dnl   -L$LEGACY_MBSTRING_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(LEGACY_MBSTRING_SHARED_LIBADD)

  dnl In case of no dependencies
  AC_DEFINE(HAVE_LEGACY_MBSTRING, 1, [ Have legacy_mbstring support ])

  PHP_NEW_EXTENSION(legacy_mbstring, legacy_mbstring.c, $ext_shared)
  dnl libmbfl is required
  PHP_MBSTRING_SETUP_LIBMBFL
  PHP_MBSTRING_EXTENSION
fi
