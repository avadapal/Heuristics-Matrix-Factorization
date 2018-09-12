#include<iostream>
#include<random>
#include <queue>
#include <algorithm>
#include <iterator>
#include<functional>
#include<vector>
#include<fstream>
#include<string>
#include<assert.h>
#include "util.h"
#include "matrix-factorization.h"

using namespace std;

 

int main(int argc, char* args[]){

	

	/*std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << args[i] << std::endl;
    }*/

ifstream file(args[1]);


 /*std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << args[i] << std::endl;
    }*/


string content;

int n_users   = 0;
int n_items   = 0;
int n_query   = 0;
int count     = 0;
int rat       = 0;
int M[2][100006];

string old_content;

while(file >> content) {

	vector<string> splitted = split(content, ";");

	if(count == 0) n_users = stoi(splitted[0]);

	if(count == 1) n_items = stoi(splitted[0]);

	if(count == 2) n_query = stoi(splitted[0]);

	if(count >=3){
		splitted = split(content, ",");
		{	
		M[0][rat] = stoi(splitted[0]);
		assert(M[0][rat] <= n_users);
		}

		{

		M[1][rat] = stoi(splitted[1]);
		assert(M[1][rat] <= n_items);
		rat++;
		}	
	}

count++;
	
}

//cout << "count = " << count << endl;

cout << "Number of users: " << n_users << endl;
cout << "Number of items: " << n_items << endl;
cout << "Query Size: " << n_query << endl;

double Gamma  =  0.0015; //atof(args[6]); 
double Mu     = 1.0; 
double Lambda = 1.0; 

int n = n_users;
int m = n_items;

double** RatingMatrix;

RatingMatrix = new double* [n + 1];

for(int i = 0; i < n + 1 ; i++){
	RatingMatrix[i] = new double[m + 1];
	for(int j = 0; j < m + 1; j++){
		RatingMatrix[i][j] = 0;
	}
}

vector<vector<int>> UserItems(n + 1, vector<int>(m + 1)); // This two dimensional array stores (i,j): Number of items user i was recommended item j.

for(int i = 1; i <= n; i++){

	for(int j = 1; j <= m; j++){

		UserItems[i][j] = 0;
	}
}

cout << "n = " << n << endl;
cout << "m = " << m << endl;
int profile_size = 10;  
cout << "The Profile size is: " << profile_size << endl;

for(int aa = 1; aa <= 1; aa++){

	vector<vector<double>> item_profiles(m + 1);
	vector<vector<double>> user_profiles(n + 1);

	initialize_with_random_profiles(user_profiles, item_profiles, m, n, profile_size);
	
	int n_servers = 2;
	double factor = 1/double(n_servers);

	initialize_with_random_profiles(user_profiles, item_profiles, m, n, profile_size);
	
	for(int ii = 0; ii <= 220; ii++){

//		cout << "ii = " << ii << endl;
	
		if (ii < 100)  n_servers = 1;
		if (ii >= 100) n_servers = atoi(args[2]); 
		factor = 1/double(n_servers);
	
		for(int server = 0; server < n_servers; server++){

		int n_queries =  n_query; 
		double r = 	atof(args[3]);
		std::vector<vector<int>> user_purchases  (n + 1);
		std::vector<vector<int>> item_purchasers (m + 1);	
		pick_random_subset(user_purchases, item_purchasers, M, rat, server, r,n_servers);
		run_gradient_descent(user_purchases, item_purchasers, user_profiles, item_profiles, n, m, n_query, Gamma, Mu, Lambda, profile_size);		
		}		
	}

	std::vector<vector<int> > user_purchases (n + 1);
	std::vector<vector<int> > item_purchasers (m + 1);
	pick_random_subsetP(user_purchases, item_purchasers, M, rat);
	distance_to_optimality(user_profiles, item_profiles, item_purchasers);	

	vector<vector<double>> PredictedMatrix(n + 1, vector<double>(m + 1));

	for(int i = 1; i <=n; i++){

		for(int j = 1; j <=m; j++){
			double dot = dot_product(item_profiles[j], user_profiles[i]);
			PredictedMatrix[i][j] = dot;			
		}
	}

	cout << "f";

	updatePredictionMatrix(PredictedMatrix, UserItems, n, m);
}


cout << "============================================" << endl;

for(int  i = 1; i <= n; i++){

	for(int j = 1; j <=m; j++){

		if(UserItems[i][j] != 0){

			cout << i << " , " << j << " : " << UserItems[i][j] << endl;
		}		
	}
}
cout << "============================================" << endl; 



}