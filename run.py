import os
print('Input the test case you want to run!')
x = int(input())

os.system('./clean.sh')

for i in range(1, 11):
    com = 'rm '+str(i)+'.out'
    os.system(com)
    print('Running test data', i)
    com = 'time ./code < ComplexDataSet/testcase' + str(x) + '/'+ str(i) + '.in >> '+str(i)+'.out'
    os.system(com)
    print("Diffing Output")
    com = 'diff ' + str(i)+'.out '+'ComplexDataSet/testcase'+str(x)+'/'+str(i)+'.out' 
    os.system(com)

os.system('./clean.sh')

