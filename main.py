import yfinance as yf
import datetime
import pandas as pd
import matplotlib.pyplot as plt
import os
import plotly.graph_objects as go

#sorting for averagr price tech filter
def merge_sort(array):
    if len(array)>1:
        mid=len(array)//2
        left_half=array[:mid]
        right_half=array[mid:]
        merge_sort(left_half)
        merge_sort(right_half)
        merge(array,left_half,right_half)
def merge(array,left_half,right_half):
    i=0;j=0;k=0
    
    while i<len(left_half) and j<len(right_half):
        if left_half[i][1]<right_half[j][1]:
            array[k]=left_half[i]
            i+=1;k+=1
        else:
            array[k]=right_half[j]
            j+=1;k+=1
    while(i<len(left_half)):
        array[k]=left_half[i]
        i+=1;k+=1
    while(j<len(right_half)):
        array[k]=right_half[j]
        j+=1;k+=1

#for stock graph
def run(array, filter):
    graph_data=[]
    data_not_found=[]
    
    for i in range(len(array)):
        symbol = array[i]
        
        if symbol!='':
            symbol += '.NS'
            
            if filter == 'daily':
                delay = '1d'
                inter = '1m'
            elif filter == 'weekly':
                delay = '7d'
                inter = '5m'
            elif filter == 'monthly':
                delay = '31d'
                inter = '30m'
            elif filter == 'yearly':
                delay = '365d'
                inter = '1d'

            data = yf.download(symbol, period=delay, interval=inter)
            df = pd.DataFrame(data)
            if df.empty:
                data_not_found.append(symbol[:-3])
                continue
            
            my_list1=df['High'].to_list()
            my_list2=df['Low'].to_list()
            my_list=[]
            
            mylist_min=((my_list1[0]+my_list2[0])/2)
            mylist_max=((my_list1[0]+my_list2[0])/2)
            for j in range(len(my_list1)):
                avg=((my_list1[j]+my_list2[j])/2)
                my_list.append(avg)
                if mylist_min>avg:
                    mylist_min=avg
                if mylist_max<avg:
                    mylist_max=avg

            trace=go.Scatter(y=my_list,mode='lines',name=symbol)
            graph_data.append(trace)

    fig=go.Figure(data=graph_data)
    layout=go.Layout(
        height=100,
        width=100
    )
    for i in range(1,len(graph_data)+1):
        layout[f"yaxis{i}"]=dict(
            title=f"data {i}",
            overlaying='y',
            side='right' if i%2==0 else 'left',
            range=[mylist_min,mylist_max]
        )
    return fig,data_not_found

##
#technical filter p/e,average price

