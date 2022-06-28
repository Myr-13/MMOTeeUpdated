def parse(filename):
	f = open(filename, "r")

	data = {}

	for line in f:
		line = line.replace(" ", "")

		if line[0] == "#" or line[0] == "\n":
			continue

		line_data = line.strip("\n").split("=")

		data[line_data[0]] = line_data[1]

	f.close()

	return data
