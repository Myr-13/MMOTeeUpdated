from colorama import Fore, Style
import colorama
import mysql_bc as mysql
import os
from confparser import parse
#from var_name import get_name

colorama.init(convert = True)

config = parse("connect_settings.cfg")

data = {}

def main():
	os.system("cls")

	print(Fore.YELLOW + "MySQL Updater for mmotee.")
	print(Fore.GREEN + "    By Myr")
	print(Style.BRIGHT)

	mysql.mysql_connect()
	print(Fore.GREEN + "[+]" + Fore.BLUE + " Connected to MySQL\n" + Fore.WHITE)

	with mysql.connection.cursor(buffered = True) as cursor:
		cursor.execute("use %s" % config["DB_Name"])

	# Save values
	kek = None
	with mysql.connection.cursor(buffered = True) as cursor:
		cursor.execute("SHOW tables;")
		kek = cursor.fetchall()
	for i in kek:
		data[i[0]] = []

		with mysql.connection.cursor(buffered = True) as cursor:
			cursor.execute("SELECT * FROM %s" % i[0])
			for j in cursor.fetchall():
				data[i[0]].append(j)

	# Return values
	for k in data:
		for v in data[k]:
			cmd = "INSERT INTO %s VALUES %s;" % (k, v)
			print(cmd)

if __name__ == "__main__":
	main()