def techfilter(filter,passing_value,max_value,option,custom_companies):

    remove_companies=[]

    symbols = []
    if option=='n':
        symbols=[
        'ADANIENT', 'ADANIPORTS', 'APOLLOHOSP', 'ASIANPAINT', 'AXISBANK', 'BAJAJ-AUTO',
        'BAJFINANCE', 'BAJAJFINSV', 'BPCL', 'BHARTIARTL', 'BRITANNIA', 'CIPLA', 'COALINDIA',
        'DIVISLAB', 'DRREDDY', 'EICHERMOT', 'GRASIM', 'HCLTECH', 'HDFCBANK', 'HDFCLIFE',
        'HEROMOTOCO', 'HINDALCO', 'HINDUNILVR', 'ICICIBANK', 'ITC', 'INDUSINDBK', 'INFY',
        'JSWSTEEL', 'KOTAKBANK', 'LTIM', 'LT', 'MARUTI', 'NTPC', 'NESTLEIND', 'ONGC',
        'POWERGRID', 'RELIANCE', 'SBILIFE', 'SBIN', 'SUNPHARMA', 'TCS', 'TATACONSUM',
        'TATAMOTORS', 'TATASTEEL', 'TECHM', 'TITAN', 'UPL', 'ULTRACEMCO', 'WIPRO']
        for i in range(len(symbols)):
            symbols[i]+='.NS'
    elif option=='a':
        with open('symbols.csv','r') as file:
            for line in file:
                symbol=str(line)[:-1]
                symbol+='.NS'
                symbols.append(symbol)
    
    elif option=='c':
        symbols=custom_companies
        new_symbols=[]
        for i in range(len(symbols)-1):
            new_symbols.append(symbols[i]+'.NS')
        symbols=new_symbols
    passing_value=int(passing_value)
    max_value=int(max_value)
    #1
    if filter=='P/E ratio':
        passing_pbye_ratios=[]
        passing_companies=[]

        for i in range(len(symbols)):

            stock=yf.Ticker(symbols[i])
            try:
                eps=stock.info['trailingEps']
            except:
                remove_companies.append(symbols[i][:-3])
                continue

            data=yf.download(symbols[i],period='1d',interval='1d')
            df=pd.DataFrame(data)
            if df.empty:
                remove_companies.append(symbols[i])
                continue
            close_value=df['Close'].to_list()
            market_price_per_share=close_value[0]
            pbye=market_price_per_share/eps
            if pbye>=passing_value and pbye<=max_value:
                passing_pbye_ratios.append(pbye)
                symbols[i]=symbols[i][:-3]
                passing_companies.append(symbols[i])
        
        answer=[]
        for x in range(len(passing_companies)):
            x=[passing_companies[x],passing_pbye_ratios[x]]
            answer.append(x)
        merge_sort(answer) 
        return answer,remove_companies
    #2
    elif filter=='daily average price':

        passing_averages=[]
        passing_companies=[]
        for i in range(len(symbols)):

            data = yf.download(symbols[i], period='1d', interval='2m')
            df = pd.DataFrame(data)
            if df.empty:
                remove_companies.append(symbols[i])
                continue
            my_list1=df['High'].to_list()
            my_list2=df['Low'].to_list()
            sum=0
            for j in range(len(my_list1)):
                sum+=((my_list1[j]+my_list2[j])/2)
            average=sum/len(my_list1)
            if average>=passing_value and average<=max_value:
                passing_averages.append(average)
                symbols[i]=symbols[i][:-3]
                passing_companies.append(symbols[i])
        answer=[]
        for x in range(len(passing_companies)):
            x=[passing_companies[x],passing_averages[x]]
            answer.append(x)
        merge_sort(answer) 
        return answer,remove_companies
    #3
    elif filter=='weekly average price':

        passing_averages=[]
        passing_companies=[]
        for i in range(len(symbols)):

            data = yf.download(symbols[i], period='7d', interval='5m')
            df = pd.DataFrame(data)
            if df.empty:
                remove_companies.append(symbols[i])
                continue
            my_list1=df['High'].to_list()
            my_list2=df['Low'].to_list()
            sum=0
            for j in range(len(my_list1)):
                sum+=((my_list1[j]+my_list2[j])/2)
            average=sum/len(my_list1)
            if average>=passing_value and average<=max_value:
                passing_averages.append(average)
                symbols[i]=symbols[i][:-3]
                passing_companies.append(symbols[i])
        answer=[]
        for x in range(len(passing_companies)):
            x=[passing_companies[x],passing_averages[x]]
            answer.append(x)
        merge_sort(answer) 
        return answer,remove_companies
    #4
    elif filter=='monthly average price':

        passing_averages=[]
        passing_companies=[]
        for i in range(len(symbols)):

            data = yf.download(symbols[i], period='31d', interval='30m')
            df = pd.DataFrame(data)
            if df.empty:
                remove_companies.append(symbols[i])
                continue
            my_list1=df['High'].to_list()
            my_list2=df['Low'].to_list()
            sum=0
            for j in range(len(my_list1)):
                sum+=((my_list1[j]+my_list2[j])/2)
            average=sum/len(my_list1)
            if average>=passing_value and average<=max_value:
                passing_averages.append(average)
                symbols[i]=symbols[i][:-3]
                passing_companies.append(symbols[i])
        answer=[]
        for x in range(len(passing_companies)):
            x=[passing_companies[x],passing_averages[x]]
            answer.append(x)
        merge_sort(answer) 
        return answer,remove_companies
    #5
    elif filter=='yearly average price':

        passing_averages=[]
        passing_companies=[]
        for i in range(len(symbols)):

            data = yf.download(symbols[i], period='1y', interval='1d')
            df = pd.DataFrame(data)
            if df.empty:
                remove_companies.append(symbols[i])
                continue
            my_list1=df['High'].to_list()
            my_list2=df['Low'].to_list()
            sum=0
            for j in range(len(my_list1)):
                sum+=((my_list1[j]+my_list2[j])/2)
            average=sum/len(my_list1)
            if average>=passing_value and average<=max_value:
                passing_averages.append(average)
                symbols[i]=symbols[i][:-3]
                passing_companies.append(symbols[i])

        answer=[]
        for x in range(len(passing_companies)):
            x=[passing_companies[x],passing_averages[x]]
            answer.append(x)
        merge_sort(answer)
        return answer,remove_companies

    elif filter=='rsi':
        answer=[]
        
        for i in range(len(symbols)):
            data=yf.download(symbols[i],period='6d',interval='1d')
            delta=data['Close'].diff()

            gains=delta.where(delta>0,0)
            losses=delta.where(delta<0,0)

            avg_gain = gains.rolling(window=14, min_periods=1).mean()
            avg_loss = losses.rolling(window=14, min_periods=1).mean()

            rs=avg_gain/avg_loss

            rsi=100-(100/(1+rs))

            answer.append(rsi)
        answer=pd.DataFrame(answer)
        answer.insert(0,'Company',symbols)
        answer=answer.drop(answer.columns[1],axis=1)
        
        final_answer=answer.values.tolist()

        prompt=['Company','RSI values of recent 5 days']
        final_answer.insert(0,prompt)
        
        return final_answer,[]
