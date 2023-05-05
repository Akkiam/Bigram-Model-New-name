import random
from collections import defaultdict, Counter
import pandas as pd
#reading names from file
def read_data(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        names = [name.strip() for name in file.readlines()]
    return names


#create bigrams
def create_bigram_map(names):
    bigram_map = defaultdict(list)
    for name in names:
        prev = '^'
        for char in name:
            bigram_map[prev].append(char)
            prev = char
        bigram_map[prev].append('$')
    return bigram_map


#generating new name
def generate_name(bigram_map):
    name = ""
    prev = "^"
    next_char = ""
    while next_char != "$":
        next_char = random.choice(bigram_map[prev])
        if next_char != "$":
            name += next_char
        prev = next_char
    return name


#find probabilities of bigrams
def bigram_probabilities(bigram_map):
    prob_map = {}
    for key, value in bigram_map.items():
        total = len(value)
        prob_map[key] = {char: count / total for char, count in Counter(value).items()}
    return prob_map


#create bigram probability table
def visualize_bigram_probabilities(prob_map):
    df = pd.DataFrame(prob_map).fillna(0)
    return df

#create bigram probability table and write to file
def visualize_bigram_probabilities_file(prob_map, output_path):
    df = pd.DataFrame(prob_map).fillna(0)
    df.to_csv(output_path, sep='\t', index=False)
    return df


file_path = "names.txt"
output_path = "bigram_probabilities.txt"
names = read_data(file_path)
bigram_map = create_bigram_map(names)
prob_map = bigram_probabilities(bigram_map)
new_name = generate_name(bigram_map)
df = visualize_bigram_probabilities(prob_map)
#output new name
print("Generated name:", new_name)


#output table
df = visualize_bigram_probabilities_file(prob_map, output_path)
print("\nBigram probabilities:")
print(df)

