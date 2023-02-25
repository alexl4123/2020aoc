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


    priorities = []


    base_lower = ord("a")
    base_higher = ord("A")

    for index in range(26):
        priorities.append(f"priority({index + base_lower},{index+1}).")
        priorities.append(f"priority({index + base_higher},{index+27}).")


    index = 0
    group = 0

    instances = []

    for line in read_lines:

        chars = []
        chars.extend(line)

        del chars[len(chars) - 1]

        half = int(len(chars)/2)

        first = chars[:half]
        second = chars[half:]

        for elem in first:
            instances.append(f"item({index},1,{group},{ord(elem)}).")
 
        for elem in second:
            instances.append(f"item({index},2,{group},{ord(elem)}).")

        index += 1

        if index % 3 == 0:
            group += 1

    instances += priorities

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




