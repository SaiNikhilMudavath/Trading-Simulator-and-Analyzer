run_cpp:
	@if [ "$(strategy)" = "LINEAR_REGRESSION" ]; then \
		python3 read.py $(strategy) $(symbol) $(x) $(p) $(train_start_date) $(train_end_date) $(start_date) $(end_date);\
		g++ -o r s2.cpp;\
	 	./r $(x) $(p);\
	elif [ "$(strategy)" = "BASIC" ]; then \
		python3 read.py $(strategy) $(symbol) $(n) $(x) $(start_date) $(end_date);\
		g++ -o r s1_1.cpp;\
		./r;\
	elif [ "$(strategy)" = "DMA" ]; then \
		python3 read.py $(strategy) $(symbol) $(n) $(x) $(start_date) $(end_date);\
		g++ -o r s1_2.cpp;\
		./r $(p);\
	elif [ "$(strategy)" = "DMA++" ]; then \
		python3 read.py $(strategy) $(symbol) $(n) $(x) $(start_date) $(end_date);\
	 	g++ -o r s1_3.cpp;\
	 	./r $(p) $(max_hold_days) $(c1) $(c2);\
	elif [ "$(strategy)" = "MACD" ]; then \
		python3 read.py $(strategy) $(symbol) $(x) $(start_date) $(end_date);\
		g++ -o r s1_4-1.cpp;\
		./r;\
	elif [ "$(strategy)" = "RSI" ]; then \
		python3 read.py $(strategy) $(symbol) $(n) $(x) $(start_date) $(end_date);\
		g++ -o r s1_4-2.cpp;\
		./r $(overbought_threshold) $(oversold_threshold);\
	elif [ "$(strategy)" = "ADX" ]; then \
		python3 read.py $(strategy) $(symbol) $(n) $(x) $(start_date) $(end_date);\
		g++ -o r s1_4-3.cpp;\
		./r $(adx_threshold);\
	elif [ "$(strategy)" = "BEST_OF_ALL" ]; then \
		python3 read.py $(strategy) $(symbol) $(start_date) $(end_date);\
		g++ -o r s3.cpp;\
		./r ;\
	elif [ "$(strategy)" = "PAIRS" ]; then \
		if [ -n "$(stop_loss_threshold)" ]; then \
			python3 read.py $(strategy) $(symbol1) $(symbol2) $(n) $(x) $(start_date) $(end_date);\
			g++ -o r s4-1.cpp;\
			./r $(threshold) $(stop_loss_threshold);\
		else\
			python3 read.py $(strategy) $(symbol1) $(symbol2) $(n) $(x) $(start_date) $(end_date);\
			g++ -o r s4.cpp;\
			./r $(threshold);\
		fi;\
	fi
.PHONY:run_cpp