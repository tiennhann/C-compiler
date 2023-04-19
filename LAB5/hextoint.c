#include <stdio.h>
#include <math.h>
#include <string.h>

int hexToInt(char *hex){
    int length = strlen(hex)-1;
    long long decimal = 0, base = 1;
    int i = 0, value;
    /* Get hexadecimal value from user */
    for(i = length--; i >= 0; i--) {
        if(hex[i] >= '0' && hex[i] <= '9'){
            decimal += (hex[i] - 48) * base;
            base *= 16;
        }
        else if(hex[i] >= 'A' && hex[i] <= 'F'){
            decimal += (hex[i] - 55) * base;
            base *= 16;
        }
        else if(hex[i] >= 'a' && hex[i] <= 'f'){
            decimal += (hex[i] - 87) * base;
            base *= 16;
        }
    }
    return decimal;
}

int main(){
   char hex[100];
   printf("Enter Hexadecimal: ");
   scanf("%s", hex);
   printf("\nDecimal: %d", hexToInt(hex));
   return 0;
}

