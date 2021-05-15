from flask import Flask, render_template

#Initalize
app = Flask(__name__, static_folder="static")

@app.route("/")
def hello():
    return render_template("Flood-It-Web.html")

#Run the server localy
if __name__ == '__main__':
    app.run(host="localhost", port=5000)

