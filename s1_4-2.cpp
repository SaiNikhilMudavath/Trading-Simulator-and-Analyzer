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

void s4_2(double overbought_threshold,double oversold_threshold)
{
    if(overbought_threshold>=oversold_threshold)
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
        // cout<<"n,x is:"<<n<<" "<<x<<endl;

        values.pop_back();values.pop_back();

        std::ofstream file2("daily_cashflow.csv");
        file2 << "Date,Cashflow"<<endl;
        std::ofstream file3("order_statistics.csv");
        file3<<"Date,Order_dir,Quantity,Price"<<endl;
        if(!file3.is_open())
        {
            std::cerr<<"file3 not open"<<endl;
        }

        double running_sum=0;double x_counter=0;double final_closing_price;

        for (int i=0;i<=values.size()-n-1;i++)
        {
            vector<double> n_values(values.begin()+i,values.begin()+i+n+1);
            double gain=0;double loss=0;
            double gain_counter=0;double loss_counter=0;
            for(int j=0;j<n_values.size()-1;j++)
            {
                double t=std::max(n_values[j+1]-n_values[j],0.0);
                gain+=t;
                if(t!=0.0)
                {
                    gain_counter++;
                }
            }
            for(int j=1;j<n_values.size();j++)
            {
                double t=std::max(n_values[j-1]-n_values[j],0.0);
                loss+=t;
                if(t!=0.0)
                {
                    loss_counter++;
                }
            }
            // gain=gain/gain_counter;
            // loss=loss/loss_counter;
            // cout<<"avg gain is "<<gain<<endl;
            // cout<<"avg loss is "<<loss<<endl;

            double relative_strength=gain/loss;
            double rsi=100-(100/(1+relative_strength));

            if((rsi<oversold_threshold)&&(x_counter<x))
            {
                //buy
                x_counter++;
                running_sum+=(n_values[n_values.size()-1]*(-1.0));
                file3<<temp[i+n].substr(0,11)<<"BUY,1,"<<n_values[n_values.size()-1]<<endl;
            }
            else if((rsi>overbought_threshold)&&(x_counter>(-1)*x))
            {
                //sell
                x_counter--;
                running_sum+=n_values[n_values.size()-1];
                file3<<temp[i+n].substr(0,11)<<"SELL,1,"<<n_values[n_values.size()-1]<<endl;
            }
            file2<<temp[i+n].substr(0,10);
            file2<<",";
            file2<<running_sum<<endl;
            if(i==values.size()-n-1)
            {
                double a=running_sum+(x_counter*values[values.size()-1]);
                std::ofstream file4("final_pnl.txt");
                file4<<a<<endl;
            }
        }
    }
    remove("temp1.csv");
}
int main(int argc, char *argv[])
{
    s4_2(stod(argv[1]),stod(argv[2]));
}