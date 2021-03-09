import csv
import matplotlib.pyplot as plt
import numpy as np
#Helper script for generating the plot_speed

#configure folders
data_file = '../program_output.txt'
result_file = '../images/'
data_path = '../logs/'

#plot the cte based on the list of data files
def plot_cte_from_multiple_files(data_files):
    cte = [0 for i in range(len(data_files))]
    index = 0
    mse = [0 for i in range(len(data_files))]
    for data_file in data_files:
        #open the file
        csv_file  = open(data_path + data_file)
        csv_reader = csv.reader (csv_file)
        #skip next 3 lines
        next(csv_reader)
        next(csv_reader)
        next(csv_reader)
        #load the data
        data = list(csv_reader)
        data_array = np.array(data)
        cte[index] = (data_array[:,0]).astype(np.double)
        mse[index] = (cte[index]**2).mean()
        index = index + 1
    #plot the graph
    plt.figure(figsize=(20, 12))
    plt.rc('xtick', labelsize=20)    # fontsize of the tick labels
    plt.rc('ytick', labelsize=20)    # fontsize of the tick labels
    for i in range(index):
        #prepare the legends
        name = data_files[i][:-4].split('_')
        label = 'Kp = {:.2f} Ki = {:.5f} Kd = {:.2f} MSE = {:.4f}'.format(float(name[1]),float(name[2]),float(name[3]), mse[i]) #Kp = {:4.2f} Ki = {:6.5f} Kd = {:4.2f} MSE: {:4.3}'.format(i)
        plt.plot (cte[i], label=label, linewidth=5)
    # Set the x axis label of the current axis.
    plt.xlabel('Time steps',  fontsize = 24)
    # Set the y axis label of the current axis.
    plt.ylabel('Cross Track Error' , fontsize = 24)
    plt.title('Cross Track Error - CTE', fontsize = 24)
    plt.grid(b=True, which='major', color='#666666', linestyle='-', alpha=0.6)
    plt.minorticks_on()
    plt.grid(b=True, which='minor', color='#999999', linestyle='-', alpha=0.2)
    plt.legend(loc="upper right", fontsize = 24)
    plt.savefig(result_file + 'plot_cte.jpg')
    # Save the plot
    print ('Plot stored at ' + result_file + 'plot_cte.jpg' )


if __name__ == '__main__':
    ## datafile for step 1:
    #data_files = ['output_0_0_0.txt']
    
    ## datafile for step 2:
    #data_files = ['output_0.1_0_0.txt', 'output_0.2_0_0.txt', 'output_0.15_0_0.txt']
    
    ## datafile for step 3:
    #data_files = ['output_0.15_0_2.0.txt', 'output_0.15_0_2.5.txt', 'output_0.15_0_2.75.txt', 'output_0.15_0_3.0.txt']
    
    ## datafile for step 4:
    data_files = ['output_0.15_0.0001_3.0.txt', 'output_0.15_0.0002_3.0.txt', 'output_0.15_0.00001_3.0.txt']
    
    plot_cte_from_multiple_files(data_files)
