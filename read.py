from datetime import datetime,timedelta
import pandas as pd
import time
from jugaad_data.nse import stock_df
from datetime import datetime
import csv
import sys

# stg=input("ENTER STRATEGY:")
stg=sys.argv[1]
# print(stg)

#BASIC #for calculation of back_date
def past1(start,y,sym):
    # print(start)
    while y>0:
        start=start-timedelta(days=1)  
        # print(start) 
        try:
            data=stock_df(symbol=sym,from_date=start,to_date=start,series="EQ")
            y-=1
        except:
            continue
    return start
        
def ver1(start,w,sym):
    # print("entered ver1")
    while start!=end_date:
        try:
            # print(start)
            data=stock_df(symbol=sym,from_date=start,to_date=start,series="EQ")
            return past1(start,w,sym) 
        except:
            start=start+timedelta(days=1)

#LINEAR REGRESSION

# def prev_date(start):
#     start=start-timedelta(days=1)
#     while 1>0:
#         try:
#             data=stock_df(symbol=sym,from_date=start,to_date=start,series="EQ")
#             return start
#         except:
#             start=start-timedelta(days=1)

#begingng of if 

if stg=='BASIC':
    sym=sys.argv[2]
    n=int(sys.argv[3])
    x=int(sys.argv[4])
    start=sys.argv[5]
    end=sys.argv[6]
    str_date = datetime.strptime(start, '%d/%m/%Y')
    end_date = datetime.strptime(end, '%d/%m/%Y')
    back_date=ver1(str_date,n,sym)
    # print(back_date)
    #dataframe
    my_stk=stock_df(symbol=sym,from_date=back_date,to_date=end_date,series="EQ")
    reqs=['DATE','CLOSE']
    #writing to csv
    my_stk = my_stk[reqs].sort_values(by='DATE', ascending=True)
    my_stk['DATE'] = pd.to_datetime(my_stk['DATE']).dt.strftime('%d/%m/%Y')
    my_stk[reqs].to_csv('temp1.csv',index=False,header=None)
    #for adding x,n at last
    others=[['n',n],['x',x]]
    with open('temp1.csv','a',newline='') as file:
        write=csv.writer(file)
        write.writerows(others)
elif stg=='DMA':
    sym=sys.argv[2]
    n=int(sys.argv[3])
    x=int(sys.argv[4])
    start=sys.argv[5]
    end=sys.argv[6]
    str_date = datetime.strptime(start, '%d/%m/%Y')
    end_date = datetime.strptime(end, '%d/%m/%Y')
    back_date=ver1(str_date,n-1,sym)
    # print(back_date)
    #dataframe
    my_stk=stock_df(symbol=sym,from_date=back_date,to_date=end_date,series="EQ")
    reqs=['DATE','CLOSE']
    #writing to csv
    my_stk = my_stk[reqs].sort_values(by='DATE', ascending=True)
    my_stk['DATE'] = pd.to_datetime(my_stk['DATE']).dt.strftime('%d/%m/%Y')
    my_stk[reqs].to_csv('temp1.csv',index=False,header=None)
    #for adding x,n at last
    others=[['n',n],['x',x]]
    with open('temp1.csv','a',newline='') as file:
        write=csv.writer(file)
        write.writerows(others)
elif stg=='DMA++':
    sym=sys.argv[2]
    n=int(sys.argv[3])
    x=int(sys.argv[4])
    start=sys.argv[5]
    end=sys.argv[6]
    str_date = datetime.strptime(start, '%d/%m/%Y')
    end_date = datetime.strptime(end, '%d/%m/%Y')
    back_date=ver1(str_date,n,sym)
    # print(back_date)
    #dataframe
    my_stk=stock_df(symbol=sym,from_date=back_date,to_date=end_date,series="EQ")
    reqs=['DATE','CLOSE']
    #writing to csv
    my_stk = my_stk[reqs].sort_values(by='DATE', ascending=True)
    my_stk['DATE'] = pd.to_datetime(my_stk['DATE']).dt.strftime('%d/%m/%Y')
    my_stk[reqs].to_csv('temp1.csv',index=False,header=None)
    #for adding x,n at last
    others=[['n',n],['x',x]]
    with open('temp1.csv','a',newline='') as file:
        write=csv.writer(file)
        write.writerows(others)
