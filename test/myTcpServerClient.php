<?php 

set_time_limit(0);   
     
$host = "127.0.0.1";   
$port = 13120;   
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)or die("Could not create  socket\n");   
      
$connection = socket_connect($socket, $host, $port) or die("Could not connet server\n");  
socket_write($socket, "hello socket") or die("Write failed\n"); 
while ($buff = socket_read($socket, 1024, PHP_NORMAL_READ)) {   
    echo("Response was:" . $buff . "\n"); 
    echo("input what you want to say to the server:\n"); 
    $text = fgets(STDIN); 
    socket_write($socket, $text); 
}   
socket_close($socket); 