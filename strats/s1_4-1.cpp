#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

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
vector<double> ewm(vector<double> values, double alpha)
{
    vector<double> ewm_values;
    ewm_values.push_back(values[0]);
    for (int i = 1; i < values.size(); i++)
    {
        ewm_values.push_back(alpha * (values[i] - ewm_values.back()) + ewm_values.back());
    }
    return ewm_values;
}
void s4_1()
{
    std::ifstream file("temp1.csv");
    if (!file.is_open())
    {
    std:
        cerr << "failed to open the file. " << endl;
    }
    string line;
    vector<string> temp;
    while (getline(file, line))
    {
        temp.push_back(line);
    }
    file.close();
    vector<double> values = strtodouble(temp);
    double x = values.back();
    values.pop_back();
    values.pop_back();
    std::ofstream file2("daily_cashflow.csv");
    file2 << "Date,Cashflow"<<endl;
    std::ofstream file3("order_statistics.csv");
    file3 << "Date,Order_dir,Quantity,Price" << endl;
    double running_sum = 0;
    double x_counter = 0;

    vector<double> short_ewm = ewm(values, 2.0 / 13.0);
    vector<double> long_ewm = ewm(values, 2.0 / 27.0);
    vector<double> macd_values;
    for (int i = 0; i < short_ewm.size(); i++)
    {
        macd_values.push_back(short_ewm[i] - long_ewm[i]);
    }
    vector<double> signal = ewm(macd_values, 2.0 / 10.0);
    for (int i = 0; i < short_ewm.size(); i++)
    {
        if ((macd_values[i] > signal[i]) && (x_counter < x))
        {
            // buy
            running_sum += (values[i] * (-1.0));
            file2 << temp[i].substr(0, 10) << ",";
            file2 << running_sum << endl;
            x_counter++;
            file3 << temp[i].substr(0, 11) << "BUY,1," << values[i] << endl;
        }

        else if ((macd_values[i] < signal[i]) && (x_counter > (-1.0) * x))
        {
            running_sum += values[i];
            file2 << temp[i].substr(0, 10) << ",";
            file2 << running_sum << endl;
            x_counter--;
            file3 << temp[i].substr(0, 11) << "SELL,1," << values[i] << endl;
            // sell
        }
        else
        {
            file2 << temp[i].substr(0, 10) << ",";
            file2 << running_sum << endl;
        }
        if (i == short_ewm.size() - 1)
        {
            std::ofstream file4("final_pnl.txt");
            double a = running_sum + (x_counter * values[i]);
            file4 << a << endl;
        }
    }
    remove("temp1.csv");
}
int main()
{
    s4_1();
}