# Program that converts a table data to a single column

def main ():
    input_file = open("table-AT-Total.dat","r")
    output_file = open("out2","w+")
    for line in input_file:
        words = line.split()
        for word in words:
            output_file.write("%f\n" % float(word))
    input_file.close()
    output_file.close()


if __name__ == "__main__":
    main()