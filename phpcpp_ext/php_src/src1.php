<?php
class User{
    function hello(){
        global $a ;
        $a = 'hello';
    }
}
$obj = new User();
$obj->hello();
echo $a;