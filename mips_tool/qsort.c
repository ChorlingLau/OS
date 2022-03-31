#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int array[10]={2,5,4,6,1,3};
int getStandard(int i, int j){
	int key=array[i];
	while(i<j){
		while(i<j&&array[j]>=key){
			j--;
		}
		if(i<j){
			array[i]=array[j];
		}
		while(i<j&&array[i]<=key){
			i++;
		}
		if(i<j){
			array[j]=array[i];
		}
	}
	array[i]=key;
	return i;
}
void QuickSort(int low, int high){
	if(low<high){
		int standard=getStandard(low, high);
		QuickSort(low,standard-1);
		QuickSort(standard+1,high);
	}
}
int main(){
	//input
	QuickSort(0,5);
	return 0;
}

