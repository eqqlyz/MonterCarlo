#include "MonteCarlo.h"

solidParticles::solidParticles(int len,int width, double growRatio, double vRatio, double *n8Ratio){
	//实例化各个数据段
	gridLen = len;
	gridWidth = width;
	solidGrowRatio = growRatio;
	voidRatio = vRatio;
	N8GrowRatio = n8Ratio;
	solidParticleNum = 0;
	//构造gridData
	srand(unsigned(time(0)));
	gridData = new double*[gridLen];
	for(int i = 0; i < gridLen; i++)
		gridData[i] = new double[gridWidth];
	for(int i= 0;i<gridLen;i++)
		for(int j=0;j<gridWidth;j++)
			gridData[i][j] = rand()/(RAND_MAX+ 1.0);

	//构造bwData
	bwData = new long long*[gridLen];
	for(int i = 0; i < gridLen; i++)
		bwData[i] = new long long[gridWidth];
	for(int i= 0;i<gridLen;i++)
		for(int j=0;j<gridWidth;j++)
			bwData[i][j] = BACK_GROUND;

	//根据规则，生成数据
	generateSolidParticle();
}

solidParticles::~solidParticles(){
	for(int i= 0;i<gridLen;i++){
		if(gridData)
			delete [] gridData[i];
		if(bwData)
			delete [] bwData[i];
	}
	if(gridData)
		delete [] gridData;
	if(bwData)
		delete []  bwData;
}

/*==============================================================================
* FUNCTION:		solidParticles::generateSolidParticle
* OVERVIEW:		根据算法规则，生成数据矩阵，将最终数据存储在bwData数组中，删除gridData
* PARAMETERS:		<none>
* RETURNS:			 bwData 
*============================================================================*/
void solidParticles::generateSolidParticle(){
	//1. 生成初始固相颗粒
	solidParticleNum = 0;
	for(int i= 0;i<gridLen;i++)
		for(int j=0;j<gridWidth;j++)
			if(gridData[i][j] < solidGrowRatio){
				bwData[i][j] = FORE_GROUND;
				solidParticleNum++;
			}

	//2. 生长固相颗粒
	int maxParticleNum = solidGrowRatio*gridLen*gridWidth;
	bool enough = false;
	while(solidParticleNum < maxParticleNum){
		//为了编程的方便，不遍历外围的网格
		for(int i= 1;i<gridLen - 1;i++){
			for(int j = 1;j<gridWidth - 1;j++){
				if(bwData[i][j] == FORE_GROUND){
					//如果(i,j)是固相颗粒

					//2.1 随机生成(i,j)的八个领域的随机数，对应关系为 [0,1,2,3,4,5,6,7] 分别对应于{1,2,3,4,5,6,7,8}领域
					for(int k = 0; k < 8; k++){
						double tmpN8 = rand()/(RAND_MAX+ 1.0);
						if(tmpN8 < N8GrowRatio[k]){
							switch(k){
							case 0:
								if(bwData[i][j-1] != FORE_GROUND){
									bwData[i][j-1] = FORE_GROUND;
									solidParticleNum++;
								}
								break;
							case 1:
								if(bwData[i][j+1] != FORE_GROUND){
									bwData[i][j+1] = FORE_GROUND;
									solidParticleNum++;
								}
								break;
							case 2:
								if(bwData[i][j-1] != FORE_GROUND){
									bwData[i][j-1] = FORE_GROUND;
									solidParticleNum++;
								}
								break;
							case 3:
								if(bwData[i-1][j] != FORE_GROUND){
									bwData[i-1][j] = FORE_GROUND;
									solidParticleNum++;
								}
								break;
							case 4:
								if(bwData[i-1][j-1] != FORE_GROUND){
									bwData[i-1][j-1] = FORE_GROUND;
									solidParticleNum++;
								}
								break;
							case 5:
								if(bwData[i-1][j+1] != FORE_GROUND){
									bwData[i-1][j+1] = FORE_GROUND;
									solidParticleNum++;
								}
								break;
							case 6:
								if(bwData[i+1][j-1] != FORE_GROUND){
									bwData[i+1][j-1] = FORE_GROUND;
									solidParticleNum++;
								}
								break;
							case 7:
								if(bwData[i+1][j+1] != FORE_GROUND){
									bwData[i+1][j+1] = FORE_GROUND;
									solidParticleNum++;
								}
								break;
							default:
								break;
							}
						}
						if(solidParticleNum > maxParticleNum){
							enough = true;
							break;
						}
					}
					if(enough)
						break;
				}
				if(enough)
					break;
			}
			if(enough)
				break;
		}
		if(enough)
			break;
	}

	//3. 删除无用的gridData
	for(int i= 0;i<gridLen;i++){
		if(gridData)
			delete [] gridData[i];
	}
	if(gridData)
		delete [] gridData;
}

