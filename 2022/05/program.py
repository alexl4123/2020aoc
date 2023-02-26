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



    instances = []

    positions = {}

    index = 0
    mode = 1

    for line in read_lines:
        if mode == 1:
            if line == "\n":
                mode = 2
                continue

            splits = line.split(" ")

            stack = 1
            splits_pos = 0

            while splits_pos < len(splits):
                if splits[splits_pos].startswith("["):
                    string = splits[splits_pos]
                    res = (string.split("[")[1]).split("]")[0]

                    if stack not in positions:
                        positions[stack] = []

                    positions[stack].append(res)

                    splits_pos += 1

                    stack += 1
                else:
                    splits_pos += 4
                    stack += 1
                    
        else:
            splits = line.split(" ")
            amount = splits[1]
            from_ = splits[3]
            to_ = (splits[5]).split("\n")[0]

            instances.append(f"move({index},{amount},{from_},{to_}).")

            index += 1


    for stack_index in positions.keys():
        stack = positions[stack_index]

        for item_index in range(len(stack)):
            crate = stack[item_index]

            instances.append(f"position({stack_index},{item_index + 1},{crate.lower()}).")



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




