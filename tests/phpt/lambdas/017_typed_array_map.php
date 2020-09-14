@ok
<?php

require_once 'kphp_tester_include.php';

function test_primitive_types() {
    $to_string = function(int $x) {
        return (string)$x;
    };

    /** @var string[] */
    $mapped = array_map($to_string, [1, 2, 3]);
    var_dump($mapped);
}

function test_classes() {
    $f = function($c) {
        return new Classes\AnotherIntHolder($c->a);
    };

    $filtered = array_map($f, [new Classes\IntHolder(1), new Classes\IntHolder(3)]);
    $filtered = array_map($f, [new Classes\AnotherIntHolder(1), new Classes\AnotherIntHolder(3)]);
}

function test_map_before_filter() {
    /** @var Classes\IntHolder[] */
    $mapped =array_map(function ($x) { return new Classes\IntHolder($x); }, [1, 2, 3]);

    /** @var Classes\IntHolder[] */
    $filtered = array_filter($mapped, function ($c) { return $c->a < 3; });

    var_dump($filtered[0]->a);
}

/**
 * @kphp-infer
 * @param int[][] $arr
 */
function test_recursive_array_map($arr) {
    $arr = $arr[0];
    $arr = array_map(function ($x) { return -$x; }, $arr);
    var_dump($arr);
}

test_primitive_types();
test_classes();
test_map_before_filter();
test_recursive_array_map([[1, 2, 3]]);
