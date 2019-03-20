<?php 
namespace ccc;
    function bbbbbbb(){
        echo "ok",PHP_EOL;
    
        return 0;
    }

namespace bbbb;



// register_shutdown_function("mybacktrace");
// dl("mytest");
mytest_disbaled(1);

function bbbbbbb(){
    echo "ok",PHP_EOL;

    return 0;
}
// print_r([100=>"ok"]);

// eval("echo \"OK\";");
// echo("pppp");
bbbbbbb();

\ccc\bbbbbbb();
// exit;
class a{
    public function bb(){
        echo __METHOD__,PHP_EOL;
    }

    public static function cc(){
        echo __METHOD__,PHP_EOL;
    }
}

//var_dump(ini_get("mytest.disable"));
$o1 = new a();
$o1->bb();
$o1::cc();

echo "a::cc()",PHP_EOL;

a::cc();
// ini_set("mytest.disable",true);
// var_dump(ini_get("mytest.disable"));
// //mytest_disbaled(1);
// $o1->bb();
// $o1->bb();

mybacktrace();
