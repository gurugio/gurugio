


def print_lol(the_list, level):
    for each_item in the_list:
        if isinstance(each_item, list):
            print_lol(each_item, level + 1)
        else:
            #tabs = []
            tabs = ["#" for count in range(level)] + [each_item]
            # for count in range(level):
            #     tabs.append("#")
            print tabs


if __name__ == "__main__":
    test_list = ['a', ['b', ['c', 'd'], 'e', ['f', 'g']], 'h', 'i', ['j', [['x']], 'k']]
    print_lol(test_list, 0)
    
