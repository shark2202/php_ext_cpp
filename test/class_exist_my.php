<?php 

namespace bb;

$c = new \trComplex\Complex(1,2);

class a{};
$class_name = strtolower('trComplex\Complex');
echo $class_name,PHP_EOL;
var_dump($c->class_exist_my($class_name));
var_dump($c->class_exist_my("no_function"));