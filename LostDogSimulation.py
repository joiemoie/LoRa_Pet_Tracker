import random
import numpy
import sys
import csv

file = open('dog_data.csv', 'wb')

num_iterations = 100
results = numpy.zeros((num_iterations, 2))
for it in range(num_iterations):
  size_array = 100

  array = numpy.zeros((size_array,size_array))
  num_owners = random.randint(1,20)

  count = 0
  for x in range(num_owners):
    x_rand = random.randint(0,size_array)
    y_rand = random.randint(0,size_array)
    for y in range(5):
      for z in range(5):
        new_x = x_rand - y + 2
        new_y = y_rand - z + 2
        if (new_x >= 0 and new_x < size_array):
          if (new_y >= 0 and new_y < size_array):
            array[new_x][new_y] = 1


  for x in range(size_array):
    for y in range(size_array):
      if (array[x][y] == 1):
        count += 1

  result = float(count) / (size_array * size_array)
  results[it][0] = num_owners
  results[it][1] = result
  writer = csv.writer(file)
  writer.writerow(results[it])


