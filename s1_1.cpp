#include<iostream>
#include<fstream>
#include<string>
#include<vector>
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

void basic()
{
    std::ifstream file("temp1.csv");
    if (!file.is_open())
    {
        std:cerr<<"failed to open the file. "<<endl;
    }
    string line;
    vector<string> temp;
    while(getline(file,line))
    {
        temp.push_back(line);
    }
    file.close();
    vector<double> values=strtodouble(temp);

    int n=values[values.size()-2];
    int x=values[values.size()-1];

    values.pop_back();values.pop_back();

    std::ofstream file2("daily_cashflow.csv");
    if (!file2.is_open())
    {
        std::cerr<<"failed to open the file2. "<<endl;
    }
    file2<<"Date,Cashflow"<<endl;

    std::ofstream file4("order_statistics.csv");
    if (!file4.is_open())
    {
        std::cerr<<"failed to open the file3. "<<endl;
    }
    file4<<"Date,Order_dir,Quantity,Price"<<endl;

    double running_sum=0;double x_counter=0;double final_closing_price;

    for (int i=0;i<=values.size()-n-1;i++)
    {
        vector<double> n_values(values.begin()+i,values.begin()+i+n+1);
        
        bool str_inc=true;bool str_dec=true;
    
        for (int k=0;k<n_values.size()-1;k++)
        {
            if(n_values[k+1]<=n_values[k])
            {
                str_inc=false;
                break;
            }
        }
        for (int k=0;k<n_values.size()-1;k++)
        {
            if(n_values[k+1]>=n_values[k])
            {
                str_dec=false;
                break;
            }
        }
        string closing_value=",";
        for(int k=0;k<10;k++)
        {
            file2<<temp[i+n][k];
        }
        for(int k=11;k<temp[i+n].length();k++)
        {
            closing_value+=temp[i+n][k];
        }
        vector<double> int_closing_value=strtodouble({closing_value});
        
        final_closing_price=int_closing_value[0];
        double minus_1=-1;
        if((str_inc)&&(x_counter<x))
        {
            //buy
            running_sum+=((int_closing_value[0])*(minus_1));
            //running_sum+=(n_values[n_values.size()-1])*(-1.0);
            x_counter++;
            file4<<temp[i+n].substr(0,11)<<"BUY,1,"<<values[i+n]<<endl;
        }
        else if((str_dec)&&(x_counter>(minus_1)*x))
        {
            //sell
            running_sum+=(int_closing_value[0]);
            //running_sum+=n_values[n_values.size()-1];
            x_counter--;
            file4<<temp[i+n].substr(0,11)<<"SELL,1,"<<values[i+n]<<endl;
        }
        file2<<","<<static_cast<double>(running_sum)<<endl;
        if(i==values.size()-n-1)
        {
            std::ofstream file3("final_pnl.txt");
            file3<<static_cast<double>(running_sum+(x_counter*final_closing_price));
        }
    }
    remove("temp1.csv");
}

int main()
{
    // cout<<"dsjfhsj"<<endl;
    basic();
}