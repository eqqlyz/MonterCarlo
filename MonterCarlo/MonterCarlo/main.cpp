#ifndef MAIN_H
#define MAIN_H

#include"MonteCarlo.h"

double N8[] = {0.1,0.14,0.15,0.16,0.4,0.56,0.60,0.64};

int main(int argc, char* argv[]){
	
	//�Լ��Ӵ��룬ʵ�ֽ���ʽ
	int gridLen =0;
	int gridWidth = 0;
	double	solidGrowRatio;
	double voidRatio;
	double *N8GrowRatio = new double[8];

	//��ʵ�Ĳ�������
	cout<< "-------�������ģ�����ʵ��-------" << endl;
	cout<< "����������ߴ�(e.g w= 100, h = 100): ";
	cin >> gridLen >> gridWidth;
	cout<<"���������������(0~1) : ";
	cin >> solidGrowRatio;
	cout<<"�������϶��(0~1) : ";
	cin >> voidRatio;
	cout<<"��������Χ������(1*8 ����) : ";
	cin >> N8GrowRatio[0] >> N8GrowRatio[1] >> N8GrowRatio[2] >> N8GrowRatio[3] >> N8GrowRatio[4] >> N8GrowRatio[5] >> N8GrowRatio[6] >> N8GrowRatio[7];

	//����������
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
	//����1. ����DFS�ݹ���㣬Ч�ʲ���
	//sPart->getDFS_Result("resultDFS.txt");
	//����2. ����BFS���㣬Ч�ʷǳ���
	sPart->getBFS_Result("resultBFS.txt");
	int stop;
	cin >> stop;
	return 1;
}

#endif //MAIN_H