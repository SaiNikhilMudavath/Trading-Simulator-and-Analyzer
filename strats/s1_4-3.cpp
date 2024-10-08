#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cmath>
using namespace std;

vector<double> strtodouble(vector<string> values)
{
    vector<double> result;
    for (int i=0;i<values.size();i++)
    {
        bool is=false;string x="";
        for(int j=0;j<values[i].size();j++)
        {
            if(is)
            {
                x=x+values[i][j];
            }
            if(values[i][j]==',')
            {
                is=true;
            }
        }
        
        result.push_back(stod(x));
    }
    return result;
}
vector<double> ewm(vector<double> values,double alpha)
{
    vector<double> ewm_values;
    ewm_values.push_back(values[0]);
    for(int i=1;i<values.size();i++)
    {
        ewm_values.push_back(alpha*(values[i]-ewm_values.back())+ewm_values.back());
    }
    return ewm_values;
}
void s4_3(double adx_threshold)
{
    std::ifstream file1("high.csv");
    std::ifstream file2("low.csv");
    std::ifstream file3("prev_close.csv");
    std::ifstream file4("temp1.csv");
    string line1;string line2;string line3;string line4;
    vector<string> high_strings;vector<string> low_strings;vector<string> prev_close_strings;
    vector<string> temp;
    while(getline(file1,line1))
    {
        high_strings.push_back(line1);
    }
    file1.close();
    
    while(getline(file2,line2))
    {
        low_strings.push_back(line2);
    }
    file2.close();
    
    while(getline(file3,line3))
    {
        prev_close_strings.push_back(line3);
    }
    file3.close();
    
    while(getline(file4,line4))
    {
        temp.push_back(line4);
    }
    file4.close();

    vector<double> high_values=strtodouble(high_strings);
    vector<double> low_values=strtodouble(low_strings);
    vector<double> prev_close_values=strtodouble(prev_close_strings);
    vector<double> values=strtodouble(temp);

    double x=values.back();values.pop_back();double n=values.back();values.pop_back();
    double alpha=(2.0)/(n+1.0);
    double x_counter=0;double running_sum=0;

    vector<double> tr;
    
    for(int i=1;i<high_strings.size();i++)
    {
        double a=(high_values[i]-low_values[i]);
        double b=(high_values[i]-prev_close_values[i]);
        double c=(low_values[i]-prev_close_values[i]);
        tr.push_back(max(max(a,b),c));
    }
   
    vector<double> atr=ewm(tr,alpha);
    vector<double> modified_atr(atr.begin(),atr.begin()+atr.size());

    //dmplus and dmminus have one index less than the main 3 vectors
    //MAIN 3 vectors have n+1 size while these have n size
    vector<double> dmplus;vector<double> dmminus;
    for(int i=0;i<high_values.size()-1;i++)
    {
        double d=high_values[i+1]-high_values[i];
        dmplus.push_back(max(0.0,d));
        double e=low_values[i]-low_values[i+1];
        dmminus.push_back(max(0.0,e));
    }


    vector<double> diplus;vector<double> diminus;
    for(int i=0;i<dmplus.size();i++)
    {
        diplus.push_back(dmplus[i]/modified_atr[i]);
        diminus.push_back(dmminus[i]/modified_atr[i]);
    }
    diplus=ewm(diplus,alpha);
    diminus=ewm(diminus,alpha);

    vector<double> dx;vector<double> excludes;
    for(int i=0;i<dmplus.size();i++)
    {
        double var1=diplus[i]-diminus[i];
        double var2=diplus[i]+diminus[i];
        if(var2!=0)
        {
            dx.push_back((var1/var2)*100.0);
        }
        else
        {
            dx.push_back(0);
        } 
    }

    vector<double> adx=ewm(dx,alpha);

    std::ofstream file5("daily_cashflow.csv");
    std::ofstream file6("order_statistics.csv");
    file5<<"Date,Cashflow"<<endl;
    file6<<"Date,Order_dir,Quantity,Price"<<endl;


    for(int i=0;i<adx.size();i++)
    {
        
        if((adx[i]>adx_threshold)&&(x_counter<x))
        {
            //buy
            
            running_sum+=(values[i+1]*(-1.0));
            file5<<high_strings[i+1].substr(0,10)<<",";
            file5<<running_sum<<endl;
            x_counter++;
            file6<<high_strings[i+1].substr(0,11)<<"BUY,1,"<<values[i+1]<<endl;
        }
        else if((adx[i]<adx_threshold)&&(x_counter>(-1.0)*x))
        {
            //sell
            
            running_sum+=(values[i+1]);
            file5<<high_strings[i+1].substr(0,10)<<",";
            file5<<running_sum<<endl;
            x_counter--;
            file6<<high_strings[i+1].substr(0,11)<<"SELL,1,"<<values[i+1]<<endl;
        }
        else
        {
            file5<<high_strings[i+1].substr(0,10)<<","<<running_sum<<endl;
        }
        if(i==adx.size()-1)
        {
            std::ofstream file6("final_pnl.txt");
            double a=running_sum+(x_counter*values[i+1]);
            file6<<a<<endl;
        }
    }
    remove("temp1.csv");
    remove("low.csv");
    remove("high.csv");
    remove("prev_close.csv");
}
int main(int argc, char *argv[])
{
    s4_3(stod(argv[1]));
}