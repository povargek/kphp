@ok
<?php

require_once 'kphp_tester_include.php';

#ifndef KittenPHP
function new_Long($x) { return floatval($x); }
#endif

class A { public $x = 10; }

class B { public $y = 20; }

$strs_a = array_map("instance_to_array", [new A(), new A()]);
$strs_b = array_map("instance_to_array", [new B()]);
var_dump($strs_a);
var_dump($strs_b);

var_dump(array_map("is_numeric", array_map("instance_to_array", [new A()])));

var_dump(array_reduce(["123", "245", "111", "0"], "bcadd", "0"));

var_dump(array_map("array_filter", [[1, 0], [false], [2, NULL]]));

$ints = [[1, 2], [3, 4]];
$sums = array_map('intval', array_map('array_sum', $ints));
var_dump($sums);

$longs = array_map("new_Long", [1, "0", 2, 3]);
var_dump((string)$longs[0]);
