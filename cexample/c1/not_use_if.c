/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>

int main(){
    int i;
    printf("input:\n");
    scanf("%d",&i);
    
    int x = i - 10;
    if(x<=0) goto NOT_POSITIVE;
    printf(">10\n");
    goto END;
    
    NOT_POSITIVE:
    if(x >=0) goto NOT_NEGATIVE;
    printf("<10\n");
    goto END;
    
    NOT_NEGATIVE:
    printf("=10\n");
    
    END:
    return 0;
}
