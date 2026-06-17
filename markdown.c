#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "ext/spl/spl_exceptions.h"
#include "zend_exceptions.h"
#include "zend_smart_str.h"
#include "php_markdown.h"
#include "third_party/md4c/md4c.h"
#include "third_party/md4c/md4c-html.h"
#include "markdown_arginfo.h"

#include <limits.h>
#include <stddef.h>

static zend_class_entry *markdown_renderer_ce;
static zend_class_entry *markdown_options_ce;
static zend_class_entry *markdown_flag_ce;
static zend_class_entry *markdown_dialect_ce;
static zend_class_entry *markdown_html_flag_ce;
static zend_object_handlers markdown_renderer_handlers;

typedef struct _markdown_renderer_object {
	zend_long parser_flags;
	zend_long renderer_flags;
	zend_object std;
} markdown_renderer_object;

typedef struct _markdown_output_buffer {
	smart_str html;
} markdown_output_buffer;

static zend_always_inline markdown_renderer_object *markdown_renderer_from_obj(zend_object *obj)
{
	return (markdown_renderer_object *) ((char *) obj - XtOffsetOf(markdown_renderer_object, std));
}

#define Z_MARKDOWN_RENDERER_P(zv) markdown_renderer_from_obj(Z_OBJ_P((zv)))

static zend_object *markdown_renderer_create_object(zend_class_entry *ce)
{
	markdown_renderer_object *intern = zend_object_alloc(sizeof(markdown_renderer_object), ce);

	intern->parser_flags = 0;
	intern->renderer_flags = 0;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &markdown_renderer_handlers;

	return &intern->std;
}

static void markdown_renderer_free_object(zend_object *object)
{
	markdown_renderer_object *intern = markdown_renderer_from_obj(object);

	zend_object_std_dtor(&intern->std);
}

static zend_long markdown_default_parser_flags(void)
{
	return MD_FLAG_TABLES
		| MD_FLAG_STRIKETHROUGH
		| MD_FLAG_TASKLISTS
		| MD_FLAG_PERMISSIVEURLAUTOLINKS
		| MD_FLAG_PERMISSIVEEMAILAUTOLINKS
		| MD_FLAG_PERMISSIVEWWWAUTOLINKS
		| MD_FLAG_COLLAPSEWHITESPACE
		| MD_FLAG_HARD_SOFT_BREAKS
		| MD_FLAG_ADMONITIONS
		| MD_FLAG_FOOTNOTES;
}

static bool markdown_read_option_bool(zend_object *object, const char *name, size_t name_len)
{
	zval rv;
	zval *value = zend_read_property(markdown_options_ce, object, name, name_len, 0, &rv);

	return zend_is_true(value);
}

static zend_long markdown_read_option_long(zend_object *object, const char *name, size_t name_len)
{
	zval rv;
	zval *value = zend_read_property(markdown_options_ce, object, name, name_len, 0, &rv);

	return zval_get_long(value);
}

static zend_long markdown_options_to_parser_flags(zend_object *options)
{
	zend_long flags = 0;

	if (markdown_read_option_bool(options, ZEND_STRL("tables"))) {
		flags |= MD_FLAG_TABLES;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("strikethrough"))) {
		flags |= MD_FLAG_STRIKETHROUGH;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("tasklists"))) {
		flags |= MD_FLAG_TASKLISTS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("urlAutolinks"))) {
		flags |= MD_FLAG_PERMISSIVEURLAUTOLINKS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("emailAutolinks"))) {
		flags |= MD_FLAG_PERMISSIVEEMAILAUTOLINKS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("wwwAutolinks"))) {
		flags |= MD_FLAG_PERMISSIVEWWWAUTOLINKS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("collapseWhitespace"))) {
		flags |= MD_FLAG_COLLAPSEWHITESPACE;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("latexMath"))) {
		flags |= MD_FLAG_LATEXMATHSPANS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("wikilinks"))) {
		flags |= MD_FLAG_WIKILINKS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("underline"))) {
		flags |= MD_FLAG_UNDERLINE;
	}
	if (!markdown_read_option_bool(options, ZEND_STRL("htmlBlocks"))) {
		flags |= MD_FLAG_NOHTMLBLOCKS;
	}
	if (!markdown_read_option_bool(options, ZEND_STRL("htmlSpans"))) {
		flags |= MD_FLAG_NOHTMLSPANS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("permissiveAtxHeaders"))) {
		flags |= MD_FLAG_PERMISSIVEATXHEADERS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("noIndentedCodeBlocks"))) {
		flags |= MD_FLAG_NOINDENTEDCODEBLOCKS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("hardSoftBreaks"))) {
		flags |= MD_FLAG_HARD_SOFT_BREAKS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("spoilers"))) {
		flags |= MD_FLAG_SPOILERS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("superscripts"))) {
		flags |= MD_FLAG_SUPERSCRIPTS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("subscripts"))) {
		flags |= MD_FLAG_SUBSCRIPTS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("admonitions"))) {
		flags |= MD_FLAG_ADMONITIONS;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("footnotes"))) {
		flags |= MD_FLAG_FOOTNOTES;
	}
	if (markdown_read_option_bool(options, ZEND_STRL("highlight"))) {
		flags |= MD_FLAG_HIGHLIGHT;
	}

	return flags;
}

