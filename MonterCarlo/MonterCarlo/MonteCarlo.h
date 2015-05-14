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
	//数据段
	double **gridData;																	//记录网格数据
	long long **bwData;																	//记录二值数据
	int gridLen, gridWidth;																//记录网格的长和宽
	double	solidGrowRatio;															//记录固相生长率 ，范围[0,1]
	double voidRatio;																		//记录孔隙率，范围[0,1]
	double *N8GrowRatio;																//周围生长概率 ，[0,1,2,3,4,5,6,7] 分别对应于{1,2,3,4,5,6,7,8}领域
	long long solidParticleNum;														//记录所有固体颗粒数目

public:
	solidParticles(int len,int width, double growRatio, double vRatio, double *n8Ratio);
	~solidParticles();

	void generateSolidParticle();					//生成数据矩阵
	long long getSolidParticleNum();			//获取所有固体颗粒的总数
	int getArea_DFS(int x,int y);					//根据坐标，利用DFS计算面积
	int getArea_BFS(int tx,int ty);				//根据坐标，利用BFS计算面积
	void getDFS_Result(char* fileName);	//将DFS统计数据输出到文件中			
	void getBFS_Result(char* fileName);		//将BFS统计数据输出到文件中
	void printData(char* fileName);				//将bwData打印到文件中，便于调试
};
#endif	//MONTECARLO_H