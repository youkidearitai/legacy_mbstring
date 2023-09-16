--TEST--
 mb_detect_encoding() results for UTF-7 differ between PHP 8.0 and 8.1 (if UTF-7 is present in the encodings list and the string contains '+' character) #10192
--SKIPIF--
<?php extension_loaded('legacy_mbstring') or die('skip mbstring not available'); ?>
--INI--
mbstring.language=Japanese
--FILE--
<?php

ini_set('display_errors', '1');	// display runtime errors
error_reporting(E_ALL & ~E_NOTICE & ~E_STRICT & ~E_DEPRECATED); // error reporting
date_default_timezone_set('UTC');

function detect_encoding($ystr, $csetlist='UTF-8, ISO-8859-1, ISO-8859-15, ISO-8859-2, ISO-8859-9, ISO-8859-3, ISO-8859-4, ISO-8859-5, ISO-8859-6, ISO-8859-7, ISO-8859-8, ISO-8859-10, ISO-8859-13, ISO-8859-14, ISO-8859-16, UTF-7, ASCII, SJIS, EUC-JP, JIS, ISO-2022-JP, EUC-CN, GB18030, ISO-2022-KR, KOI8-R, KOI8-U') {
	return legacy_mb_detect_encoding((string)$ystr, (string)$csetlist, true); // mixed: (bool) FALSE or (string) 'CHARSET'
}

echo detect_encoding('A + B'); // expected output: UTF-8, but on PHP 8.1.x / 8.2.x returns UTF-7 if the '+' character is present in a string ; the PHP 8.0.x and 7.4.x behaves correctly and outputs UTF-8 also in this case
echo "\n";
echo detect_encoding('A - B'); // expected output: UTF-8 ; correct on all PHP versions 8.2.x / 8.1.x / 8.0.x / 7.4.x
?>
--EXPECT--
UTF-8
UTF-8
