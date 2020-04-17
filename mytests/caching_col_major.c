/* 
* Accessing matrix elements in Column-major order.
*
* Author: Nishant Malpani (Nishant.Malpani@iiitb.org)
*
*/

#include <stdio.h>

int main()
{
        int i, j;
        int array[1000][1000] = {0}; 

        for(i = 0; i < 1000; i++) {
                for(j = 0; j < 1000; j++) {
                        array[j][i] = 1;
                }
        }

        return 0;
}