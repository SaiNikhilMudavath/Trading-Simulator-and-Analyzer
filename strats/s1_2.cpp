#include<iostream>
#include<fstream>
#include<vector>
#include<string>
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

vector<double> std_dev(vector<double> values)
{
    double sum=0.0;
    double squares_sum=0;
    for(int i=0;i<values.size();i++)
    {
        sum+=values[i];
        squares_sum+=(values[i]*values[i]);
    }

    double mean=sum/values.size();
    squares_sum=squares_sum/values.size();

    double var=squares_sum-(mean*mean);
    vector<double> result;

    result.push_back(mean);
    result.push_back(std::sqrt(var));
    return result;
}

void dma(double p)
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
    std::ofstream file4("order_statistics.csv");
    
    if (!file2.is_open())
    {
        std::cerr<<"failed to open the file2. "<<endl;
    }
    file2<<"Date,Cashflow"<<endl;

    if (!file4.is_open())
    {
        std::cerr<<"failed to open the file4. "<<endl;
    }
    file4<<"Date,Order_dir,Quantity,Price"<<endl;

    double final_closing_price;double running_sum=0;double x_counter=0;
    //cout<<"n is:"<<n<<endl;

    for (int i=0;i<=values.size()-n;i++)
    {
        vector<double> n_values(values.begin()+i,values.begin()+i+n);
        
        //clear
        // for(int i=0;i<n_values.size();i++)
        // {
        //     cout<<n_values[i]<<" ";
        // }
        // cout<<"temp[i+n-1] value"<<temp[i+n-1].substr(0,10)<<endl;
        //clear

        //cout<<"n_values size:"<<n_values.size()<<endl;
        vector<double> mean_std_dev=std_dev(n_values);
        string closing_value=",";
        // cout<<"mean is :"<<mean_std_dev[0]<<endl;
        // cout<<"sd is: "<<mean_std_dev[1]<<endl;
    
        final_closing_price=values[i+n-1];
        
        if((final_closing_price-mean_std_dev[0]>=(p*mean_std_dev[1]))&&(x_counter<x))
        {
            //buy
            //cout<<"buying "<<final_closing_price<<endl;
            running_sum+=(final_closing_price*(-1));
            x_counter++;
            file4<<temp[i+n-1].substr(0,11)<<"BUY,1,"<<final_closing_price<<endl;
        }
        else if((final_closing_price+p*mean_std_dev[1]<=mean_std_dev[0]) &&(x_counter>(-1*x)))
        {
            //sell
            //cout<<"selling "<<final_closing_price<<endl;
            running_sum+=(final_closing_price);
            x_counter--;
            file4<<temp[i+n-1].substr(0,11)<<"SELL,1,"<<final_closing_price<<endl;
        }
        
        file2<<temp[i+n-1].substr(0,10);
        file2<<","<<static_cast<double>(running_sum)<<endl;
        
        if(i==values.size()-n)
        {
            std::ofstream file3("final_pnl.txt");
            if (!file3.is_open())
            {
                std::cerr<<"failed to open the file3."<<endl;
            }
            double a=running_sum+(x_counter*final_closing_price);
            // cout<<"running sum "<<running_sum<<endl;
            // cout<<"x_counter"<<x_counter<<endl;
            // cout<<"finla closing price "<<final_closing_price<<endl;
            // cout<<a<<endl;
            file3<<static_cast<double>(a);
        }
    }
    remove("temp1.csv");
}
int main(int argc, char *argv[])
{
    dma(stod(argv[1]));
}