#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int i,j,k;
int array1[8][8]={0};
int array2[8][8]={0};
int result;
int main(){
	int n;
	scanf("%d",&n);
	for(i=0;i<n;i++){//for1
		for(j=0;j<n;j++){//for2
			scanf("%d",&array1[i][j]);
		}
	} 
	for(i=0;i<n;i++){//for3
		for(j=0;j<n;j++){//for4
			scanf("%d",&array2[i][j]);
		}
	} 
	for(i=0;i<n;i++){//for5
		for(j=0;j<n;j++){//for6
			for(k=0,result=0;k<n;k++){//for7
				result+=array1[i][k]*array2[k][j];
			}
			printf("%d ",result);
		}
		printf("\n");
	} 
	return 0;
}

