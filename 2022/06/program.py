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
    encoding_filename = "encoding_2.lp"

    read_file = open(filename, "r")

    read_lines = read_file.readlines()

    read_file.close()

   
    encoding = []

    upper = 15
    for i in range(1,upper):
        encoding.append(f"character(POSITION{i},VALUE{i})")

        if i > 1:
            encoding.append(f"POSITION{i} = POSITION{i - 1} + 1")

        for j in range(i+1,upper):
            encoding.append(f"VALUE{i} != VALUE{j}")

    encoding_2_filename = "encoding_2.lp"

    f = open(encoding_2_filename, "w")
    f.write(f"diff(POSITION{upper-1}) :- {','.join(encoding)}.\nmin_diff(POSITION) :- POSITION = #min{{X : diff(X)}}.\n#show min_diff/1.")
    f.close()


    instances = []

    positions = {}


    for line in read_lines:
        characters = [*line]
        del characters[len(characters) - 1]

        for character_index in range(len(characters)):
            character = characters[character_index]

            instances.append(f"character({character_index + 1}, {ord(character)}).")

            



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




