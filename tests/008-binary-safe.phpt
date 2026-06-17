--TEST--
Embedded NUL bytes do not truncate input
--EXTENSIONS--
markdown
--FILE--
<?php
$html = (new YiiPress\Markdown\MarkdownRenderer())->render("a\0b");
if (!str_contains($html, 'a') || !str_contains($html, 'b')) {
    throw new RuntimeException($html);
}
echo "OK\n";
?>
--EXPECT--
OK
