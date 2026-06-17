--TEST--
Renderer flags support XHTML and verbatim entities
--EXTENSIONS--
markdown
--FILE--
<?php
$xhtml = new YiiPress\Markdown\MarkdownRenderer(new YiiPress\Markdown\MarkdownOptions(
    rendererFlags: YiiPress\Markdown\HtmlFlag::XHTML,
));
if (!str_contains($xhtml->render("a\nb"), '<br />')) {
    throw new RuntimeException('XHTML flag was not applied');
}

$entities = new YiiPress\Markdown\MarkdownRenderer(new YiiPress\Markdown\MarkdownOptions(
    rendererFlags: YiiPress\Markdown\HtmlFlag::VERBATIM_ENTITIES,
));
if (!str_contains($entities->render('&copy;'), '&copy;')) {
    throw new RuntimeException('Verbatim entities flag was not applied');
}

echo "OK\n";
?>
--EXPECT--
OK
