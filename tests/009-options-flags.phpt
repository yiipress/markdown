--TEST--
MarkdownOptions toParserFlags maps booleans to MD4C flags
--EXTENSIONS--
markdown
--FILE--
<?php
$default = new YiiPress\Markdown\MarkdownOptions();
foreach ([
    YiiPress\Markdown\Flag::TABLES,
    YiiPress\Markdown\Flag::STRIKETHROUGH,
    YiiPress\Markdown\Flag::TASKLISTS,
    YiiPress\Markdown\Flag::PERMISSIVE_URL_AUTOLINKS,
    YiiPress\Markdown\Flag::PERMISSIVE_EMAIL_AUTOLINKS,
    YiiPress\Markdown\Flag::PERMISSIVE_WWW_AUTOLINKS,
    YiiPress\Markdown\Flag::COLLAPSE_WHITESPACE,
    YiiPress\Markdown\Flag::HARD_SOFT_BREAKS,
    YiiPress\Markdown\Flag::ADMONITIONS,
    YiiPress\Markdown\Flag::FOOTNOTES,
] as $flag) {
    if (($default->toParserFlags() & $flag) !== $flag) {
        throw new RuntimeException("Missing default flag $flag");
    }
}

$disabledHtml = new YiiPress\Markdown\MarkdownOptions(htmlBlocks: false, htmlSpans: false);
if (($disabledHtml->toParserFlags() & YiiPress\Markdown\Flag::NO_HTML) !== YiiPress\Markdown\Flag::NO_HTML) {
    throw new RuntimeException('HTML disabling flags were not mapped');
}

echo "OK\n";
?>
--EXPECT--
OK
