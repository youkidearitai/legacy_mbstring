--TEST--
mb_detect_encoding() detects UTF-8 emoji byte sequence as ISO-8859-1 since PHP 8.1 #7871
--SKIPIF--
<?php extension_loaded('legacy_mbstring') or die('skip mbstring not available'); ?>
--FILE--
<?php
echo legacy_mb_detect_encoding('🥳', ['UTF-8', 'ISO-8859-1']);
?>
--EXPECT--
UTF-8
