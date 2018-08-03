#!/usr/bin/python

import sys, timeit

def usec(function, arguments):
    modname, funcname = __name__, function.__name__
    timer = timeit.Timer(stmt='%(funcname)s(*args)' % vars(),
                         setup='from %(modname)s import %(funcname)s; args=%(arguments)r' % vars())
    try:
        t, N = 0, 1
        while t < 0.2:            
            t = min(timer.repeat(repeat=3, number=N))            
            N *= 10
        microseconds = round(10000000 * t / N, 1) # per loop
        return microseconds 
    except:
        timer.print_exc(file=sys.stderr)
        raise

def sieve():
    int_list = []
    prime_list = []

    for i in range(0, upper_lim):
        int_list.append(i)

    i = 2
    while(i*i < upper_lim):
        if(int_list[i] != -1):
            j = i*2
            while(j < upper_lim):
                int_list[j] = -1
                j+=i
        i+=1

    prime_counter = 0
    i = 2
    while(i < upper_lim):
        if(int_list[i] != -1):
            prime_list.append(int_list[i])
            prime_counter += 1
        i+=1

    #print("Number: " + str(prime_counter))

    fp = open("output.txt", 'w')
    for i in prime_list:
        fp.write(str(i) + ", ")

    fp.close()

done = False

while done != True:
    upper_lim = raw_input('Enter the upper limit of primes to find: ')

    try:
        val = int(upper_lim)
    except ValueError:
        print("Please enter an integer")
        continue
    
    upper_lim = int(upper_lim)

    if (upper_lim == 0 or upper_lim <= 2):
        print("Please enter an integer greater than 2")
        continue

    done = True

print usec(sieve, [])