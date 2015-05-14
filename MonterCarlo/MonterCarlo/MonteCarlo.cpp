#include "MonteCarlo.h"

solidParticles::solidParticles(int len,int width, double growRatio, double vRatio, double *n8Ratio){
	//ʵ�����������ݶ�
	gridLen = len;
	gridWidth = width;
	solidGrowRatio = growRatio;
	voidRatio = vRatio;
	N8GrowRatio = n8Ratio;
	solidParticleNum = 0;
	//����gridData
	srand(unsigned(time(0)));
	gridData = new double*[gridLen];
	for(int i = 0; i < gridLen; i++)
		gridData[i] = new double[gridWidth];
	for(int i= 0;i<gridLen;i++)
		for(int j=0;j<gridWidth;j++)
			gridData[i][j] = rand()/(RAND_MAX+ 1.0);

	//����bwData
	bwData = new long long*[gridLen];
	for(int i = 0; i < gridLen; i++)
		bwData[i] = new long long[gridWidth];
	for(int i= 0;i<gridLen;i++)
		for(int j=0;j<gridWidth;j++)
			bwData[i][j] = BACK_GROUND;

	//���ݹ�����������
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
* OVERVIEW:		�����㷨�����������ݾ��󣬽��������ݴ洢��bwData�����У�ɾ��gridData
* PARAMETERS:		<none>
* RETURNS:			 bwData 
*============================================================================*/
void solidParticles::generateSolidParticle(){
	//1. ���ɳ�ʼ�������
	solidParticleNum = 0;
	for(int i= 0;i<gridLen;i++)
		for(int j=0;j<gridWidth;j++)
			if(gridData[i][j] < solidGrowRatio){
				bwData[i][j] = FORE_GROUND;
				solidParticleNum++;
			}

	//2. �����������
	int maxParticleNum = solidGrowRatio*gridLen*gridWidth;
	bool enough = false;
	while(solidParticleNum < maxParticleNum){
		//Ϊ�˱�̵ķ��㣬��������Χ������
		for(int i= 1;i<gridLen - 1;i++){
			for(int j = 1;j<gridWidth - 1;j++){
				if(bwData[i][j] == FORE_GROUND){
					//���(i,j)�ǹ������

					//2.1 �������(i,j)�İ˸���������������Ӧ��ϵΪ [0,1,2,3,4,5,6,7] �ֱ��Ӧ��{1,2,3,4,5,6,7,8}����
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

	//3. ɾ�����õ�gridData
	for(int i= 0;i<gridLen;i++){
		if(gridData)
			delete [] gridData[i];
	}
	if(gridData)
		delete [] gridData;
}

/*==============================================================================
* FUNCTION:		long long solidParticles::getSolidParticleNum
* OVERVIEW:		��ȡ���й������������
* PARAMETERS:		<none>
* RETURNS:			 �����������
*============================================================================*/
long long solidParticles::getSolidParticleNum(){
	return solidParticleNum;
}

/*==============================================================================
* FUNCTION:		int solidParticles::getArea_DFS(int x,int y)
* OVERVIEW:		�������꣬���հ������壬���õݹ��㷨����ù�����������
* PARAMETERS:		int x, int y
* RETURNS:			    ��� 
*============================================================================*/
int solidParticles::getArea_DFS(int x,int y){
	//��ʼ������(x,y)����ΪVISITED
	bwData[x][y] = VISITED;

	//����0���жϱ߽�����
	/*
	 * ���� 0.1	���Ͻ� ::ֻ����������
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
	 * ���� 0.2	���
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
	 * ���� 0.3	���½�
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
	 * ���� 0.4	�±�
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
	 * ���� 0.5	���½�
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
	 * ���� 0.6	�ұ�
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
	 * ���� 0.7	���Ͻ�
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
	 * ���� 0.8	�ϱ�
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
	 * ���� 0.8	�ڲ���
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
* OVERVIEW:		�������꣬���հ������壬����BFS�����㷨����ù�����������
* PARAMETERS:		int x, int y
* RETURNS:			    ��� 
*============================================================================*/
int solidParticles::getArea_BFS(int tx,int ty){
	list<int> *unvisitedList = new list<int>;
	bwData[tx][ty] = VISITED;
	int area = 0;
	unvisitedList->push_back(tx*gridWidth + ty);
	while(!unvisitedList->empty()){
		//1. ��(x,y)����ΪVISITED
		area ++;
		int currPix = unvisitedList->front();
		int x = currPix/gridWidth;
		int y = currPix%gridWidth;
		unvisitedList->pop_front();

		//2. ��(x,y)�ġ�1�� �����򶼼��뵽unvisitedList��
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
* OVERVIEW:		��ͳ�ƽ����ӡ���ļ���
* PARAMETERS:		����ļ���
* RETURNS:			    NONE
*============================================================================*/
void solidParticles::getDFS_Result(char* fileName){
	// ͳ�ƿ�����Ŀ�Ϳ���������������������ļ���	
	ofstream of;
	of.open(fileName,'w');
	of << "---------------------------------------------------"<<endl;
	of << "               ����ֵ         ���                  "<<endl;
	of << "---------------------------------------------------"<<endl;
	solidParticleNum = 0;
	for(int i= 0;i<gridLen;i++){
		for(int j=0;j<gridWidth;j++){
			if(bwData[i][j] == FORE_GROUND){
				int area = getArea_DFS(i,j) + 1;
				solidParticleNum++;
				//��(i,j) area���뵽�ļ�
				of << "              ("<< i <<" , " << j<<")          "<< area << "                  "<<endl;
			}
		}
	}
	of << "---------------------------------------------------"<<endl;
	of << "����������� ��  "<< solidParticleNum;
	of.close();
}

/*==============================================================================
* FUNCTION:		void solidParticles::getBFS_Result(char* fileName)
* OVERVIEW:		��ͳ�ƽ����ӡ���ļ���
* PARAMETERS:		����ļ���
* RETURNS:			    NONE
*============================================================================*/
void solidParticles::getBFS_Result(char* fileName){
	// ͳ�ƿ�����Ŀ�Ϳ���������������������ļ���	
	ofstream of;
	of.open(fileName,'w');
	of << "---------------------------------------------------"<<endl;
	of << "               ����ֵ         ���                  "<<endl;
	of << "---------------------------------------------------"<<endl;
	solidParticleNum = 0;
	for(int i= 0;i<gridLen;i++){
		for(int j=0;j<gridWidth;j++){
			if(bwData[i][j] == FORE_GROUND){
				int area = getArea_BFS(i,j) ;
				solidParticleNum++;
				//��(i,j) area���뵽�ļ�
				of << "              ("<< i <<" , " << j<<")          "<< area << "                  "<<endl;
			}
		}
	}
	of << "---------------------------------------------------"<<endl;
	of << "����������� ��  "<< solidParticleNum;
	of.close();
}

/*==============================================================================
* FUNCTION:		void solidParticles::printData(char* fileName)
* OVERVIEW:		��bwData��ӡ���ļ���
* PARAMETERS:		����ļ���
* RETURNS:			    NONE
*============================================================================*/
void solidParticles::printData(char* fileName){
	//��bwData��ӡ���ļ��У����ڵ���
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