--TEST--
Basic CommonMark rendering
--EXTENSIONS--
markdown
--FILE--
<?php
$renderer = new YiiPress\Markdown\MarkdownRenderer();
$html = $renderer->render("# Title\n\nHello **world**.");
if (!str_contains($html, '<h1>Title</h1>')) {
    throw new RuntimeException($html);
}
if (!str_contains($html, '<p>Hello <strong>world</strong>.</p>')) {
    throw new RuntimeException($html);
}
echo "OK\n";
?>
--EXPECT--
OK
