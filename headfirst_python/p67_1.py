


def print_lol(the_list, indent=False, level=0):
    for each_item in the_list:
        if isinstance(each_item, list):
            print_lol(each_item, indent, level + 1)
        else:
            if indent:
                for tab_stop in range(level):
                    print("$")
            print each_item



if __name__ == "__main__":
    test_list = ['a', ['b', ['c', 'd'], 'e', ['f', 'g']], 'h', 'i', ['j', [['x']], 'k']]

    print "NO INDENT"
    print_lol(test_list)

    print "INDENT"
    print_lol(test_list, True)
    
