import random

# Online Python - IDE, Editor, Compiler, Interpreter
error_rate = 0.1
allbuses = []

def gen_bus(period, length, firstbus):
    buses = []
    err = int(period * error_rate)
    
    for i in range(length):
        expected_time = i * period
        if random.randrange(0, 10) > 5:
            buses.append(firstbus + expected_time + random.randrange(-err +1, err -1))
        else:
            buses.append(firstbus + expected_time)
            
    return buses

allbuses += gen_bus(30, 6, 15) + gen_bus(40, 6, 20) + gen_bus(65, 6, 25) + gen_bus(20, 6, 10) + gen_bus(50, 6, 0)
allbuses.sort()
print(allbuses)
        