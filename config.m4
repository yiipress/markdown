PHP_ARG_ENABLE([markdown],
  [whether to enable markdown support],
  [AS_HELP_STRING([--enable-markdown], [Enable Markdown support])],
  [no])

if test "$PHP_MARKDOWN" != "no"; then
  if test -z "$MARKDOWN_VERSION"; then
    MARKDOWN_VERSION="0.1.0"
  fi
  AC_DEFINE_UNQUOTED([PHP_MARKDOWN_VERSION], ["$MARKDOWN_VERSION"], [Markdown extension version])

  PHP_NEW_EXTENSION([markdown],
    [markdown.c third_party/md4c/md4c.c third_party/md4c/md4c-html.c third_party/md4c/entity.c],
    [$ext_shared])
fi