static void markdown_update_bool_property(zend_object *object, const char *name, size_t name_len, bool value)
{
	zend_update_property_bool(markdown_options_ce, object, name, name_len, value);
}

static void markdown_update_long_property(zend_object *object, const char *name, size_t name_len, zend_long value)
{
	zend_update_property_long(markdown_options_ce, object, name, name_len, value);
}

static void markdown_process_output(const MD_CHAR *text, MD_SIZE size, void *userdata)
{
	markdown_output_buffer *buffer = (markdown_output_buffer *) userdata;

	smart_str_appendl(&buffer->html, text, size);
}

PHP_METHOD(YiiPress_Markdown_MarkdownOptions, __construct)
{
	bool tables = true;
	bool strikethrough = true;
	bool tasklists = true;
	bool url_autolinks = true;
	bool email_autolinks = true;
	bool www_autolinks = true;
	bool collapse_whitespace = true;
	bool latex_math = false;
	bool wikilinks = false;
	bool underline = false;
	bool html_blocks = true;
	bool html_spans = true;
	bool permissive_atx_headers = false;
	bool no_indented_code_blocks = false;
	bool hard_soft_breaks = true;
	bool spoilers = false;
	bool superscripts = false;
	bool subscripts = false;
	bool admonitions = true;
	bool footnotes = true;
	bool highlight = false;
	zend_long renderer_flags = 0;
	zend_object *object = Z_OBJ_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_START(0, 22)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(tables)
		Z_PARAM_BOOL(strikethrough)
		Z_PARAM_BOOL(tasklists)
		Z_PARAM_BOOL(url_autolinks)
		Z_PARAM_BOOL(email_autolinks)
		Z_PARAM_BOOL(www_autolinks)
		Z_PARAM_BOOL(collapse_whitespace)
		Z_PARAM_BOOL(latex_math)
		Z_PARAM_BOOL(wikilinks)
		Z_PARAM_BOOL(underline)
		Z_PARAM_BOOL(html_blocks)
		Z_PARAM_BOOL(html_spans)
		Z_PARAM_BOOL(permissive_atx_headers)
		Z_PARAM_BOOL(no_indented_code_blocks)
		Z_PARAM_BOOL(hard_soft_breaks)
		Z_PARAM_BOOL(spoilers)
		Z_PARAM_BOOL(superscripts)
		Z_PARAM_BOOL(subscripts)
		Z_PARAM_BOOL(admonitions)
		Z_PARAM_BOOL(footnotes)
		Z_PARAM_BOOL(highlight)
		Z_PARAM_LONG(renderer_flags)
	ZEND_PARSE_PARAMETERS_END();

	markdown_update_bool_property(object, ZEND_STRL("tables"), tables);
	markdown_update_bool_property(object, ZEND_STRL("strikethrough"), strikethrough);
	markdown_update_bool_property(object, ZEND_STRL("tasklists"), tasklists);
	markdown_update_bool_property(object, ZEND_STRL("urlAutolinks"), url_autolinks);
	markdown_update_bool_property(object, ZEND_STRL("emailAutolinks"), email_autolinks);
	markdown_update_bool_property(object, ZEND_STRL("wwwAutolinks"), www_autolinks);
	markdown_update_bool_property(object, ZEND_STRL("collapseWhitespace"), collapse_whitespace);
	markdown_update_bool_property(object, ZEND_STRL("latexMath"), latex_math);
	markdown_update_bool_property(object, ZEND_STRL("wikilinks"), wikilinks);
	markdown_update_bool_property(object, ZEND_STRL("underline"), underline);
	markdown_update_bool_property(object, ZEND_STRL("htmlBlocks"), html_blocks);
	markdown_update_bool_property(object, ZEND_STRL("htmlSpans"), html_spans);
	markdown_update_bool_property(object, ZEND_STRL("permissiveAtxHeaders"), permissive_atx_headers);
	markdown_update_bool_property(object, ZEND_STRL("noIndentedCodeBlocks"), no_indented_code_blocks);
	markdown_update_bool_property(object, ZEND_STRL("hardSoftBreaks"), hard_soft_breaks);
	markdown_update_bool_property(object, ZEND_STRL("spoilers"), spoilers);
	markdown_update_bool_property(object, ZEND_STRL("superscripts"), superscripts);
	markdown_update_bool_property(object, ZEND_STRL("subscripts"), subscripts);
	markdown_update_bool_property(object, ZEND_STRL("admonitions"), admonitions);
	markdown_update_bool_property(object, ZEND_STRL("footnotes"), footnotes);
	markdown_update_bool_property(object, ZEND_STRL("highlight"), highlight);
	markdown_update_long_property(object, ZEND_STRL("rendererFlags"), renderer_flags);
}

