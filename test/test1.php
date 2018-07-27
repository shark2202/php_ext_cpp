<?php 

$c = new \trComplex\Complex(1,2);
$c0 = new \trComplex\Complex(1,2);

var_dump($c == $c0);
var_dump($c === $c0);
var_dump($c->getReal());

$c1 = $c->this1();
var_dump($c1 == $c);
var_dump($c1 === $c);

$c3 = $c->this2();
var_dump($c3 == $c);
var_dump($c3 === $c);

try{
	$c1->e1();
}catch(Exception $e){
	var_dump($e);
}

var_dump($c->e2(function(){
	throw new Exception("Sorry...\n");
    return "Second call\n";
}));

var_dump([1,2]);
$c->callPhpFn([1,2]);

// $d = include22("include22.php");
// var_dump($d);
// var_dump($abc);

// var_dump(trComplex\getConf());
echo "StaticCnew";
$o1 = new StaticCnew();
$o2 = new StaticCnew();
var_dump($o1->getSS());

var_dump($o2->setSS("111",222));
var_dump($o2->setSS("333",444));

var_dump($o1->getSS());