#include <bits/stdc++.h>
using namespace std;
// for printing
// void print(vector<vector<double>> v)
// {
//     for (const auto q : v)
//     {
//         for (const auto i : q)
//         {
//             cout << i << " ";
//         }
//         cout << endl;
//     }
// }
// for making the matrix
vector<vector<double>> to_double(vector<string> &p)
{
    vector<vector<double>> res;
    res.resize(p.size() - 1);
    vector<double> c;
    for (int i = 0; i < p.size() - 1; i++)
    {
        bool is = false;
        string x = "";
        res[i].push_back(1);
        for (int j = 0; j < p[i + 1].size(); j++)
        {
            if (is && p[i + 1][j] != ',')
            {
                x = x + p[i + 1][j];
            }
            else if (p[i + 1][j] == ',')
            {
                is = true;
                if (x != "")
                {
                    res[i].push_back(stod(x));
                    x = "";
                }
            }
        }
        res[i].push_back(stod(x));
        is = false;
        x = "";
        int e = 0;
        for (int j = 0; j < p[i].size(); j++)
        {
            if (is)
            {
                x = x + p[i][j];
            }
            if (p[i][j] == ',')
            {
                is = true;
                if (x != "")
                {
                    if (e == 0)
                    {
                        c.push_back(stod(x));
                        e++;
                        x = "";
                    }
                    else
                    {
                        res[i].push_back(stod(x));
                        break;
                    }
                }
            }
        }
    }
    res.push_back(c);
    return res;
}
vector<vector<double>> transpose(vector<vector<double>> &v)
{
    int r = v.size();
    int c = v[0].size();
    vector<vector<double>> t(c, vector<double>(r, 0));
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            t[j][i] = v[i][j];
        }
    }
    return t;
}

vector<vector<double>> multiply(const vector<vector<double>>a, const vector<vector<double>>b)
{
    int r = a.size();
    int c1 = a[0].size();
    int c2 = b[0].size();
    vector<vector<double>> mul(r, vector<double>(c2, 0));
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c2; j++)
        {
            for (int k = 0; k < c1; k++)
            {
                mul[i][j] += (a[i][k] * b[k][j]);
            }
        }
    }
    return mul;
}
// for modifying matrix by removing the rows
vector<vector<double>> modify(vector<vector<double>> v, vector<int> index)
{
    vector<vector<double>> p;
    for (auto i : index)
    {
        p.push_back(v[i]);
    }
    return p;
}
// for y modifications
vector<vector<double>> mod_y(vector<double> v, vector<int> index)
{
    // cout<<"bb"<<endl;
    vector<vector<double>> p;
    p.resize(index.size());
    for (int i = 0; i < index.size(); i++)
    {
        p[i].push_back(v[index[i]]);
    }
    return p;
}
// for finding inverse
vector<vector<double>>inv_rse(const vector<vector<double>> &w)
{
    int n = w.size();
    vector<vector<double>> aug_mat(n, vector<double>(2 * n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            aug_mat[i][j] = w[i][j];
        }
        aug_mat[i][i + n] = 1;
    }

    for (int i = 0; i < n; i++)
    {

        int p = i;
        for (int j = i + 1; j < n; j++)
        {
            if (abs(aug_mat[j][i]) > abs(aug_mat[p][i]))
            {
                p = j;
            }
        }
       
        swap(aug_mat[i], aug_mat[p]);

        double d = aug_mat[i][i];
        if (d == 0)
        {
            // cout << "sfsdjfsf" << endl;
            return vector<vector<double>>();
        }
        for (int j = 0; j < 2 * n; j++)
        {
            aug_mat[i][j] /= d;
        }

        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                double m = aug_mat[j][i];
                for (int k = 0; k < 2 * n; k++)
                {
                    aug_mat[j][k] -= m * aug_mat[i][k];
                }
            }
        }
    }

    vector<vector<double>> ver(n, vector<double>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            ver[i][j] = aug_mat[i][j + n];
        }
    }

    return ver;
}

