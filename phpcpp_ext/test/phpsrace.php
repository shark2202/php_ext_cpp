<?php 

function bbbbbbb(){
    echo "ok";
}
// print_r([100=>"ok"]);

// eval("echo \"OK\";");
// echo("pppp");
// bbbbbbb();

class a{
    public function bb(){
        echo __METHOD__,PHP_EOL;
    }
}
$o1 = new a();
$o1->bb();

mytest_disbaled(1);
$o1->bb();
$o1->bb();