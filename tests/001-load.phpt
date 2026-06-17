--TEST--
Extension loads and registers classes
--EXTENSIONS--
markdown
--FILE--
<?php
ob_start();
phpinfo(INFO_MODULES);
$info = ob_get_clean();

if (!extension_loaded('markdown')) {
    throw new RuntimeException('Extension is not loaded');
}
if (phpversion('markdown') === false) {
    throw new RuntimeException('Missing extension version');
}
foreach ([
    YiiPress\Markdown\MarkdownRenderer::class,
    YiiPress\Markdown\MarkdownOptions::class,
    YiiPress\Markdown\Flag::class,
    YiiPress\Markdown\Dialect::class,
    YiiPress\Markdown\HtmlFlag::class,
] as $class) {
    if (!class_exists($class)) {
        throw new RuntimeException("Missing class $class");
    }
}
if (!str_contains($info, 'markdown support') || !str_contains($info, 'Bundled MD4C')) {
    throw new RuntimeException('Missing phpinfo rows');
}
if (YiiPress\Markdown\Flag::FOOTNOTES !== 0x100000 || YiiPress\Markdown\Flag::HIGHLIGHT !== 0x200000) {
    throw new RuntimeException('Missing newer MD4C flags');
}
echo "OK\n";
?>
--EXPECT--
OK