elif stg=='MACD':
    sym=sys.argv[2]
    x=int(sys.argv[3])
    start=sys.argv[4]
    end=sys.argv[5]
    str_date = datetime.strptime(start, '%d/%m/%Y')
    end_date = datetime.strptime(end, '%d/%m/%Y')
    back_date=ver1(str_date,0,sym)
    # print(back_date)
    #dataframe
    my_stk=stock_df(symbol=sym,from_date=back_date,to_date=end_date,series="EQ")
    reqs=['DATE','CLOSE']
    #writing to csv
    my_stk = my_stk[reqs].sort_values(by='DATE', ascending=True)
    my_stk['DATE'] = pd.to_datetime(my_stk['DATE']).dt.strftime('%d/%m/%Y')
    my_stk[reqs].to_csv('temp1.csv',index=False,header=None)
    #for adding x,n at last
    others=[['n',0],['x',x]]
    with open('temp1.csv','a',newline='') as file:
        write=csv.writer(file)
        write.writerows(others)
elif stg=='RSI':
    sym=sys.argv[2]
    n=int(sys.argv[3])
    x=int(sys.argv[4])
    start=sys.argv[5]
    end=sys.argv[6]
    str_date = datetime.strptime(start, '%d/%m/%Y')
    end_date = datetime.strptime(end, '%d/%m/%Y')
    back_date=ver1(str_date,n,sym)
    # print(back_date)
    #dataframe
    my_stk=stock_df(symbol=sym,from_date=back_date,to_date=end_date,series="EQ")
    reqs=['DATE','CLOSE']
    #writing to csv
    my_stk = my_stk[reqs].sort_values(by='DATE', ascending=True)
    my_stk['DATE'] = pd.to_datetime(my_stk['DATE']).dt.strftime('%d/%m/%Y')
    my_stk[reqs].to_csv('temp1.csv',index=False,header=None)
    #for adding x,n at last
    others=[['n',n],['x',x]]
    with open('temp1.csv','a',newline='') as file:
        write=csv.writer(file)
        write.writerows(others)
elif stg=='ADX':
    sym=sys.argv[2]
    n=int(sys.argv[3])
    x=int(sys.argv[4])
    start=sys.argv[5]
    end=sys.argv[6]
    str_date = datetime.strptime(start, '%d/%m/%Y')
    end_date = datetime.strptime(end, '%d/%m/%Y')
    back_date=ver1(str_date,1,sym)
    # print(back_date)
    #dataframe
    my_stk=stock_df(symbol=sym,from_date=back_date,to_date=end_date,series="EQ")
    req1=['DATE','CLOSE']
    req2=['DATE','LOW']
    req3=['DATE','HIGH']
    req4=['DATE','PREV. CLOSE']
    #writing to csv
    my_stk = my_stk.sort_values(by='DATE', ascending=True)
    my_stk['DATE'] = pd.to_datetime(my_stk['DATE']).dt.strftime('%d/%m/%Y')
    my_stk[req1].to_csv('temp1.csv',index=False,header=None)
    my_stk[req2].to_csv('low.csv',index=False,header=None)
    my_stk[req3].to_csv('high.csv',index=False,header=None)
    my_stk[req4].to_csv('prev_close.csv',index=False,header=None)
    #for adding x,n at last
    others=[['n',n],['x',x]]
    with open('temp1.csv','a',newline='') as file:
        write=csv.writer(file)
        write.writerows(others)
