<?php

/** @generate-function-entries */

namespace YiiPress\Markdown;

/**
 * Renders Markdown strings to HTML using MD4C.
 *
 * A renderer is configured once at construction time. Reuse the same instance
 * for multiple render calls when the same options should apply.
 */
final class MarkdownRenderer
{
    /**
     * Creates a Markdown renderer with the given parser and HTML output options.
     */
    public function __construct(MarkdownOptions $options = new MarkdownOptions()) {}

    /**
     * Converts Markdown input to an HTML fragment.
     *
     * The input is binary-safe and may contain embedded NUL bytes. Empty input
     * returns an empty string.
     *
     * @throws \LengthException If the input is too large for MD4C.
     * @throws \RuntimeException If MD4C fails to render the input.
     */
    public function render(string $markdown): string {}
}

/**
 * Immutable parser and HTML renderer configuration.
 *
 * Boolean options map to MD4C parser flags. `rendererFlags` accepts a bitmask
 * of `HtmlFlag::*` constants for MD4C's HTML renderer.
 */
final readonly class MarkdownOptions
{
    /** Enables the table extension. */
    public bool $tables;
    /** Enables ~~strikethrough~~ spans. */
    public bool $strikethrough;
    /** Enables GitHub-style task list items. */
    public bool $tasklists;
    /** Autolinks bare URLs such as https://example.com. */
    public bool $urlAutolinks;
    /** Autolinks bare email addresses. */
    public bool $emailAutolinks;
    /** Autolinks bare WWW links such as www.example.com. */
    public bool $wwwAutolinks;
    /** Collapses non-trivial whitespace in normal text to a single space. */
    public bool $collapseWhitespace;
    /** Enables inline and display LaTeX math spans. */
    public bool $latexMath;
    /** Enables wiki links. */
    public bool $wikilinks;
    /** Enables underline spans and disables underscore emphasis. */
    public bool $underline;
    /** Allows raw HTML blocks in the output. */
    public bool $htmlBlocks;
    /** Allows raw inline HTML spans in the output. */
    public bool $htmlSpans;
    /** Allows ATX headings without a space after the opening hash sequence. */
    public bool $permissiveAtxHeaders;
    /** Disables indented code blocks while keeping fenced code blocks. */
    public bool $noIndentedCodeBlocks;
    /** Renders soft line breaks as hard breaks. */
    public bool $hardSoftBreaks;
    /** Enables spoiler spans using ||hidden text|| syntax. */
    public bool $spoilers;
    /** Enables superscript spans using ^text^ syntax. */
    public bool $superscripts;
    /** Enables subscript spans using ~text~ syntax. */
    public bool $subscripts;
    /** Enables GitHub-style admonition blocks. */
    public bool $admonitions;
    /** Enables footnote references and definitions. */
    public bool $footnotes;
    /** Enables highlight spans using ==text== syntax. */
    public bool $highlight;
    /** Bitmask of `HtmlFlag::*` constants passed to MD4C's HTML renderer. */
    public int $rendererFlags;

    /**
     * Creates immutable Markdown rendering options.
     *
     * Raw HTML is allowed by default. Set both `htmlBlocks` and `htmlSpans` to
     * false to escape raw HTML instead of passing it through.
     */
    public function __construct(
        bool $tables = true,
        bool $strikethrough = true,
        bool $tasklists = true,
        bool $urlAutolinks = true,
        bool $emailAutolinks = true,
        bool $wwwAutolinks = true,
        bool $collapseWhitespace = true,
        bool $latexMath = false,
        bool $wikilinks = false,
        bool $underline = false,
        bool $htmlBlocks = true,
        bool $htmlSpans = true,
        bool $permissiveAtxHeaders = false,
        bool $noIndentedCodeBlocks = false,
        bool $hardSoftBreaks = true,
        bool $spoilers = false,
        bool $superscripts = false,
        bool $subscripts = false,
        bool $admonitions = true,
        bool $footnotes = true,
        bool $highlight = false,
        int $rendererFlags = 0,
    ) {}

    /**
     * Returns the MD4C parser flag bitmask represented by these options.
     *
     * This is useful for diagnostics and for code that needs to interoperate
     * with lower-level MD4C flag values.
     */
    public function toParserFlags(): int {}
}

/**
 * MD4C parser flags.
 *
 * These constants expose the raw parser flag values for advanced use and for
 * comparing with `MarkdownOptions::toParserFlags()`.
 */
