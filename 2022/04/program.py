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
    #filename = "sample_input.txt"
    encoding_filename = "encoding.lp"

    read_file = open(filename, "r")

    read_lines = read_file.readlines()

    read_file.close()


    index = 0

    instances = []

    for line in read_lines:

        splits = line.split(",")

        first = splits[0]
        first_splits = first.split("-")

        first_first = first_splits[0]
        first_second = first_splits[1]

        second = splits[1]
        second_splits = second.split("-")

        second_first = second_splits[0]
        second_second = ((second_splits[1]).split("\n"))[0]

        instances.append(f"assignments({index},{first_first},{first_second},{second_first},{second_second}).")

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