elif stg=='PAIRS':
    sym1=sys.argv[2]
    sym2=sys.argv[3]
    n=int(sys.argv[4])
    x=int(sys.argv[5])
    start=sys.argv[6]
    end=sys.argv[7]
    str_date = datetime.strptime(start, '%d/%m/%Y')
    end_date = datetime.strptime(end, '%d/%m/%Y')
    back_date=ver1(str_date,n-1,sym1)
    # print(back_date)
    #dataframe
    my_stk=stock_df(symbol=sym1,from_date=back_date,to_date=end_date,series="EQ")
    reqs=['DATE','CLOSE']
    #writing to csv
    my_stk = my_stk[reqs].sort_values(by='DATE', ascending=True)
    my_stk['DATE'] = pd.to_datetime(my_stk['DATE']).dt.strftime('%d/%m/%Y')
    my_stk[reqs].to_csv('temp1.csv',index=False,header=None)
    #for adding x,n at last
    others=[['n',n],['x',x]]
    with open('temp1.csv','a',newline='') as file:
        write=csv.writer(file)
        write.writerows(others)
    #dataframe for company2 
    back_date2=ver1(str_date,n-1,sym2)
    my_stk2=stock_df(symbol=sym2,from_date=back_date2,to_date=end_date,series="EQ")
    #writing to csv
    my_stk2 = my_stk2[reqs].sort_values(by='DATE', ascending=True)
    my_stk2['DATE'] = pd.to_datetime(my_stk2['DATE']).dt.strftime('%d/%m/%Y')
    my_stk2[reqs].to_csv('temp2.csv',index=False,header=None)
    #for adding x,n at last
    others=[['n',n],['x',x]]
    with open('temp2.csv','a',newline='') as file:
        write=csv.writer(file)
        write.writerows(others)
        
elif stg=='LINEAR_REGRESSION':
    # print("i am in")
    sym=sys.argv[2]
    x=int(sys.argv[3])
    p=int(sys.argv[4])
    train_start=sys.argv[5]
    train_end=sys.argv[6]
    start=sys.argv[7]
    end=sys.argv[8]
    #tarining data
    str_date = datetime.strptime(train_start, '%d/%m/%Y')
    end_date = datetime.strptime(train_end, '%d/%m/%Y')
    back_date=ver1(str_date,1,sym)
    # print(back_date)
    my_stk=stock_df(symbol=sym,from_date=back_date,to_date=end_date,series="EQ")
    reqs=['DATE','CLOSE','OPEN','VWAP','LOW','HIGH','NO OF TRADES']
    #writing to csv
    my_stk['DATE'] = pd.to_datetime(my_stk['DATE']).dt.strftime('%d/%m/%Y')
    my_stk[reqs].to_csv('temp4_1.csv',index=False,header=None)
    
    #main data
    main_str = datetime.strptime(start, '%d/%m/%Y')
    main_end = datetime.strptime(end, '%d/%m/%Y')
    main_back_date=ver1(main_str,1,sym)
    my_stk=stock_df(symbol=sym,from_date=main_back_date,to_date=main_end,series="EQ")
    reqs=['DATE','CLOSE','OPEN','VWAP','LOW','HIGH','NO OF TRADES']
    #writing to csv
    my_stk['DATE'] = pd.to_datetime(my_stk['DATE']).dt.strftime('%d/%m/%Y')
    my_stk[reqs].to_csv('temp4_2.csv',index=False,header=None)
elif stg=='BEST_OF_ALL':
    print("read")



# if stg=='BASIC':
#     back_date=ver1(str_date)
#     # print(back_date)
#     #dataframe
#     my_stk=stock_df(symbol=sym,from_date=back_date,to_date=end_date,series="EQ")
#     reqs=['DATE','CLOSE']
#     #writing to csv
#     my_stk = my_stk[reqs].sort_values(by='DATE', ascending=True)
#     my_stk[reqs].to_csv('temp.csv',index=False,header=None)
#     #for adding x,n at last
#     others=[['n',n],['x',x]]
#     with open('temp.csv','a',newline='') as file:
#         write=csv.writer(file)
#         write.writerows(others)
# elif sym=='LINEAR_REGRESSION':
#     my_stk=stock_df(symbol=sym,from_date=str_date,to_date=end_date,series="EQ")
#     #writing to csv
#     my_stk.to_csv('temp1.csv',index=False)

