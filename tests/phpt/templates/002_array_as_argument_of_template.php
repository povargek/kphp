@ok
<?php

require_once 'kphp_tester_include.php';

/**
 * @kphp-template $obj
 */
function f($obj) {
    foreach ($obj as $o) {
        $o->magic = 777;
        $o->print_magic();
    }
}

f([new \Classes\A, new \Classes\A]);
f([new \Classes\B, new \Classes\B]);
