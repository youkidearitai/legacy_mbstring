# legacy_mbstring
PHP 8.0 fallback mbstring functions. However, I hope without using this...

## Why legacy_mbstring?

`mb_detect_encoding` is not have backward compatible since PHP 8.1. I'm not have interesting this function's changes behavior, but many PHP users are seems confused.

examples:

- [mb_detect_encoding() results for UTF-7 differ between PHP 8.0 and 8.1 (if UTF-7 is present in the encodings list and the string contains '+' character) #10192](https://github.com/php/php-src/issues/10192)
- [mb_detect_encoding() detects UTF-8 emoji byte sequence as ISO-8859-1 since PHP 8.1 #7871](https://github.com/php/php-src/issues/7871)

This function should to moving forward. However, some PHP users left behind old behavior. This fact is created that motivate it.

## Instllation

I recommend using `phpize`.

    $ git clone https://github.com/youkidearitai/legacy_mbstring
    $ cd legacy_mbstring
    $ phpize
    $ ./configure
    $ make
    $ make install

## Functions

### `legacy_mb_detect_encoding`

Old behavior (<= PHP 8.0) `mb_detect_encoding`

```php
legacy_mb_detect_encoding(string $string, array|string|null $encodings = null, bool $strict = false): string|false
```

