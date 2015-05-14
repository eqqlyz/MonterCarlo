#ifndef MAIN_H
#define MAIN_H

#include"MonteCarlo.h"

double N8[] = {0.1,0.14,0.15,0.16,0.4,0.56,0.60,0.64};

int main(int argc, char* argv[]){
	
	//自己加代码，实现交互式
	int gridLen =0;
	int gridWidth = 0;
	double	solidGrowRatio;
	double voidRatio;
	double *N8GrowRatio = new double[8];

	//真实的操作数据
	cout<< "-------固相颗粒模拟计算实验-------" << endl;
	cout<< "请输入网格尺寸(e.g w= 100, h = 100): ";
	cin >> gridLen >> gridWidth;
	cout<<"请输入固相生长率(0~1) : ";
	cin >> solidGrowRatio;
	cout<<"请输入孔隙率(0~1) : ";
	cin >> voidRatio;
	cout<<"请输入周围生长率(1*8 向量) : ";
	cin >> N8GrowRatio[0] >> N8GrowRatio[1] >> N8GrowRatio[2] >> N8GrowRatio[3] >> N8GrowRatio[4] >> N8GrowRatio[5] >> N8GrowRatio[6] >> N8GrowRatio[7];

	//测试用数据
	//gridLen = gridWidth = 500;
	//solidGrowRatio = 0.5;
	//voidRatio = 0.2;
	//N8GrowRatio[0] = 0.1;
	//N8GrowRatio[1] = 0.2;
	//N8GrowRatio[2] = 0.2;
	//N8GrowRatio[3] = 0.15;
	//N8GrowRatio[4] = 0.4;
	//N8GrowRatio[5] = 0.8;
	//N8GrowRatio[6] = 0.8;
	//N8GrowRatio[7] = 0.6;

	solidParticles *sPart = new solidParticles(gridLen,gridWidth,solidGrowRatio,voidRatio,N8GrowRatio);
	sPart->printData("test.txt");
	//方法1. 采用DFS递归计算，效率不高
	//sPart->getDFS_Result("resultDFS.txt");
	//方法2. 采用BFS计算，效率非常高
	sPart->getBFS_Result("resultBFS.txt");
	int stop;
	cin >> stop;
	return 1;
}

#endif //MAIN_H