/*==============================================================================
* FUNCTION:		long long solidParticles::getSolidParticleNum
* OVERVIEW:		获取所有固体颗粒的总数
* PARAMETERS:		<none>
* RETURNS:			 固体颗粒总数
*============================================================================*/
long long solidParticles::getSolidParticleNum(){
	return solidParticleNum;
}

/*==============================================================================
* FUNCTION:		int solidParticles::getArea_DFS(int x,int y)
* OVERVIEW:		根据坐标，按照八邻域定义，利用递归算法计算该固体颗粒的面积
* PARAMETERS:		int x, int y
* RETURNS:			    面积 
*============================================================================*/
int solidParticles::getArea_DFS(int x,int y){
	//初始化：将(x,y)设置为VISITED
	bwData[x][y] = VISITED;

	//情形0：判断边界条件
	/*
	 * 情形 0.1	左上角 ::只有三个领域
	*/
	if(x == 0 && y == 0){
		if(bwData[x][y+1] == BACK_GROUND && bwData[x+1][y] == BACK_GROUND && bwData[x+1][y+1] == BACK_GROUND)
			return 0;
		int rightArea = 0;
		int downArea = 0;
		int downRightArea = 0;
		if(bwData[x][y+1] == FORE_GROUND){
			rightArea += getArea_DFS(x,y+1) + 1;
		}
		if(bwData[x+1][y] == FORE_GROUND){
			downArea += getArea_DFS(x+1,y) + 1;
		}
		if(bwData[x+1][y+1] == FORE_GROUND){
			downRightArea += getArea_DFS(x+1,y+1) + 1;
		}
		return rightArea + downArea + downRightArea;
	}

	/*
	 * 情形 0.2	左边
	*/	
	if(y == 0 && ( 0 < x && x < gridLen - 1)){
		if(bwData[x-1][y] == BACK_GROUND && bwData[x-1][y+1] == BACK_GROUND && bwData[x][y+1] == BACK_GROUND && bwData[x+1][y+1] == BACK_GROUND && bwData[x+1][y] == BACK_GROUND)
			return 0;
		int upArea = 0;
		int upRightArea = 0;
		int rightArea = 0;
		int downRightArea = 0;
		int downArea = 0;

		if(bwData[x-1][y] == FORE_GROUND){
			upArea += getArea_DFS(x-1,y) + 1;
		}
		if(bwData[x-1][y+1] == FORE_GROUND){
			upRightArea += getArea_DFS(x-1,y+1) + 1;
		}
		if(bwData[x][y+1] == FORE_GROUND){
			rightArea += getArea_DFS(x,y+1) + 1;
		}
		if(bwData[x+1][y+1] == FORE_GROUND){
			downRightArea += getArea_DFS(x+1,y+1) + 1;
		}
		if(bwData[x+1][y] == FORE_GROUND){
			downArea += getArea_DFS(x+1,y) + 1;
		}
		return upArea + upRightArea + rightArea + downRightArea + downArea ;
	}
	/*
	 * 情形 0.3	左下角
	*/	
	if((x ==  gridLen-1) && y == 0){
		if(bwData[x-1][y] == BACK_GROUND && bwData[x-1][y+1] == BACK_GROUND && bwData[x][y+1] == BACK_GROUND)
			return 0;
		int upArea = 0;
		int upRightArea = 0;
		int rightArea = 0;
		if(bwData[x-1][y] == FORE_GROUND){
			upArea += getArea_DFS(x-1,y) + 1;
		}
		if(bwData[x-1][y+1] == FORE_GROUND){
			upRightArea += getArea_DFS(x-1,y+1) + 1;
		}
		if(bwData[x][y+1] == FORE_GROUND){
			rightArea += getArea_DFS(x,y+1) + 1;
		}
		return upArea + upRightArea + rightArea;
	}
	/*
	 * 情形 0.4	下边
	*/	
	if((x ==  gridLen-1) && (0 < y && y < gridWidth - 1)){
		if(bwData[x][y-1] == BACK_GROUND && bwData[x-1][y-1] == BACK_GROUND && bwData[x-1][y] == BACK_GROUND && bwData[x-1][y+1] == BACK_GROUND && bwData[x][y+1] == BACK_GROUND)
			return 0;
		int leftArea = 0;
		int upLeftArea = 0;
		int upArea = 0;
		int upRightArea = 0;
		int rightArea = 0;
		if(bwData[x][y-1] == FORE_GROUND){
			leftArea += getArea_DFS(x,y-1) + 1;
		}
		if(bwData[x-1][y-1] == FORE_GROUND){
			upLeftArea += getArea_DFS(x-1,y-1) + 1;
		}
		if(bwData[x-1][y] == FORE_GROUND){
			upArea += getArea_DFS(x-1,y) + 1;
		}
		if(bwData[x-1][y+1] == FORE_GROUND){
			upRightArea += getArea_DFS(x-1,y+1) + 1;
		}
		if(bwData[x][y+1] == FORE_GROUND){
			rightArea += getArea_DFS(x,y+1) + 1;
		}
		return leftArea + upLeftArea + upArea + upRightArea + rightArea;
	}
	/*
	 * 情形 0.5	右下角
	*/	
	if((x ==  gridLen-1) && (y ==gridWidth -1 )){
		if(bwData[x][y-1] == BACK_GROUND && bwData[x-1][y-1] == BACK_GROUND && bwData[x-1][y] == BACK_GROUND)
			return 0;
		int leftArea = 0;
		int upLeftArea = 0;
		int upArea = 0;
		if(bwData[x][y-1] == FORE_GROUND){
			leftArea += getArea_DFS(x,y-1) + 1;
		}
		if(bwData[x-1][y-1] == FORE_GROUND){
			upLeftArea += getArea_DFS(x-1,y-1) + 1;
		}
		if(bwData[x-1][y] == FORE_GROUND){
			upArea += getArea_DFS(x-1,y) + 1;
		}
		return leftArea + upLeftArea + upArea;
	}
	/*
	 * 情形 0.6	右边
	*/	
	if((y ==  gridWidth-1) && (0 < x && x < gridLen - 1)){
		if(bwData[x+1][y] == BACK_GROUND &&bwData[x+1][y-1] == BACK_GROUND &&bwData[x][y-1] == BACK_GROUND && bwData[x-1][y-1] == BACK_GROUND && bwData[x-1][y] == BACK_GROUND)
			return 0;
		int downArea = 0;
		int downLeftArea = 0;
		int leftArea = 0;
		int upLeftArea = 0;
		int upArea = 0;
		if(bwData[x+1][y] == FORE_GROUND){
			downArea += getArea_DFS(x+1,y) + 1;
		}
		if(bwData[x+1][y-1] == FORE_GROUND){
			downLeftArea += getArea_DFS(x+1,y-1) + 1;
		}
		if(bwData[x][y-1] == FORE_GROUND){
			leftArea += getArea_DFS(x,y-1) + 1;
		}
		if(bwData[x-1][y-1] == FORE_GROUND){
			upLeftArea += getArea_DFS(x-1,y-1) + 1;
		}
		if(bwData[x-1][y] == FORE_GROUND){
			upArea += getArea_DFS(x-1,y) + 1;
		}
		return downArea + downLeftArea + leftArea + upLeftArea + upArea;
	}
	/*
	 * 情形 0.7	右上角
	*/	
	if((y ==  gridWidth-1 ) && x == 0){
		if(bwData[x+1][y] == BACK_GROUND && bwData[x+1][y-1] == BACK_GROUND && bwData[x][y-1] == BACK_GROUND)
			return 0;
		int downArea = 0;
		int downLeftArea = 0;
		int leftArea = 0;
		if(bwData[x+1][y] == FORE_GROUND){
			downArea += getArea_DFS(x+1,y) + 1;
		}
		if(bwData[x+1][y-1] == FORE_GROUND){
			downLeftArea += getArea_DFS(x+1,y-1) + 1;
		}
		if(bwData[x][y-1] == FORE_GROUND){
			leftArea += getArea_DFS(x,y-1) + 1;
		}
		return downArea + downLeftArea + leftArea;
	}
	/*
	 * 情形 0.8	上边
	*/	
	if( x == 0 &&( 0 < y && y < gridWidth - 1)){
		if(bwData[x][y+1] == BACK_GROUND &&bwData[x+1][y+1] == BACK_GROUND &&bwData[x+1][y] == BACK_GROUND &&bwData[x+1][y-1] == BACK_GROUND && bwData[x][y-1] == BACK_GROUND)
			return 0;
		int rightArea = 0;
		int downRightArea = 0;
		int downArea = 0;
		int downLeftArea = 0;
		int leftArea = 0;
		if(bwData[x][y+1] == FORE_GROUND){
			rightArea += getArea_DFS(x,y+1) + 1;
		}
		if(bwData[x+1][y+1] == FORE_GROUND){
			downRightArea += getArea_DFS(x+1,y+1) + 1;
		}
		if(bwData[x+1][y] == FORE_GROUND){
			downArea += getArea_DFS(x+1,y) + 1;
		}
		if(bwData[x+1][y-1] == FORE_GROUND){
			downLeftArea += getArea_DFS(x+1,y-1) + 1;
		}
		if(bwData[x][y-1] == FORE_GROUND){
			leftArea += getArea_DFS(x,y-1) + 1;
		}
		return rightArea + downRightArea + downArea + downLeftArea + leftArea ;
	}
	/*
	 * 情形 0.8	内部点
	*/	
	if((0 < x && x < gridLen - 1) && (0 < y && y < gridWidth - 1)){
		if(bwData[x-1][y-1] == BACK_GROUND && bwData[x-1][y] == BACK_GROUND &&bwData[x-1][y+1] ==BACK_GROUND 
			&&bwData[x][y-1] == BACK_GROUND && bwData[x][y+1] == BACK_GROUND
			&&bwData[x+1][y-1] == BACK_GROUND && bwData[x+1][y] == BACK_GROUND && bwData[x+1][y+1] == BACK_GROUND)
			return 0;

		int upLeftArea = 0;
		int upArea = 0;
		int upRightArea = 0;
		int leftArea = 0;
		int rightArea = 0;
		int downLeftArea = 0;
		int downArea = 0;
		int downRightArea = 0;
		if(bwData[x-1][y-1] == FORE_GROUND){
			upLeftArea += getArea_DFS(x-1,y-1) + 1;
		}
		if(bwData[x-1][y] == FORE_GROUND){
			upArea += getArea_DFS(x-1,y) + 1;
		}
		if(bwData[x-1][y+1] == FORE_GROUND){
			upRightArea += getArea_DFS(x-1,y+1) + 1;
		}
		if(bwData[x][y-1] == FORE_GROUND){
			leftArea += getArea_DFS(x,y-1) + 1;
		}
		if(bwData[x][y+1] == FORE_GROUND){
			rightArea += getArea_DFS(x,y+1) + 1;
		}
		if(bwData[x+1][y-1] == FORE_GROUND){
			downLeftArea += getArea_DFS(x+1,y-1) + 1;
		}
		if(bwData[x+1][y] == FORE_GROUND){
			downArea += getArea_DFS(x+1,y) + 1;
		}
		if(bwData[x+1][y+1] == FORE_GROUND){
			downRightArea += getArea_DFS(x+1,y+1) + 1;
		}
		return upLeftArea + upArea + upRightArea + leftArea + rightArea + downLeftArea + downArea + downRightArea;
	}
}

