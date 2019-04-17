#ifndef PHP_SOUNDS_H
#define PHP_SOUNDS_H 1

#define PHP_SOUNDS_VERSION "1.0"
#define PHP_SOUNDS_EXTNAME "sounds"

PHP_FUNCTION(sounds_crop);
PHP_FUNCTION(sounds_volume);
PHP_FUNCTION(sounds_info);
PHP_FUNCTION(sounds_classify);

extern zend_module_entry sounds_module_entry;
#define phpext_sounds_ptr &sounds_module_entry

#endif