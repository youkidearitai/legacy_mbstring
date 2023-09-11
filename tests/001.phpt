--TEST--
Check if legacy_mbstring is loaded
--EXTENSIONS--
legacy_mbstring
--FILE--
<?php
echo 'The extension "legacy_mbstring" is available';
?>
--EXPECT--
The extension "legacy_mbstring" is available
