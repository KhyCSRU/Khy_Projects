"""
    Author: Khylei Neal
    This program will take in basic geographic data on 128 cities in North America, then respond with the information about those cities.
"""
from sys import argv
import networkx as nx
from networkx.algorithms import approximation as approx


class Gis:
    """

    """

    
    def __init__(self):
        """
            Returns:
            Parameter:
        """
        self.cities = [] # list of cities
        self.edges = [] # list of edges
        self.select_city = set()
        self.select_edge = set()
        # Empty graph
        self.graph1 = nx.Graph()
        # reading from gis file (City, State, long/lati, distances from the two previous cities)
        with open('gis.dat', 'r') as gisfile:
            for line in gisfile.readlines():
                line.strip()
                if not line.startswith('*'):    
                    if line[0].upper() in "ABCDEFGHIJKLMNOPQRSTUVWXYZ":
                        city_state, popu_coord= line.split('[') # Splitting the name and state of cities and the data
                        name, st = city_state.split(', ') # Splitting the name and state
                        coord , popu = popu_coord.split(']') # Splitting data
                        self.cities.append(name)
                        popu = int(popu)
                        lati, long = coord.split(',') # Splitting coordinates
                        lati = float(lati)/100 # latitude
                        long = float(long)/ 100 # longitude 
                        self.graph1.add_node(name, state = st, population = popu, latitude = lati, longitude = long)
                    if line[0].isdigit():
                        cnt = 0
                        edgeW = line.split()
                        rev = edgeW[::-1]
                        for w in rev:
                            self.graph1.add_edge(name, self.cities[cnt], weight = int(w))
                            cnt += 1
        self.graph2 = nx.Graph()
        gisfile.close()                            


    def selectCities(self, attribute, lowerbound, upperbound = None):
        """
            Function: This function will take in the parameters described
                      and adds the cities that fall between those parameters into a graph as nodes(like a filter)
                      
            Returns: None
            
            Parameter: self
                       attribute: The piece of data that you would like to filter the cities by
                       lowerbound: Passes a lowerboundery for which cities fall under
                       upperbound: Passes a upperboundery for which cities fall under 
        """
        city_filter = self.graph2
        cities = []
        for c, v_dict in city_filter.nodes.items():
            if attribute == 'name':
                    if lowerbound.upper() <= c.upper() <= upperbound.upper():
                        cities.append(c)
            cities = sorted(cities)
            if attribute == 'state':
                st = v_dict[attribute]
                if lowerbound.upper() == st:
                    cities.append(c)
            if attribute == 'latitude':
                lt = v_dict[attribute]
                if lowerbound <= lt <= upperbound:
                    cities.append(c)
            if attribute == 'longitude':
                ln = v_dict[attribute]
                if lowerbound <= ln <= upperbound:
                    cities.append(c)
            if attribute == 'population':
                p = v_dict[attribute]
                if lowerbound <= p <= upperbound:
                    cities.append(c)
        self.graph2.remove_nodes_from([n for n in self.graph2 if n not in set(sorted(cities))])
    def selectAllCities(self):
        """
            Function: This function will select all of the
                      cities from the gis.dat file and add them as nodes
                      in the graph
                      
            Returns: None
            
            Parameter: self
        """
        # selecting all of the cities
        self.graph2.add_nodes_from(self.graph1.nodes(data = True))

        
    def unselectAllCities(self):
        """
            Function: This function will unselect all of the
                      cities from the gis.dat file by clearing the graph
                      
            Returns: None
            
            Parameter: self
        """
        # Un-selecting all of the cities
        self.graph2.clear()

        
    def selectEdges(self, lowerbound, upperbound):
        """
            Function: This function will select all of the
                      edges from the gis.dat file and add them as nodes
                      in the graph
                      
            Returns: None
            
            Parameter: self
                       lowerbound: Passes a lowerboundery for the distance or edge between two nodes or cities
                       upperbound: Passes a upperboundery for the distance or edge between two nodes or cities
        """
        # Select all edges with in the boundery
        edgelst= []
        for n, w in self.graph2.edges.items():
            if lowerbound <= w['weight'] <= upperbound:
                edgelst.append(n)
        self.graph2.remove_edges_from([n for n in self.graph2 if n not in set((edgelst))])
        

    def selectAllEdges(self):
        """
            Function: This function will add all the edges or
                      distances from one city or node to the next
                      
            Returns: None
            
            Parameter: self
        """
        # Selecting all edges between cities
        
        self.graph2.add_edges_from(self.graph1.edges(data = True))

        
    def unselectAllEdges(self):
        """
            Function: This function will remove all the edges or
                      distances from one city or node to the next
                      
            Returns: None
            
            Parameter: self
        """
        self.graph2.remove_edges_from(self.graph2.edges())

        
    def makeGraph(self):
        """
            Function: This function will create a graph of all cities as nodes
                      and distances between them as edges
                      
            Returns: The graph of all cities and distances
            
            Parameter: self
        """
        
        return self.Graph1

    
    def printCities(self,attribute = 'name', choice = 'S'):
        """
            Function: This function will print out the city information based off of arguments passed, and sort by the attribute given.
            
            Returns: None
            
            Parameter: attribute: The piece of data that you would like to sort the cities by
                       choice: Whether you would like a full representation of the cities information or a short representation
        """
        # Returns the sorted selected cities
        for c,v in sorted(self.graph2.nodes.items()):
            if attribute == 'name' or attribute == 'state':
                if choice == 'F':
                    print('{}, {}[{}, {}]{}'.format(c, self.graph2.nodes[c]['state'], self.graph2.nodes[c]['latitude'],self.graph2.nodes[c]['longitude'],self.graph2.nodes[c]['population']))
                else:
                    print('{}, {}'.format(c,v['state']))
        dictattr = nx.get_node_attributes(self.graph2,attribute)
        sortD = sorted(dictattr.items(), key=lambda x:x[1])
        #sortD = sortD[::-1] #reverse lst
        sortedD = dict(sortD)
        for i,j in sortedD.items():
            if choice == 'F':
                print('{}, {}[{}, {}]{}'.format(i,self.graph2.nodes[i]['state'], self.graph2.nodes[i]['latitude'],self.graph2.nodes[i]['longitude'],self.graph2.nodes[i]['population']))
            else:
                print('{}, {}'.format(i, self.graph2.nodes[i]['state']))

                
    def printPopulationDistr(self, split = 20000):
        """
            Function: This function will print out the population distribution of the cities
            
            Returns: None
            
            Parameter: self
                       split - This is just a population splitter to sort the cities in their respective population range
        """
        d = {}
        final_dict = {}
        lst = []
        for i in self.graph2.nodes(data = 'population'):
            d[i[0]] = i[1]
        if len(d.values()) == 0:
            print("No cities found")
            return 0
        else:
            num = max(d.values())
            c = num // split
        for i in range(c+2):
            lst.append(split * i)
        for i in range(len(lst)-1):
            cnt = 1
            for k,v in d.items():
                p = lst[i+1]
                
                if lst[i]<= v <= lst[i+1]:
                    m = '['+str(lst[i])+ ' - ' + str(lst[i+1])+ ']'
                    final_dict[m] = cnt
                    cnt += 1
        for key,val in final_dict.items():
            print('{} : {}'.format(key,val))
        
            

    def printPopulatedStates(self, num):
        """
            Function: This function will go through the nodes and find out the (num) most populated states
            
            Returns: None
            
            Parameter: num: The number of most populated cities you would like to print
        """
        print('{} most populated states.'.format(num))
        b = '-'* 40
        print(b)
        emptypop = {}
        for i in self.graph1.nodes(data = 'state'):
            sum1 = 0
            self.selectAllCities()
            x = self.selectCities('state', i[1])
            for k, j in nx.get_node_attributes(self.graph2,'population').items():
                    sum1 += j
            emptypop[i[1]] = sum1
        v = sorted(emptypop.values(),reverse = True)
        num1 = 0
        while num1 != num:
            for key,val in emptypop.items():
                if num1 != num:
                    if emptypop[key] == v[num1]:
                        print("{} {}".format(key,v[num1]))
                        num1 +=1
                
    def testMinMaxConsDistance(self):
        """
            Function: This function will find the maximum path and take the minimum edge out of that path
            
            Returns: The shortest edge out of the largest weighted path
            
            Parameter: self
        """
        usr_inp = 'a'
        while usr_inp != '':
            usr_inp = 'a'
            try:
                if usr_inp == 'a':
                    
                    self.selectAllCities()
                    self.selectAllEdges()
                    usr_inp = input("Please enter a source and a target in the format (<source,target>)")
                    x = usr_inp.split(',')
                    source = x[0]
                    lst = []
                    target = x[1]
                    if source in self.graph2.nodes() and target in self.graph2.nodes():
                        y = nx.all_simple_paths(self.graph2, source, target)
                        b = list(nx.shortest_simple_paths(self.graph2, source, target))
                        dict_path = nx.get_edge_attributes(self.graph2,'weight')
                        highestP = (b[-1])
                        for i in range(len(highestP)-1):
                            if i != len(highestP)+1:
                                dictW = self.graph2.get_edge_data(highestP[i],highestP[i+1], 'weight')
                                for i,j in dictW.items():
                                    lst.append(j)
                        print('Source (City, State): {}, {}'.format(source,self.graph2.nodes[source]['state']))
                        print('Target (City, State): {}, {}'.format(target, self.graph2.nodes[target]['state']))
                        print('')
                        print('Cost of optimal solution: {}'.format(min(lst)))
            except:
                print('')
            

    def tour(self,start):
        """
            Function: This function will print out the path from the start point
            
            Returns:
            
            Parameter: start - Just the starting city and state of where you want to start the salesman tour 
        """
        try:
            lst = start.split(',')
            cycle = approx.greedy_tsp(self.graph2, source= lst[0])
            weight_dict = nx.get_edge_attributes(self.graph2,'weight')
            empty = ''
            for i in range(len(cycle)):
                if cycle[-1] == cycle[i] and i != 0:
                    empty += '\n' + str(cycle[i])
                else:
                    empty += str(cycle[i])+ ' --> '
            print("Traveling Salesman Tour starting from {} is as follows.".format(start))
            print()
            print(empty)
            print()
            x = nx.path_weight(self.graph2, cycle, weight="weight")
            print('Tour length: {}'.format(x))
            
        except:
            print('Traveling Salesman Tour starting from {} is not possible'.format(start))
            
    def minCut(self):
        """
            Function: This function will run the stoer wagner alg to find the minimum cut and the weight
            Returns:
            Parameter:
        """
        cut, part = nx.stoer_wagner(self.graph2)
        cutE = ''
        #for n1 in part[0]:
         #   for n2 in part[1]:
          #      if self.graph2.has_edge(n1, n2):
        cutE += '(' + str(part[0]) + ' ' + str(part[1]) + ')'
        final = "The edges in min-cut are as follows\n\n" + cutE + "\n\nWeight of the min-cut: "
        print('{} {}'.format(final, cut))
if __name__ == '__main__':
    g = Gis()
    g.selectAllCities()
    g.selectAllEdges()
    g.selectCities('state', 'VA')
    g.selectCities('name', 'R', 'V')
    g.printCities()
    g.selectAllCities()

    g.printCities('longitude','S')
    
    
    
    


            
            
        
