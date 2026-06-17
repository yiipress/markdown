--TEST--
HTML blocks and spans are enabled by default and can be disabled
--EXTENSIONS--
markdown
--FILE--
<?php
$default = new YiiPress\Markdown\MarkdownRenderer();
$disabled = new YiiPress\Markdown\MarkdownRenderer(new YiiPress\Markdown\MarkdownOptions(
    htmlBlocks: false,
    htmlSpans: false,
));

$html = "<section>\nraw\n</section>";
if (!str_contains($default->render($html), '<section>')) {
    throw new RuntimeException('HTML block should be preserved by default');
}
if (str_contains($disabled->render($html), '<section>')) {
    throw new RuntimeException('HTML block should be escaped when disabled');
}
if (!str_contains($default->render('A <span>raw</span> span'), '<span>raw</span>')) {
    throw new RuntimeException('HTML span should be preserved by default');
}
if (str_contains($disabled->render('A <span>raw</span> span'), '<span>raw</span>')) {
    throw new RuntimeException('HTML span should be escaped when disabled');
}

echo "OK\n";
?>
--EXPECT--
OK
