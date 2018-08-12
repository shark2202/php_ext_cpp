<?php 

class a {
	public $_t;

	public function init(){
		var_dump($this);
	}
}

$ob = new ObjectPool1("a");

var_dump($ob);

$ob2 = $ob->getObject("a");
var_dump($ob2);