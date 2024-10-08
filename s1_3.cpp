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
double sf(double c1,double c2,int i,double er)
{
    if(i==0)
    {
        return 0.5;
    }
    else
    {
        double sft_1=sf(c2,c2,i-1,er);
        double var1=(2*er)/(1+c2);double var2=(var1-1)/(var1+1);
        return sft_1+(c1*(var2-sft_1));
    }
}
double ama(vector<double> closing_price,int i,double c1,double c2,double er)
{
    if(i==0)
    {
        return closing_price[0];
    }
    else
    {
        double sf_t=sf(c1,c2,i,er);
        double closing_price_t=closing_price.back();
        closing_price.pop_back();

        double amat_minus_1=ama(closing_price,i-1,c1,c2,er);
        return amat_minus_1+sf_t*(closing_price_t-amat_minus_1);

    }
}
bool calc_hold_days(vector<double>& max_hold_vector)
{
    bool is=false;
    
    for(int i=0;i<max_hold_vector.size();i++)
    {
        max_hold_vector[i]=max_hold_vector[i]-1;
    }
    
    if((max_hold_vector.size()!=0))
    {
        if(max_hold_vector[0]==0)
        {
            max_hold_vector.erase(max_hold_vector.begin());
            return true;
        }
    }
    return false;
}
void s3(double p,double max_hold_days,double c1,double c2)
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
    file2<<"Date,Cashflow"<<endl;

    std::ofstream file3("order_statistics.csv");
    file3<<"Date,Order_dir,Quantity,Price"<<endl;

    double running_sum=0;double x_counter=0;
    vector<double> buy_days;vector<double> sell_days;
    
    for (int i=0;i<=values.size()-n-1;i++)
    {
        //to buy and sell:

        vector<double> n_values(values.begin()+i,values.begin()+i+n+1);

        double change_over_n_days=std::abs(n_values[n_values.size()-1]-n_values[0]);
        double sum_of_abs_price=0;
        for(int j=0;j<n_values.size()-1;j++)
        {
            sum_of_abs_price+=std::abs(n_values[j+1]-n_values[j]);
        }
        if(sum_of_abs_price==0)
        {
            continue; //adjust accordingly
        }

        double efficieny_ratio=change_over_n_days/sum_of_abs_price;
        double ama_t=ama(n_values,n_values.size()-1,c1,c2,efficieny_ratio);

        double var1=ama_t*(1+(p/100));
        
        if((n_values[n_values.size()-1]>=var1)&&(x_counter<x))
        {
            //buy
            file2<<temp[i+n].substr(0,11);
            file2<<(running_sum-n_values[n_values.size()-1])<<endl;
            file3<<temp[i+n].substr(0,11)<<"BUY,1,"<<n_values[n_values.size()-1]<<endl;
            running_sum-=n_values[n_values.size()-1];
            x_counter++;
        }
        double var2=ama_t*(1-(p/100));
        if((n_values[n_values.size()-1]<=var2)&&(x_counter>(-1.0)*x))
        {
            //sell
            file2<<temp[i+n].substr(0,11);
            file2<<(running_sum+n_values[n_values.size()-1])<<endl;
            file3<<temp[i+n].substr(0,11)<<"SELL,1,"<<n_values[n_values.size()-1]<<endl;
            running_sum+=n_values[n_values.size()-1];
            x_counter--;
        }

        if(i==values.size()-n-1)
        {
            //write into final pnl
            std::ofstream file4("final_pnl.txt");
            double a=running_sum+(x_counter*n_values[n_values.size()-1]);
            file4<<static_cast<double>(a);
        }
    }
}
int main(int argc,char* argv[])
{
    s3(stod(argv[1]),stod(argv[2]),stod(argv[3]),stod(argv[4]));
}