/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

int main(){
    //定义的常量是不能修改的
    int const a = 100;
    a = 200;
    
    const struct {
        int sa;
    } b = {.sa=100};
    
    b.sa = 200;
    
    struct {
        int const sa;
        int sb;
    } c = {.sa=100,.sb=200};
    
    c.sa = 200;
    c.sb = 300;
    
    return 0;
}

//
//gcc -Wall -std=c11 -o const1 const1.c
//const1.c: In function 'main':
//const1.c:10:5: error: assignment of read-only variable 'a'
//const1.c:16:5: error: assignment of member 'sa' in read-only object
//const1.c:23:5: error: assignment of read-only member 'sa'
//const1.c:21:7: warning: variable 'c' set but not used [-Wunused-but-set-variable]
//const1.c:14:7: warning: variable 'b' set but not used [-Wunused-but-set-variable]
//const1.c:9:15: warning: variable 'a' set but not used [-Wunused-but-set-variable]