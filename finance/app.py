import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
import sqlite3
from datetime import datetime


from helpers import apology, login_required, lookup, usd, success #,validate

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd
app.jinja_env.globals.update(lookup=lookup)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]
    rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = float(rows[0]["cash"])
    stocks = db.execute("SELECT symbol, shares, price FROM stocks WHERE user_id = ?", user_id)
    total = 0;
    for m in stocks:
        shares=m["shares"]
        stock= lookup(m["symbol"])
        total_s=(int(shares)*stock["price"])
        total=total_s+total
        db.execute("UPDATE stocks SET price = ? WHERE user_id = ? AND symbol =? ",stock["price"],user_id, stock["symbol"])
    return render_template("index.html", stocks=stocks, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    user_id = session["user_id"]
    rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = float(rows[0]["cash"])
    if request.method == "POST":
        if request.form.get("shares") is None:
            if request.form.get("symbol") is None:
                return apology("Symbol cannot be blank",400)
            symbol=request.form.get("symbol")
            sdata=lookup(symbol)
            if sdata is None:
                return apology("Symbol does not exist",400)
            return render_template("buy.html", sdata=sdata, cash=cash)
        else:
            if request.form.get("symbol") is None:
                return apology("Symbol cannot be blank",400)
            symbol=request.form.get("symbol")
            shares=request.form.get("shares")
            sdata=(lookup(symbol))
            if sdata is None:
                return apology("Symbol does not exist",400)
            try:
                shares = int(shares)
                if shares < 1:
                    return apology("Shares must be a positive integer", 400)
            except ValueError:
                return apology("Shares must be a positive integer", 400)
            cost = float(shares) * float(sdata["price"])
            if cost > cash:
                return apology("Not enough cash",400)
            new_cash = cash - cost
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)
            rows = db.execute("SELECT shares FROM stocks WHERE user_id = ? AND symbol = ?", user_id, symbol)
            if len(rows) == 0:
                # user doesn't have the stock, insert new row into stocks table
                db.execute("INSERT INTO stocks (symbol, user_id, shares, price) VALUES (?, ?, ?, ?)", symbol, user_id, shares, sdata["price"])
            else:
                # user already has the stock, update shares in stocks table
                new_shares = int(rows[0]["shares"]) + int(shares)
                db.execute("UPDATE stocks SET shares = ? WHERE user_id = ? AND symbol = ? AND price = ?", new_shares, user_id, symbol, sdata["price"])
            date = datetime.now()
            db.execute("INSERT INTO transactions (symbol, shares, price, cost, date, user_id) VALUES (?, ?, ?, ?, ?,?)", symbol, shares, sdata["price"], cost*-1, date, user_id)
            flash("Successfully bought " +str(shares)+ " shares of "+ sdata["symbol"])
            return redirect("/")
    else:
        return render_template("buylookup.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    hist = db.execute("SELECT symbol,shares,price,cost,date FROM transactions WHERE user_id = ?", user_id)
    return render_template("history.html",hist=hist)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username",400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password",400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password",400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Symbol cannot be blank",400)
        symbol=request.form.get("symbol")
        sdata=lookup(symbol)
        if sdata == None:
            return apology("Symbol does not exist",400)
        return render_template("quoted.html", sdata=sdata)
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username",400)
        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password",400)
        elif not request.form.get("confirmation"):
            return apology("must confirm password",400)
        username=request.form.get("username")
        password=request.form.get("password")
        confirmpass=request.form.get("confirmation")
        checkuser=db.execute("SELECT username FROM users where username = ?", username)
        if len(checkuser)>0:
            return apology("Username is taken",400)
        # elif  not validate(password):
        #     return apology("a number\n a capital Letter\n at least 8 letters", "password\n must have")
        elif password == confirmpass:
            passhash=generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)
            db.execute("INSERT INTO users (username,hash) VALUES(?,?)", username, passhash)
            flash("Registration Successful")
            return success("Login", 200)
        else:
            return apology("Try agian",400)
    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = float(rows[0]["cash"])
    if request.method == "POST":
        if request.form.get("shares") is None:
            if request.form.get("symbol") is None:
                return apology("Symbol cannot be blank",400)
            symbol=request.form.get("symbol")
            sdata=lookup(symbol)
            stocks = db.execute("SELECT symbol, shares, price FROM stocks WHERE user_id = ?", user_id)
            return render_template("sell.html",stocks=stocks, sdata=sdata, cash=cash)
        else:
            if request.form.get("symbol") is None:
                return apology("Symbol cannot be blank",400)
            symbol=request.form.get("symbol")
            shares=request.form.get("shares")
            sdata=(lookup(symbol))
            if sdata is None:
                return apology("Symbol does not exist",400)
            rows = db.execute("SELECT shares FROM stocks WHERE user_id = ? AND symbol = ?", user_id, symbol)
            if len(rows) == 0 or int(rows[0]["shares"]) < int(shares):
                # user doesn't have enough shares to sell, or the stock is not in their portfolio
                return apology("Invalid number of shares", 400)
            cost = float(shares) * float(sdata["price"])
            new_cash = cash + cost
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)
            new_shares = int(rows[0]["shares"]) - int(shares)
            if new_shares == 0:
                # user sold all shares of the stock, delete row from stocks table
                db.execute("DELETE FROM stocks WHERE user_id = ? AND symbol = ?", user_id, symbol)
            else:
                # user still has some shares of the stock, update shares in stocks table
                db.execute("UPDATE stocks SET shares = ? WHERE user_id = ? AND symbol = ?", new_shares, user_id, symbol)
            date = datetime.now()
            db.execute("INSERT INTO transactions (symbol, shares, price, cost, date, user_id) VALUES (?, ?, ?, ?, ?,?)", symbol, shares, sdata["price"], cost, date, user_id)
            flash("Successfully sold " +shares+ " share(s) of "+ sdata["symbol"])
            return redirect("/")
    else:
        stocks = db.execute("SELECT symbol, shares, price FROM stocks WHERE user_id = ?", user_id)
        return render_template("sell.html",stocks=stocks)

