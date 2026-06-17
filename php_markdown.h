#ifndef PHP_MARKDOWN_H
#define PHP_MARKDOWN_H

extern zend_module_entry markdown_module_entry;
#define phpext_markdown_ptr &markdown_module_entry

#ifndef PHP_MARKDOWN_VERSION
# define PHP_MARKDOWN_VERSION "0.1.0"
#endif

#endif
