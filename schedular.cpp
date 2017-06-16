#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <random>
#include <iomanip>
#include <algorithm>
#include <list>
#include "GNUplot.h"
#include <queue>
#include <ctime>
#include <chrono>
#include <concurrent_priority_queue.h>
using namespace std;

struct process
{
	int ID;
	double bursttime;
	double arrivtime;
	int priority;
	double quantc;
	double finishtime;
	double tmpbtime;
};

void myplot(double xvalues[], double yvalues[], int length)
{
	GNUPlotInterface pi;
	pi.CreatePipes();
	pi.StartProcess();
	string gnuplotOutput = pi.Init("lines", "y");

	pi.Plot(xvalues, yvalues, length);

	system("pause");
	pi.CloseAll();

}


vector<process> sortproc(vector<double>arr, vector<process>p)
{
	for (unsigned int i = 0;i < p.size();i++)
		for (unsigned int j = 0;j < p.size();j++)

		{
			if (arr[i] == p[j].arrivtime)
			{
				process temp;
				temp = p[j];
				p[j] = p[i];
				p[i] = temp;
			}
		}
	return p;
}
bool sort_arrtime(const process& x, const process& y)
{
	return x.arrivtime < y.arrivtime;
}

process getmax(vector<process> l)
{
	process max = l[0];
	int maxpr = l[0].priority;
	int minid = l[0].ID;
	for (unsigned int i = 0;i < l.size();i++)
	{
		if (l[i].priority > maxpr)
		{
			max = l[i];
			maxpr = l[i].priority;
			minid = l[i].ID;
		}
		if (l[i].priority == maxpr)
		{
			if (l[i].ID < minid)
			{
				max = l[i];
				maxpr = l[i].priority;
				minid = l[i].ID;
			}
		}
	}
	return max;
}

process getmin(vector<process> l)
{
	process min = l[0];
	int mintime = l[0].bursttime;
	int minid = l[0].ID;
	for (unsigned int i = 0;i < l.size();i++)
	{
		if (l[i].bursttime < mintime)
		{
			min = l[i];
			mintime = l[i].bursttime;
			minid = l[i].ID;
		}
		if (l[i].bursttime == mintime)
		{
			if (l[i].ID < minid)
			{
				min = l[i];
				mintime = l[i].bursttime;
				minid = l[i].ID;
			}
		}
	}
	return min;
}

