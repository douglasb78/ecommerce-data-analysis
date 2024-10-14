#include <stdio.h>
#include "btree.h"
#include "parser.h"

void insert_value(BTree *btree, LineData *line_data){
	return;
}

int main(){
	LineData *teste1, *teste2;
	teste1 = read_line("2019-11-01 00:00:00 UTC,view,1003461,2053013555631882655,electronics.smartphone,xiaomi,489.07,520088904,4d3b30da-a5e4-49df-b1a8-ba5943f1dd33\n");
	teste2 = read_line("2019-11-01 00:00:04 UTC,view,34600011,2060981320581906480,,,20.54,512416379,4dfe2c67-e537-4dc2-ae69-0b65263db091\n");
	
	print_data(teste1);
	print_data(teste2);
	return 0;
}