final class Flag
{
    /** Collapses non-trivial whitespace in normal text to a single space.
     * @cvalue MD_FLAG_COLLAPSEWHITESPACE */
    public const int COLLAPSE_WHITESPACE = UNKNOWN;
    /** Allows ATX headings without a required space.
     * @cvalue MD_FLAG_PERMISSIVEATXHEADERS */
    public const int PERMISSIVE_ATX_HEADERS = UNKNOWN;
    /** Recognizes bare URLs as autolinks.
     * @cvalue MD_FLAG_PERMISSIVEURLAUTOLINKS */
    public const int PERMISSIVE_URL_AUTOLINKS = UNKNOWN;
    /** Recognizes bare email addresses as autolinks.
     * @cvalue MD_FLAG_PERMISSIVEEMAILAUTOLINKS */
    public const int PERMISSIVE_EMAIL_AUTOLINKS = UNKNOWN;
    /** Disables indented code blocks.
     * @cvalue MD_FLAG_NOINDENTEDCODEBLOCKS */
    public const int NO_INDENTED_CODE_BLOCKS = UNKNOWN;
    /** Disables raw HTML blocks.
     * @cvalue MD_FLAG_NOHTMLBLOCKS */
    public const int NO_HTML_BLOCKS = UNKNOWN;
    /** Disables raw inline HTML spans.
     * @cvalue MD_FLAG_NOHTMLSPANS */
    public const int NO_HTML_SPANS = UNKNOWN;
    /** Enables the table extension.
     * @cvalue MD_FLAG_TABLES */
    public const int TABLES = UNKNOWN;
    /** Enables strikethrough spans.
     * @cvalue MD_FLAG_STRIKETHROUGH */
    public const int STRIKETHROUGH = UNKNOWN;
    /** Recognizes bare WWW links as autolinks.
     * @cvalue MD_FLAG_PERMISSIVEWWWAUTOLINKS */
    public const int PERMISSIVE_WWW_AUTOLINKS = UNKNOWN;
    /** Enables task list items.
     * @cvalue MD_FLAG_TASKLISTS */
    public const int TASKLISTS = UNKNOWN;
    /** Enables LaTeX math spans.
     * @cvalue MD_FLAG_LATEXMATHSPANS */
    public const int LATEX_MATH_SPANS = UNKNOWN;
    /** Enables wiki links.
     * @cvalue MD_FLAG_WIKILINKS */
    public const int WIKILINKS = UNKNOWN;
    /** Enables underline spans.
     * @cvalue MD_FLAG_UNDERLINE */
    public const int UNDERLINE = UNKNOWN;
    /** Renders soft line breaks as hard breaks.
     * @cvalue MD_FLAG_HARD_SOFT_BREAKS */
    public const int HARD_SOFT_BREAKS = UNKNOWN;
    /** Enables spoiler spans.
     * @cvalue MD_FLAG_SPOILERS */
    public const int SPOILERS = UNKNOWN;
    /** Enables superscript spans.
     * @cvalue MD_FLAG_SUPERSCRIPTS */
    public const int SUPERSCRIPTS = UNKNOWN;
    /** Enables subscript spans.
     * @cvalue MD_FLAG_SUBSCRIPTS */
    public const int SUBSCRIPTS = UNKNOWN;
    /** Enables admonition blocks.
     * @cvalue MD_FLAG_ADMONITIONS */
    public const int ADMONITIONS = UNKNOWN;
    /** Enables footnotes.
     * @cvalue MD_FLAG_FOOTNOTES */
    public const int FOOTNOTES = UNKNOWN;
    /** Enables highlight spans.
     * @cvalue MD_FLAG_HIGHLIGHT */
    public const int HIGHLIGHT = UNKNOWN;
    /** Enables URL, email, and WWW autolinks.
     * @cvalue MD_FLAG_PERMISSIVEAUTOLINKS */
    public const int PERMISSIVE_AUTOLINKS = UNKNOWN;
    /** Disables raw HTML blocks and inline HTML spans.
     * @cvalue MD_FLAG_NOHTML */
    public const int NO_HTML = UNKNOWN;
}

/**
 * MD4C parser dialect presets.
 */
final class Dialect
{
    /** CommonMark-compatible parser flags.
     * @cvalue MD_DIALECT_COMMONMARK */
    public const int COMMONMARK = UNKNOWN;
    /** GitHub-style parser flags supported by MD4C.
     * @cvalue MD_DIALECT_GITHUB */
    public const int GITHUB = UNKNOWN;
}

/**
 * MD4C HTML renderer flags.
 */
final class HtmlFlag
{
    /** Emits MD4C debug output to stderr.
     * @cvalue MD_HTML_FLAG_DEBUG */
    public const int DEBUG = UNKNOWN;
    /** Keeps recognized entities verbatim instead of translating them.
     * @cvalue MD_HTML_FLAG_VERBATIM_ENTITIES */
    public const int VERBATIM_ENTITIES = UNKNOWN;
    /** Skips a UTF-8 byte order mark at the beginning of input.
     * @cvalue MD_HTML_FLAG_SKIP_UTF8_BOM */
    public const int SKIP_UTF8_BOM = UNKNOWN;
    /** Uses XHTML-compatible empty-element syntax.
     * @cvalue MD_HTML_FLAG_XHTML */
    public const int XHTML = UNKNOWN;
}
