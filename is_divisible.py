import random
import numpy as np


def is_divisible(n, divisor):
    bucket_list = np.full(divisor, 0)
    for i in range(n):
        bucket_list[random.choice(np.arange(0, divisor))] += 1
    return np.all(bucket_list == bucket_list[0])


def better_is_divisible(n, divisor, iterations):
    for i in range(iterations):
        if is_divisible(n, divisor):
            return True
    return False


def test_is_divisible(test_integers, divisor, iterations):
    print("-" * len("Iterations | Divisor | Success rate"))
    for iteration in iterations:
        correct = 0
        for integer in test_integers:
            result = better_is_divisible(integer, divisor, iteration)
            if result and integer % divisor == 0:
                correct += 1
            elif not result and integer % divisor != 0:
                correct += 1
        print("{iteration:^11}|{divisor:^9}|{rate:^13}"
              .format(iteration=iteration, divisor=divisor, rate=str(correct/(len(test_integers)))))


test_numbers = np.arange(0, 100)
iterations = [1, 25, 50, 125, 250]
print("Iterations | Divisor | Success rate")
test_is_divisible(test_numbers, 2, iterations)
test_is_divisible(test_numbers, 3, iterations)
test_is_divisible(test_numbers, 4, iterations)
