#include<iostream>
#include<stdio.h>  
#include<string>  
#include<math.h>   
#include<vector>  
#include<ctime>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;
 
typedef struct
{
	int x;
	int y;
	float b;
	float g;
	float r;
}Tuple;
 
bool Isequal(vector<Tuple> a, vector<Tuple> b)
{
	int s = a.size();
	for (int i = 0; i < s; i++)
	{
		if (a[i].x == b[i].x&&a[i].y == b[i].y&&a[i].r == b[i].r&&a[i].g == b[i].g&&a[i].b == b[i].b)
			continue;
		else
			return false;
	}
	return true;
}
 
int main()
{
	srand(time(0));

    // 图片地址
	Mat src = imread("/Users/frank/workspace/github/c_practice/opencv/image/test.jpg", 1);
    if (src.empty())
    {
        cout << "Error : Image cannot be loaded..!!" << endl;
        return -1;
    }
    else
    {
        namedWindow("MyWindow", 0);
        imshow("MyWindow", src);
        waitKey(5000);
    }


    int width = src.cols;
	int height = src.rows;
	int dims = src.channels();
	Tuple t;

    // Kmeans
	vector<Tuple> sum_points;
	vector<Tuple> points;
	int K = 6;
	int temp_K = K;

    // 图中所点存入sum_points
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Vec3b rgb = src.at<Vec3b>(i, j);
			t.x = j;
			t.y = i;
			t.b = (float)(rgb[0]);
			t.g = (float)(rgb[1]);
			t.r = (float)(rgb[2]);
			sum_points.push_back(t);
		}
	}
	
    // 随机K个点
	for (; temp_K > 0; temp_K--)
	{
		int w = rand() % (width);
		int h = rand() % (height);
		Vec3b rgb = src.at<Vec3b>(w, h);
		Tuple temppoint;
		temppoint.x = w;
		temppoint.y = h;
		temppoint.b = (float)(rgb[0]);
		temppoint.g = (float)(rgb[1]);
		temppoint.r = (float)(rgb[2]);
		points.push_back(temppoint);
	}
	// 记录label值
	vector<int> label(sum_points.size(), 0);
	// 每次更新迭代的质心点坐标
	vector<Tuple> new_points(points.size());
	// 记录迭代多少次
	int index = 0;
	do
	{
		if (index != 0)
		{
			for (int i = 0; i < points.size(); i++)
			{
				points[i].x = new_points[i].x;
				points[i].y = new_points[i].y;
				points[i].b = new_points[i].b;
				points[i].r = new_points[i].r;
				points[i].g = new_points[i].g;
			}
		}
		
		// 欧式距离判断并更新质心点位置
		// 1、欧式距离计算分类
		for (int j = 0; j < sum_points.size(); j++)
		{
			float min_distance = float(INT_MAX);
			for (int i = 0; i < points.size(); i++)
			{
				float distance = sqrt((float)pow((sum_points[j].b - points[i].b), 2) + pow((sum_points[j].g - points[i].g), 2) + pow((sum_points[j].r - points[i].r), 2));
				if (distance < min_distance)
				//if (distance < 1)
				{
					min_distance = distance;
					label[j] = i;
				}
			}
		}
 
		// 2、更新质心的坐标点（通过label值）
		// 定义k类分别的点值变量
		vector<int> k(points.size(), 0);
		vector<float> b(points.size(), 0.0);
		vector<float> g(points.size(), 0.0);
		vector<float> r(points.size(), 0.0);
		for (int i = 0; i < label.size(); i++)
		{
			for (int j = 0; j < points.size(); j++)
			{
				// 找到对应的类别进行后续的质心坐标运算
				if (j == label[i])
				{
					k[j]++;
					b[j] += sum_points[i].b;
					g[j] += sum_points[i].g;
					r[j] += sum_points[i].r;
					break;
				}
			}
		}
 
		// 更新坐标
		for (int i = 0; i < points.size(); i++)
		{
			b[i] /= k[i];
			g[i] /= k[i];
			r[i] /= k[i];
		}
 
		// 质心坐标值填补
		for (int i = 0; i < points.size(); i++)
		{
			Tuple temp;
			temp.x = 0;
			temp.y = 0;
			temp.b = b[i];
			temp.g = g[i];
			temp.r = r[i];
			new_points[i] = temp;
		}
		index++;
	} while (!Isequal(points,new_points));
	
 
	Mat result = Mat::zeros(src.size(), CV_8UC3);
 
	for (int i = 0; i < sum_points.size(); i++)
	{
		sum_points[i].b = points[label[i]].b;
		sum_points[i].g = points[label[i]].g;
		sum_points[i].r = points[label[i]].r;
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			result.at<Vec3b>(i, j)[0] = sum_points[j+i*width].b;
			result.at<Vec3b>(i, j)[1] = sum_points[j + i * width].g;
			result.at<Vec3b>(i, j)[2] = sum_points[j + i * width].r;
		}
	}
 

    namedWindow("kmeans-demo", 0);
    imshow("kmeans-demo", result);
	waitKey(0);
 
	return 0;
}