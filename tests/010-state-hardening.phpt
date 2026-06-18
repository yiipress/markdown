--TEST--
Renderer state hardening and invalid Options rejection
--EXTENSIONS--
markdown
--FILE--
<?php
function check(string $label, bool $condition): void {
    echo ($condition ? "OK" : "FAIL"), ": $label\n";
}

$renderer = new YiiPress\Markdown\MarkdownRenderer(new YiiPress\Markdown\MarkdownOptions(
    htmlBlocks: false,
    htmlSpans: false,
));

try {
    serialize($renderer);
    check('serialize(MarkdownRenderer) blocked', false);
} catch (Throwable $e) {
    check('serialize(MarkdownRenderer) blocked', str_contains($e->getMessage(), 'not allowed'));
}

try {
    unserialize('O:34:"YiiPress\\Markdown\\MarkdownRenderer":0:{}');
    check('unserialize(MarkdownRenderer) blocked', false);
} catch (Throwable $e) {
    check('unserialize(MarkdownRenderer) blocked', str_contains($e->getMessage(), 'not allowed'));
}

try {
    clone $renderer;
    check('clone(MarkdownRenderer) blocked', false);
} catch (Throwable) {
    check('clone(MarkdownRenderer) blocked', true);
}

try {
    $renderer->typo = true;
    check('dynamic property on MarkdownRenderer blocked', false);
} catch (Throwable $e) {
    check('dynamic property on MarkdownRenderer blocked', str_contains($e->getMessage(), 'dynamic property'));
}

$options = new YiiPress\Markdown\MarkdownOptions();
try {
    $options->typo = true;
    check('dynamic property on MarkdownOptions blocked', false);
} catch (Throwable $e) {
    check('dynamic property on MarkdownOptions blocked', str_contains($e->getMessage(), 'dynamic property'));
}

$badOptions = (new ReflectionClass(YiiPress\Markdown\MarkdownOptions::class))->newInstanceWithoutConstructor();

try {
    new YiiPress\Markdown\MarkdownRenderer($badOptions);
    check('reflection-bypassed Options rejected by renderer', false);
} catch (RuntimeException $e) {
    check(
        'reflection-bypassed Options rejected by renderer',
        str_contains($e->getMessage(), 'uninitialized') && str_contains($e->getMessage(), 'MarkdownOptions::$tables')
    );
}

try {
    $badOptions->toParserFlags();
    check('reflection-bypassed Options rejected by toParserFlags', false);
} catch (RuntimeException $e) {
    check(
        'reflection-bypassed Options rejected by toParserFlags',
        str_contains($e->getMessage(), 'uninitialized') && str_contains($e->getMessage(), 'MarkdownOptions::$tables')
    );
}

?>
--EXPECT--
OK: serialize(MarkdownRenderer) blocked
OK: unserialize(MarkdownRenderer) blocked
OK: clone(MarkdownRenderer) blocked
OK: dynamic property on MarkdownRenderer blocked
OK: dynamic property on MarkdownOptions blocked
OK: reflection-bypassed Options rejected by renderer
OK: reflection-bypassed Options rejected by toParserFlags
