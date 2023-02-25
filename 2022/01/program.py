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

    cur_elf_id = 0
    cur_elf_calory_id = 0

    for line in read_lines:

        if line == "\n":
            cur_elf_id += 1
            cur_elf_calory_id = 0
            continue
        else:
            elf_calories.append(f"calory({cur_elf_id},{cur_elf_calory_id},{int(line)}).")
            cur_elf_calory_id += 1

    instance_contents = "\n".join(elf_calories)

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




