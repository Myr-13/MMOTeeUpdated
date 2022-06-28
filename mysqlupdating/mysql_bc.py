from mysql.connector import connect
from confparser import parse

data = parse("connect_settings.cfg")

db_username = data["DB_Username"]
db_password = data["DB_Password"]
db_name = data["DB_Name"]

connection = None

def mysql_connect():
	global connection
	
	connection = connect(
		host = "localhost",
		user = db_username,
		password = db_password,
		database = db_name
		)

def run_query(sql):
	with connection.cursor(buffered = True) as cursor:
		cursor.execute(sql, multi = True)
		return cursor

	#connection.commit()
