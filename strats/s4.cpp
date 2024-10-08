#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include<bits/stdc++.h>
using namespace std;
// files are temp1 and temp2;
// i think we require past n-1 days for this.

vector<double> strtodouble(vector<string> values)
{
    vector<double> result;
    for (int i = 0; i < values.size(); i++)
    {
        bool is = false;
        string x = "";
        for (int j = 0; j < values[i].size(); j++)
        {
            if (is)
            {
                x = x + values[i][j];
            }
            if (values[i][j] == ',')
            {
                is = true;
            }
        }
        result.push_back(stod(x));
    }
    return result;
}

vector<double> cal_mean_std_dev(vector<double> values)
{
    double sum = 0.0;
    double squares_sum = 0;
    for (int i = 0; i < values.size(); i++)
    {
        sum += values[i];
        squares_sum += (values[i] * values[i]);
    }

    double mean = sum / values.size();
    squares_sum = squares_sum / values.size();

    double var = squares_sum - (mean * mean);
    vector<double> result;

    result.push_back(mean);
    result.push_back(std::sqrt(var));
    return result;
}

void mrt(double threshold)
{
    // reading values1
    std::ifstream file1("temp1.csv");
    if (!file1.is_open())
    {
    std:
        cerr << "failed to open the file1. " << endl;
    }
    string line;
    vector<string> temp1;
    while (getline(file1, line))
    {
        temp1.push_back(line);
    }
    file1.close();

    // reading values2
    std::ifstream file2("temp2.csv");
    if (!file2.is_open())
    {
        std::cerr << "failed to open the file2. " << endl;
    }
    string line2;
    vector<string> temp2;
    while (getline(file2, line2))
    {
        temp2.push_back(line2);
    }
    file2.close();

    vector<double> values1 = strtodouble(temp1);
    vector<double> values2 = strtodouble(temp2);
    int x1 = values1.back();
    values1.pop_back();
    int n1 = values1.back();
    values1.pop_back();
    int x2 = values2.back();
    values2.pop_back();
    int n2 = values2.back();
    values2.pop_back();
    double x1_counter = 0;
    double x2_counter = 0;
    double running_sum_1 = 0;
    std::ofstream file3("daily_cashflow.csv");
    file3 << "Date,Cashflow" << endl;
    std::ofstream file4("order_statistics_1.csv");
    file4 << "Date,Order_dir,Quantity,Price" << endl;
    std::ofstream file5("order_statistics_2.csv");
    file5 << "Date,Order_dir,Quantity,Price" << endl;
    std::ofstream file6("final_pnl.txt");

    vector<double> spread;
    for (int i = 0; i < values1.size(); i++)
    {
        spread.push_back(values1[i] - values2[i]);
    }

    for (int i = 0; i <= spread.size() - n1; i++) // because n1 and n2 are same.
    {
        vector<double> n_spread(spread.begin() + i, spread.begin() + i + n1);

        vector<double> a = cal_mean_std_dev(n_spread);
        double r_mean = a[0];
        double r_sd = a[1];
        double z_score = ((spread[i + n1 - 1] - r_mean) / r_sd);

        if ((z_score > threshold) && (x1_counter > (-1.0) * x1) && (x2_counter < x2))
        {
            // sell s1 and buy s2
            running_sum_1 += (values1[i + n1 - 1]);
            running_sum_1 += ((-1.0) * (values2[i + n2 - 1]));
            x1_counter--;
            x2_counter++;
            file4 << temp1[i + n1 - 1].substr(0, 11) << "SELL,1," << values1[i + n1 - 1] << endl;
            file5 << temp2[i + n2 - 1].substr(0, 11) << "BUY,1," << values2[i + n2 - 1] << endl;
        }
        else if (z_score < ((-1.0) * threshold) && (x1_counter < x1) && (x2_counter > (-1.0) * x2))
        {
            // buy s1 and sell s2
            running_sum_1 += ((-1.0) * (values1[i + n1 - 1]));
            running_sum_1 += (values2[i + n2 - 1]);
            x1_counter++;
            x2_counter--;
            file4 << temp1[i + n1 - 1].substr(0, 11) << "BUY,1," << values1[i + n1 - 1] << endl;
            file5 << temp2[i + n2 - 1].substr(0, 11) << "SELL,1," << values2[i + n2 - 1] << endl;
        }
        file3 << temp1[i + n1 - 1].substr(0, 11) <<fixed<<setprecision(10)<< running_sum_1 << endl;
        if (i == spread.size() - n1)
        {
            double a = running_sum_1 + (x1_counter * values1[i + n1 - 1]) + (x2_counter * values2[i + n2 - 1]);
            file6 << std::fixed << std::setprecision(2)<<a<<endl;
        }
    }
    remove("temp1.csv");
    remove("temp2.csv");
}
int main(int argc, char *argv[])
{
    mrt(stod(argv[1]));
}