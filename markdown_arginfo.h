/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 3859b2ba8294d5f3697a056a3412ebf09e6394f8 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_YiiPress_Markdown_MarkdownRenderer___construct, 0, 0, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, YiiPress\\Markdown\\MarkdownOptions, 0, "new YiiPress\\Markdown\\MarkdownOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_YiiPress_Markdown_MarkdownRenderer_render, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, markdown, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_YiiPress_Markdown_MarkdownOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, tables, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, strikethrough, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, tasklists, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, urlAutolinks, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, emailAutolinks, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, wwwAutolinks, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, collapseWhitespace, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, latexMath, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, wikilinks, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, underline, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, htmlBlocks, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, htmlSpans, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, permissiveAtxHeaders, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, noIndentedCodeBlocks, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, hardSoftBreaks, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, spoilers, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, superscripts, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, subscripts, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, admonitions, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, footnotes, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, highlight, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, rendererFlags, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_YiiPress_Markdown_MarkdownOptions_toParserFlags, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_METHOD(YiiPress_Markdown_MarkdownRenderer, __construct);
ZEND_METHOD(YiiPress_Markdown_MarkdownRenderer, render);
ZEND_METHOD(YiiPress_Markdown_MarkdownOptions, __construct);
ZEND_METHOD(YiiPress_Markdown_MarkdownOptions, toParserFlags);

static const zend_function_entry class_YiiPress_Markdown_MarkdownRenderer_methods[] = {
	ZEND_ME(YiiPress_Markdown_MarkdownRenderer, __construct, arginfo_class_YiiPress_Markdown_MarkdownRenderer___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(YiiPress_Markdown_MarkdownRenderer, render, arginfo_class_YiiPress_Markdown_MarkdownRenderer_render, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_YiiPress_Markdown_MarkdownOptions_methods[] = {
	ZEND_ME(YiiPress_Markdown_MarkdownOptions, __construct, arginfo_class_YiiPress_Markdown_MarkdownOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(YiiPress_Markdown_MarkdownOptions, toParserFlags, arginfo_class_YiiPress_Markdown_MarkdownOptions_toParserFlags, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
