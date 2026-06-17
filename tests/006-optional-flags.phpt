--TEST--
Optional parser flags
--EXTENSIONS--
markdown
--FILE--
<?php
function check(string $markdown, YiiPress\Markdown\MarkdownOptions $options, string $expected): void {
    $html = (new YiiPress\Markdown\MarkdownRenderer($options))->render($markdown);
    if (!str_contains($html, $expected)) {
        throw new RuntimeException("Expected $expected in $html");
    }
}

check('$x$', new YiiPress\Markdown\MarkdownOptions(latexMath: true), '<x-equation>');
check('[[Page|Label]]', new YiiPress\Markdown\MarkdownOptions(wikilinks: true), '<x-wikilink data-target="Page">Label</x-wikilink>');
check('__under__', new YiiPress\Markdown\MarkdownOptions(underline: true), '<u>under</u>');
check('    code', new YiiPress\Markdown\MarkdownOptions(noIndentedCodeBlocks: true), '<p>code</p>');
check('#title', new YiiPress\Markdown\MarkdownOptions(permissiveAtxHeaders: true), '<h1>title</h1>');
check('||hidden||', new YiiPress\Markdown\MarkdownOptions(spoilers: true), '<x-spoiler>hidden</x-spoiler>');
check('x^2^', new YiiPress\Markdown\MarkdownOptions(superscripts: true), '<sup>2</sup>');
check('H~2~O', new YiiPress\Markdown\MarkdownOptions(subscripts: true), '<sub>2</sub>');
check('==marked==', new YiiPress\Markdown\MarkdownOptions(highlight: true), '<mark>marked</mark>');

echo "OK\n";
?>
--EXPECT--
OK
