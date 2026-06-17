--TEST--
Empty Markdown renders as empty string
--EXTENSIONS--
markdown
--FILE--
<?php
$renderer = new YiiPress\Markdown\MarkdownRenderer();
var_dump($renderer->render(''));
?>
--EXPECT--
string(0) ""
