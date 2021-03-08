import csv
import matplotlib.pyplot as plt
import numpy as np

data_file = '../program_output.txt'
result_file = '../images/'

def plot_cte_data():
    data =  list(csv.reader(open(data_file)))
    data_array = np.array(data)
    
    cte = (data_array[:,0]).astype(np.double)
    #speed = (data_array[:,1]).astype(np.double) 
    #steering = (data_array[:,2]).astype(np.double) 
    
    fig = plt.figure(figsize=(28, 16))
    plt.rc('xtick', labelsize=20)    # fontsize of the tick labels
    plt.rc('ytick', labelsize=20)    # fontsize of the tick labels
    plt.plot (cte)
    #plt.plot (steering)
    
    plt.title('Cross Track Error - CTE', fontsize = 24)           
    plt.grid(b=True, which='major', color='#666666', linestyle='-', alpha=0.6)        
    plt.minorticks_on()
    plt.grid(b=True, which='minor', color='#999999', linestyle='-', alpha=0.2)
            
    plt.savefig(result_file + 'plot_cte.jpg')
    
    print ('Plot stored at ' + result_file + 'plot_cte.jpg' )
    
def plot_speed_data():
    data =  list(csv.reader(open(data_file)))
    data_array = np.array(data)
    
    speed = (data_array[:,1]).astype(np.double) 
    #steering = (data_array[:,2]).astype(np.double) 
    
    plt.figure(figsize=(28, 16))
    plt.rc('xtick', labelsize=20)    # fontsize of the tick labels
    plt.rc('ytick', labelsize=20)    # fontsize of the tick labels
    plt.plot (speed)
    #plt.plot (steering)
    
    plt.title('Speed ', fontsize = 24)           
    plt.grid(b=True, which='major', color='#666666', linestyle='-', alpha=0.6)        
    plt.minorticks_on()
    plt.grid(b=True, which='minor', color='#999999', linestyle='-', alpha=0.2)
            
    plt.savefig(result_file + 'plot_speed.jpg')
    
    print ('Plot stored at ' + result_file + 'plot_speed.jpg' )
    
def plot_steering_data():
    data =  list(csv.reader(open(data_file)))
    data_array = np.array(data)
    
    steering = (data_array[:,2]).astype(np.double) 
    
    plt.figure(figsize=(28, 16))
    plt.rc('xtick', labelsize=20)    # fontsize of the tick labels
    plt.rc('ytick', labelsize=20)    # fontsize of the tick labels
    plt.plot (steering)
    
    plt.title('Steering Angle ', fontsize = 24)           
    plt.grid(b=True, which='major', color='#666666', linestyle='-', alpha=0.6)        
    plt.minorticks_on()
    plt.grid(b=True, which='minor', color='#999999', linestyle='-', alpha=0.2)
            
    plt.savefig(result_file + 'plot_steering.jpg')
    
    print ('Plot stored at ' + result_file + 'plot_steering.jpg' )

def plot_data():
    data =  list(csv.reader(open(data_file)))
    data_array = np.array(data)
    
    cte = (data_array[:,0]).astype(np.double)    
    speed = (data_array[:,1]).astype(np.double)
    steering = (data_array[:,2]).astype(np.double) 
    
    fig, axes = plt.subplots(nrows=3, ncols=1, sharex=True, figsize=(28, 20))
    plt.subplots_adjust(wspace=0.01, hspace=0.01)
    axes[0].plot (cte)
    
    axes[0].set_title('Cross Track Error - CTE ', color='blue', fontsize = 24)           
    axes[0].grid(b=True, which='major', color='#666666', linestyle='-', alpha=0.6)        
    axes[0].minorticks_on()
    axes[0].grid(b=True, which='minor', color='#999999', linestyle='-', alpha=0.2)
    
    axes[1].plot (steering, color='red')    
    axes[1].set_title('Steering Angle ', color='red', fontsize = 24)           
    axes[1].grid(b=True, which='major', color='#666666', linestyle='-', alpha=0.6)        
    axes[1].minorticks_on()
    axes[1].grid(b=True, which='minor', color='#999999', linestyle='-', alpha=0.2)
            
    axes[2].plot (speed, color='green')    
    axes[2].set_title('Speed ', color = 'green' , fontsize = 24)           
    axes[2].grid(b=True, which='major', color='#666666', linestyle='-', alpha=0.6)        
    axes[2].minorticks_on()
    axes[2].grid(b=True, which='minor', color='#999999', linestyle='-', alpha=0.2)
    
                      
    plt.savefig(result_file + 'plot_vehicle_data.jpg')
    
    print ('Plot stored at ' + result_file + 'plot_vehicle_data.jpg' )
    
if __name__ == '__main__':
    #plot_cte_data()
    #plot_speed_data()
    #plot_steering_data()
    plot_data()