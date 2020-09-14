@ok
<?php

require_once 'kphp_tester_include.php';

$a = new Classes\B();
$a = new Classes\A();
$b = new Classes\B();

if ($a instanceof Classes\A || $a instanceof Classes\B) {
    var_dump('$a is A or B');
}

$is_instance = true;
$is_instance &= $a instanceof Classes\A;
var_dump($is_instance);

$value = 10 + $b instanceof Classes\A;
var_dump($value);

