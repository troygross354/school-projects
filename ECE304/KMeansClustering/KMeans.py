#ECE 304 - K Means Clustering
import sys
import math
import numpy as np
import matplotlib.pyplot as plt
import random
import test_data

''' TODO '''
# 1. Change all euclidian distances to manhattan because it's backwards right now
# 2. Fix everthing that restricts the ability to process n-dimensions


# Number of clusters
k = 5
# Dimension of the data
dim = 2
# Boolean value to whether the initial cluster values are chosen randomly
# True = random and False = first k points
random_points = True

# Simple class to store data point information
class DataPoint:
	clust_count_euc = [0] * k
	clust_count_man = [0] * k

	def __init__(self, coord_array):
		self.coord_array = coord_array
		self.euc_cluster = -1
		self.man_cluster = -1

	def set_clust_euc(self, clust):
		# First decrement original cluster count
		if(self.euc_cluster != -1):
			DataPoint.clust_count_euc[self.euc_cluster] -= 1
		# Assign new cluster
		self.euc_cluster = clust
		# Increment new cluster count
		DataPoint.clust_count_euc[self.euc_cluster] += 1

	def get_clust_euc(self):
		return self.euc_cluster

	def get_point(self, by_coord=False, index=0):
		if by_coord:
			return float(self.coord_array[index])
		else:
			return self.coord_array

	def set_point(self, index, value):
		self.coord_array[index] = value

	def Count_Euc(index):
		return DataPoint.clust_count_euc[index]

# Function to calculate the Euclidian distance between two n-dimensional points
def euclidian_dist(point1, point2):
	euc_dist = 0
	for c1, c2 in zip(point1, point2):
		euc_dist += math.pow(c1 - c2, 2)
	return math.sqrt(euc_dist)

# Function to calculate the Manhattan distance between two n-dimensional points
def manhattan_dist(point1, point2):
	man_dist = 0
	for c1, c2 in zip(point1, point2):
		man_dist += abs(c1 - c2)
	return man_dist

# Re-fits the centers to the average of all of
# the data points within their respective clusters
def Calc_Centers(center_list, data_list):
	new_list =[]
	for i, c in enumerate(center_list):
		# Find average point of all points for every cluster
		for coord in range(dim):
			total = 0
			for dp in data_list:
				if(dp.get_clust_euc() == i):
					total += dp.get_point(by_coord=True, index=coord)
			total = total / float(DataPoint.Count_Euc(i))
			c.set_point(index=coord, value=total)
		new_list.append(c)
	return new_list

def Print_Centers(center_list):
	for i, c in enumerate(center_list):
		sys.stdout.write("Center " + str(i) + ": (" + str(c.get_point()) + ", ")
		print(")")

def Display_Plot(centers, data_list):
	# Process data to be graphed (List of numpy arrays)
	clust_plot_x = []
	clust_plot_y = []
	cent_plot_x = []
	cent_plot_y = []
	for i in range(k):
		# Arrays to hold the x and y values
		clust_plot_x.append(np.zeros(DataPoint.Count_Euc(i)))
		clust_plot_y.append(np.zeros(DataPoint.Count_Euc(i)))
		index = 0
		for point in data_list:
			if(point.get_clust_euc() == i):
				clust_plot_x[i][index] = point.get_point(by_coord=True, index=0)
				clust_plot_y[i][index] = point.get_point(by_coord=True, index=1)
				index += 1
		for point in centers:
			cent_plot_x.append(point.get_point(by_coord=True, index=0))
			cent_plot_y.append(point.get_point(by_coord=True, index=1))


	for i in range(k):
		plt.plot(clust_plot_x[i], clust_plot_y[i], 'o')
		plt.plot(cent_plot_x[i], cent_plot_y[i], 'kx')
	plt.show()

# Function to calculate the Manhattan distance between two n-dimensional points

# Grade data
tr_data = test_data.data2()

# Fill data array with DataPoint objects

dp_list = []
for dp in tr_data:
	dp_list.append(DataPoint(dp))

# Acquire training data
n = tr_data.shape[0]
#dim = None


# n-dimensional data (ROWS = number of data points	COLS = number of dimensions
#t_data = np.zeros(n, dim)

# Assign the t_data array from the input data "file"

# Pick K unique random points between 0 and N-1 to be initial cluster centers
euc_centers = []
for i in range(k):
	if random_points:
		rand_ind = random.randint(0, n-1)
		while rand_ind in euc_centers:
			rand_ind = random.randint(0, n-1)
		euc_centers.append(dp_list[rand_ind])
	else:
		euc_centers.append(dp_list[i])

man_centers = euc_centers

print("Before cluster fitting:")
Print_Centers(euc_centers)
''' TRAINING SECTION '''
# Calculate distances of every point relative to the K points for 3 epochs
for epoch in range(3):
	# Determine which center distance is the shortest for each data point
	for point in dp_list:
		min_euc = float("inf")
		min_man = float("inf")
		for j, c in enumerate(euc_centers):
			euc_dist =  euclidian_dist(c.get_point(), point.get_point())
			#man_dist = None # TODO: Calculate the actual manhattan distance
			if(euc_dist < min_euc):
				min_euc = euc_dist
				point.set_clust_euc(j)
			'''if(man_dist < min_man):
				min_man	= man_dist
				point.man_cluster = j'''
	Display_Plot(euc_centers, dp_list)
	# Calculate the new centers each cluster by by averaging all of the coordinates
	centers = Calc_Centers(euc_centers, dp_list)

print("\nAfter cluster fitting:")
Print_Centers(centers)

# Display final plot
Display_Plot(euc_centers, dp_list)

''' CLASIFICATION '''
# Classify test data
# Display plot containing labeled test data




