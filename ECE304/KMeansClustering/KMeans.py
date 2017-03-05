#ECE 304 - K Means Clustering
import sys
import numpy as np
import matplotlib.pyplot as plt
import random

# Number of clusters
k = 5
# Dimension of the data
dim = 1
# Boolean value to whether the initial cluster values are chosen randomly
# True = random and False = first k points
random_points = True

# Simple class to store data point information
class DataPoint:
	clust_count_euc = [0] * k
	clust_count_man = [0] * k

	def __init__(self, coord_array):
		self.coord_array = np.ndarray(shape=(1, len(coord_array)), buffer=np.array(coord_array), dtype=float)
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
	def get_point(self, index=0):
		return float(self.coord_array[0])
	def set_point(self, index, value):
		self.coord_array[index] = value
	def Count_Euc(index):
		return DataPoint.clust_count_euc[index]

# Function to calculate the Euclidian distance between two n-dimensional points
def euclidian_dist(point1, point2):
	if point1.size != point2.size:
		return "Point dimensions don't match\n"
	euc_dist = 0
	for c1, c2 in zip(point1, point2):
		euc_dist = abs(c1 - c2)
	return euc_dist

# Re fits the centers to the center of all of
# the data points within their respective clusters
def Calc_Centers(center_list, data_list):
	new_list =[]
	for i, c in enumerate(center_list):
		# Find average point of all points for every cluster
		for coord in range(dim):
			total = 0
			for dp in data_list:
				if(dp.get_clust_euc() == i):
					total += dp.get_point(coord)
			total = total / DataPoint.Count_Euc(i)
			c.set_point(index=coord, value=total)
		new_list.append(c)
	return new_list

def Print_Centers(center_list):
	for i, c in enumerate(center_list):
		sys.stdout.write("Center " + str(i) + ": (")
		for i in range(dim):
			sys.stdout.write(str(c.get_point(i)) + ", ")
		print(")")

# Function to calculate the Manhattan distance between two n-dimensional points

# Grade data
tr_data = np.array([
	83.70,
	82.04,
	81.49,
	86.95,
	79.09,
	85.12,
	85.61,
	86.30,
	79.89,
	47.55,
	88.67,
	74.57,
	90.90,
	92.63,
	74.33,
	80.58,
	75.20,
	79.24,
	79.68,
	86.88,
	73.43,
	84.63,
	85.82,
	77.60,
	63.84,
	75.80,
	71.18,
	82.03,
	66.13,
	83.32,
	76.75,
	70.38,
	79.03,
	47.89,
	86.99,
	93.85,
	88.83,
	84.01])

# Fill data array with DataPoint objects

dp_list = []
for dp in tr_data:
	dp_list.append(DataPoint([dp]))

# Acquire training data
n = tr_data.size
#dim = None


# n-dimensional data (ROWS = number of data points	COLS = number of dimensions
#t_data = np.zeros(n, dim)

# Assign the t_data array from the input data "file"

# Pick K unique random points between 0 and N-1 to be initial cluster centers
centers = []
for i in range(k):
	if random_points:
		rand_ind = random.randint(0, n-1)
		while rand_ind in centers:
			rand_ind = random.randint(0, n-1)
		centers.append(dp_list[rand_ind])
	else:
		centers.append(dp_list[i])

print("Before cluster fitting:")
Print_Centers(centers)
''' TRAINING SECTION '''
# Calculate distances of every point relative to the K points for 3 epochs
for epoch in range(1000):
	# Determine which center distance is the shortest for each data point
	for point in dp_list:
		min_euc = float("inf")
		min_man = float("inf")
		for j, c in enumerate(centers):
			euc_dist =  euclidian_dist(c.coord_array, point.coord_array)
			#man_dist = None # TODO: Calculate the actual manhattan distance
			if(euc_dist < min_euc):
				min_euc = euc_dist
				point.set_clust_euc(j)
			'''if(man_dist < min_man):
				min_man	= man_dist
				point.man_cluster = j'''
	# Display plot with with old centers
	# Calculate the new centers each cluster by by averaging all of the coordinates
	centers = Calc_Centers(centers, dp_list)

print("\nAfter cluster fitting:")
Print_Centers(centers)
# Process data to be graphed (List of numpy arrays)
clust_plot = []
cent_plot = []
for i in range(k):
	#print("Cluster " + str(i) + " size = " + str(DataPoint.Count_Euc(i)) + "\n")
	clust_plot.append(np.zeros(DataPoint.Count_Euc(i)))
	index = 0
	for point in dp_list:
		if(point.get_clust_euc() == i):
			clust_plot[i][index] = point.get_point()
			index += 1
	for point in centers:
		cent_plot.append(np.array(point.get_point(0)))


for i in range(k):
	plt.plot(clust_plot[i], np.zeros(clust_plot[i].size), 'o')
	plt.plot(cent_plot[i], np.zeros(cent_plot[i].size), 'ko')
plt.show()
# Display plot with new and old cluster centers

''' CLASIFICATION '''
# Classify test data
# Display plot containing labeled test data




