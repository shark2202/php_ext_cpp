<?php 

/* class ClassInPhp{};

function my_array_map($array, $callback) {

        var_dump($callback);

        // initial result variable
        $result = array();
    
        // loop through the array
        foreach ($array as $index => $item) {
    
            // call the callback on the item
            $result[$index] = $callback($item);
            var_dump($result[$index]);
        }
    
        // done
        return $result;
}


var_dump(multiply_by_two(100));
// exit;

var_dump(is_callable('newFunction'));
if(is_callable('newFunction')){
    newFunction();
}

var_dump(is_callable("CppTest\\newFunction"));
if(is_callable('CppTest\\newFunction')){
    CppTest\newFunction();
}

// myFunction("daidhai");

evalPHP("echo 'ok';");
echo PHP_EOL;

$a = fnInCpp(2);
$a->ok = 100;
var_dump($a);

$f = returnfunction();
var_dump(is_callable($f));
var_dump($f instanceof Closure); */
// var_dump($f(3));

$a = new CppTest\CppClassReg();

$a->caller = function(){
    var_dump($this);
};
var_dump($a);

$a->setVal();

/* $b = $a->caller;
// $b = $b->bindTo($a);
$b = Closure::bind($b,$a);
$b = $b();
var_dump($a,$b); */