--TEST--
test1() Basic test
--EXTENSIONS--
legacy_mbstring
--FILE--
<?php
$ret = test1();

var_dump($ret);
?>
--EXPECT--
The extension legacy_mbstring is loaded and working!
NULL
