from flask import Flask, render_template, request, redirect, url_for, flash, session
from flask_sqlalchemy import SQLAlchemy
from werkzeug.security import generate_password_hash, check_password_hash
import main
import plotly.graph_objects as go
import subprocess
import csv

app = Flask(__name__)
app.secret_key = 'your_secret_key'  # Replace with your actual secret keyf

# Database Configuration
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///users.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)

# User Model
class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(100), unique=True, nullable=False)
    password_hash = db.Column(db.String(200), nullable=False)

# Initialize Database within Application Context
with app.app_context():
    db.create_all()

@app.route('/')
def index():
    return render_template('login.html')

@app.route('/register', methods=['GET', 'POST'])
def register():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        hashed_password = generate_password_hash(password, method='pbkdf2:sha256')

        new_user = User(username=username, password_hash=hashed_password)
        db.session.add(new_user)
        db.session.commit()

        flash('Registration successful! Please login.')
        return redirect(url_for('index'))

    return render_template('register.html')

@app.route('/login', methods=['POST'])
def login():
    username = request.form['username']
    password = request.form['password']
    user = User.query.filter_by(username=username).first()

    if user and check_password_hash(user.password_hash, password):
        session['user_id'] = user.id
        session['username'] = user.username
        return redirect(url_for('dashboard'))
    else:
        flash('Invalid username or password')
        return redirect(url_for('index'))

@app.route('/dashboard')
def dashboard():
    if 'user_id' in session:
        return render_template('welcome.html', username=session['username'])
    else:
        return redirect(url_for('index'))

@app.route('/dashstock')
def dashstock():
    return render_template('graph_home.html')
@app.route('/techfilters')
def techfilters():
    return render_template('tech_filter_home.html')
@app.route('/strategies')
def strategies():
    return render_template('strategies.html')

@app.route('/stockinf',methods=['POST'])
def stockinf():

    selected_options=request.form.getlist('filterOption')
    s2=request.form.getlist('selectedCompanies[]')
    s2=[item.strip() for item in s2[0].split(',')]
    my_list=selected_options+s2

    drop_option=request.form.get('dropfilter')

    if drop_option!='none':
        if len(my_list)!=0:
            
            return redirect(url_for('fildash',a=my_list,b=drop_option))
        else:
            return redirect(url_for('dashstock'))
    else:
        return redirect(url_for('dashstock'))

@app.route('/fildash')
def fildash():
    v=request.args.getlist('a')
    q=request.args.get('b')
    
    my_fig,data_not_found=main.run(v,q)
    if my_fig!=None:
        plot_html=my_fig.to_html(full_html=False)
    else:
        plot_html='Data Not Found'
    return render_template('stock.html',plot_html=plot_html,d=data_not_found)

@app.route('/techfilter',methods=['POST'])
def techfilter():
    app.logger.info("in /techfilter")
    
    tech_option=request.form.get('techfilter')
    Minvalue=request.form.get('minvalue')
    Maxvalue=request.form.get('maxvalue')
    
    tech_filt=request.form.get('nifty')
    my_list=[]
    if(tech_filt=="c"):
        selected_options=request.form.getlist('filterOption')
        s2=request.form.getlist('selectedCompanies[]')
        s2=[item.strip() for item in s2[0].split(',')]

        my_list=selected_options+s2
    
    a,b=main.techfilter(tech_option,Minvalue,Maxvalue,tech_filt,my_list)
    return render_template('tech_filter_results.html',a=a,b=b)

@app.route('/strategies_home',methods=['POST'])
def strategies_home():
    try:
        make_command=['make','strategy=BASIC','symbol=SBIN','n=5','x=2','start_date=01/01/2020','end_date=31/01/2020']
        subprocess.run(make_command,check=True,capture_output=True,text=True)
    except subprocess.CalledProcessError as e:
        print(e)
    
    date=[];cashflow=[]
    with open('daily_cashflow.csv','r') as file:
        csv_reader=csv.reader(file,delimiter=',')
        for line in csv_reader:
            date_d,number=line
            date.append(date_d)
            cashflow.append(number)

    a=[];b=[];c=[];d=[]
    with open('order_statistics.csv','r') as file:
        csv_reader=csv.reader(file,delimiter=',')
        for line in csv_reader:
            e,f,g,h=line
            a.append(e);b.append(f);c.append(g);d.append(h)

    table_html_cashflow="<table>\n"
    for item1,item2 in zip(date,cashflow):
        table_html_cashflow+=f"<tr><td>{item1}</td><td>{item2}</td></tr>\n"
    table_html_cashflow+="</table>"
    
    table_order_statistics="<table>\n"
    for item1,item2,item3,item4 in zip(a,b,c,d):
        table_order_statistics+=f"<tr><td>{item1}</td><td>{item2}</td><td>{item3}</td><td>{item4}</td></tr>\n"
    table_order_statistics+="</table>"

    return render_template('strategies_result.html',table1=table_html_cashflow,table2=table_order_statistics)
@app.route('/logout')
def logout():
    session.pop('user_id', None)
    session.pop('username', None)
    return redirect(url_for('index'))

if __name__ == '__main__':
    app.run(debug=True)
