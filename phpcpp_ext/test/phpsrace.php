<?php 
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
// exit;
class a{
    public function bb(){
        echo __METHOD__,PHP_EOL;
    }
}

//var_dump(ini_get("mytest.disable"));
$o1 = new a();
// $o1->bb();

// ini_set("mytest.disable",true);
// var_dump(ini_get("mytest.disable"));
// //mytest_disbaled(1);
// $o1->bb();
// // $o1->bb();