PHP_METHOD(YiiPress_Markdown_MarkdownOptions, toParserFlags)
{
	ZEND_PARSE_PARAMETERS_NONE();

	RETURN_LONG(markdown_options_to_parser_flags(Z_OBJ_P(ZEND_THIS)));
}

PHP_METHOD(YiiPress_Markdown_MarkdownRenderer, __construct)
{
	zval *options = NULL;
	markdown_renderer_object *renderer = Z_MARKDOWN_RENDERER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options, markdown_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (options == NULL) {
		renderer->parser_flags = markdown_default_parser_flags();
		renderer->renderer_flags = 0;
		return;
	}

	renderer->parser_flags = markdown_options_to_parser_flags(Z_OBJ_P(options));
	renderer->renderer_flags = markdown_read_option_long(Z_OBJ_P(options), ZEND_STRL("rendererFlags"));
}

PHP_METHOD(YiiPress_Markdown_MarkdownRenderer, render)
{
	char *markdown;
	size_t markdown_len;
	markdown_renderer_object *renderer = Z_MARKDOWN_RENDERER_P(ZEND_THIS);
	markdown_output_buffer buffer = {0};
	int result;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(markdown, markdown_len)
	ZEND_PARSE_PARAMETERS_END();

	if (markdown_len == 0) {
		RETURN_EMPTY_STRING();
	}

	if (markdown_len > UINT_MAX) {
		zend_throw_exception(spl_ce_LengthException, "Markdown input is too large for MD4C", 0);
		RETURN_THROWS();
	}

	result = md_html(markdown, (MD_SIZE) markdown_len, markdown_process_output, &buffer, (unsigned) renderer->parser_flags, (unsigned) renderer->renderer_flags);
	if (result != 0) {
		smart_str_free(&buffer.html);
		zend_throw_exception(spl_ce_RuntimeException, "MD4C failed to render Markdown", 0);
		RETURN_THROWS();
	}

	smart_str_0(&buffer.html);

	if (buffer.html.s == NULL) {
		RETURN_EMPTY_STRING();
	}

	RETURN_STR(buffer.html.s);
}

static void markdown_declare_readonly_typed_property(zend_class_entry *ce, const char *name, size_t name_len, zend_type type)
{
	zval default_value;
	zend_string *property_name = zend_string_init(name, name_len, 1);

	ZVAL_UNDEF(&default_value);
	zend_declare_typed_property(ce, property_name, &default_value, ZEND_ACC_PUBLIC | ZEND_ACC_READONLY, NULL, type);
	zend_string_release(property_name);
}