/*==============================================================================
* FUNCTION:		int solidParticles::getArea_BFS(int x,int y)
* OVERVIEW:		根据坐标，按照八邻域定义，利用BFS遍历算法计算该固体颗粒的面积
* PARAMETERS:		int x, int y
* RETURNS:			    面积 
*============================================================================*/
int solidParticles::getArea_BFS(int tx,int ty){
	list<int> *unvisitedList = new list<int>;
	bwData[tx][ty] = VISITED;
	int area = 0;
	unvisitedList->push_back(tx*gridWidth + ty);
	while(!unvisitedList->empty()){
		//1. 将(x,y)设置为VISITED
		area ++;
		int currPix = unvisitedList->front();
		int x = currPix/gridWidth;
		int y = currPix%gridWidth;
		unvisitedList->pop_front();

		//2. 将(x,y)的‘1’ 八领域都加入到unvisitedList中
		if(x-1 >=0 && y-1>=0 && bwData[x-1][y-1] == FORE_GROUND){
			unvisitedList->push_back((x-1)*gridWidth + (y-1));
			bwData[x-1][y-1]= VISITED;
		}
		
		if(x-1 >= 0 && bwData[x-1][y] == FORE_GROUND){
			unvisitedList->push_back((x-1)*gridWidth + (y));
			bwData[x-1][y]= VISITED;
		}

		if((x-1 >= 0 && y+1 <= gridWidth-1 && bwData[x-1][y+1] == FORE_GROUND)){
			unvisitedList->push_back((x-1)*gridWidth + (y+1));
			bwData[x-1][y+1]= VISITED;
		}

		if(y-1 >= 0 && bwData[x][y-1] == FORE_GROUND){
			unvisitedList->push_back((x)*gridWidth + (y-1));
			bwData[x][y-1]= VISITED;
		}

		if(y+1 <= gridWidth-1  && bwData[x][y+1] == FORE_GROUND){
			unvisitedList->push_back((x)*gridWidth + (y+1));
			bwData[x][y+1]= VISITED;
		}

		if(x+1 <= gridLen-1 && y-1 >= 0 && bwData[x+1][y-1] == FORE_GROUND){
			unvisitedList->push_back((x+1)*gridWidth + (y-1));
			bwData[x+1][y-1]= VISITED;
		}

		if(x+1 <= gridLen-1 && bwData[x+1][y] == FORE_GROUND){
			unvisitedList->push_back((x+1)*gridWidth + (y));
			bwData[x+1][y]= VISITED;
		}

		if(x+1 <= gridLen-1 && y+1 <= gridWidth-1 && bwData[x+1][y+1] == FORE_GROUND){
			unvisitedList->push_back((x+1)*gridWidth + (y+1));
			bwData[x+1][y+1]= VISITED;
		}
	}

	delete unvisitedList;
	return area;
}