// for finding the coefficients
void b4(double x, double p)
{
    // reading temp1
    ifstream file("temp4_1.csv");
    if (!file.is_open())
    {
    std:
        cerr << "failed to open the file1. " << endl;
    }
    string line;
    vector<string> y;
    while (getline(file, line))
    {
        y.push_back(line);
    }
    file.close();
    // converting to desired matrix
    vector<vector<double>> v = to_double(y);
    // for y
    vector<double> b = v.back();
    v.pop_back();
    // print(v);
    // start of finding betas
    // transpose of the v
    vector<vector<double>> trsp = transpose(v);
    // print(trsp);
    // v trans*v
    // vector<vector<double>> mul = multiply(trsp, v);
    // print(mul);
    // finding the rre
    // vector<int> ind = rre(v);
    // // cout<<"ind size is"<<ind.size();
    // // modified v
    // vector<vector<double>> v_m = modify(v, ind);
    // // cout<<"v_m"<<endl;
    // // print(v_m);
    // // finding transpose of modifed v
    // vector<vector<double>> trsp_m = transpose(v_m);
    // cout<<"trsp_m"<<endl;
    // print(trsp_m);
    // multiplying
    vector<vector<double>> mul_m = multiply(trsp, v);
    // cout<<"mul_m"<<endl;
    // print(mul_m);
    // modify the b
    // cout<<"hi"<<endl;
    vector<vector<double>> u;
    u.resize(b.size(), vector<double>(1));
    for (int i = 0; i < b.size(); i++)
    {
        u[i][0] = b[i];
    }
    // vector<vector<double>> b_m = mod_y(b, ind);
    // cout<<"modified b"<<endl;
    // print(b_m);
    // multiply trsp_m and b_m
    vector<vector<double>> r_m = multiply(trsp, u);
    // cout<<"r_m"<<endl;
    // print(r_m);
    // inverse of mul_m
    vector<vector<double>> inverse = inv_rse(mul_m);
    // cout<<"inverse"<<inverse.size()<<endl;
    // print(inverse);
    // final beta matrix
    vector<vector<double>> final = multiply(inverse, r_m);
    // cout<<"final"<<endl;
    // print(final);

    // reading temp2
    ifstream fil("temp4_2.csv");
    if (!fil.is_open())
    {
        cerr << "failed to open the file1. " << endl;
    }
    string lin;
    vector<string> y2;
    while (getline(fil, lin))
    {
        y2.push_back(lin);
    }
    file.close();
    // converting to desired matrix
    vector<vector<double>> v2 = to_double(y2);
    // actual close price
    vector<double> act_close = v2.back();
    v2.pop_back();
    // cout<<"v2 is"<<endl;
    // print(v2);
    // predicted close price
    vector<vector<double>> pre_close = multiply(v2, final);
    // cout<<"pre_close is"<<endl;
    // print(pre_close);
    // cout<<x<<p<<endl;

    // the actual computation starts
    std::ofstream file3("daily_cashflow.csv");
    std::ofstream file4("order_statistics.csv");
    int n = act_close.size();
    double run_sum = 0;
    double x_count = 0;
    // cout<<"act_close"<<endl;
    for (int q = n - 1; q >= 0; q--)
    {
        // cout<<act_close[n-1-q]<<endl;
        // cout<<pre_close[n-1-q]<<endl;
        double h = (pre_close[q][0] - act_close[q]) / act_close[q];
        // cout <<act_close[q]<<" "<<pre_close[q][0]<<" "<< h << endl;
        // cout<<"loop "<<endl;
        // h*=100.0;
        if (abs(h)>=(p/100.0))
        {
            // cout<<h<<endl;
            if (h > 0 && (x_count < x))
            {
                // buy
                run_sum += (-1.0) * act_close[q];
                x_count++;
                file4 << y2[q].substr(0, 11) << "BUY,1," << act_close[q] << endl;
            }
            else if (h < 0 && (x_count > (-1) * x))
            {
                // sell
                run_sum += act_close[q];
                x_count--;
                file4 << y2[q].substr(0, 11) << "SELL,1," << act_close[q] << endl;
            }
            file3 << y2[q].substr(0, 11);
            file3 << run_sum << endl;
        }
        
        std::ofstream file5("final_pnl.txt");
        // cout<<"abdbjsd"<<act_close[q]<<" "<<run_sum<<" "<<x_count<<endl;
        file5 << run_sum + (x_count * act_close[q]);
         
    }
    remove("temp4_1.csv");
    remove("temp4_2.csv");
}

int main(int argc, char *argv[])
{
    // cout<<"mofjdsfsf"<<endl;
    b4(stod(argv[1]),stod(argv[2]));
}