# Program that converts a table data to a single column

def main ():
    input_file = open("table-AT-Delay.dat","r")
    output_file = open("out","w+")
    for line in input_file:
        words = line.split()
        for word in words:
            output_file.write("%f\n" % float(word))
    input_file.close()
    output_file.close()


if __name__ == "__main__":
    main()