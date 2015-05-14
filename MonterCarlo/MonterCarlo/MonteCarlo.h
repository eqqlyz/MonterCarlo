#ifndef MONTECARLO_H
#define MONTECARLO_H

#include<iostream>
#include <fstream>
#include <sstream>
#include<ctime>
#include<list>

using namespace std;

#define BACK_GROUND		0
#define FORE_GROUND			1
#define VISITED						2

class solidParticles{

private:
	//���ݶ�
	double **gridData;																	//��¼��������
	long long **bwData;																	//��¼��ֵ����
	int gridLen, gridWidth;																//��¼����ĳ��Ϳ�
	double	solidGrowRatio;															//��¼���������� ����Χ[0,1]
	double voidRatio;																		//��¼��϶�ʣ���Χ[0,1]
	double *N8GrowRatio;																//��Χ�������� ��[0,1,2,3,4,5,6,7] �ֱ��Ӧ��{1,2,3,4,5,6,7,8}����
	long long solidParticleNum;														//��¼���й��������Ŀ

public:
	solidParticles(int len,int width, double growRatio, double vRatio, double *n8Ratio);
	~solidParticles();

	void generateSolidParticle();					//�������ݾ���
	long long getSolidParticleNum();			//��ȡ���й������������
	int getArea_DFS(int x,int y);					//�������꣬����DFS�������
	int getArea_BFS(int tx,int ty);				//�������꣬����BFS�������
	void getDFS_Result(char* fileName);	//��DFSͳ������������ļ���			
	void getBFS_Result(char* fileName);		//��BFSͳ������������ļ���
	void printData(char* fileName);				//��bwData��ӡ���ļ��У����ڵ���
};
#endif	//MONTECARLO_H