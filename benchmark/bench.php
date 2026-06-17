<?php

use YiiPress\Markdown\MarkdownRenderer;

if (!extension_loaded('markdown')) {
    fwrite(STDERR, "The markdown extension must be loaded.\n");
    exit(1);
}

$size = $argv[1] ?? 'medium';
$iterations = isset($argv[2]) ? (int) $argv[2] : 1000;
$renderer = new MarkdownRenderer();

$paragraph = "## Heading\n\nThis is **bold**, ~~deleted~~, and linked: https://example.com.\n\n";
$table = "| A | B |\n| - | - |\n| 1 | 2 |\n\n";

$markdown = match ($size) {
    'short' => "# Hello\n\nOne paragraph with https://example.com.\n",
    'medium' => str_repeat($paragraph . $table, 50),
    'large' => str_repeat($paragraph . $table . "- [x] task\n\n", 1000),
    default => throw new InvalidArgumentException('Use short, medium, or large.'),
};

$start = hrtime(true);
for ($i = 0; $i < $iterations; $i++) {
    $renderer->render($markdown);
}
$elapsed = (hrtime(true) - $start) / 1e9;

printf(
    "%s: %d iterations, %.3f seconds, %.3f ms/render\n",
    $size,
    $iterations,
    $elapsed,
    ($elapsed * 1000) / $iterations
);
