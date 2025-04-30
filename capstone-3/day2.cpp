#include<iostream>
#include<fstream>
#include<sstream>
#include<thread>
#include<chrono>
using namespace std;

struct CPUData{ 
long user , nice , system , idle , iowait , irq ,softirq , steal , guest , guest_nice;
};
CPUData getCPUData(){
ifstream file("/proc/stat");
string line;
CPUData cpu={}; //initialize struct with zero value 
if(file.is_open()){
getline(file,line); //Read first line 
istringstream ss(line);
string cpuLabel;
ss>>cpuLabel>>cpu.user>>cpu.nice>>cpu.system>>cpu.idle>>cpu.iowait>>cpu.irq>>cpu.softirq>>cpu.steal>>cpu.guest>>cpu.guest_nice;

}
return cpu;
}
int main(){
CPUData cpu = getCPUData();
cout<<"User Time :" <<cpu.user<<"\n";
cout<<"Nice Time :" <<cpu.nice<<"\n";
cout<<"System Time :" <<cpu.system<<"\n";
cout<<"Idle Time :" <<cpu.idle<<"\n";
cout<<"iowait Time :" <<cpu.iowait<<"\n";
cout<<"irq Time :" <<cpu.irq<<"\n";
cout<<"softirq Time :" <<cpu.softirq<<"\n";
cout<<"steal Time :" <<cpu.steal<<"\n";
cout<<"guest Time :" <<cpu.guest<<"\n";
cout<<"guest_nice Time :" <<cpu.guest_nice<<"\n";
int idle = cpu.idle + cpu.iowait;
cout<<"idle_time:"<<idle<<"\n";
long totalCPUTime=cpu.user + cpu.nice + cpu.system + cpu.idle + cpu.iowait + cpu.irq + cpu.softirq + cpu.steal + cpu.guest + cpu.guest_nice;
cout<<"Total_CPU_Time:"<<totalCPUTime<<"\n";
int ActiveTime = totalCPUTime - idle;
cout<<"Active_Time :"<<ActiveTime<<"\n";

int CPU_Usage = ((totalCPUTime - idle) / totalCPUTime)*100;
cout<<"CPU_Usage"<<CPU_Usage <<"\n";
}

