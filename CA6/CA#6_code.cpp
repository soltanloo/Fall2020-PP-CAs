#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define N  12

int solutions;

int put(int Queens[], int row, int column)
{
  int i;
    for(i=0; i<row; i++) {
        if (Queens[i] == column  || abs( Queens[i]-column ) == ( row-i ) )  
            return -1;
   }
    Queens[row]=column; 
    if(row==N-1) {
        solutions++;
    }else{
        for(i=0; i<N; i++) { // increment row
            put(Queens, row+1, i);
        }
    }
 return 0;
}


void solve(int Queens[]) {
    int i;
    for(i=0; i<N; i++) {
        put(Queens, 0, i);
    }
}



int main()
{
    int Queens[N];
    time_t t0=0, tf=0,t0s=0,tfs=0;              

    t0 = time(NULL);
    solve(Queens);
    tf = time(NULL);

    printf( "# solutions %d time: %f \n",solutions, difftime(tf, t0));

 return 0;

}