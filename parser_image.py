def main(ent, out):
    with open(ent, 'r') as arc, open(out, 'w') as res:
        sizes = []
        data = {}
        for line in arc:
            ls = line.split(" ")
            if len(ls) > 2:
                new_line = ls[2] + " " + ls[3] + " " + ls[4]
                coords = (int(ls[0]), int(ls[1]))
                data[coords] = new_line
            elif len(ls) == 2:
                sizes.append(int(ls[0]))
                sizes.append(int(ls[1]))
                res.write(ls[0] + " " + ls[1])
            else:
                res.write(ls[0])
        parse_data(data, res, sizes)


def parse_data(data, res, sizes):
    for j in range(sizes[1]):
        for i in range(sizes[0]):
            res.write(data[(i,j)])


if __name__ == "__main__":
    main("image.txt", "image.ppm") #Nombre de fichero .txt datos de píxeles, nombre de fichero .ppm de salida