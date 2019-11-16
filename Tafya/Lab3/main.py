import re
import pprint
import argparse
import fileinput

class Node:
    rules = {}
    variants = []

    @staticmethod
    def replace(char, input_string, unfolded_string):

        if char not in Node.rules:
            unfolded_string += input_string[0]
            input_string = input_string[1:]
            if len(input_string) == 0:
                return unfolded_string
            unfolded_string = Node.replace(input_string[0], input_string, unfolded_string)
        else:
            #for substitute in Node.rules[char]:
            input_string = input_string.replace(char,Node.rules[char][0])
            unfolded_string = unfolded_string + input_string
            #if set(unfolded_string).issubset(P):
            #    break
            #if len(input_string) == 0:
            #    break
                #unfolded_string = Node.replace(unfolded_string[position], unfolded_string, unfolded_string)
        return unfolded_string

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-s' , '--string' , action='store')
    parser.add_argument('-r' , '--rules' , action='store')
    return parser.parse_args()

def save_stack_state(stack):
    stack_states.append(stack.copy())

def get_stack_state():
    return stack_states.pop()

rule_re = re.compile("(.)>(.*)")
args = get_args()
stack_states = []

stack = []

for line in fileinput.input(args.rules):
    r_match = rule_re.match(line.rstrip())
    Node.rules[r_match.group(1)] = r_match.group(2).split('|')

P = set()
Z = set()
for s in Node.rules:
    for p in Node.rules[s]:
        for char in p:
            if char not in Node.rules:
                P.add(char)
            Z.add(char)
    Z.add(s)

print('-----')
print('Rules')
pprint.pprint(Node.rules)
print('-----')
print('  P')
pprint.pprint(P)
print('-----')
print('  Z')
pprint.pprint(Z)
print('-----')

unfolded_string = Node.replace(args.string[0], args.string, '')
i = 0
while True:
    i += 1
    print(unfolded_string)
    if not set(unfolded_string).issubset(P):
        unfolded_string = Node.replace(unfolded_string[0],unfolded_string,'')
    else:
        print('Строка допустима')
        break
    if i > 1000000:
        print('Строка недопустима')
        break
