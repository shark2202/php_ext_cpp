<?php 
$GLOBALS['tttt'] = "yyyy";
$s = new myTcpServer();
$s->port = "13120";


$s->onStart = function(){
	var_dump($GLOBALS['tttt']);
	$GLOBALS['tttt'] = "changed";
};

$s->onConnect = function(){
	var_dump($GLOBALS['tttt']);
	$GLOBALS['tttt'] = "changed2";
};

$s->onClose = function(){
	var_dump($GLOBALS['tttt']);
};
var_dump($s);
$s->run();