int geteq(vector<process> l, process p)
{
	int loc = 0;
	for (unsigned int i = 0;i < l.size();i++)
	{
		if (p.ID == l[i].ID)
		{
			loc = i;
			break;
		}
	}
	return loc;
}
int main()
{
	//srand(time(NULL));
	while (true)
	{
		vector<double> arrivalTime;
		vector<double> burstTime;
		vector<int> priorities;
		vector <int> IDs;
		int processNumber;
		double arrival_mew;
		double arrival_sigma;
		double burst_mew;
		double burst_sigma;
		double lamda;
		double random;
		int p_random;
		string filename;
		cout << "Enter Name of file: ";
		cin >> filename;
		filename = filename + ".txt";
		ifstream inFile;
		ofstream outFile;
		ofstream outfile2;
		outFile.open(filename);
		inFile.open("SRTN2.txt");
		outfile2.open("output.txt");

		//inFile >> processNumber;
		//inFile >> arrival_mew;
		//inFile >> arrival_sigma;
		//inFile >> burst_mew;
		//inFile >> burst_sigma;
		//inFile >> lamda;
		int id;
		double arrival;
		double burst;
		int priority;
		inFile >> processNumber;
		for (int i = 0;i < processNumber;i++)
		{
			inFile >> id;
			IDs.push_back(id);
			inFile >> arrival;
			arrivalTime.push_back(arrival);
			inFile >> burst;
			burstTime.push_back(burst);
			inFile >> priority;
			priorities.push_back(priority);
		}
		
		/*unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);
		normal_distribution<double> N_distribution(arrival_mew, arrival_sigma);
		poisson_distribution<int> P_distribution(lamda);
		outFile << processNumber << endl;
		for (int i = 0; i < processNumber;i++)
		{
			outFile << i + 1 << " ";
			IDs.push_back(i + 1);
			random = N_distribution(generator);
			arrivalTime.push_back(random);
			outFile << setprecision(2) << random << " ";
			random = N_distribution(generator);
			burstTime.push_back(random);
			outFile << setprecision(2) << random << " ";
			p_random = P_distribution(generator);
			priorities.push_back(p_random);
			outFile << setprecision(2) << p_random << endl;
		}*/
		vector <process> vec;
		for (int i = 0;i < processNumber;i++)
		{
			process x;
			x.arrivtime = arrivalTime[i];
			x.bursttime = burstTime[i];
			x.tmpbtime = burstTime[i];
			x.priority = priorities[i];
			x.ID = IDs[i];
			vec.push_back(x);
		}
		//sort(arrivalTime.begin(), arrivalTime.end());
		//vec = sortproc(arrivalTime, vec);
		sort(vec.begin(), vec.end(), sort_arrtime);

		int choice;
		cout << "Enter 1 for HPF , 2 for SRTN, 3 for RR , 4 for Custom RR, -1 to Exit : ";
		cin >> choice;
		double c;
		if (choice != -1)
		{
			cout << "Enter Context Switching : ";
			cin >> c;
		}

		if (choice == 1)
		{
			double t = vec[0].arrivtime;
			vector <process> tmp;
			vector<int>pids;
			vector<double>time;
			vector<double>turnaroundtime;
			vector<double>waitingtime;
			vector<double>wturnaroundtime;
			vector <double> yaxis;
			vector <double> xaxis;
			int lenghth;
			vector<int>tmpIDs;
			//double avgturn;
			//double avgwturn;
			int protemp = processNumber;
			vector<process>vectemp;
			vectemp = vec;
			while (1)
			{
				double ind = 10000;

				for (int i = 0;i < protemp;i++)
				{
					if (t >= vectemp[i].arrivtime)
					{
						tmp.push_back(vectemp[i]);

						if ((i + 1) < protemp)
						{
							ind = vectemp[i + 1].arrivtime;
						}

						vectemp.erase(vectemp.begin() + (i));

						protemp--;
						i--;
					}
				}

				if (tmp.size() != 0)
				{
					process processedprocess;
					processedprocess = getmax(tmp);
					waitingtime.push_back((t - processedprocess.arrivtime));
					turnaroundtime.push_back(processedprocess.bursttime + (t - processedprocess.arrivtime));
					wturnaroundtime.push_back((processedprocess.bursttime + (t - processedprocess.arrivtime)) / processedprocess.bursttime);
					yaxis.push_back(processedprocess.ID);
					xaxis.push_back(t);
					t += processedprocess.bursttime;
					yaxis.push_back(processedprocess.ID);
					xaxis.push_back(t);
					tmpIDs.push_back(processedprocess.ID);
					int b = geteq(tmp, processedprocess);
					tmp.erase(tmp.begin() + b);
					if (tmp.size() != 0 || (vectemp.size() != 0 && ind < t) && c > 0)
					{
						xaxis.push_back(t);
						yaxis.push_back(0);
						t += c;
						xaxis.push_back(t);
						yaxis.push_back(0);
					}
				}
				else
				{
					t++;
				}
				if (vectemp.size() == 0 && tmp.size() == 0)
				{

					break;
				}
				//t++;
			}
			outfile2 << processNumber << endl;

			for (int i = 0;i < processNumber;i++)
			{
				outfile2 << tmpIDs[i] << "\t";
				outfile2 << setprecision(3) << waitingtime[i] << "\t";
				outfile2 << setprecision(3) << turnaroundtime[i] << "\t";
				outfile2 << setprecision(3) << wturnaroundtime[i] << endl;

				//cout << waitingtime[i] << " " << turnaroundtime[i] << " " << wturnaroundtime[i] << endl;
			}
			lenghth = xaxis.size();
			double* p = yaxis.data();
			double* z = xaxis.data();
			myplot(z, p, lenghth);

		}
		if (choice == 2)
		{
			double t = vec[0].arrivtime;
			vector <process> tmp;
			vector<int>pids;
			vector<double>time;
			vector<double>turnaroundtime;
			vector<double>waitingtime;
			vector<double>xaxis;
			vector<double>yaxis;
			vector<double>wturnaroundtime;
			vector<int>tmpIDs;
			int len;
			//double avgturn;
			//double avgwturn;
			int protemp = processNumber;
			//double quant;
			//	cin >> quant;
			vector<process>vectemp;

			vectemp = vec;
			while (1)
			{
				double ind = 10000;
				double lastfinish = 0;
				bool ch = 0;
				double diff = 10000;
				for (int i = 0;i < protemp;i++)
				{
					if (t >= vectemp[i].arrivtime)
					{
						if (tmp.size() == 0)
						{
							tmp.push_back(vectemp[i]);
							if ((i + 1) < protemp)
							{
								diff = vectemp[i + 1].arrivtime;
							}

							vectemp.erase(vectemp.begin() + (i));
							protemp--;
							i--;
							ch = 1;

						}
						else
						{
							if (vectemp[i].bursttime < tmp[0].bursttime)
							{
								tmp.insert(tmp.begin(), vectemp[i]);
								if (c > 0)
								{

									xaxis.push_back(t);
									yaxis.push_back(0);
									t += c;
									xaxis.push_back(t);
									yaxis.push_back(0);
								}
								if ((i + 1) < protemp)
								{
									diff = vectemp[i + 1].arrivtime;
								}

								vectemp.erase(vectemp.begin() + (i));
								protemp--;
								i--;
								ch = 1;
							}
							else
							{
								tmp.push_back(vectemp[i]);
								if ((i + 1) < protemp)
								{
									diff = vectemp[i + 1].arrivtime;
								}

								vectemp.erase(vectemp.begin() + (i));
								protemp--;
								i--;
								ch = 1;
							}
						}
						//break;
					}
				}

				if (tmp.size() != 0)
				{
					if (vectemp.size() != 0)
					{
						xaxis.push_back(t);
						yaxis.push_back(tmp[0].ID);
						tmp[0].bursttime -= 1;
						t += 1;
						xaxis.push_back(t);
						yaxis.push_back(tmp[0].ID);

						for (int i = 0;i < protemp;i++)
						{
							if (t >= vectemp[i].arrivtime)
							{
								if (vectemp[i].bursttime < tmp[0].bursttime)
								{
									if ((i + 1) < protemp)
									{
										diff = vectemp[i + 1].arrivtime;
									}
									tmp.insert(tmp.begin(), vectemp[i]);
									vectemp.erase(vectemp.begin() + (i));

									if (c > 0)
									{
										xaxis.push_back(t);
										yaxis.push_back(0);
										t += c;
										xaxis.push_back(t);
										yaxis.push_back(0);
									}
									protemp--;
									i--;
									ch = 1;
								}
								else
								{
									tmp.push_back(vectemp[i]);
									if ((i + 1) < protemp)
									{
										diff = vectemp[i + 1].arrivtime;
									}
									vectemp.erase(vectemp.begin() + (i));
									protemp--;
									i--;
									ch = 1;
								}
							}
						}
						if (tmp[0].bursttime <= 0)
						{
							if (tmp[0].bursttime < 0)
							{
								tmp[0].finishtime = t + tmp[0].bursttime;
								lastfinish = t + tmp[0].bursttime;;
							}
							else if (tmp[0].bursttime == 0)
							{
								tmp[0].finishtime = t;
								lastfinish = t;
							}

							double tt = tmp[0].finishtime - tmp[0].arrivtime;
							turnaroundtime.push_back(tt);
							wturnaroundtime.push_back(tt / tmp[0].tmpbtime);
							if (tt - tmp[0].tmpbtime < 0)
							{
								waitingtime.push_back(0);
							}
							else
							{
								waitingtime.push_back(tt - tmp[0].tmpbtime);
							}
							tmpIDs.push_back(tmp[0].ID);
							tmp.erase(tmp.begin());
							process nextprocessed = getmin(tmp);
							int b = geteq(tmp, nextprocessed);
							tmp.erase(tmp.begin() + b);
							if ((tmp.size() != 0 || (vectemp.size() != 0 && diff < t)) && c > 0)
							{
								xaxis.push_back(t);
								yaxis.push_back(0);
								t += c;
								xaxis.push_back(t);
								yaxis.push_back(0);
							}
							tmp.insert(tmp.begin(), nextprocessed);

						}
					}
					else if (vectemp.size() == 0) //no change in ready queue so SJF
					{
						process processed = getmin(tmp);
						xaxis.push_back(t);
						yaxis.push_back(processed.ID);
						t += processed.bursttime;
						xaxis.push_back(t);
						yaxis.push_back(processed.ID);
						//waitingtime.push_back((t - processed.arrivtime));
						////turnaroundtime.push_back(processedprocess.bursttime + (t - processedprocess.arrivtime));
						//wturnaroundtime.push_back((processedprocess.bursttime + (t - processedprocess.arrivtime)) / processedprocess.bursttime);
						//t += processedprocess.bursttime;
						tmpIDs.push_back(processed.ID);
						processed.finishtime = t;
						double tt = processed.finishtime - processed.arrivtime;
						turnaroundtime.push_back(tt);
						wturnaroundtime.push_back(tt / processed.tmpbtime);
						if (tt - processed.tmpbtime < 0)
						{
							waitingtime.push_back(0);
						}
						else
						{
							waitingtime.push_back(tt - processed.tmpbtime);
						}
						int b = geteq(tmp, processed);
						tmp.erase(tmp.begin() + b);
						if (tmp.size() != 0 && c > 0)
						{
							xaxis.push_back(t);
							yaxis.push_back(0);
							t += c;
							xaxis.push_back(t);
							yaxis.push_back(0);
						}
					}
				}

				if (vectemp.size() == 0 && tmp.size() == 0)
				{
					break;
				}
			}
			outfile2 << processNumber << endl;
			for (int i = 0;i < processNumber;i++)
			{
				outfile2 << tmpIDs[i] << "\t";
				outfile2 << setprecision(3) << waitingtime[i] << "\t";
				outfile2 << setprecision(3) << turnaroundtime[i] << "\t";
				outfile2 << setprecision(3) << wturnaroundtime[i] << endl;
				//cout << tmpIDs[i] << " " << waitingtime[i] << " " << turnaroundtime[i] << " " << wturnaroundtime[i] << endl;
			}

			len = xaxis.size();
			double *x = xaxis.data();
			double *y = yaxis.data();
			myplot(x, y, len);
		}
		if (choice == 3)
		{
			double t = vec[0].arrivtime;
			vector <process> tmp;
			vector<int>pids;
			int len;
			vector<double>time;
			vector<double>turnaroundtime;
			vector<double>waitingtime;
			vector <double>xaxis;
			vector <double> yaxis;
			vector<double>wturnaroundtime;
			vector<int>tmpIDs;
			//double avgturn;
			//double avgwturn;
			int protemp = processNumber;
			double quant;
			cout << "Enter Quantum: ";
			cin >> quant;
			vector<process>vectemp;
			vectemp = vec;
			//vector <double>tmpbtime;
			while (1)
			{
				double ind = 10000;
				for (int i = 0;i < protemp;i++)
				{
					if (t >= vectemp[i].arrivtime)
					{
						tmp.push_back(vectemp[i]);
						if ((i + 1) < protemp)
						{
							ind = vectemp[i + 1].arrivtime;
						}
						vectemp.erase(vectemp.begin() + (i));
						protemp--;
						i--;
					}
				}
				if (tmp.size() != 0)
				{
					if (tmp[0].bursttime > quant)
					{
						xaxis.push_back(t);
						yaxis.push_back(tmp[0].ID);
						tmp[0].bursttime -= quant;
						t += quant;
						xaxis.push_back(t);
						yaxis.push_back(tmp[0].ID);
						for (int i = 0;i < protemp;i++)
						{
							if (t >= vectemp[i].arrivtime)
							{
								tmp.push_back(vectemp[i]);
								if ((i + 1) < protemp)
								{
									ind = vectemp[i + 1].arrivtime;
								}
								vectemp.erase(vectemp.begin() + (i));
								protemp--;
								i--;
							}
						}
						tmp.push_back(tmp[0]);
						tmp.erase(tmp.begin());
						if ((tmp.size() != 0 || (vectemp.size() != 0 && ind < t)) && c > 0)
						{
							xaxis.push_back(t);
							yaxis.push_back(0);
							t += c;
							xaxis.push_back(t);
							yaxis.push_back(0);
						}
					}
					else if (tmp[0].bursttime < quant)
					{
						//tmp[0].bursttime -= quant;
						xaxis.push_back(t);
						yaxis.push_back(tmp[0].ID);
						t += tmp[0].bursttime;
						xaxis.push_back(t);
						yaxis.push_back(tmp[0].ID);
						tmp[0].finishtime = t;
						double tt = tmp[0].finishtime - tmp[0].arrivtime;
						turnaroundtime.push_back(tt);
						wturnaroundtime.push_back(tt / tmp[0].tmpbtime);
						waitingtime.push_back(tt - tmp[0].tmpbtime);
						//tmp.push_back(tmp[0]);
						tmpIDs.push_back(tmp[0].ID);
						tmp.erase(tmp.begin());
						if ((tmp.size() != 0 || (vectemp.size() != 0 && ind < t)) && c > 0)
						{
							xaxis.push_back(t);
							yaxis.push_back(0);
							t += c;
							xaxis.push_back(t);
							yaxis.push_back(0);
						}
					}
					else if (tmp[0].bursttime == quant)
					{
						//tmp[0].bursttime -= quant;
						xaxis.push_back(t);
						yaxis.push_back(tmp[0].ID);
						t += quant;
						xaxis.push_back(t);
						yaxis.push_back(tmp[0].ID);
						tmp[0].finishtime = t;
						//tmp.push_back(tmp[0]);
						double tt = tmp[0].finishtime - tmp[0].arrivtime;
						turnaroundtime.push_back(tt);
						wturnaroundtime.push_back(tt / tmp[0].tmpbtime);
						waitingtime.push_back(tt - tmp[0].tmpbtime);
						tmpIDs.push_back(tmp[0].ID);
						tmp.erase(tmp.begin());
						if ((tmp.size() != 0 || (vectemp.size() != 0 && ind < t)) && c > 0)
						{
							xaxis.push_back(t);
							yaxis.push_back(0);
							t += c;
							xaxis.push_back(t);
							yaxis.push_back(0);
						}
					}
				}
				else
				{
					t++;
				}
				if (vectemp.size() == 0 && tmp.size() == 0)
				{
					break;
				}
			}
			outfile2 << processNumber << endl;

			for (int i = 0;i < processNumber;i++)
			{
				outfile2 << tmpIDs[i] << "\t";
				outfile2 << setprecision(3) << waitingtime[i] << "\t";
				outfile2 << setprecision(3) << turnaroundtime[i] << "\t";
				outfile2 << setprecision(3) << wturnaroundtime[i] << endl;
				//cout << tmpIDs[i] << " " << turnaroundtime[i] << " " << waitingtime[i] << " " << wturnaroundtime[i] << endl;
			}
			double *p = yaxis.data();
			double *z = xaxis.data();
			len = xaxis.size();
			myplot(z, p, len);
		}
		else if (choice == 4)
		{
			double t = vec[0].arrivtime;
			vector <process> tmp;
			vector<int>pids;
			vector<double>time;
			int len;
			vector<double>turnaroundtime;
			vector<double>waitingtime;
			vector <double>xaxis;
			vector <double> yaxis;
			vector<double>wturnaroundtime;
			vector<int>tmpIDs;
			double avgturn;
			double avgwturn;
			int protemp = processNumber;
			double quant;
			cout << "Enter Quantam: ";
			cin >> quant;
			vector<process>vectemp;
			vectemp = vec;
			for (int i = 0;i < vectemp.size();i++)
			{
				vectemp[i].quantc = quant*vectemp[i].priority/3;
			}
			//vector <double>tmpbtime;
			while (1) //the algorithm goes as following check if the arriving process has higher priority if so place as to be processed
					  // also checks the queue after processing the current process acting as round robin with quantum proprotional to priority 
			{
				double ind = 10000;
				for (int i = 0;i < protemp;i++)
				{
					if (t >= vectemp[i].arrivtime)
					{
						if (tmp.size() == 0 || (vectemp[i].priority < tmp[0].priority))
						{
							tmp.push_back(vectemp[i]);

							if ((i + 1) < protemp)
							{
								ind = vectemp[i + 1].arrivtime;
							}
							vectemp.erase(vectemp.begin() + (i));
							protemp--;
							i--;
						}
						else
						{
							if (vectemp[i].priority > tmp[0].priority)
							{
								tmp.insert(tmp.begin(), vectemp[i]);
								if (c > 0)
								{
									xaxis.push_back(t);
									yaxis.push_back(0);
									t += c;
									xaxis.push_back(t);
									yaxis.push_back(0);
								}
								if ((i + 1) < protemp)
								{
									ind = vectemp[i + 1].arrivtime;
								}
								vectemp.erase(vectemp.begin() + (i));
								protemp--;
								i--;
							}
						}
					}
				}
				if (tmp.size() != 0 && vectemp.size() != 0)
				{
					double z = tmp[0].quantc;
					bool test = 0;
					while (z > 0)
					{
						if (z <= 1)
						{
							xaxis.push_back(t);
							yaxis.push_back(tmp[0].ID);
							tmp[0].bursttime -= z;
							t += z;
							xaxis.push_back(t);
							yaxis.push_back(tmp[0].ID);
							z -= z;
							if (tmp[0].bursttime > 0)
							{
								process tempor = tmp[0];
								tmp.erase(tmp.begin());
								//process nx = getmax(tmp);
								//int b = geteq(tmp, nx);
								//tmp.erase(tmp.begin() + b);
								tmp.push_back(tempor);
								if ((tmp.size() > 1 || ind<t) && c>0)
								{
									xaxis.push_back(t);
									yaxis.push_back(0);
									t += c;
									xaxis.push_back(t);
									yaxis.push_back(0);
								}
							}
						}
						else
						{
							xaxis.push_back(t);
							yaxis.push_back(tmp[0].ID);
							tmp[0].bursttime--;
							t++;

							z--;
						}
						if (tmp[0].bursttime <= 0)
						{
							if (tmp[0].bursttime == 0)
							{
								tmp[0].finishtime = t;
								xaxis.push_back(t);
								yaxis.push_back(tmp[0].ID);
							}
							else if (tmp[0].bursttime < 0)
							{
								tmp[0].finishtime = t + tmp[0].bursttime;
								xaxis.push_back(t);
								yaxis.push_back(tmp[0].ID);
							}
							double tt = tmp[0].finishtime - tmp[0].arrivtime;
							turnaroundtime.push_back(tt);
							wturnaroundtime.push_back(tt / tmp[0].arrivtime);
							waitingtime.push_back(tt - tmp[0].tmpbtime);
							tmpIDs.push_back(tmp[0].ID);
							tmp.erase(tmp.begin());
							if ((tmp.size() > 0 || ind<t) && c>0)
							{
								xaxis.push_back(t);
								yaxis.push_back(0);
								t += c;
								xaxis.push_back(t);
								yaxis.push_back(0);
							}
							/*	if (tmp.size() != 0)
							{
							process next = getmax(tmp);
							int b = geteq(tmp, next);
							tmp.erase(tmp.begin() + b);
							tmp.insert(tmp.begin(),next);
							}*/
							break;
						}
						else
						{
							xaxis.push_back(t);
							yaxis.push_back(tmp[0].ID);
						}
						for (int i = 0;i < protemp;i++)
						{
							if (t >= vectemp[i].arrivtime)
							{
								if (tmp.size() == 0 || (vectemp[i].priority < tmp[0].priority))
								{
									tmp.push_back(vectemp[i]);

									if ((i + 1) < protemp)
									{
										ind = vectemp[i + 1].arrivtime;
									}
									vectemp.erase(vectemp.begin() + (i));
									protemp--;
									i--;
								}
								else
								{
									if (vectemp[i].priority > tmp[0].priority)
									{
										tmp.insert(tmp.begin(), vectemp[i]);
										if (c > 0)
										{
											xaxis.push_back(t);
											yaxis.push_back(0);
											t += c;
											xaxis.push_back(t);
											yaxis.push_back(0);
										}
										test = 1;
										if ((i + 1) < protemp)
										{
											ind = vectemp[i + 1].arrivtime;
										}
										vectemp.erase(vectemp.begin() + (i));
										protemp--;
										i--;
										break;
									}
								}
							}
						}
						if (test == 1)
						{
							break;
						}
					}

				}
				else if (vectemp.size() == 0 && tmp.size() != 0)
				{
					//process processed = getmax(tmp);
					if (tmp[0].bursttime == tmp[0].quantc)
					{
						xaxis.push_back(t);
						yaxis.push_back(tmp[0].ID);
						t += tmp[0].quantc;
						xaxis.push_back(t);
						yaxis.push_back(tmp[0].ID);
						tmp[0].finishtime = t;
						double tt = tmp[0].finishtime - tmp[0].arrivtime;
						turnaroundtime.push_back(tt);
						wturnaroundtime.push_back(tt / tmp[0].tmpbtime);
						waitingtime.push_back(tt - tmp[0].tmpbtime);
						tmpIDs.push_back(tmp[0].ID);
						tmp.erase(tmp.begin());
						if (tmp.size() != 0 && c>0)
						{
							xaxis.push_back(t);
							yaxis.push_back(0);
							t += c;
							xaxis.push_back(t);
							yaxis.push_back(0);
						}
					}
					if (tmp.size() != 0)
					{
						if (tmp[0].bursttime < tmp[0].quantc)
						{
							xaxis.push_back(t);
							yaxis.push_back(tmp[0].ID);
							t += tmp[0].bursttime;
							xaxis.push_back(t);
							yaxis.push_back(tmp[0].ID);
							tmp[0].finishtime = t;
							double tt = tmp[0].finishtime - tmp[0].arrivtime;
							turnaroundtime.push_back(tt);
							wturnaroundtime.push_back(tt / tmp[0].tmpbtime);
							waitingtime.push_back(tt - tmp[0].tmpbtime);
							tmpIDs.push_back(tmp[0].ID);
							tmp.erase(tmp.begin());
							if (tmp.size() != 0 && c>0)
							{
								xaxis.push_back(t);
								yaxis.push_back(0);
								t += c;
								xaxis.push_back(t);
								yaxis.push_back(0);
							}
						}
					}
					if (tmp.size() != 0)
					{
						if (tmp[0].bursttime > tmp[0].quantc)
						{
							xaxis.push_back(t);
							yaxis.push_back(tmp[0].ID);
							t += tmp[0].quantc;
							tmp[0].bursttime -= tmp[0].quantc;
							xaxis.push_back(t);
							yaxis.push_back(tmp[0].ID);
							tmp[0].tmpbtime -= tmp[0].quantc;
							//tmp[0].finishtime = t;
							//double tt = tmp[0].finishtime - tmp[0].arrivtime;
							//turnaroundtime.push_back(tt);
							//wturnaroundtime.push_back(tt / tmp[0].tmpbtime);
							//waitingtime.push_back(tt - tmp[0].tmpbtime);
							process qq = tmp[0];
							tmp.erase(tmp.begin());
							tmp.push_back(qq);
							if (tmp.size() > 1 && c>0)
							{
								xaxis.push_back(t);
								yaxis.push_back(0);
								t += c;
								xaxis.push_back(t);
								yaxis.push_back(0);
							}
						}
					}
				}
				else
				{
					t++;
				}
				if (vectemp.size() == 0 && tmp.size() == 0)
				{
					break;
				}
			}
			outfile2 << processNumber << endl;

			for (int i = 0;i < processNumber;i++)
			{
				outfile2 << tmpIDs[i] << "\t";
				outfile2 << setprecision(3) << waitingtime[i] << "\t";
				outfile2 << setprecision(3) << turnaroundtime[i] << "\t";
				outfile2 << setprecision(3) << wturnaroundtime[i] << endl;
				//cout << tmpIDs[i] << " " << turnaroundtime[i] << " " << waitingtime[i] << " " << wturnaroundtime[i] << endl;
			}
			double *p = yaxis.data();
			double *z = xaxis.data();
			len = xaxis.size();
			myplot(z, p, len);
		}

		else if (choice == -1)
		{
			break;
		}
	}
	return 0;
}