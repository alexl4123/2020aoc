import clingo

class Context:
    def id(self, x): 
        return x

    def seq(self, x, y): 
        return [x, y]

def on_model(self, m):

    symbols = m.symbols(shown=True)

    for symbol in symbols:
        print(symbol)


if __name__ == "__main__":

    filename = "input.txt"
    encoding_filename = "encoding.lp"

    read_file = open(filename, "r")

    read_lines = read_file.readlines()

    read_file.close()

    elf_calories = []

    index = 0

    instances = []

    for line in read_lines:

        splits = line.split(" ")

        enemy_move = None
        my_move = None

        if splits[0] == "A":
            enemy_move = 1
        elif splits[0] == "B":
            enemy_move = 2
        elif splits[0] == "C":
            enemy_move = 3

        if splits[1][0] == "X":
            my_move = 1
        elif splits[1][0] == "Y":
            my_move = 2
        elif splits[1][0] == "Z":
            my_move = 3

        instances.append(f"move({index},{enemy_move},{my_move}).")

        index += 1


    instance_contents = "\n".join(instances)

    encoding_file = open(encoding_filename, "r")

    encoding_contents = ""
    for line in encoding_file.readlines():
        if line == "\n":
            continue
        else:
            encoding_contents += line

    encoding_file.close()


    ctl = clingo.Control()
    ctl.add('base',[],instance_contents + encoding_contents)
    ctl.ground([('base',[])], context=Context())
    ctl.solve(on_model = lambda m: print(m))