static void markdown_register_options_properties(void)
{
	zend_type bool_type = ZEND_TYPE_INIT_CODE(_IS_BOOL, 0, 0);
	zend_type int_type = ZEND_TYPE_INIT_CODE(IS_LONG, 0, 0);

	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("tables"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("strikethrough"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("tasklists"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("urlAutolinks"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("emailAutolinks"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("wwwAutolinks"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("collapseWhitespace"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("latexMath"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("wikilinks"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("underline"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("htmlBlocks"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("htmlSpans"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("permissiveAtxHeaders"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("noIndentedCodeBlocks"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("hardSoftBreaks"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("spoilers"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("superscripts"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("subscripts"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("admonitions"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("footnotes"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("highlight"), bool_type);
	markdown_declare_readonly_typed_property(markdown_options_ce, ZEND_STRL("rendererFlags"), int_type);
}

static void markdown_declare_class_constant_long(zend_class_entry *ce, const char *name, size_t name_len, zend_long value)
{
	zend_declare_class_constant_long(ce, name, name_len, value);
}

static PHP_MINIT_FUNCTION(markdown)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "YiiPress\\Markdown", "MarkdownRenderer", class_YiiPress_Markdown_MarkdownRenderer_methods);
	markdown_renderer_ce = zend_register_internal_class(&ce);
	markdown_renderer_ce->ce_flags |= ZEND_ACC_FINAL;
	markdown_renderer_ce->create_object = markdown_renderer_create_object;

	memcpy(&markdown_renderer_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	markdown_renderer_handlers.offset = XtOffsetOf(markdown_renderer_object, std);
	markdown_renderer_handlers.free_obj = markdown_renderer_free_object;

	INIT_NS_CLASS_ENTRY(ce, "YiiPress\\Markdown", "MarkdownOptions", class_YiiPress_Markdown_MarkdownOptions_methods);
	markdown_options_ce = zend_register_internal_class(&ce);
	markdown_options_ce->ce_flags |= ZEND_ACC_FINAL;
#if PHP_VERSION_ID >= 80200
	markdown_options_ce->ce_flags |= ZEND_ACC_READONLY_CLASS;
#endif
	markdown_register_options_properties();

	INIT_NS_CLASS_ENTRY(ce, "YiiPress\\Markdown", "Flag", NULL);
	markdown_flag_ce = zend_register_internal_class(&ce);
	markdown_flag_ce->ce_flags |= ZEND_ACC_FINAL;
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("COLLAPSE_WHITESPACE"), MD_FLAG_COLLAPSEWHITESPACE);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("PERMISSIVE_ATX_HEADERS"), MD_FLAG_PERMISSIVEATXHEADERS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("PERMISSIVE_URL_AUTOLINKS"), MD_FLAG_PERMISSIVEURLAUTOLINKS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("PERMISSIVE_EMAIL_AUTOLINKS"), MD_FLAG_PERMISSIVEEMAILAUTOLINKS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("NO_INDENTED_CODE_BLOCKS"), MD_FLAG_NOINDENTEDCODEBLOCKS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("NO_HTML_BLOCKS"), MD_FLAG_NOHTMLBLOCKS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("NO_HTML_SPANS"), MD_FLAG_NOHTMLSPANS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("TABLES"), MD_FLAG_TABLES);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("STRIKETHROUGH"), MD_FLAG_STRIKETHROUGH);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("PERMISSIVE_WWW_AUTOLINKS"), MD_FLAG_PERMISSIVEWWWAUTOLINKS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("TASKLISTS"), MD_FLAG_TASKLISTS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("LATEX_MATH_SPANS"), MD_FLAG_LATEXMATHSPANS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("WIKILINKS"), MD_FLAG_WIKILINKS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("UNDERLINE"), MD_FLAG_UNDERLINE);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("HARD_SOFT_BREAKS"), MD_FLAG_HARD_SOFT_BREAKS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("SPOILERS"), MD_FLAG_SPOILERS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("SUPERSCRIPTS"), MD_FLAG_SUPERSCRIPTS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("SUBSCRIPTS"), MD_FLAG_SUBSCRIPTS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("ADMONITIONS"), MD_FLAG_ADMONITIONS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("FOOTNOTES"), MD_FLAG_FOOTNOTES);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("HIGHLIGHT"), MD_FLAG_HIGHLIGHT);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("PERMISSIVE_AUTOLINKS"), MD_FLAG_PERMISSIVEAUTOLINKS);
	markdown_declare_class_constant_long(markdown_flag_ce, ZEND_STRL("NO_HTML"), MD_FLAG_NOHTML);

	INIT_NS_CLASS_ENTRY(ce, "YiiPress\\Markdown", "Dialect", NULL);
	markdown_dialect_ce = zend_register_internal_class(&ce);
	markdown_dialect_ce->ce_flags |= ZEND_ACC_FINAL;
	markdown_declare_class_constant_long(markdown_dialect_ce, ZEND_STRL("COMMONMARK"), MD_DIALECT_COMMONMARK);
	markdown_declare_class_constant_long(markdown_dialect_ce, ZEND_STRL("GITHUB"), MD_DIALECT_GITHUB);

	INIT_NS_CLASS_ENTRY(ce, "YiiPress\\Markdown", "HtmlFlag", NULL);
	markdown_html_flag_ce = zend_register_internal_class(&ce);
	markdown_html_flag_ce->ce_flags |= ZEND_ACC_FINAL;
	markdown_declare_class_constant_long(markdown_html_flag_ce, ZEND_STRL("DEBUG"), MD_HTML_FLAG_DEBUG);
	markdown_declare_class_constant_long(markdown_html_flag_ce, ZEND_STRL("VERBATIM_ENTITIES"), MD_HTML_FLAG_VERBATIM_ENTITIES);
	markdown_declare_class_constant_long(markdown_html_flag_ce, ZEND_STRL("SKIP_UTF8_BOM"), MD_HTML_FLAG_SKIP_UTF8_BOM);
	markdown_declare_class_constant_long(markdown_html_flag_ce, ZEND_STRL("XHTML"), MD_HTML_FLAG_XHTML);

	return SUCCESS;
}

static PHP_MINFO_FUNCTION(markdown)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "markdown support", "enabled");
	php_info_print_table_row(2, "Version", PHP_MARKDOWN_VERSION);
	php_info_print_table_row(2, "Bundled MD4C", "enabled");
	php_info_print_table_end();
}

zend_module_entry markdown_module_entry = {
	STANDARD_MODULE_HEADER,
	"markdown",
	NULL,
	PHP_MINIT(markdown),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(markdown),
	PHP_MARKDOWN_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_MARKDOWN
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(markdown)
#endif
