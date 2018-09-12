#include<vector>
#include<iostream>
#include <fstream>

using namespace std;


void pick_random_subset(std::vector<vector<int> >& user_purchases, std::vector<vector<int> >& item_purchasers, int M[2][100006], 
                        int rat, int server, double rr, int n_servers){


    int  kk = 200; 

    int c = (rat - kk)/n_servers;

    for(int j = 1; j <= kk; j++){
        
        if(kk == 0) break;

        int u = M[0][j];
        int v = M[1][j];

        user_purchases[u].push_back(v);
        item_purchasers[v].push_back(u);

    }

    for(int i = kk + (server * c)  ; i < (server + 1) * (c) + kk; i++){

    //cout << server * c/2 << " <-> " << (server + 1) * (c) << endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
  
    double r = dis(gen);

    if(r > rr ) continue;

    int u = M[0][i];
    int v = M[1][i];

    user_purchases[u].push_back(v);
    item_purchasers[v].push_back(u);

    }
}

void pick_random_subsetP(std::vector<vector<int> >& user_purchases, std::vector<vector<int> >& item_purchasers, int M[2][100006], int rat){

    for(int i = 0 ; i < rat; i++){

        int u = M[0][i];
        int v = M[1][i];

        user_purchases[u].push_back(v);
        item_purchasers[v].push_back(u);
    }
}



void updatePredictionMatrix(std::vector<std::vector<double>>& PredictedMatrix, std::vector<std::vector<int>>& UserItems, int n, int m){

    for(int u = 1; u <= n; u++){
 
        std::priority_queue<std::pair<double, int>> q;
        for (int i = 1; i <=m; ++i) {
        q.push(std::pair<double, int>( PredictedMatrix[u][i], i));
        }


        const int k = 100; 

        auto index = new int[k];
  
        for (int i = 0; i < k; ++i) {
        int ki = q.top().second;
        index[i] = ki;
        q.pop();
        }

        sort(index, index + k);

        for (int i = 0; i < k; ++i) {
        //std::cout << "index[" << i << "] = " << index[i] << " " << PredictedMatrix[u][index[i]] << std::endl;
        UserItems[u][index[i]] += 1;
        //cout << u << " , " << index[i] << " : " << UserItems[u][index[i]] << endl;
       }
    }


}


void update_user_profiles(std::vector<std::vector<double>>& user_profiles, std::vector<std::vector<double>>& item_profiles, 
                                            std::vector<std::vector<int>> user_purchases, int n, double Gamma, double Lambda, int profile_size){

    //cout << "update_user_profiles" << endl;
    
    std::vector<double> sum_vec(profile_size, 0.0);
    
    for(int i = 1; i <= n; i++){

        intitialize_vector_zero(sum_vec);
        std::vector<double> Lambda_u_i = mul_vec(user_profiles[i], -2 * Lambda * Gamma);
        for(auto a: user_purchases[i]){
            
            double dot_product = dot_product_circuit(item_profiles[a], user_profiles[i], Gamma);

            std::vector<double> v = mul_vec(item_profiles[a] ,dot_product);     
            sum_vec = sum_vectors(v , sum_vec);     
            
        }
        sum_vec = sum_vectors(sum_vec, Lambda_u_i);
        user_profiles[i] = sum_vectors(user_profiles[i], sum_vec);
    }
    
}


void update_item_profiles(std::vector<std::vector<double>>& user_profiles, std::vector<std::vector<double>>& item_profiles, 
                                            std::vector<std::vector<int>> item_purchasers, int m, double Gamma, double Mu, int profile_size){

    //cout << "update item profiles" << endl;
    
    std::vector<double> sum_vec(profile_size, 0.0);

    for(int i = 1; i <=  m; i++){
        double sum = 0.0;
        intitialize_vector_zero(sum_vec);
   
        std::vector<double> Mu_v_i = mul_vec(item_profiles[i], -2 * Mu * Gamma);

        for(auto a: item_purchasers[i]){
            double dot_product = dot_product_circuit(item_profiles[i], user_profiles[a], Gamma);
            std::vector<double> v = mul_vec(user_profiles[a], dot_product);
            sum_vec = sum_vectors(v, sum_vec);
        }
        sum_vec = sum_vectors(sum_vec, Mu_v_i);
        item_profiles[i] = sum_vectors(item_profiles[i], sum_vec);
    }
    
}


void distance_to_optimality(std::vector<std::vector<double>>& user_profiles, std::vector<std::vector<double>>& item_profiles, 
                                            std::vector<std::vector<int>> item_purchasers){


    double dist = 0.0;
    for(int i = 1; i < item_purchasers.size(); i++){

        for(auto a: item_purchasers[i]){
            double liking = dot_product(item_profiles[i], user_profiles[a]);
            dist += (1 - liking) * (1 - liking);
        }
        

    }
    
    cout << "distance_to_optimality = " << dist << endl;
}



void initialize_with_random_profiles(std::vector<std::vector<double>>& user_profiles, std::vector<std::vector<double>>& item_profiles, int m, int n, int profile_size){


    for(int i = 1; i <= m; i++){
    item_profiles[i] = generate_random_vec(profile_size);
    double norm = squared_norm(item_profiles[i]);
    norm = sqrt(norm);
    double  factor = 1/double(norm);
    item_profiles[i] = mul_vec(item_profiles[i], factor);
    }

    for(int i = 1; i <=n; i++){
    user_profiles[i] = generate_random_vec(profile_size);
    double norm = squared_norm(user_profiles[i]);
    norm = sqrt(norm);
    double  factor = 1/double(norm);
    user_profiles[i] = mul_vec(user_profiles[i], factor);
    }

}

void run_gradient_descent(std::vector<vector<int>>& user_purchases, std::vector<vector<int>>& item_purchasers, std::vector<std::vector<double>>& user_profiles, 
                            std::vector<std::vector<double>>& item_profiles, int n, int m, int n_query, double Gamma, double Mu, double Lambda, int profile_size){

   // cout << "This function runs gradient descent on the following Ratings matrix" << endl;
update_item_profiles(user_profiles, item_profiles, item_purchasers, n, Gamma, Mu, profile_size);

update_user_profiles(user_profiles, item_profiles, user_purchases, n, Gamma, Lambda, profile_size);


}



