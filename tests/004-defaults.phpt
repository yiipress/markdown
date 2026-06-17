--TEST--
Default Markdown options
--EXTENSIONS--
markdown
--FILE--
<?php
$renderers = [
    new YiiPress\Markdown\MarkdownRenderer(),
    new YiiPress\Markdown\MarkdownRenderer(new YiiPress\Markdown\MarkdownOptions()),
];

$cases = [
    ['| A |\n| - |\n| B |', '<table>'],
    ['- [x] Done', 'type="checkbox"'],
    ['~~old~~', '<del>old</del>'],
    ['https://example.com', '<a href="https://example.com">'],
    ['me@example.com', '<a href="mailto:me@example.com">'],
    ['www.example.com', '<a href="http://www.example.com">'],
    ["a \t  b", '<p>a b</p>'],
    ["a\nb", '<br>'],
    ["Text[^1].\n\n[^1]: Footnote.", '<section class="footnotes">'],
    ["> [!NOTE]\n> Body", '<div class="admonition-note">'],
];

foreach ($renderers as $renderer) {
    foreach ($cases as [$markdown, $expected]) {
        $html = $renderer->render(str_replace('\n', "\n", $markdown));
        if (!str_contains($html, $expected)) {
            throw new RuntimeException("Expected $expected in $html");
        }
    }
}

echo "OK\n";
?>
--EXPECT--
OK
