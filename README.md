# Markdown Extension for PHP

`yiipress/markdown` is a native PHP extension for rendering Markdown to HTML
with bundled [MD4C](https://github.com/mity/md4c).

## Install

Install the extension with PIE:

```bash
pie install yiipress/markdown
```

Then require the extension from applications that use it:

```bash
composer require ext-markdown:*
```

## Usage

Create a renderer once, then call `render()` for each Markdown string:

```php
use YiiPress\Markdown\MarkdownOptions;
use YiiPress\Markdown\MarkdownRenderer;

$renderer = new MarkdownRenderer(new MarkdownOptions(
    tables: true,
    tasklists: true,
    htmlBlocks: true,
    htmlSpans: true,
));

echo $renderer->render("# Hello");
```

`MarkdownOptions` is immutable. Pass options to the constructor to choose the
Markdown features accepted by the parser and the HTML renderer flags used for
output.

Defaults enable common extensions:

- tables
- strikethrough
- task lists
- URL, email, and WWW autolinks
- whitespace collapse
- hard soft breaks
- footnotes
- admonitions
- raw HTML blocks and spans

Disable raw HTML by turning off both HTML options:

```php
$renderer = new MarkdownRenderer(new MarkdownOptions(
    htmlBlocks: false,
    htmlSpans: false,
));
```

Optional MD4C extensions can be enabled individually:

```php
$renderer = new MarkdownRenderer(new MarkdownOptions(
    latexMath: true,
    wikilinks: true,
    spoilers: true,
    superscripts: true,
    subscripts: true,
    highlight: true,
));
```

## MD4C Constants

Most applications should use `MarkdownOptions`. The extension also exposes
MD4C's raw constants when you need to inspect the generated parser bitmask,
store low-level settings, or pass HTML renderer flags directly.

- `YiiPress\Markdown\Flag` contains parser feature flags, such as
  `Flag::TABLES`, `Flag::NO_HTML`, and `Flag::FOOTNOTES`.
- `YiiPress\Markdown\Dialect` contains MD4C dialect presets, such as
  `Dialect::COMMONMARK` and `Dialect::GITHUB`.
- `YiiPress\Markdown\HtmlFlag` contains flags for MD4C's HTML renderer, such as
  `HtmlFlag::XHTML` and `HtmlFlag::VERBATIM_ENTITIES`.

Use `HtmlFlag` values with `MarkdownOptions::$rendererFlags`:

```php
use YiiPress\Markdown\HtmlFlag;
use YiiPress\Markdown\MarkdownOptions;
use YiiPress\Markdown\MarkdownRenderer;

$renderer = new MarkdownRenderer(new MarkdownOptions(
    rendererFlags: HtmlFlag::XHTML | HtmlFlag::VERBATIM_ENTITIES,
));
```

Use `Flag` values when you need to inspect or compare parser settings:

```php
use YiiPress\Markdown\Flag;
use YiiPress\Markdown\MarkdownOptions;

$options = new MarkdownOptions(htmlBlocks: false, htmlSpans: false);
$parserFlags = $options->toParserFlags();

if (($parserFlags & Flag::NO_HTML) === Flag::NO_HTML) {
    // Raw HTML blocks and spans are disabled.
}
```

Use `Dialect` values when interoperating with code that expects MD4C's dialect
bitmasks:

```php
use YiiPress\Markdown\Dialect;

$githubFlags = Dialect::GITHUB;
```

## Scope

Version 1 is Markdown-to-HTML only. It intentionally does not expose a PHP
callback or event wrapper around MD4C's lower-level `md_parse()` API.

Compared with `mdparser`, this package prioritizes MD4C feature coverage and
medium/large input performance. It does not add mdparser-only features such as
smart punctuation, heading anchors, source positions, nofollow links, or tag
filtering.

## Development

Run the PHPT suite in Docker:

```bash
make test
```

Show the currently bundled MD4C revision:

```bash
make md4c-version
```

Check whether the configured upstream MD4C ref has changed:

```bash
make md4c-check-upgrade
```

Update the vendored MD4C files:

```bash
make md4c-upgrade
```

By default, MD4C targets use upstream `master`. Pass `MD4C_REF` to check or
upgrade to another branch, tag, or commit:

```bash
make md4c-upgrade MD4C_REF=release-0.5.3
```

## Bundled MD4C

This package vendors MD4C source code under `third_party/md4c/`. MD4C is
licensed under the MIT license; its license notice is included at
`third_party/md4c/LICENSE.md`.

## Credits

This extension was inspired by
[eklausme/php-md4c](https://github.com/eklausme/php-md4c), an earlier PHP
extension wrapper for MD4C.