/*==============================================================================
* FUNCTION:		void solidParticles::getDFS_Result(char* fileName)
* OVERVIEW:		将统计结果打印到文件中
* PARAMETERS:		输出文件名
* RETURNS:			    NONE
*============================================================================*/
void solidParticles::getDFS_Result(char* fileName){
	// 统计颗粒数目和颗粒面积，并将结果输出到文件中	
	ofstream of;
	of.open(fileName,'w');
	of << "---------------------------------------------------"<<endl;
	of << "               坐标值         面积                  "<<endl;
	of << "---------------------------------------------------"<<endl;
	solidParticleNum = 0;
	for(int i= 0;i<gridLen;i++){
		for(int j=0;j<gridWidth;j++){
			if(bwData[i][j] == FORE_GROUND){
				int area = getArea_DFS(i,j) + 1;
				solidParticleNum++;
				//将(i,j) area输入到文件
				of << "              ("<< i <<" , " << j<<")          "<< area << "                  "<<endl;
			}
		}
	}
	of << "---------------------------------------------------"<<endl;
	of << "固相颗粒总数 ：  "<< solidParticleNum;
	of.close();
}

/*==============================================================================
* FUNCTION:		void solidParticles::getBFS_Result(char* fileName)
* OVERVIEW:		将统计结果打印到文件中
* PARAMETERS:		输出文件名
* RETURNS:			    NONE
*============================================================================*/
void solidParticles::getBFS_Result(char* fileName){
	// 统计颗粒数目和颗粒面积，并将结果输出到文件中	
	ofstream of;
	of.open(fileName,'w');
	of << "---------------------------------------------------"<<endl;
	of << "               坐标值         面积                  "<<endl;
	of << "---------------------------------------------------"<<endl;
	solidParticleNum = 0;
	for(int i= 0;i<gridLen;i++){
		for(int j=0;j<gridWidth;j++){
			if(bwData[i][j] == FORE_GROUND){
				int area = getArea_BFS(i,j) ;
				solidParticleNum++;
				//将(i,j) area输入到文件
				of << "              ("<< i <<" , " << j<<")          "<< area << "                  "<<endl;
			}
		}
	}
	of << "---------------------------------------------------"<<endl;
	of << "固相颗粒总数 ：  "<< solidParticleNum;
	of.close();
}

/*==============================================================================
* FUNCTION:		void solidParticles::printData(char* fileName)
* OVERVIEW:		将bwData打印到文件中
* PARAMETERS:		输出文件名
* RETURNS:			    NONE
*============================================================================*/
void solidParticles::printData(char* fileName){
	//将bwData打印到文件中，便于调试
	ofstream of;
	of.open(fileName,'w');
	for(int i= 0;i<gridLen;i++){
		for(int j=0;j<gridWidth;j++){
			of << bwData[i][j] <<" ";
		}
		of << endl;
	}
	of.close();
}