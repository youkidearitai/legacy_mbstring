--TEST--
Test legacy_mb_detect_encoding() function : empty encoding list
--SKIPIF--
<?php
extension_loaded('legacy_mbstring') or die('skip');
?>
--FILE--
<?php

$string = 'Hello';

try {
    var_dump( legacy_mb_detect_encoding($string, ''));
} catch (\ValueError $e) {
    echo $e->getMessage() . \PHP_EOL;
}
try {
    var_dump( legacy_mb_detect_encoding($string, []));
} catch (\ValueError $e) {
    echo $e->getMessage() . \PHP_EOL;
}

?>
--EXPECT--
legacy_mb_detect_encoding(): Argument #2 ($encodings) must specify at least one encoding
legacy_mb_detect_encoding(): Argument #2 ($encodings) must specify at least one encoding
