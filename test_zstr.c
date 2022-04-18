#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"zstr.h"

int main() {
    zstr z = zstr_create("DeoxyribonucleicAcid");
    zstr x = zstr_substring(z, 16, 20);
    zstr_print_detailed(x);
    return 0;
}