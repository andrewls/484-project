print "\tchar * generated_prefixes[] = {"

characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef'

# end_string_length = 3
#
# def print_all_combinations_of_characters(string_so_far, characters, depth, final_depth):
#     # print "Recursing on string array %r with depth %d and final depth %d" % (string_so_far, depth, final_depth)
#     if depth == final_depth:
#         print ''.join(string_so_far)
#         # remove the last character of the array
#         del string_so_far[-1]
#         # move on to the next combination
#         return
#     for character in characters:
#         # append the current character
#         string_so_far.append(character)
#         # and now recurse
#         print_all_combinations_of_characters(string_so_far, characters, depth + 1, final_depth)
#
# print_all_combinations_of_characters([], characters, 0, end_string_length)

for character in characters:
    strings = []
    for character2 in characters:
        for character3 in characters:
            strings.append("\"%c%c%c\"" % (character, character2, character3))
    if character == characters[-1]:
        print "\t\t%s" % ', '.join(strings)
    else:
        print "\t\t%s," % ', '.join(strings)
print "\t};"
